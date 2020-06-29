#ifndef _WIFI_MODULE_H_
#define _WIFI_MODULE_H_


#undef ESP_MODEL

#define UART_RX_BUFFER_SIZE   1024
#define UART_FIFO_BUFFER_SIZE 1024

#define HLK_WIFI		0x01
#define ESP_WIFI		0x02

#define WIFI_DECODE_TYPE	1				//AUTO

#define IP_DHCP_FLAG	1

#define WIFI_AP_NAME		"TP-LINK_MKS"
#define WIFI_KEY_CODE		"makerbase"
//Clientģʽ�µ�IP����
#define IP_ADDR 	 	"192.168.3.100"
#define IP_MASK 	 	"255.255.255.0"
#define IP_GATE 	 	"192.168.3.1"
#define IP_DNS 	 	 	"192.168.3.1"

//APģʽ�µ�IP����
#define AP_IP_DHCP_FLAG	1
#define AP_IP_ADDR		"192.168.3.100"
#define AP_IP_MASK		"255.255.255.0"
#define AP_IP_GATE		"192.168.3.1"
#define AP_IP_DNS		"192.168.3.1"
#define IP_START_IP		"192.168.3.1"
#define IP_END_IP		"192.168.3.255"
//-------tan 20171008 modify begin------
#if 0
#define WIFI_SET()        GPIO_SetBits(GPIOA, GPIO_Pin_8)
#define WIFI_RESET()      GPIO_ResetBits(GPIOA, GPIO_Pin_8);
#define WIFI_IO1_SET()    GPIO_SetBits(GPIOD, GPIO_Pin_8)      
#define WIFI_IO1_RESET()  GPIO_ResetBits(GPIOD, GPIO_Pin_8)
#endif
#if 0
#define WIFI_SET()        HAL_GPIO_WritePin(GPIOA, GPIO_Pin_5, GPIO_PIN_SET)
#define WIFI_RESET()      HAL_GPIO_WritePin(GPIOA, GPIO_Pin_5, GPIO_PIN_RESET)
//#define WIFI_IO1_SET()    HAL_GPIO_WritePin(GPIOC, GPIO_Pin_6, GPIO_PIN_SET)
//#define WIFI_IO1_RESET()  HAL_GPIO_WritePin(GPIOC, GPIO_Pin_6, GPIO_PIN_RESET);

#define WIFI_IO1_SET()    GPIO_SetBits(GPIOC, GPIO_Pin_7)      
#define WIFI_IO1_RESET()  GPIO_ResetBits(GPIOC, GPIO_Pin_7)
#endif
#if defined(MKS_ROBIN)
#define WIFI_SET()        GPIO_SetBits(GPIOG, GPIO_Pin_1)
#define WIFI_RESET()      GPIO_ResetBits(GPIOG, GPIO_Pin_1)
#define WIFI_IO1_SET()    GPIO_SetBits(GPIOF, GPIO_Pin_1)      
#define WIFI_IO1_RESET()  GPIO_ResetBits(GPIOF, GPIO_Pin_1)
#elif defined(MKS_ROBIN_MINI)||defined(MKS_ROBIN_NANO)
#define WIFI_SET()        GPIO_SetBits(GPIOA, GPIO_Pin_5)
#define WIFI_RESET()      GPIO_ResetBits(GPIOA, GPIO_Pin_5)
#define WIFI_IO1_SET()    GPIO_SetBits(GPIOC, GPIO_Pin_7)
#define WIFI_IO1_RESET()  GPIO_ResetBits(GPIOC, GPIO_Pin_7)
#endif	 

#define UDISKBUFLEN 1024

typedef enum
{
	udisk_buf_empty = 0,
	udisk_buf_full,	
} UDISK_DATA_BUFFER_STATE;

#define TRANS_RCV_FIFO_BLOCK_NUM	8

#define USART1_DR_Addr ((u32)0x40011004)

typedef struct			
{
	unsigned char *bufferAddr[TRANS_RCV_FIFO_BLOCK_NUM];		//����ָ��洢�����ʵ��ַ��Ŀǰ����16K��ͼƬ����buf
	unsigned char *p;	
	UDISK_DATA_BUFFER_STATE state[TRANS_RCV_FIFO_BLOCK_NUM];		
	unsigned char read_cur; //��ǰ��������
	unsigned char write_cur;	//��ǰд������
}WIFI_DMA_RCV_FIFO;			//

//-------tan 20171008 modify endif------

typedef  struct
{
	char ap_name[32];	//wifi���������ַ���
	char keyCode[64]; //wifi�����ַ���
	int   decodeType; //�������ͣ�1��auto��2��aes��
	int baud;
	int mode;
} WIFI_PARA;

typedef  struct
{
	char state;	//������״̬��0x12:������ʹ�ܣ��������ϣ��Ѱ�;0x11:������ʹ�ܣ���������,δ��;0x10��������ʹ�ܣ���δ����;0x00:�����ӽ���
	char hostUrlLen; //�����ӵ�ַ���ȣ����ֵΪ96��
	char hostUrl[96];	//�����ӵ�ַ
	int port;		//�����Ӷ˿�
	char id[21];
} CLOUD_PARA;

typedef struct {
	char  dhcp_flag;	//��̬��ȡIP��־λ����Ϊ1����Ҫ��̬���ã�//Ϊ0������þ�̬����
	unsigned char  ip[4];
	//todo: не нужно
	char  ip_addr[16];	//��̬ip��ַ���飬��192,168,0,23
	char  mask[16];	//��̬�������룬��255,255,255,0
	char  gate[16];	//��̬���أ���192,168,0,1
	char  dns[16];		//dns
	
	char  dhcpd_flag; //apģʽ�£�DHCP����������
	char  dhcpd_ip[16];//DHCP������IP��ַ
	char  dhcpd_mask[16];//DHCP����������
	char  dhcpd_gate[16];//DHCP����������
	char  dhcpd_dns[16];//DHCP������DNS��ַ	
	char  start_ip_addr[16];//��ʼ��ַ
	char  end_ip_addr[16];//������ַ
} IP_PARA;


typedef enum
{
	WIFI_NOT_CONFIG,
	WIFI_CONFIG_MODE,
	WIFI_CONFIG_DHCP,
	WIFI_CONFIG_AP,
	WIFI_CONFIG_IP_INF,
	WIFI_CONFIG_DNS,
	WIFI_CONFIG_TCP,
	WIFI_CONFIG_SERVER,
	WIFI_CONFIG_REMOTE_PORT,
	WIFI_CONFIG_BAUD,
	WIFI_CONFIG_COMMINT,
	WIFI_CONFIG_OK,
	WIFI_GET_IP_OK,
	WIFI_RECONN,
	WIFI_CONNECTED,
	WIFI_WAIT_TRANS_START,
	WIFI_TRANS_FILE,
	WIFI_CONFIG_DHCPD,
	WIFI_COFIG_DHCPD_IP,
	WIFI_COFIG_DHCPD_DNS,
	WIFI_EXCEPTION,
} WIFI_STATE;

typedef struct
{
	char buf[20][80];
	int rd_index;
	int wt_index;
} QUEUE;


typedef enum
{
	WIFI_PARA_SET, 	//0x0:�����������
	WIFI_PRINT_INF, 	//0x1:��ӡ����Ϣ
	WIFI_TRANS_INF, 	//0x2:͸����Ϣ
	WIFI_EXCEP_INF,	 //0x3:�쳣��Ϣ
	WIFI_CLOUD_CFG,	//0x4:�ƶ�����
	WIFI_CLOUD_UNBIND, //0x5�����ID
} WIFI_RET_TYPE;


typedef struct 
{  
  uint32_t uart_read_point;
  uint32_t uart_write_point;
  uint8_t uartTxBuffer[UART_FIFO_BUFFER_SIZE];
} SZ_USART_FIFO;

#define WIFI_GCODE_BUFFER_LEAST_SIZE	96
#define WIFI_GCODE_BUFFER_SIZE	(WIFI_GCODE_BUFFER_LEAST_SIZE * 3)
typedef struct
{
	uint8_t Buffer[WIFI_GCODE_BUFFER_SIZE];
	uint32_t r;
	uint32_t w;
} WIFI_GCODE_BUFFER;

#define WIFI_TOTAL_NUMBER 20
#define WIFI_NAME_BUFFER_SIZE 33

typedef struct {
    char RSSI;
    char name[WIFI_NAME_BUFFER_SIZE];
} WIFI_NETWORK;

typedef struct {
    int8_t count;
    int8_t selected;
    int8_t currentWifipage;
    int8_t getPage;
    WIFI_NETWORK wifi[WIFI_TOTAL_NUMBER];
    //int8_t RSSI[WIFI_TOTAL_NUMBER];
    //uint8_t wifiName[WIFI_TOTAL_NUMBER][WIFI_NAME_BUFFER_SIZE];
    char wifiConnectedName[WIFI_NAME_BUFFER_SIZE];
} WIFI_LIST;

extern WIFI_LIST wifi_list;

extern volatile WIFI_STATE wifi_link_state;
extern WIFI_PARA wifiPara;
extern IP_PARA ipPara;
extern CLOUD_PARA cloud_para;
extern uint8_t wifi_list_received_flag;
extern WIFI_GCODE_BUFFER espGcodeFifo; 

extern uint32_t  getWifiTick();
extern uint32_t  getWifiTickDiff(int32_t lastTick, int32_t  curTick);

void wifi_init();

extern char wifi_loop_time;

extern int cfg_wifi_flag;
extern int cfg_cloud_flag;
extern int send_to_wifi(char *buf, int len);
extern void wifi_looping();
extern int raw_send_to_wifi(char *buf, int len);
extern int package_to_wifi(WIFI_RET_TYPE type, char *buf, int len);
extern void get_wifi_list_command_send();

#endif

