#include "stdint.h"
#include "string.h"
#include "stm32f10x_gpio.h"

#include "draw_ui.h"
#include "ui_tools.h"
#include "wifi_module.h"
#include "wifi_upload.h"
#include "usart.h"
#include "stm32f10x_usart.h"
#include "draw_pause_ui.h"
#include "UI.h"
#include "draw_dialog.h"
#include "draw_FileTransfer_ui.h"
#include "misc.h"
#include "stm32f10x_rcc.h"
#include "ff_gen_drv.h"
#include "sd_diskio.h"
#include "fatfs.h"
#include "stm32f1xx_hal_dma.h"
#include "tim.h"
#include "fastio_Robin2.h"

#include "Marlin.h"
#include "cardreader.h"
#include "mks_reprint.h"
#include "draw_wifi_list.h"
#include<cstring>
#include "ff.h"	

extern CardReader card;

extern uint8_t from_flash_pic;
extern DMA_HandleTypeDef hdma_spi2_tx;

extern "C" void exchangeFlashMode(char dmaMode);

extern uint8_t Explore_Disk (char* path , uint8_t recu_level);

extern uint8_t commands_in_queue;

int usartFifoAvailable(SZ_USART_FIFO *fifo);
int readUsartFifo(SZ_USART_FIFO *fifo, int8_t *buf, int32_t len);
int writeUsartFifo(SZ_USART_FIFO * fifo, int8_t * buf, int32_t len);

extern DMA_HandleTypeDef hdma_usart1_rx;

volatile SZ_USART_FIFO  WifiRxFifo;

#define WAIT_ESP_TRANS_TIMEOUT_TICK	10500

int cfg_wifi_flag = 0;
int cfg_cloud_flag = 0;

extern PRINT_TIME print_time;
extern FIL *srcfp;

char wifi_firm_ver[20] = {0};
WIFI_GCODE_BUFFER espGcodeFifo; 
extern uint8_t serial_wait_tick;
extern uint8_t pause_resum;

uint8_t wifi_connect_flg = 0;
extern volatile uint8_t get_temp_flag;


#define WIFI_MODE	2	//WIFI MODE
#define WIFI_AP_MODE	3//AP??

int upload_result = 0;

volatile WIFI_STATE wifi_link_state;
WIFI_PARA wifiPara;
IP_PARA ipPara;
CLOUD_PARA cloud_para;

char wifi_loop_time = 0;
char wifi_check_time = 0;

extern uint8_t gCurDir[100];

extern uint32_t wifi_loop_cycle;

volatile TRANSFER_STATE esp_state;

uint8_t left_to_send = 0;
uint8_t left_to_save[96] = {0};

volatile WIFI_DMA_RCV_FIFO wifiDmaRcvFifo;

volatile WIFI_TRANS_ERROR wifiTransError;
static bool need_ok_later = false;

extern volatile WIFI_STATE wifi_link_state;
extern WIFI_PARA wifiPara;
extern IP_PARA ipPara;
extern CLOUD_PARA cloud_para;

extern uint8_t command_send_flag;
extern volatile uint32_t TimeIncrease;

extern "C" void MX_SDIO_SD_Init(void);
extern FATFS fs;

void mount_file_sys(uint8_t disk_type) {
	f_mount(&fs, (TCHAR const*)SD_Path, 0);
}

uint32_t getWifiTick() {
	return TimeIncrease;
}

uint32_t getWifiTickDiff(int32_t lastTick, int32_t  curTick) {
	return ((lastTick <= curTick)?(curTick - lastTick):(0xffffffff - lastTick + curTick))* TICK_CYCLE;
}

void wifi_delay(int n) {
	uint32_t begin = getWifiTick();
	uint32_t end = begin;
	while(getWifiTickDiff(begin, end) < n)
		end = getWifiTick();
}

void wifi_reset() {
	uint32_t start, now;
	start = getWifiTick();
	now = start;
	WIFI_RESET();
	while(getWifiTickDiff(start, now) < 500)
		now = getWifiTick();
    WIFI_SET();
}

static void dma_init() {
	int i;
	__HAL_RCC_DMA1_CLK_ENABLE();
	//HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_0);
	HAL_NVIC_SetPriority(DMA1_Channel5_IRQn, 4, 0);
 	HAL_NVIC_EnableIRQ(DMA1_Channel5_IRQn);

	hdma_usart1_rx.Instance = DMA1_Channel5;
	//hdma_usart1_rx.Init.Channel = DMA_CHANNEL_4;
	hdma_usart1_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
	hdma_usart1_rx.Init.PeriphInc = DMA_PINC_DISABLE;
	hdma_usart1_rx.Init.MemInc = DMA_MINC_ENABLE;
	hdma_usart1_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	hdma_usart1_rx.Init.MemDataAlignment = DMA_PDATAALIGN_BYTE;
	hdma_usart1_rx.Init.Mode = DMA_NORMAL;
	hdma_usart1_rx.Init.Priority = DMA_PRIORITY_VERY_HIGH;
	if (HAL_DMA_Init((DMA_HandleTypeDef *)&hdma_usart1_rx) != HAL_OK)
	  Error_Handler();

	
	HAL_DMA_Start_IT((DMA_HandleTypeDef *)&hdma_usart1_rx,
	  	(uint32_t)&huart1.Instance->DR, 
	  	(uint32_t)(&WifiRxFifo.uartTxBuffer[0]), 
	  	UART_RX_BUFFER_SIZE);
	SET_BIT(huart1.Instance->CR3, USART_CR3_DMAR);
	
	for(i = 0; i < TRANS_RCV_FIFO_BLOCK_NUM; i++) {
		wifiDmaRcvFifo.bufferAddr[i] = &bmp_public_buf[1024 * i];		
		wifiDmaRcvFifo.state[i] = udisk_buf_empty;	
	}
	memset(wifiDmaRcvFifo.bufferAddr[0], 0, 1024 * TRANS_RCV_FIFO_BLOCK_NUM);
	wifiDmaRcvFifo.read_cur = 0;
	wifiDmaRcvFifo.write_cur = 0;
}

static void wifi_deInit() {
	HAL_DMA_Abort((DMA_HandleTypeDef *)&hdma_usart1_rx);
	HAL_DMA_DeInit((DMA_HandleTypeDef *)&hdma_usart1_rx);
	__HAL_DMA_DISABLE((DMA_HandleTypeDef *)&hdma_usart1_rx);
}

extern uint8_t mksUsart1Rx;

void esp_port_begin(uint8_t interrupt) {
	NVIC_InitTypeDef NVIC_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStruct;
	WifiRxFifo.uart_read_point = 0;
	WifiRxFifo.uart_write_point = 0;
	memset((uint8_t*)WifiRxFifo.uartTxBuffer, 0, sizeof(WifiRxFifo.uartTxBuffer));
	if(interrupt) {
		HAL_UART_DeInit(&huart1);
		MX_USART1_UART_Init(3);
		HAL_UART_Receive_IT(&huart1,&mksUsart1Rx,1);
	} else {
		HAL_UART_DeInit(&huart1);
		MX_USART1_UART_Init(5);
		dma_init();
	}
}

int raw_send_to_wifi(char *buf, int len) {
	uint32_t i;
	volatile uint32_t delayIndex;
	if(buf == 0  ||  len <= 0)
		return 0;
	for(i = 0; i < len; i++) {
		USART_SendData(huart1.Instance, *(buf+i));
		// Loop until USART DR register is empty
		while(USART_GetFlagStatus(huart1.Instance, USART_FLAG_TXE) == RESET){}
	}
	return len;
}

void wifi_ret_ack() {
	if(gCfgItems.wifi_type == HLK_WIFI) 
		raw_send_to_wifi("ok\r\n", strlen("ok\r\n"));
}

char buf_to_wifi[256];
int index_to_wifi = 0;
int package_to_wifi(WIFI_RET_TYPE type, char *buf, int len) {
	char wifi_ret_head = 0xa5;
	char wifi_ret_tail = 0xfc;
	
	if(type == WIFI_PARA_SET) {
 		int data_offset = 4;
		int apLen = strlen((const char *)gCfgItems.wifi_ap);
		int keyLen = strlen((const char *)gCfgItems.wifi_key);
		
 		memset(buf_to_wifi, 0, sizeof(buf_to_wifi));
		index_to_wifi = 0;

		buf_to_wifi[data_offset] = gCfgItems.wifi_mode_sel;
		buf_to_wifi[data_offset + 1]  = apLen;
		strncpy(&buf_to_wifi[data_offset + 2], (const char *)gCfgItems.wifi_ap, apLen);
		buf_to_wifi[data_offset + apLen + 2]  = keyLen;
		strncpy(&buf_to_wifi[data_offset + apLen + 3], (const char *)gCfgItems.wifi_key, keyLen);
		buf_to_wifi[data_offset + apLen + keyLen + 3] = wifi_ret_tail;

		index_to_wifi = apLen + keyLen + 3;

		buf_to_wifi[0] = wifi_ret_head;
		buf_to_wifi[1] = type;
		buf_to_wifi[2] = index_to_wifi & 0xff;
		buf_to_wifi[3] = (index_to_wifi >> 8) & 0xff;

		raw_send_to_wifi(buf_to_wifi, 5 + index_to_wifi);

		memset(buf_to_wifi, 0, sizeof(buf_to_wifi));
		index_to_wifi = 0;
 	} else if(type == WIFI_TRANS_INF) {
		if(len > sizeof(buf_to_wifi) - index_to_wifi - 5) {
			memset(buf_to_wifi, 0, sizeof(buf_to_wifi));
			index_to_wifi = 0;
			return;
		}
		if(len > 0) {
			memcpy(&buf_to_wifi[4 + index_to_wifi], buf, len);
			index_to_wifi += len;
			if(index_to_wifi < 1)
				return;

			if(buf_to_wifi[index_to_wifi + 3] == '\n') {
			 	//mask "wait" "busy" "X:"
			 	if(((buf_to_wifi[4] == 'w') && (buf_to_wifi[5] == 'a') && (buf_to_wifi[6] == 'i')  && (buf_to_wifi[7] == 't') )
					|| ((buf_to_wifi[4] == 'b') && (buf_to_wifi[5] == 'u') && (buf_to_wifi[6] == 's')  && (buf_to_wifi[7] == 'y') )
					|| ((buf_to_wifi[4] == 'X') && (buf_to_wifi[5] == ':') )
					) {
			 		memset(buf_to_wifi, 0, sizeof(buf_to_wifi));
				 	index_to_wifi = 0;
					return;
			 	}
				buf_to_wifi[0] = wifi_ret_head;
				buf_to_wifi[1] = type;
				buf_to_wifi[2] = index_to_wifi & 0xff;
				buf_to_wifi[3] = (index_to_wifi >> 8) & 0xff;	
				buf_to_wifi[4 + index_to_wifi] = wifi_ret_tail;
				raw_send_to_wifi(buf_to_wifi, 5 + index_to_wifi);
				memset(buf_to_wifi, 0, sizeof(buf_to_wifi));
				index_to_wifi = 0;
			 }
		}
	} else if(type == WIFI_EXCEP_INF) {
		memset(buf_to_wifi, 0, sizeof(buf_to_wifi));		

		buf_to_wifi[0] = wifi_ret_head;
		buf_to_wifi[1] = type;
		buf_to_wifi[2] = 1;
		buf_to_wifi[3] = 0;
		buf_to_wifi[4] = *buf;
		buf_to_wifi[5] = wifi_ret_tail;

		raw_send_to_wifi(buf_to_wifi, 6);

		memset(buf_to_wifi, 0, sizeof(buf_to_wifi));
		index_to_wifi = 0;
	} else if(type == WIFI_CLOUD_CFG) {
		int data_offset = 4;
		int urlLen = strlen((const char *)gCfgItems.cloud_hostUrl);
 		memset(buf_to_wifi, 0, sizeof(buf_to_wifi));
		index_to_wifi = 0;

		if(gCfgItems.cloud_enable == 0x1)
			buf_to_wifi[data_offset] = 0x0a;
		else
			buf_to_wifi[data_offset] = 0x05;
		
		buf_to_wifi[data_offset + 1]  = urlLen;
		strncpy(&buf_to_wifi[data_offset + 2], (const char *)gCfgItems.cloud_hostUrl, urlLen);
		buf_to_wifi[data_offset + urlLen + 2]  = gCfgItems.cloud_port & 0xff;
		buf_to_wifi[data_offset + urlLen + 3]  = (gCfgItems.cloud_port >> 8) & 0xff;
		buf_to_wifi[data_offset + urlLen + 4] = wifi_ret_tail;

		index_to_wifi = urlLen + 4;

		buf_to_wifi[0] = wifi_ret_head;
		buf_to_wifi[1] = type;
		buf_to_wifi[2] = index_to_wifi & 0xff;
		buf_to_wifi[3] = (index_to_wifi >> 8) & 0xff;

		raw_send_to_wifi(buf_to_wifi, 5 + index_to_wifi);

		memset(buf_to_wifi, 0, sizeof(buf_to_wifi));
		index_to_wifi = 0;
	} else if(type == WIFI_CLOUD_UNBIND) {
		memset(buf_to_wifi, 0, sizeof(buf_to_wifi));		

		buf_to_wifi[0] = wifi_ret_head;
		buf_to_wifi[1] = type;
		buf_to_wifi[2] = 0;
		buf_to_wifi[3] = 0;
		buf_to_wifi[4] = wifi_ret_tail;

		raw_send_to_wifi(buf_to_wifi, 5);

		memset(buf_to_wifi, 0, sizeof(buf_to_wifi));
		index_to_wifi = 0;
	}
}


int send_to_wifi(char *buf, int len){
	return package_to_wifi(WIFI_TRANS_INF, buf, len);
}

void set_cur_file_sys(int fileType) {
	gCfgItems.fileSysType = fileType;
}

void get_file_list(char *path) {
	if( path == 0)
		return;
	if(gCfgItems.fileSysType == FILE_SYS_SD) {
		if(SD_DET_IP == SD_DETECT_INVERTED) {
			//SD_Initialize();
			strcpy(card.gCurDir, "1:");
			ui_print_process.file_name[0]=(char )'1';
			f_mount(&fs, (TCHAR const*)card.gCurDir, 0);	
		}
	}
	Explore_Disk(path, 0);
}

char wait_ip_back_flag = 0;

void init_queue(QUEUE *h_queue) {
	if(h_queue == 0)
		return;
	h_queue->rd_index = 0;
	h_queue->wt_index = 0;
	memset(h_queue->buf, 0, sizeof(h_queue->buf));
}

int push_queue(QUEUE *h_queue, char *data_to_push, int data_len) {
	if(h_queue == 0)
		return -1;

	if(data_len > sizeof(h_queue->buf[h_queue->wt_index]))
		return -1;

	if((h_queue->wt_index + 1) % 20 == h_queue->rd_index)
		return -1;

	memset(h_queue->buf[h_queue->wt_index], 0, sizeof(h_queue->buf[h_queue->wt_index]));
	memcpy(h_queue->buf[h_queue->wt_index], data_to_push, data_len);

	h_queue->wt_index = (h_queue->wt_index + 1) % 20;
	
	return 0;
}

int pop_queue(QUEUE *h_queue, char *data_for_pop, int data_len) {
	if(h_queue == 0)
		return -1;

	if(data_len < strlen(h_queue->buf[h_queue->rd_index]))
		return -1;

	if(h_queue->rd_index == h_queue->wt_index)
		return -1;

	memset(data_for_pop, 0, data_len);
	memcpy(data_for_pop, h_queue->buf[h_queue->rd_index], strlen(h_queue->buf[h_queue->rd_index]));

	h_queue->rd_index = (h_queue->rd_index + 1) % 20;
	
	return 0;
}

unsigned char computeBinarySize(char *ptr) {
    unsigned char s = 2; // not include checksum
    unsigned short bitfield = *(unsigned short*)ptr;
    if(bitfield & 1) s+=2;
    if(bitfield & 8) s+=4;
    if(bitfield & 16) s+=4;
    if(bitfield & 32) s+=4;
    if(bitfield & 64) s+=4;
    if(bitfield & 256) s+=4;
    if(bitfield & 512) s+=1;
    if(bitfield & 1024) s+=4;
    if(bitfield & 2048) s+=4;
 
    if(bitfield & 2) s+=1;
    if(bitfield & 4) s+=1;
    if(bitfield & 32768) s+=16;
    return s;
}

typedef struct {
	FIL  *file;
	char write_buf[513];
	int write_index;	
	uint8_t saveFileName[30];
	uint32_t fileLen;
	uint32_t tick_begin;
	uint32_t tick_end;
} FILE_WRITER;

FILE_WRITER file_writer;
FIL save_File;

int32_t lastFragment = 0;

char lastBinaryCmd[50] = {0};

int total_write = 0;	
char binary_head[2] = {0, 0};
unsigned char binary_data_len = 0;

int write_to_file(char *buf, int len) {
	int num_write;
	int i;
	FRESULT res;
	for(i = 0; i < len; i++) {
		file_writer.write_buf[file_writer.write_index++] = buf[i];
		if(file_writer.write_index >= 512/*1024*/) {
			res =  f_write (file_writer.file, file_writer.write_buf, (uint32_t)file_writer.write_index,  (uint32_t *)&num_write);
			if((res != FR_OK) || (num_write != file_writer.write_index))
				return  -1;
			memset(file_writer.write_buf, 0, sizeof(file_writer.write_buf));
			file_writer.write_index = 0;		
		}
	}
	return 0;
}

#define ESP_PROTOC_HEAD				(uint8_t)0xa5
#define ESP_PROTOC_TAIL				(uint8_t)0xfc

#define ESP_TYPE_NET				(uint8_t)0x0
#define ESP_TYPE_GCODE				(uint8_t)0x1
#define ESP_TYPE_FILE_FIRST			(uint8_t)0x2
#define ESP_TYPE_FILE_FRAGMENT		(uint8_t)0x3

#define ESP_TYPE_WIFI_LIST			(uint8_t)0x4

uint8_t esp_msg_buf[UART_RX_BUFFER_SIZE] = {0};
uint16_t esp_msg_index = 0;

typedef struct {
	uint8_t head; //0xa5
	uint8_t type; //0x0:????,0x1:gcode??,0x2:??????,0x3:??????(?????),0x4:????????
	uint16_t dataLen; //????
	uint8_t *data; //????
	uint8_t tail; // 0xfc
} ESP_PROTOC_FRAME;

static int cut_msg_head(uint8_t *msg, uint16_t msgLen, uint16_t cutLen) {
	int i;
	if(msgLen < cutLen) {
		return 0;
	} else if(msgLen == cutLen) {
		memset(msg, 0, msgLen);
		return 0;
	}
	for(i = 0; i < (msgLen - cutLen); i++)
		msg[i] = msg[cutLen + i];
	memset(&msg[msgLen - cutLen], 0, cutLen);
	return msgLen - cutLen;
}


uint8_t Explore_Disk (char* path , uint8_t recu_level) {
	FILINFO fno;
	DIR dir;
	SD_CardInfo cardinfo;
	char *fn;
	char tmp[200];
	char Fstream[200];
	int local_offset;
	int file_offset = 0;
	uint8_t res;
  
	#if _USE_LFN
		static char lfn[_MAX_LFN + 1];
		fno.lfname = lfn;
		fno.lfsize = sizeof(lfn);
	#endif
    if(path == 0)
		return 0;

	if(path[0] == '0')
		f_mount(&fs, (TCHAR const*)path, 0);
	else if(path[0] == '1')
		f_mount(&fs, (TCHAR const*)path, 0);
	else
		return 0;
	
	for(;;) {
		local_offset = 0;
		if (f_opendir(&dir, path) == FR_OK) {
		    while(1) {
				res = f_readdir(&dir, &fno);
				if (res != FR_OK || fno.fname[0] == 0) 
					return 0;
				if (fno.fname[0] == '.')
					continue;
				if(local_offset >= file_offset) {
					file_offset++;
					break;
				}
				local_offset++;
		    }
			
		    fn = ((fno.lfname[0] == 0) || (fno.lfname == 0))?fno.fname:fno.lfname;
		    if((strstr(fn, ".g")) || (strstr(fn, ".G")) || (fno.fattrib & AM_DIR)) {
				strcpy(tmp, fn);
				memset(Fstream, 0, sizeof(Fstream));
				strcpy(Fstream, tmp);
				if((fno.fattrib & AM_DIR)&&(recu_level <= 10)) {
					strcat(Fstream, ".DIR\r\n");
					send_to_wifi(Fstream, strlen(Fstream));
				} else {
					strcat(Fstream, "\r\n");
					send_to_wifi(Fstream, strlen(Fstream));
				}
		    }
		} else
			break;
	}
	return res;
}

static void wifi_gcode_exec(uint8_t *cmd_line) {
	int8_t  tempBuf[100] = {0};
	uint8_t *tmpStr = 0;
	int  cmd_value;
	volatile int print_rate;
	//cmd_line[0]!='\n'
	if((strstr((char *)&cmd_line[0], "\n") != 0) && ((strstr((char *)&cmd_line[0], "G") != 0) || (strstr((char *)&cmd_line[0], "M") != 0) || (strstr((char *)&cmd_line[0], "T") != 0) )) {
		
		tmpStr = (uint8_t *)strstr((char *)&cmd_line[0], "\n");
		if(tmpStr)
			*tmpStr = '\0';
		tmpStr = (uint8_t *)strstr((char *)&cmd_line[0], "\r");
		if(tmpStr)
			*tmpStr = '\0';
		tmpStr = (uint8_t *)strstr((char *)&cmd_line[0], "*");
		if(tmpStr)
			*tmpStr = '\0';
		tmpStr = (uint8_t *)strstr((char *)&cmd_line[0], "M");
		if( tmpStr) {
			cmd_value = atoi((char *)(tmpStr + 1));
			tmpStr = (uint8_t *)strstr((char *)tmpStr, " ");
			switch(cmd_value) {
				case 20: //print sd / udisk file
					if(mksReprint.mks_printer_state == MKS_IDLE) {
						int index = 0;
						char *det_pos;
						if(tmpStr == 0) {
							gCfgItems.fileSysType = FILE_SYS_SD;	
							send_to_wifi("Begin file list\r\n", strlen("Begin file list\r\n"));
							get_file_list(SD_Path);
							send_to_wifi("End file list\r\n", strlen("End file list\r\n"));
							send_to_wifi("ok\r\n", strlen("ok\r\n"));
							break;
						}
						while(tmpStr[index] == ' ')
							index++;
						if(gCfgItems.wifi_type == ESP_WIFI) {
							char *path = (char *)tempBuf;

							if(strlen((char *)&tmpStr[index]) < 80) {
								send_to_wifi("Begin file list\r\n", strlen("Begin file list\r\n"));
								if(strncmp((char *)&tmpStr[index], "1:", 2) == 0)
									gCfgItems.fileSysType = FILE_SYS_SD;
								strcpy((char *)path, (char *)&tmpStr[index]);	
								get_file_list(path);
								send_to_wifi("End file list\r\n", strlen("End file list\r\n"));
							}
							send_to_wifi("ok\r\n", strlen("ok\r\n"));
						}
					}
					break;
				case 21:
					/*init sd card*/
					send_to_wifi("ok\r\n", strlen("ok\r\n"));
					break;
				case 23:					
					/*select the file*/
					if(mksReprint.mks_printer_state == MKS_IDLE) {
						char *det_pos;
						int index = 0;
						FIL temp_file;
						FRESULT res ;
						while(tmpStr[index] == ' ')
							index++;

						if(strstr((char *)&tmpStr[index], ".g") || strstr((char *)&tmpStr[index], ".G")) {
							if(strlen((char *)&tmpStr[index]) < 80) {
								memset(ui_print_process.file_name, 0, sizeof(ui_print_process.file_name));
								if(gCfgItems.wifi_type == ESP_WIFI) {
									if(strncmp((char *)&tmpStr[index], "1:", 2) == 0) {
										gCfgItems.fileSysType = FILE_SYS_SD;
									} else if(strncmp((char *)&tmpStr[index], "0:", 2) == 0) {
									} else {
										if(gCfgItems.fileSysType == FILE_SYS_SD) {
											strcat((char *)ui_print_process.file_name, "1:");
										}
										if(tmpStr[index] != '/')
											strcat((char *)ui_print_process.file_name, "/");
									}
									strcat((char *)ui_print_process.file_name, (char *)&tmpStr[index]);
								} else
									strcpy(ui_print_process.file_name, (char *)&tmpStr[index]);
								res = f_open(&temp_file, ui_print_process.file_name, FA_OPEN_EXISTING | FA_READ);
								if(res == FR_OK) {
									send_to_wifi("File selected\r\n", strlen("File selected\r\n"));
								} else {
									send_to_wifi("file.open failed\r\n", strlen("file.open failed\r\n"));
									ui_print_process.file_name[0] = 0;
								}
								send_to_wifi("ok\r\n", strlen("ok\r\n"));
							}
						}
					}
					break;

				case 24:
					if(ui_print_process.file_name[0] != 0) {
						if(mksReprint.mks_printer_state == MKS_IDLE) {
							ui_app.startPrintFile();
						} else if(mksReprint.mks_printer_state == MKS_PAUSED) {
							pause_resum = 1;
							mksReprint.mks_printer_state = MKS_RESUMING;
							clear_cur_ui();
							start_print_time();
							printing_ui.show();
						} else if(mksReprint.mks_printer_state == MKS_REPRINTING) {
							pause_resum = 1;
							mksReprint.mks_printer_state = MKS_REPRINTED;
							clear_cur_ui();
							start_print_time();
							printing_ui.show();
						}		
					}
					send_to_wifi("ok\r\n", strlen("ok\r\n"));
					break;
				case 25:
					/*pause print file*/						
					if(mksReprint.mks_printer_state == MKS_WORKING) {
						stop_print_time();							
						clear_cur_ui();
						card.pauseSDPrint();
      					print_job_timer.pause();
						mksReprint.mks_printer_state = MKS_PAUSING;
						printing_ui.show();
						send_to_wifi("ok\r\n", strlen("ok\r\n"));
					}					
					break;
					
				case 26:
					/*stop print file*/						
					if((mksReprint.mks_printer_state == MKS_WORKING) || (mksReprint.mks_printer_state == MKS_PAUSED) || (mksReprint.mks_printer_state == MKS_REPRINTING)) {
						stop_print_time();							
						clear_cur_ui();
					    card.stopSDPrint();
                        wait_for_heatup = false;
						mksReprint.mks_printer_state = MKS_STOP;
						reset_file_info();
						ui_app.showMainWidget();
						send_to_wifi("ok\r\n", strlen("ok\r\n"));
					}					
					break;

				case 27:
					/*report print rate*/
					//if((printerStaus == pr_working) || (printerStaus == pr_pause))
					if((mksReprint.mks_printer_state == MKS_WORKING) || (mksReprint.mks_printer_state == MKS_PAUSED)|| (mksReprint.mks_printer_state == MKS_REPRINTING)) {
						print_rate = ui_print_process.rate;
						memset((char *)tempBuf, 0, sizeof(tempBuf));
						sprintf((char *)tempBuf, "M27 %d\r\n", print_rate);
						wifi_ret_ack();
						send_to_wifi((char *)tempBuf, strlen((char *)tempBuf));
					}
					break;

				case 28:
					/*begin to transfer file to filesys*/
					if(mksReprint.mks_printer_state == MKS_IDLE) {
						int index = 0;
						while(tmpStr[index] == ' ')
							index++;

						if(strstr((char *)&tmpStr[index], ".g") || strstr((char *)&tmpStr[index], ".G")) {
							FRESULT res;
							strcpy((char *)file_writer.saveFileName, (char *)&tmpStr[index]);
							
							if(gCfgItems.fileSysType == FILE_SYS_SD) {
								memset(tempBuf, 0, sizeof(tempBuf));
								sprintf((char *)tempBuf, "1:/%s", file_writer.saveFileName);
							} else if(gCfgItems.fileSysType == FILE_SYS_USB) {
								memset(tempBuf, 0, sizeof(tempBuf));
								sprintf((char *)tempBuf, "0:/%s", (char *)file_writer.saveFileName);
							}
							mount_file_sys(gCfgItems.fileSysType);
							
							res = f_open(&save_File, (char *)tempBuf, FA_CREATE_ALWAYS | FA_WRITE);
							
							if(res == FR_OK) {
								memset(file_writer.saveFileName, 0, sizeof(file_writer.saveFileName));
								strcpy((char *)file_writer.saveFileName, (char *)&tmpStr[index]);
								memset(tempBuf, 0, sizeof(tempBuf));
								sprintf((char *)tempBuf, "Writing to file: %s\r\n", (char *)file_writer.saveFileName);
								wifi_ret_ack();
								send_to_wifi((char *)tempBuf, strlen((char *)tempBuf));

								total_write = 0;	
								wifi_link_state = WIFI_WAIT_TRANS_START;
							} else {
								wifi_link_state = WIFI_CONNECTED;
								clear_cur_ui();
								draw_dialog_filetransfer(2);
							}
						}
					}
					break;
				case 105:
				case 991:
					memset(tempBuf, 0, sizeof(tempBuf));
					if(cmd_value == 105) {
						send_to_wifi("ok\r\n", strlen("ok\r\n"));
						sprintf((char *)tempBuf,"T:%.1f /%.1f B:%.1f /%.1f T0:%.1f /%.1f T1:%.1f /%.1f @:0 B@:0\r\n",
								thermalManager.current_temperature[0],(float)thermalManager.target_temperature[0],
								thermalManager.current_temperature_bed,(float)thermalManager.target_temperature_bed,
								thermalManager.current_temperature[0],(float)thermalManager.target_temperature[0],
								thermalManager.current_temperature[1],(float)thermalManager.target_temperature[1]
						);
					} else {
						sprintf((char *)tempBuf,"T:%d /%d B:%d /%d T0:%d /%d T1:%d /%d @:0 B@:0\r\n", 
								(int)thermalManager.current_temperature[0],(int)thermalManager.target_temperature[0],
								(int)thermalManager.current_temperature_bed,(int)thermalManager.target_temperature_bed,
								(int)thermalManager.current_temperature[0],(int)thermalManager.target_temperature[0],
								(int)thermalManager.current_temperature[1],(int)thermalManager.target_temperature[1]
						);
					}
					send_to_wifi((char *)tempBuf, strlen((char *)tempBuf));
					enqueue_and_echo_commands_P(PSTR("M105"));
					
					break;
				case 992:
					if((mksReprint.mks_printer_state == MKS_WORKING) || (mksReprint.mks_printer_state == MKS_PAUSED)) {
						memset(tempBuf,0,sizeof(tempBuf));
						sprintf((char *)tempBuf, "M992 %d%d:%d%d:%d%d\r\n", print_time.hours/10, print_time.hours%10, print_time.minutes/10, print_time.minutes%10,	print_time.seconds/10, print_time.seconds%10);
						wifi_ret_ack();
						send_to_wifi((char *)tempBuf, strlen((char *)tempBuf));	
					}
					break;
				case 994:
					if((mksReprint.mks_printer_state == MKS_WORKING) || (mksReprint.mks_printer_state == MKS_PAUSED)) {
						memset(tempBuf,0,sizeof(tempBuf));
						if(strlen((char *)ui_print_process.file_name) > (100-1))
							return;
						sprintf((char *)tempBuf, "M994 %s;%d\n", ui_print_process.file_name, f_size(srcfp));
						wifi_ret_ack();
						send_to_wifi((char *)tempBuf, strlen((char *)tempBuf));	
					}
					break;
				case 997:
					if(mksReprint.mks_printer_state == MKS_IDLE) {
						wifi_ret_ack();
						send_to_wifi("M997 IDLE\r\n", strlen("M997 IDLE\r\n"));
					} else if(mksReprint.mks_printer_state == MKS_WORKING) {
						wifi_ret_ack();
						send_to_wifi("M997 PRINTING\r\n", strlen("M997 PRINTING\r\n"));
					} else if(mksReprint.mks_printer_state == MKS_PAUSED) {
						wifi_ret_ack();
						send_to_wifi("M997 PAUSE\r\n", strlen("M997 PAUSE\r\n"));
					} else if(mksReprint.mks_printer_state == MKS_REPRINTING) {
						wifi_ret_ack();
						send_to_wifi("M997 PAUSE\r\n", strlen("M997 PAUSE\r\n"));
					}		
					if (command_send_flag==0)
						get_wifi_list_command_send();
					break;
				case 998:
					if(mksReprint.mks_printer_state == MKS_IDLE) {
						if(atoi((char *)tmpStr) == 0) {
							set_cur_file_sys(0);
						} else if(atoi((char *)tmpStr) == 1) {
							set_cur_file_sys(1);
						}
						wifi_ret_ack();
					}
					break;
				case 115:
					memset(tempBuf,0,sizeof(tempBuf));
					send_to_wifi("ok\r\n", strlen("ok\r\n"));
					send_to_wifi("FIRMWARE_NAME:MKS ROBIN NANO\r\n", strlen("FIRMWARE_NAME:MKS ROBIN NANO\r\n"));
					break;

				default:
					strcat((char *)cmd_line, "\n");
					if(serial_wait_tick > 5) {
						uint32_t left;
						if(espGcodeFifo.r >  espGcodeFifo.w)
							left =  espGcodeFifo.r - espGcodeFifo.w - 1;
						else
							left = WIFI_GCODE_BUFFER_SIZE + espGcodeFifo.r - espGcodeFifo.w - 1;
						if(left >= strlen((const char *)cmd_line)) {
							uint32_t index = 0;
							while(index < strlen((const char *)cmd_line)) {
								espGcodeFifo.Buffer[espGcodeFifo.w] = cmd_line[index] ;
								espGcodeFifo.w =  (espGcodeFifo.w + 1) % WIFI_GCODE_BUFFER_SIZE;
								index++;
							}
							if(left - WIFI_GCODE_BUFFER_LEAST_SIZE >= strlen((const char *)cmd_line))
								send_to_wifi("ok\r\n", strlen("ok\r\n"));
							else
								need_ok_later = true;
						}
					}
					break;
			}
		} else {
			strcat((char *)cmd_line, "\n");
			uint32_t left_g;
			if(serial_wait_tick > 5) {
				if(espGcodeFifo.r >  espGcodeFifo.w)
					left_g =  espGcodeFifo.r - espGcodeFifo.w - 1;
				else
					left_g = WIFI_GCODE_BUFFER_SIZE + espGcodeFifo.r - espGcodeFifo.w - 1;
				if(left_g >= strlen((const char *)cmd_line)) {
					uint32_t index = 0;
					while(index < strlen((const char *)cmd_line)) {
						espGcodeFifo.Buffer[espGcodeFifo.w] = cmd_line[index] ;
						espGcodeFifo.w =  (espGcodeFifo.w + 1) % WIFI_GCODE_BUFFER_SIZE;
						index++;
					}
					if(left_g - WIFI_GCODE_BUFFER_LEAST_SIZE >= strlen((const char *)cmd_line))
						send_to_wifi("ok\r\n", strlen("ok\r\n"));
					else
						need_ok_later = true;
				}
			}
		}
	}
}

/*????????????????,?0??,???????-1*/
static int32_t charAtArray(const uint8_t *_array, uint32_t _arrayLen, uint8_t _char) {
	uint32_t i;
	for(i = 0; i < _arrayLen; i++)
		if(*(_array + i) == _char)
			return i;
	return -1;
}

void get_wifi_list_command_send() {
	char buf[6]={0};
	buf[0] = 0xA5;
	buf[1] = 0x07;
	buf[2] = 0x00;
	buf[3] = 0x00;
	buf[4] = 0xFC;
	raw_send_to_wifi(buf, 5);
}

static void net_msg_handle(uint8_t * msg, uint16_t msgLen) {
	int wifiNameLen, wifiKeyLen, hostLen, id_len, ver_len;
	char ipStr[16];
	if(msgLen <= 0)
		return;
	sprintf(ipPara.ip_addr, "%d.%d.%d.%d", msg[0], msg[1], msg[2], msg[3]);
	if(msg[6] == 0x0a) {
		wifi_link_state = WIFI_CONNECTED;
	} else if(msg[6] == 0x0e) {
		wifi_link_state = WIFI_EXCEPTION;
	} else {
		wifi_link_state = WIFI_NOT_CONFIG;
	}

	//mode
	wifiPara.mode = msg[7];
	

	//wifi name
	wifiNameLen = msg[8];
	wifiKeyLen = msg[9 + wifiNameLen];
	if(wifiNameLen < 32) {
		memset(wifiPara.ap_name, 0, sizeof(wifiPara.ap_name));
		memcpy(wifiPara.ap_name, &msg[9], wifiNameLen);

		memset(&wifi_list.wifiConnectedName,0,sizeof(wifi_list.wifiConnectedName));
		memcpy(&wifi_list.wifiConnectedName,&msg[9],wifiNameLen);

		//wifi key		
		if(wifiKeyLen < 64) {
			memset(wifiPara.keyCode, 0, sizeof(wifiPara.keyCode));
			memcpy(wifiPara.keyCode, &msg[10 + wifiNameLen], wifiKeyLen);
		}
	}
	cloud_para.state =msg[10 + wifiNameLen + wifiKeyLen];
	hostLen = msg[11 + wifiNameLen + wifiKeyLen];
	if(cloud_para.state) {
		if(hostLen < 96) {
			memset(cloud_para.hostUrl, 0, sizeof(cloud_para.hostUrl));
			memcpy(cloud_para.hostUrl, &msg[12 + wifiNameLen + wifiKeyLen], hostLen);
		}
		cloud_para.port = msg[12 + wifiNameLen + wifiKeyLen + hostLen] + (msg[13 + wifiNameLen + wifiKeyLen + hostLen] << 8);
	}

	// id
	id_len = msg[14 + wifiNameLen + wifiKeyLen + hostLen];
	if(id_len == 20) {
		memset(cloud_para.id, 0, sizeof(cloud_para.id));
		memcpy(cloud_para.id, (const char *)&msg[15 + wifiNameLen + wifiKeyLen + hostLen], id_len);
	}
	ver_len = msg[15 + wifiNameLen + wifiKeyLen + hostLen + id_len];
	if(ver_len < 20) {
		memset(wifi_firm_ver, 0, sizeof(wifi_firm_ver));
		memcpy(wifi_firm_ver, (const char *)&msg[16 + wifiNameLen + wifiKeyLen + hostLen + id_len], ver_len);
	}

	if(cfg_wifi_flag == 1) {
		if((wifiPara.mode != gCfgItems.wifi_mode_sel)
			|| (strncmp(wifiPara.ap_name, (const char *)gCfgItems.wifi_ap, 32) != 0)
			|| (strncmp(wifiPara.keyCode, (const char *)gCfgItems.wifi_key, 64) != 0)) {
			package_to_wifi(WIFI_PARA_SET, (char *)0, 0);
		} else
			cfg_wifi_flag = 0;
	}
	if(cfg_cloud_flag == 1) {
		if(((cloud_para.state >> 4) != gCfgItems.cloud_enable)
			|| (strncmp(cloud_para.hostUrl, (const char *)gCfgItems.cloud_hostUrl, 96) != 0)
			|| (cloud_para.port != gCfgItems.cloud_port)) {
			package_to_wifi(WIFI_CLOUD_CFG, (char *)0, 0);
		} else
			cfg_cloud_flag = 0;
	}
}

static void wifi_list_msg_handle(uint8_t * msg, uint16_t msgLen) {
	int wifiNameLen,wifiMsgIdex=1;
	int8_t wifi_name_is_same=0,wifi_name_is_empty=0;
	int8_t i,j;
	int8_t wifi_name_num=0;
	uint8_t *str=0;
	int8_t valid_name_num;
	
	if(msgLen <= 0)
		return;
	if(disp_state == KEY_BOARD_UI)
		return;

	wifi_list.getNameNum = msg[0];

	if(wifi_list.getNameNum < 20) {
		command_send_flag=1;
		
		memset(wifi_list.wifiName,0,sizeof(wifi_list.wifiName));
		
		wifi_name_num = wifi_list.getNameNum;
		
		valid_name_num=0;
		str = wifi_list.wifiName[valid_name_num];
		
		if(wifi_list.getNameNum > 0)wifi_list.currentWifipage = 1;
		
		for(i=0;i<wifi_list.getNameNum;i++) {
			wifiNameLen = msg[wifiMsgIdex];
			wifiMsgIdex  +=  1;
			if(wifiNameLen < 32) {
				memset(str, 0, WIFI_NAME_BUFFER_SIZE);
				memcpy(str, &msg[wifiMsgIdex], wifiNameLen);
				for(j=0;j<valid_name_num;j++) {
					if(strcmp((const char *)str,(const char *)wifi_list.wifiName[j]) == 0) {
						wifi_name_is_same = 1;
						break;
					}
				}
				if ((wifi_name_is_same != 1) && (str[0] > 0x80))
						wifi_name_is_same = 1;

				if(wifi_name_is_same == 1) {
					wifi_name_is_same = 0;
					wifiMsgIdex  +=  wifiNameLen;
					wifiMsgIdex  +=  1;
					wifi_name_num--;
					continue;
				}
				if(i < WIFI_TOTAL_NUMBER-1)
					str = wifi_list.wifiName[++valid_name_num];
			}
			wifiMsgIdex  +=  wifiNameLen;
			wifi_list.RSSI[i] = msg[wifiMsgIdex];
			wifiMsgIdex  +=  1;
		}
		wifi_list.getNameNum = wifi_name_num;
		if(wifi_list.getNameNum % NUMBER_OF_PAGE == 0) {
			wifi_list.getPage = wifi_list.getNameNum/NUMBER_OF_PAGE;
		} else {
			wifi_list.getPage = wifi_list.getNameNum/NUMBER_OF_PAGE + 1;
		}
		wifi_list.nameIndex = 0;
		if(disp_state == WIFI_LIST_UI)
			disp_wifi_list();
	}
}

static void gcode_msg_handle(uint8_t * msg, uint16_t msgLen) {
	uint8_t gcodeBuf[100] = {0};
	char *index_s;
	char *index_e;
	
	if(msgLen <= 0)
		return;

	index_s = (char *)msg;
	index_e = (char *)strstr((char *)msg, "\n");
	if(*msg == 'N') {
		index_s = (char *)strstr((char *)msg, " ");
		while((*index_s) == ' ')
			index_s++;
	}
	while((index_e != 0) && ((int)index_s < (int)index_e)) {
		if(index_e - index_s < sizeof(gcodeBuf)) {
			memset(gcodeBuf, 0, sizeof(gcodeBuf));
			memcpy(gcodeBuf, index_s, index_e - index_s + 1);
			wifi_gcode_exec(gcodeBuf);
		}
		while((*index_e == '\r') || (*index_e == '\n'))
			index_e++;
		index_s = index_e;
		index_e = (char *)strstr(index_s, "\n");
	}
}

void utf8_2_gbk(uint8_t *source,uint8_t Len) {
	uint8_t  i=0,char_i=0,char_byte_num=0;
	uint16_t  u16_h,u16_m,u16_l,u16_value;
	uint8_t FileName_unicode[30];
	
 	memset(FileName_unicode, 0, sizeof(FileName_unicode));
	
	while(1) {
		char_byte_num = source[i] & 0xF0;
		if(source[i] < 0X80) {
			FileName_unicode[char_i] = source[i];
			i += 1;
			char_i += 1;
		} else if(char_byte_num == 0XC0 || char_byte_num == 0XD0) {
			u16_h = (((uint16_t)source[i] <<8) & 0x1f00) >> 2;
			u16_l = ((uint16_t)source[i+1] & 0x003f);
			u16_value = (u16_h | u16_l);
			u16_value=ff_convert(u16_value,0);
			FileName_unicode[char_i] = (uint8_t)((u16_value & 0xff00) >> 8);
			FileName_unicode[char_i + 1] = (uint8_t)(u16_value & 0x00ff);
			i += 2;
			char_i += 2;
		} else if(char_byte_num == 0XE0) {
			//--3byte
			u16_h = (((uint16_t)source[i] <<8 ) & 0x0f00) << 4;
			u16_m = (((uint16_t)source[i+1] << 8) & 0x3f00) >> 2;
			u16_l = ((uint16_t)source[i+2] & 0x003f);
			u16_value = (u16_h | u16_m | u16_l);
			u16_value=ff_convert(u16_value,0);
			FileName_unicode[char_i] = (uint8_t)((u16_value & 0xff00) >> 8);
			FileName_unicode[char_i + 1] = (uint8_t)(u16_value & 0x00ff);
			i += 3;
			char_i += 2;
		} else if(char_byte_num == 0XF0) {
			//--4byte ������
			i += 4;
		} else
			break;
		if(i >= Len || i >= 255)
			break;
	}
	memcpy(source, FileName_unicode, sizeof(FileName_unicode));
}

char saveFilePath[50];

static void file_first_msg_handle(uint8_t * msg, uint16_t msgLen) {
	uint8_t fileNameLen = *msg;
	FRESULT res;
	if(msgLen != fileNameLen + 5)
		return;
	
	file_writer.fileLen = *((uint32_t *)(msg + 1));
	memset(file_writer.saveFileName, 0, sizeof(file_writer.saveFileName));
	memcpy(file_writer.saveFileName, msg + 5, fileNameLen);
	utf8_2_gbk(file_writer.saveFileName,fileNameLen);
	memset(file_writer.write_buf, 0, sizeof(file_writer.write_buf));
	if(strlen((const char *)file_writer.saveFileName) > sizeof(saveFilePath))
		return;
	memset(saveFilePath, 0, sizeof(saveFilePath));
	if(gCfgItems.fileSysType == FILE_SYS_SD) {
		if(SD_DET_IP == SD_DETECT_INVERTED) {
			sprintf((char *)saveFilePath, "1:/%s", file_writer.saveFileName);
			f_mount(&fs, (TCHAR const*)SD_Path, 0);
		}
	}
	file_writer.write_index = 0;
	file_writer.file = &save_File;
	lastFragment = -1;

	wifiTransError.flag = 0;
	wifiTransError.start_tick = 0;
	wifiTransError.now_tick = 0;

	f_close(srcfp);

	wifi_delay(1000);
		
	res = f_open(&save_File, (const TCHAR *)saveFilePath, FA_CREATE_ALWAYS | FA_WRITE);

	if(res != FR_OK) {
		clear_cur_ui();
		upload_result = 2;
		wifiTransError.flag = 1;
		wifiTransError.start_tick = getWifiTick();	
		draw_dialog(DIALOG_TYPE_UPLOAD_FILE);
		return;
	}
	wifi_link_state = WIFI_TRANS_FILE;
	upload_result = 1;
	clear_cur_ui();
	draw_dialog(DIALOG_TYPE_UPLOAD_FILE);
	GUI_Exec();
	file_writer.tick_begin = getWifiTick();
}

#define FRAG_MASK	~(1 << 31)

static void file_fragment_msg_handle(uint8_t * msg, uint16_t msgLen) {
	uint32_t num_write;
	uint32_t frag = *((uint32_t *)msg);
	if((frag & FRAG_MASK) != (uint32_t)(lastFragment  + 1)) {
		memset(file_writer.write_buf, 0, sizeof(file_writer.write_buf));
		file_writer.write_index = 0;
		wifi_link_state = WIFI_CONNECTED;	
		upload_result = 2; 
	} else {
		if(write_to_file((char *)msg + 4, msgLen - 4) < 0) {
			memset(file_writer.write_buf, 0, sizeof(file_writer.write_buf));
			file_writer.write_index = 0;
			wifi_link_state = WIFI_CONNECTED;	
			upload_result = 2; 
			return;
		}
		lastFragment = frag;

		if((frag & (~FRAG_MASK)) != 0) {
			FRESULT res =  f_write (file_writer.file, file_writer.write_buf, file_writer.write_index, &num_write);
			if((res != FR_OK) || (num_write != file_writer.write_index)) {
				memset(file_writer.write_buf, 0, sizeof(file_writer.write_buf));
				file_writer.write_index = 0;
				wifi_link_state = WIFI_CONNECTED;	
				upload_result = 2; 
				return;
			}
			memset(file_writer.write_buf, 0, sizeof(file_writer.write_buf));
			file_writer.write_index = 0;
			file_writer.tick_end = getWifiTick();

			upload_file_info.time = getWifiTickDiff(file_writer.tick_begin, file_writer.tick_end) / 1000;
			upload_file_info.size = f_size(&save_File);
			
			wifi_link_state = WIFI_CONNECTED;	

			upload_result = 3; 
		}
	}
}


void esp_data_parser(char *cmdRxBuf, int len) {
	int32_t head_pos;
	int32_t tail_pos;
	uint16_t cpyLen;
	int16_t leftLen = len; //ʣ�೤��
	uint8_t loop_again = 0;

	ESP_PROTOC_FRAME esp_frame;

	while((leftLen > 0) || (loop_again == 1)) {
		loop_again = 0;
		if(esp_msg_index != 0) {
			head_pos = 0;
			cpyLen = (leftLen < (sizeof(esp_msg_buf) - esp_msg_index)) ? leftLen : sizeof(esp_msg_buf) - esp_msg_index;
			
			memcpy(&esp_msg_buf[esp_msg_index], cmdRxBuf + len - leftLen, cpyLen);			

			esp_msg_index += cpyLen;

			leftLen = leftLen - cpyLen;
			tail_pos = charAtArray(esp_msg_buf, esp_msg_index, ESP_PROTOC_TAIL);
			if(tail_pos == -1) {
				if(esp_msg_index >= sizeof(esp_msg_buf)) {
					memset(esp_msg_buf, 0, sizeof(esp_msg_buf));
					esp_msg_index = 0;
				}
				return;
			}
		} else {
			head_pos = charAtArray((uint8_t const *)&cmdRxBuf[len - leftLen], leftLen, ESP_PROTOC_HEAD);
			if(head_pos == -1) {
				return;
			} else {
				memset(esp_msg_buf, 0, sizeof(esp_msg_buf));
				memcpy(esp_msg_buf, &cmdRxBuf[len - leftLen + head_pos], leftLen - head_pos);

				esp_msg_index = leftLen - head_pos;
				leftLen = 0;
				head_pos = 0;
				tail_pos = charAtArray(esp_msg_buf, esp_msg_index, ESP_PROTOC_TAIL);
				if(tail_pos == -1) {
					if(esp_msg_index >= sizeof(esp_msg_buf)) {
						memset(esp_msg_buf, 0, sizeof(esp_msg_buf));
						esp_msg_index = 0;
					}
					return;
				}
			}
		}

		esp_frame.type = esp_msg_buf[1];
		if((esp_frame.type != ESP_TYPE_NET) && (esp_frame.type != ESP_TYPE_GCODE)
			 && (esp_frame.type != ESP_TYPE_FILE_FIRST) && (esp_frame.type != ESP_TYPE_FILE_FRAGMENT)
			 &&(esp_frame.type != ESP_TYPE_WIFI_LIST)) {

			memset(esp_msg_buf, 0, sizeof(esp_msg_buf));
			esp_msg_index = 0;
			return;
		}
		
		esp_frame.dataLen = esp_msg_buf[2] + (esp_msg_buf[3] << 8);
		if(4 + esp_frame.dataLen > sizeof(esp_msg_buf)) {
			memset(esp_msg_buf, 0, sizeof(esp_msg_buf));
			esp_msg_index = 0;
			return;
		}

		if(esp_msg_buf[4 + esp_frame.dataLen] != ESP_PROTOC_TAIL) {
			if(esp_msg_index >= sizeof(esp_msg_buf)) {
				memset(esp_msg_buf, 0, sizeof(esp_msg_buf));
				esp_msg_index = 0;
			}
			return;
		}
		
		esp_frame.data = &esp_msg_buf[4];
		switch(esp_frame.type) {
			case ESP_TYPE_NET:
				net_msg_handle(esp_frame.data, esp_frame.dataLen);
				break;

			case ESP_TYPE_GCODE:
				gcode_msg_handle(esp_frame.data, esp_frame.dataLen);
				break;

			case ESP_TYPE_FILE_FIRST:
				file_first_msg_handle(esp_frame.data, esp_frame.dataLen);
				break;

			case ESP_TYPE_FILE_FRAGMENT:
				file_fragment_msg_handle(esp_frame.data, esp_frame.dataLen);
				break;
			case ESP_TYPE_WIFI_LIST:
				wifi_list_msg_handle(esp_frame.data, esp_frame.dataLen);
				break;
			default:
				break;
				
		}
		esp_msg_index = cut_msg_head(esp_msg_buf, esp_msg_index, esp_frame.dataLen  + 5);
		if(esp_msg_index > 0) {
			if(charAtArray(esp_msg_buf, esp_msg_index,  ESP_PROTOC_HEAD) == -1) {
				memset(esp_msg_buf, 0, sizeof(esp_msg_buf));
				esp_msg_index = 0;
				return;
			}
			if((charAtArray(esp_msg_buf, esp_msg_index,  ESP_PROTOC_HEAD) != -1) && (charAtArray(esp_msg_buf, esp_msg_index, ESP_PROTOC_TAIL) != -1))
				loop_again = 1;
		}
	}
}

void hlk_data_parser(char *cmdRxBuf, int len) { }

int32_t tick_net_time1, tick_net_time2;

static int esp_dma_pre() {
    DMA1_Channel5->CCR &= ~( 1 << 0 ) ; 
    DMA1_Channel5->CMAR = (uint32_t)WifiRxFifo.uartTxBuffer;
    DMA1_Channel5->CNDTR = 0x0000   ;
    DMA1_Channel5->CNDTR = UART_RX_BUFFER_SIZE ;
    DMA1->IFCR = 0xF0000 ;
    DMA1_Channel5->CCR |= 1 << 0 ;
	return 1;
}

static int storeRcvData(uint8_t *bufToCpy, int32_t len) {
	unsigned char tmpW = wifiDmaRcvFifo.write_cur;
	if(len > UDISKBUFLEN)
		return 0;
	
	if(wifiDmaRcvFifo.state[tmpW] == udisk_buf_empty) {
		memcpy((unsigned char *)wifiDmaRcvFifo.bufferAddr[tmpW], (uint8_t *)bufToCpy, len);
		wifiDmaRcvFifo.state[tmpW] = udisk_buf_full;
		wifiDmaRcvFifo.write_cur = (tmpW + 1) % TRANS_RCV_FIFO_BLOCK_NUM;
		return 1;
	} else
		return 0;
}

int32_t readWifiFifo(uint8_t *retBuf, uint32_t bufLen) {
	unsigned char tmpR = wifiDmaRcvFifo.read_cur;
	if(bufLen < UDISKBUFLEN)
		return 0;
	if(wifiDmaRcvFifo.state[tmpR] == udisk_buf_full) {
		memcpy(retBuf, (unsigned char *)wifiDmaRcvFifo.bufferAddr[tmpR], UDISKBUFLEN);
		wifiDmaRcvFifo.state[tmpR] = udisk_buf_empty;
		wifiDmaRcvFifo.read_cur = (tmpR + 1) % TRANS_RCV_FIFO_BLOCK_NUM;
		return UDISKBUFLEN;
	} else
		return 0;
}


void stopEspTransfer() {
	char state;
	if(wifi_link_state == WIFI_TRANS_FILE)
		wifi_link_state = WIFI_CONNECTED;
	f_close(&save_File);
	f_close(&save_File);

	if(upload_result != 3) {
		wifiTransError.flag = 1;
		wifiTransError.start_tick = getWifiTick();
		f_unlink((const char *)saveFilePath);
	}
	wifi_delay(200);
	WIFI_IO1_SET();

	DMA1_Channel5->CCR &= ~( 1 << 0 ) ;
    HAL_DMA_Abort((DMA_HandleTypeDef *)&hdma_usart1_rx);
	HAL_DMA_DeInit((DMA_HandleTypeDef *)&hdma_usart1_rx);
	CLEAR_BIT(huart1.Instance->CR3, USART_CR3_DMAR);
    
	exchangeFlashMode(1);  //change spi flash to use dma mode
	esp_port_begin(1);
	if(wifiTransError.flag != 0x1)
		WIFI_IO1_RESET();
}

void wifi_rcv_handle() {
	int8_t result;
	int32_t len = 0;
	uint8_t ucStr[(UART_RX_BUFFER_SIZE) + 1] = {0};
	uint8_t tmpBuffer1[30];
	int8_t getDataF = 0;
	if(gCfgItems.wifi_type == ESP_WIFI) {
		if(wifi_link_state == WIFI_TRANS_FILE) {
			len = readWifiFifo(ucStr, UART_RX_BUFFER_SIZE);
			if(len > 0) {
				esp_data_parser((char *)ucStr, len);
				if(wifi_link_state == WIFI_CONNECTED) {
					clear_cur_ui();
					draw_dialog(DIALOG_TYPE_UPLOAD_FILE);
					stopEspTransfer();
				}
				getDataF = 1;
			}
			if(esp_state == TRANSFER_STORE) {
				if(storeRcvData((uint8_t *)WifiRxFifo.uartTxBuffer, UART_RX_BUFFER_SIZE)) {
					esp_state = TRANSFERING;
					esp_dma_pre();
					if(wifiTransError.flag != 0x1)
						WIFI_IO1_RESET();
				} else
		            WIFI_IO1_SET();
			}
		} else {
			len = readUsartFifo((SZ_USART_FIFO *)&WifiRxFifo, (int8_t *)ucStr, UART_RX_BUFFER_SIZE);
			if(len > 0) {
				esp_data_parser((char *)ucStr, len);
				if(wifi_link_state == WIFI_TRANS_FILE) { // rcv file first frame
					exchangeFlashMode(0);  //change spi flash not use dma mode
					wifi_delay(10);
					esp_port_begin(0);
					wifi_delay(10);
					tick_net_time1 = 0;
				}
				if(wifiTransError.flag != 0x1)
					WIFI_IO1_RESET();
				getDataF = 1;
			}
			if(need_ok_later &&  (commands_in_queue < BUFSIZE)) {
				need_ok_later = false;
				send_to_wifi("ok\r\n", strlen("ok\r\n"));   
			}
		}
		if(getDataF == 1) {
			tick_net_time1 = getWifiTick();
		} else {
			tick_net_time2 = getWifiTick();
			if(wifi_link_state == WIFI_TRANS_FILE) {
				if((tick_net_time1 != 0) && (getWifiTickDiff(tick_net_time1, tick_net_time2) > 4500)) {// transfer timeout
					wifi_link_state = WIFI_CONNECTED;
					upload_result = 2;
					clear_cur_ui();
					stopEspTransfer();
					draw_dialog(DIALOG_TYPE_UPLOAD_FILE);
				}
			}
			if((tick_net_time1 != 0) && (getWifiTickDiff(tick_net_time1, tick_net_time2) > 10000)) // heart beat timeout
				wifi_link_state = WIFI_NOT_CONFIG;
			if((tick_net_time1 != 0) && (getWifiTickDiff(tick_net_time1, tick_net_time2) > 120000)) { // reset
				wifi_link_state = WIFI_NOT_CONFIG;
				wifi_reset();
				tick_net_time1 = getWifiTick();
			}
		}
		if(wifiTransError.flag == 0x1) {
			wifiTransError.now_tick = getWifiTick();
			if(getWifiTickDiff(wifiTransError.start_tick, wifiTransError.now_tick) > WAIT_ESP_TRANS_TIMEOUT_TICK) {
				wifiTransError.flag = 0;
				WIFI_IO1_RESET();
			}
		}
	}
}

void wifi_looping() {
	IP_PARA *ip_para = &ipPara;
	WIFI_PARA  *wifi_para = &wifiPara;
	char cfg_buf[100];
	if(gCfgItems.wifi_type == ESP_WIFI) {
		do {
		 	wifi_rcv_handle();
		} while(wifi_link_state == WIFI_TRANS_FILE);
	}
}


extern "C" void DMA1_Channel5_IRQHandler(void);

void DMA1_Channel5_IRQHandler() {
    if(__HAL_DMA_GET_FLAG((DMA_HandleTypeDef *)&hdma_usart1_rx, DMA_FLAG_TE5) != RESET) {
		/* Disable the transfer error interrupt */
		__HAL_DMA_DISABLE_IT((DMA_HandleTypeDef *)&hdma_usart1_rx, DMA_IT_TE);
		/* Clear the transfer error flag */
		__HAL_DMA_CLEAR_FLAG((DMA_HandleTypeDef *)&hdma_usart1_rx, DMA_FLAG_TE5);
		/* Update error code */
		SET_BIT(hdma_usart1_rx.ErrorCode, HAL_DMA_ERROR_TE);
		/* Change the DMA state */
		hdma_usart1_rx.State = HAL_DMA_STATE_ERROR;
		/* Process Unlocked */
		__HAL_UNLOCK(&hdma_usart1_rx); 
		if (hdma_usart1_rx.XferErrorCallback != NULL) {
			/* Transfer error callback */
			hdma_usart1_rx.XferErrorCallback((DMA_HandleTypeDef *)&hdma_usart1_rx);
      	}
    }  
	if(__HAL_DMA_GET_FLAG((DMA_HandleTypeDef *)&hdma_usart1_rx, DMA_FLAG_HT5) != RESET) {
		__HAL_DMA_CLEAR_FLAG((DMA_HandleTypeDef *)&hdma_usart1_rx, DMA_FLAG_HT5);
		//GPIO_SetBits(GPIOF, GPIO_Pin_1);	//stm32 clear ready signal before the transfer is finished
		WIFI_IO1_SET();
	}
	if(__HAL_DMA_GET_FLAG((DMA_HandleTypeDef *)&hdma_usart1_rx, DMA_FLAG_TC5)!= RESET) {
		__HAL_DMA_CLEAR_FLAG((DMA_HandleTypeDef *)&hdma_usart1_rx, DMA_FLAG_TC5);
		if(esp_state == TRANSFER_IDLE)
			esp_state = TRANSFERING;

		if(storeRcvData((uint8_t *)WifiRxFifo.uartTxBuffer, UART_RX_BUFFER_SIZE)) {
			esp_dma_pre();
            if(wifiTransError.flag != 0x1)
				WIFI_IO1_RESET();
		} else {
            WIFI_IO1_SET();
			esp_state = TRANSFER_STORE;
		}	
	}
}

void wifi_init() {
	GPIO_InitTypeDef GPIO_InitStruct;
	uint32_t flash_inf_valid_flag = 0;

	wifi_link_state = WIFI_NOT_CONFIG;
	/*Configure GPIO pins : PAPin */
	//__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitStruct.Pin = GPIO_PIN_5;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	WIFI_SET();

	if(gCfgItems.wifi_type == ESP_WIFI) {
		__HAL_RCC_GPIOC_CLK_ENABLE();	 
		GPIO_InitStruct.Pin = GPIO_Pin_7;//
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
		
		__HAL_RCC_GPIOC_CLK_ENABLE();
		__HAL_RCC_AFIO_CLK_ENABLE();
			CLEAR_BIT(BKP->CR, BKP_CR_TPE);
			CLEAR_BIT(BKP->CSR, BKP_CSR_TPIE);
		GPIO_InitStruct.Pin = GPIO_Pin_13;//
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
		WIFI_IO1_SET();
	}
	{
		FRESULT res;
		int update_flag = 0;
		esp_state = TRANSFER_IDLE;
		esp_port_begin(1);		
		wifi_reset();
		res = f_open(&esp_upload.uploadFile, ESP_FIRMWARE_FILE,  FA_OPEN_EXISTING | FA_READ);
		if(res ==  FR_OK) {
			f_close(&esp_upload.uploadFile);
			wifi_delay(2000);
			if(usartFifoAvailable((SZ_USART_FIFO *)&WifiRxFifo) < 20)
				return;
			clear_cur_ui();

			draw_dialog(DIALOG_TYPE_UPDATE_ESP_FIRMARE);
			
			if(wifi_upload(0) >= 0) {
				f_unlink("1:/MKS_WIFI_CUR");
				f_rename(ESP_FIRMWARE_FILE,"/MKS_WIFI_CUR");
			}
			draw_return_ui();
			update_flag = 1;
		}
		if(update_flag == 0) {
			res = f_open(&esp_upload.uploadFile, ESP_WEB_FIRMWARE_FILE,  FA_OPEN_EXISTING | FA_READ);
			if(res ==  FR_OK) {
				f_close(&esp_upload.uploadFile);
				wifi_delay(2000);
				if(usartFifoAvailable((SZ_USART_FIFO *)&WifiRxFifo) < 20)
					return;
				clear_cur_ui();
				draw_dialog(DIALOG_TYPE_UPDATE_ESP_FIRMARE);
				if(wifi_upload(1) >= 0) {
					f_unlink("1:/MKS_WIFI_CUR");
					f_rename(ESP_WEB_FIRMWARE_FILE,"/MKS_WIFI_CUR");
				}
				draw_return_ui();
				update_flag = 1;
			}
		}
		if(update_flag == 0) {
			res = f_open(&esp_upload.uploadFile, ESP_WEB_FILE,  FA_OPEN_EXISTING | FA_READ);
			if(res ==  FR_OK) {
				f_close(&esp_upload.uploadFile);
				wifi_delay(2000);
				if(usartFifoAvailable((SZ_USART_FIFO *)&WifiRxFifo) < 20)
					return;
				clear_cur_ui();
				draw_dialog(DIALOG_TYPE_UPDATE_ESP_DATA);
				if(wifi_upload(2) >= 0) {
					f_unlink("1:/MKS_WEB_CONTROL_CUR");
					f_rename(ESP_WEB_FILE,"/MKS_WEB_CONTROL_CUR");
				}
				draw_return_ui();
			}
		}
	}
	wifiPara.decodeType = WIFI_DECODE_TYPE;
	wifiPara.baud = 115200;
	wifi_link_state = WIFI_NOT_CONFIG;
}

#define BUF_INC_POINTER(p)	((p + 1 == UART_FIFO_BUFFER_SIZE) ? 0:(p + 1))

int usartFifoAvailable(SZ_USART_FIFO *fifo) {
	if(fifo->uart_read_point <= fifo->uart_write_point)
		return fifo->uart_write_point - fifo->uart_read_point;
	else
		return UART_FIFO_BUFFER_SIZE + fifo->uart_write_point - fifo->uart_read_point;
}

int readUsartFifo(SZ_USART_FIFO *fifo, int8_t *buf, int32_t len) {
	int i = 0 ;
	while(i < len ) {
		if(fifo->uart_read_point != fifo->uart_write_point) {
			buf[i] = fifo->uartTxBuffer[fifo->uart_read_point];
			fifo->uart_read_point = BUF_INC_POINTER(fifo->uart_read_point);
			i++;	
		} else
			break;
	}
	return i;
}

int writeUsartFifo(SZ_USART_FIFO *fifo, int8_t *buf, int32_t len) {
	int i = 0 ;
	if((buf == 0) || (len <= 0))
		return -1;
	while(i < len ) {
		if(fifo->uart_read_point != BUF_INC_POINTER(fifo->uart_write_point)) {
			fifo->uartTxBuffer[fifo->uart_write_point] = buf[i] ;
			fifo->uart_write_point = BUF_INC_POINTER(fifo->uart_write_point);
			i++;
		} else
			break;
	}
	return i;
}

void mksWifiIrqHandlerUser() {
	UART_HandleTypeDef *wifi_com;
    uint8_t uartRxData = 0;
    uint8_t i = 0;
	if(gCfgItems.wifi_type == ESP_WIFI) 
		wifi_com = &huart1;
	HAL_UART_IRQHandler(wifi_com);
	writeUsartFifo((SZ_USART_FIFO *)&WifiRxFifo, (int8_t *)&mksUsart1Rx, 1);
	WIFI_IO1_SET();
}

extern "C" void USART1_IRQHandler(void);

void USART1_IRQHandler(void) {
	uint16_t temp;
	__ASM volatile("cpsid i");
	if(huart1.Instance->SR & 0x0020) {
  		if(gCfgItems.wifi_type == ESP_WIFI) 
			mksWifiIrqHandlerUser();
	}
	if(huart1.Instance->SR & 0x000f) {
		huart1.Instance->SR &= 0xffdf;
		temp = huart1.Instance->DR & 0xff;
	}
	__ASM volatile("cpsie i");
}


