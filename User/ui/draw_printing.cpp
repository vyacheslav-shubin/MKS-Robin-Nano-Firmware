#include "GUI.h"
#include "BUTTON.h"
#include "PROGBAR.h"
#include "CHECKBOX.h"
#include "draw_printing.h"
#include "draw_pre_heat.h"
#include "draw_fan.h"
#include "draw_ui.h"
#include "fontLib.h"
#include "LISTBOX.h"
#include "text.h"
#include "draw_operate.h"
#include "ui_tools.h"
#include "sh_tools.h"
#include "pic_manager.h"

#include "draw_ready_print.h"

#include "draw_print_file.h"
#include "pic.h"
#include "marlin.h"
#include "mks_reprint.h"
#include "stm32f10x_gpio.h"



#ifndef GUI_FLASH
#define GUI_FLASH
#endif

extern FIL *srcfp;

extern volatile uint8_t temper_error_flg;

int8_t curFilePath[30];

static GUI_HWIN hPrintingWnd;
static PROGBAR_Handle printingBar;
static TEXT_Handle printTimeLeft;

static BUTTON_Handle buttonPause, buttonStop, buttonOperat, buttonExt1, buttonExt2, buttonFanstate, buttonBedstate, buttonTime, buttonZpos;
static BUTTON_Handle buttonAutoClose;
static TEXT_Handle E1_Temp, E2_Temp, Fan_Pwm, Bed_Temp,Zpos;

uint8_t print_start_flg = 0;

uint8_t pause_resum=0;


extern uint8_t gcode_preview_over;

extern uint8_t from_flash_pic;
extern uint8_t button_disp_pause_state;

static void update_printing_1s(void);
static void update_pause_button(void);
static void update_auto_close_button(void);


const char* FAN_STATES[3] {"bmp_fan_state0.bin", "bmp_fan_state1.bin", "bmp_fan_state2.bin"};


static void cbPrintingWin(WM_MESSAGE * pMsg) {
	switch (pMsg->MsgId) {
		case WM_PAINT:
		case WM_TOUCH:
		case WM_TOUCH_CHILD:
			break;
		case WM_NOTIFY_PARENT:
			if(pMsg->Data.v == WM_NOTIFICATION_RELEASED) {
				if(pMsg->hWinSrc == buttonOperat) {
					if(gcode_preview_over != 1) {
						last_disp_state = PRINTING_UI;
						clear_printing();
						if((mksReprint.mks_printer_state == MKS_IDLE)  &&  (ui_print_process.rate == 100)) {
							f_close(srcfp);
							reset_file_info();
							draw_ready_print();
						} else {
							draw_operate();
						}
					}
				} else if(pMsg->hWinSrc == buttonPause) {
					if(gcode_preview_over != 1) {
						if(mksReprint.mks_printer_state == MKS_WORKING) {
							stop_print_time();
							if(mksCfg.extruders==2) {
								gCfgItems.curSprayerChoose_bak= active_extruder;
								gCfgItems.moveSpeed_bak = feedrate_mm_s;
							}
							card.pauseSDPrint();
							print_job_timer.pause();
							mksReprint.mks_printer_state = MKS_PAUSING;

						} else if(mksReprint.mks_printer_state == MKS_PAUSED) {
							if (is_filament_fail()) {
								last_disp_state = PRINTING_UI;
								clear_printing();
								draw_dialog(DIALOG_TYPE_FILAMENT_NO_PRESS);
								return;
							} else {
								start_print_time();
								pause_resum = 1;
								mksReprint.mks_printer_state = MKS_RESUMING;//MKS_WORKING;
							}
						} else if(mksReprint.mks_printer_state == MKS_REPRINTING) {
							start_print_time();
							mksReprint.mks_printer_state = MKS_REPRINTED;
						}
						update_pause_button();
					}
				} else if(pMsg->hWinSrc == buttonStop) {
					if(gcode_preview_over != 1) {
						if(mksReprint.mks_printer_state != MKS_IDLE) {
							last_disp_state = PRINTING_UI;
							clear_printing();
							draw_dialog(DIALOG_TYPE_STOP);
						}
					}
				} else if (pMsg->hWinSrc == buttonFanstate) {
					last_disp_state = PRINTING_UI;
					clear_printing();
					draw_fan();
				} else if ((pMsg->hWinSrc == buttonExt1) || (pMsg->hWinSrc == buttonBedstate) || ((pMsg->hWinSrc == buttonExt1) & is_dual_extruders())){
					last_disp_state = PRINTING_UI;
					clear_printing();
					draw_preHeat();
				} else if (pMsg->hWinSrc == buttonAutoClose) {
					ui_print_process.suicide.enabled = ~ui_print_process.suicide.enabled;
					update_auto_close_button();
				}
			}
			break;
		default:
			WM_DefaultProc(pMsg);
	}
}

#define PB_HEIGHT	25
#define SB_OFFSET	(PB_HEIGHT + 10)
#define ROW_SIZE	40
#define COL(x) (200 + 5 + ((120+5) * x))
#define COL_T(x) COL(x) + STATE_PIC_X_PIXEL
#define ROW(y) (SB_OFFSET + (ROW_SIZE*y))
#define TEXT_L(phx, phy) ui_create_std_text(COL_T(phx), ROW(phy), 80, STATE_PIC_Y_PIXEL, hPrintingWnd, 0)
#define TEXT_L100(phx, phy) ui_create_std_text(COL_T(phx), ROW(phy), 100, STATE_PIC_Y_PIXEL, hPrintingWnd, 0)
#define BUTTON_L(phx, phy, file) ui_create_state_button(COL(phx), ROW(phy), hPrintingWnd, file);

static void update_pause_button() {
	const char * fn;
	if(gCfgItems.standby_mode==1 && mksReprint.mks_printer_state == MKS_REPRINTED && button_disp_pause_state==1) {
		fn="bmp_pause100.bin";
	} else {
		if(
				(mksReprint.mks_printer_state == MKS_REPRINTING)
				|| (mksReprint.mks_printer_state == MKS_PAUSING)
				|| (mksReprint.mks_printer_state == MKS_PAUSED)) {
			fn="bmp_resume100.bin";
		} else {
			fn="bmp_pause100.bin";
		}
	}
	BUTTON_SetBmpFileName(buttonPause, fn, 1);
	BUTTON_SetBitmapEx(buttonPause, 0, &bmp_struct_100x80, 0, 0);

}

void update_auto_close_button() {
	const char * fn;
	if (ui_print_process.suicide.enabled) {
		fn = "bmp_autoOffEnabled100.bin";
	} else {
		fn = "bmp_autoOffDisabled100.bin";
	}
    BUTTON_SetBmpFileName(buttonAutoClose, fn, 0);
    BUTTON_SetBitmapEx(buttonAutoClose, 0, &bmp_struct_100x80,0,0);
}


#include "spi_flash.h"
void check_files() {
	uint8_t cnt;
	SPI_FLASH_BufferRead(&cnt,PIC_COUNTER_ADDR,1);
	if(cnt == 0xff)
		cnt = 0;
	SERIAL_ECHOLNPAIR("COUNT:", cnt);

	char buf[20];
	int a = PIC_NAME_ADDR;
	for(uint8_t i = 0;i < cnt;i++) {
		uint8_t j = 0;
		do {
			SPI_FLASH_BufferRead(&buf[j],a++,1);
		} while (buf[j++] != '\0');

		//int sz;
		//SPI_FLASH_BufferRead((char*)&sz,PIC_SIZE_ADDR+i*4,4);
		SERIAL_ECHOLNPAIR("NAME:", buf);
		//SERIAL_ECHOLNPAIR(" SIZE:", sz);
	}
}

void draw_printing() {
	check_files();
	int dual_extrude;
	dual_extrude = is_dual_extruders();

	FRESULT res;
	int i;
	ui_reset_disp_stack(PRINTING_UI);
	ui_clear_screen();
	ui_initialize_screen_gui();

	hPrintingWnd = ui_std_window(cbPrintingWin);

	buttonTime = BUTTON_L(0,0,"bmp_time_state.bin");
	printTimeLeft = TEXT_L(0, 0);

	buttonExt1 = BUTTON_L(0, 1, "bmp_ext1_state.bin");
	E1_Temp = TEXT_L(0, 1);

	if (dual_extrude) {
		buttonExt2 = BUTTON_L(1, 1, "bmp_ext2_state.bin");
		E2_Temp = TEXT_L(1, 1);
	}
	buttonBedstate = BUTTON_L(0, 2, "bmp_bed_state.bin");
	Bed_Temp = TEXT_L(0, 2);

	buttonFanstate = BUTTON_L(1, 2, FAN_STATES[0]);
	Fan_Pwm = TEXT_L(1, 2);

	buttonZpos = BUTTON_L(1, 0, "bmp_zpos_state.bin");
	Zpos = TEXT_L100(1, 0);


	#define _col(ph_x) (INTERVAL_H + (100+INTERVAL_H)*ph_x)

	printingBar = ui_create_std_progbar(COL(0), 0, 270, PB_HEIGHT, hPrintingWnd);

	buttonPause = ui_create_100_80_button(_col(0), 204, hPrintingWnd, 0, 0);
	buttonStop = ui_create_100_80_button(_col(1),204, hPrintingWnd, "bmp_stop100.bin", 0);
	buttonOperat = ui_create_100_80_button(_col(2),204, hPrintingWnd, "bmp_operate100.bin", 0);

	buttonAutoClose = ui_create_100_80_button(_col(3) + 70,204, hPrintingWnd, 0, 0);

    update_auto_close_button();
	update_pause_button();
	update_printing_1s();
}


int get_rate(void) {
	int rate;
	volatile long long rate_tmp_r;
	if(from_flash_pic != 1) {
		rate_tmp_r =(long long) card.sdpos * 100;
		rate = rate_tmp_r / card.filesize;
	} else {
		rate_tmp_r =(long long)card.sdpos;
		rate = (rate_tmp_r-(PREVIEW_SIZE+To_pre_view))* 100 / (card.filesize-(PREVIEW_SIZE+To_pre_view));
	}
	ui_print_process.rate = rate;
	if(rate <= 0)
		rate = 0;
	return rate;
}

void update_progress(int rate) {
	PROGBAR_SetValue(printingBar, rate);
	if (rate!=0) {
		char buf[30] = {0};
		memset(buf, 0, sizeof(buf));
		sprintf(buf, "%d%% - ", rate);

		int total = print_time.seconds + print_time.minutes * 60 + print_time.hours * 3600;
		total = (total * 100 / rate) - total;
		PRINT_TIME pt;
		pt.seconds = total % 60;
		total = total / 60;
		pt.minutes = total % 60;
		pt.hours = total / 60;
		print_time_to_str(&pt, &buf[strlen(buf)]);
		PROGBAR_SetText(printingBar, buf);
	}
}


void do_finish_print(void) {
	stop_print_time();
	flash_preview_begin = 0;
	default_preview_flg = 0;
	clear_printing();
	draw_dialog(DIALOG_TYPE_FINISH_PRINT);
}

extern float zprobe_zoffset; // Initialized by settings.load()

void update_printing_1s(void) {
	char buf[30] = {0};
	memset(buf, 0, sizeof(buf));
	sprintf(buf, "%d/%d°", (int)thermalManager.current_temperature[0], (int)thermalManager.target_temperature[0]);
	ui_set_text_value(E1_Temp, buf);
	if(is_dual_extruders()){
		memset(buf,0,sizeof(buf));
		sprintf(buf, "%d/%d°", (int)thermalManager.current_temperature[1], (int)thermalManager.target_temperature[1]);
		ui_set_text_value(E2_Temp, buf);
	}

	memset(buf, 0, sizeof(buf));
	sprintf(buf, "%d/%d°", (int)thermalManager.current_temperature_bed,  (int)thermalManager.target_temperature_bed);
	ui_set_text_value(Bed_Temp, buf);


	memset(buf, 0, sizeof(buf));
	print_time_to_str(&print_time, buf);
	ui_set_text_value(printTimeLeft, buf);

	memset(buf, 0, sizeof(buf));
	if (abs(zprobe_zoffset)<0.001) {
		sprintf(buf,"%.3f",current_position[Z_AXIS]);
	} else {
		sprintf(buf,"%.2f/%.2f",current_position[Z_AXIS], zprobe_zoffset);
	}
	ui_set_text_value(Zpos, buf);

	memset(buf, 0, sizeof(buf));

	long fs = fanSpeeds[0] * 100;
	uint8_t pr=fs/255;
	if ((pr==0) && (fanSpeeds[0]>0))
		pr = 1;
	sprintf(buf, "%d%%", pr);
	ui_set_text_value(Fan_Pwm, buf);

}


void refresh_printing() {
	static uint8_t fan_state = 0;
	if (is_ui_timing(F_UI_TIMING_HALF_SEC)) {
		ui_timing_clear(F_UI_TIMING_HALF_SEC);
		if (fanSpeeds[0]>1) {
			fan_state++;
			if (fan_state>2)
				fan_state = 0;
			ui_update_state_button(buttonFanstate, FAN_STATES[fan_state]);
		}
	}
	if (is_ui_timing(F_UI_TIMING_SEC)) {
		ui_timing_clear(F_UI_TIMING_SEC);

		int rate = get_rate();

		if((mksReprint.mks_printer_state == MKS_IDLE) && (rate == 100)) {
			do_finish_print();
		} else {
			update_printing_1s();
			update_progress(rate);
		}
	}
}

void clear_printing()
{
	ui_drop_window(hPrintingWnd);
}
