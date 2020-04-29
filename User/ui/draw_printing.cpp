#include "GUI.h"
#include "BUTTON.h"
#include "PROGBAR.h"
#include "CHECKBOX.h"
#include "draw_printing.h"
#include "draw_ui.h"
#include "fontLib.h"
#include "LISTBOX.h"
#include "text.h"
#include "draw_operate.h"
#include "ui_tools.h"
#include "sh_tools.h"
#include "pic_manager.h"

#include "draw_ready_print.h"
//#include "others.h"
//#include "mks_tft_com.h"

#include "draw_print_file.h"
#include "pic.h"
#include "marlin.h"
#include "mks_reprint.h"
#include "stm32f10x_gpio.h"



#ifndef GUI_FLASH
#define GUI_FLASH
#endif
extern GUI_CONST_STORAGE GUI_FONT GUI_FontD36x48;

extern FIL *srcfp;

extern volatile uint8_t temper_error_flg;

int8_t curFilePath[30];


static GUI_HWIN hPrintingWnd;
static PROGBAR_Handle printingBar;
static TEXT_Handle printTimeLeft;

static BUTTON_Handle buttonPause, buttonStop, buttonOperat, buttonExt1, buttonExt2, buttonFanstate, buttonBedstate, buttonTime, buttonZpos;
static BUTTON_Handle buttonAutoClose;
static TEXT_Handle E1_Temp, E2_Temp, Fan_Pwm, Bed_Temp,Zpos, textSpeed;
static CHECKBOX_Handle auto_close;

extern FILE_PRINT_STATE gCurFileState;

uint8_t print_start_flg = 0;

uint8_t pause_resum=0;


extern uint8_t gcode_preview_over;

extern uint8_t from_flash_pic;
extern uint8_t button_disp_pause_state;

void update_printing_1s(void);
void update_pause_button(void);
void update_auto_close_button(void);

static void cbPrintingWin(WM_MESSAGE * pMsg) {

switch (pMsg->MsgId)
{
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
					if((mksReprint.mks_printer_state == MKS_IDLE)  &&  (gCurFileState.totalSend == 100)) {
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

void reset_file_info()
{
	gCurFileState.fileSize = 0;
	gCurFileState.totalRead = 0;
	gCurFileState.totalSend = 0;
	gCurFileState.bytesHaveRead = 0;
	gCurFileState.bufPoint = 0;
	gCurFileState.file_open_flag = 0;
}

#define PB_HEIGHT	25
#define SB_OFFSET	(PB_HEIGHT + 10)
#define ROW_SIZE	40
#define COL(x) (200 + 5 + ((140+5) * x))
#define COL_T(x) COL(x) + STATE_PIC_X_PIXEL
#define ROW(y) (SB_OFFSET + (ROW_SIZE*y))
#define TEXT_L(phx, phy) ui_create_std_text(COL_T(phx), ROW(phy), 80, STATE_PIC_Y_PIXEL, hPrintingWnd, 0)
#define BUTTON_L(phx, phy, file) ui_create_state_button(COL(phx), ROW(phy), hPrintingWnd, file);

static void update_pause_button() {
	char * fn;
	char * tn;
	if(gCfgItems.standby_mode==1 && mksReprint.mks_printer_state == MKS_REPRINTED && button_disp_pause_state==1) {
		fn="bmp_pause.bin";
		tn=printing_menu.pause;
	} else {
		if(
				(mksReprint.mks_printer_state == MKS_REPRINTING)
				|| (mksReprint.mks_printer_state == MKS_PAUSING)
				|| (mksReprint.mks_printer_state == MKS_PAUSED)) {
			fn="bmp_resume.bin";
			tn=printing_menu.resume;
		} else {
			fn="bmp_pause.bin";
			tn=printing_menu.pause;
		}
	}
	BUTTON_SetBmpFileName(buttonPause, fn, 1);
	if(gCfgItems.multiple_language != 0)
		BUTTON_SetText(buttonPause, tn);
}

void update_auto_close_button() {
    BUTTON_SetBitmapEx(buttonAutoClose, 0, &bmp_struct90X30,0,5);
    BUTTON_SetTextAlign(buttonAutoClose, GUI_TA_CENTER|GUI_TA_VCENTER );
	if (ui_print_process.suicide.enabled) {
	    BUTTON_SetBmpFileName(buttonAutoClose, "bmp_enable.bin",1);
	    BUTTON_SetText(buttonAutoClose, machine_menu.high_level);
	} else {
	    BUTTON_SetBmpFileName(buttonAutoClose, "bmp_disable.bin",1);
	    BUTTON_SetText(buttonAutoClose, machine_menu.low_level);
	}
}

#define is_dual_extruders() (mksCfg.extruders == 2 && gCfgItems.singleNozzle == 0)
//#define is_dual_extruders() (1)



void draw_printing()
{

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

	buttonFanstate = BUTTON_L(1, 2, "bmp_fan_state.bin");
	Fan_Pwm = TEXT_L(1, 2);

	buttonZpos = BUTTON_L(1, 0, "bmp_zpos_state.bin");
	Zpos = TEXT_L(1, 0);


	#define _col(ph_x) (INTERVAL_H + (150+INTERVAL_H)*ph_x)

	printingBar = ui_create_std_progbar(COL(0), 0, 270, PB_HEIGHT, hPrintingWnd);

	buttonPause = ui_create_150_80_button(_col(0), 204, hPrintingWnd, 0, 0);
	buttonStop = ui_create_150_80_button(_col(1),204, hPrintingWnd, "bmp_stop.bin", printing_menu.stop);
	buttonOperat = ui_create_150_80_button(_col(2),204, hPrintingWnd, "bmp_operate.bin", printing_menu.option);

	buttonAutoClose = BUTTON_CreateEx(COL(0), ROW(3), 90, 40, hPrintingWnd, BUTTON_CF_SHOW, 0, alloc_win_id());
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
	gCurFileState.totalSend = rate;
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

void update_printing_1s(void) {
	char buf[30] = {0};
	memset(buf, 0, sizeof(buf));
	sprintf(buf, printing_menu.temp1, (int)thermalManager.current_temperature[0], (int)thermalManager.target_temperature[0]);
	ui_set_text_value(E1_Temp, buf);
	if(is_dual_extruders()){
		memset(buf,0,sizeof(buf));
		sprintf(buf, printing_menu.temp2, (int)thermalManager.current_temperature[1], (int)thermalManager.target_temperature[1]);
		ui_set_text_value(E2_Temp, buf);
	}

	memset(buf, 0, sizeof(buf));
	sprintf(buf, printing_menu.bed_temp, (int)thermalManager.current_temperature_bed,  (int)thermalManager.target_temperature_bed);
	ui_set_text_value(Bed_Temp, buf);


	memset(buf, 0, sizeof(buf));
	print_time_to_str(&print_time, buf);
	ui_set_text_value(printTimeLeft, buf);

	memset(buf, 0, sizeof(buf));
	sprintf(buf,"%.3f",current_position[Z_AXIS]);
	ui_set_text_value(Zpos, buf);

	memset(buf, 0, sizeof(buf));
	sprintf(buf, "%3d", fanSpeeds[0]);
	ui_set_text_value(Fan_Pwm, buf);

}

void refresh_printing() {
	if (is_ui_timing(F_UI_TIMING_SEC)) {
		ui_timing_clear(F_UI_TIMING_SEC);

		int rate = get_rate();

		//TODO: убрать отсюда
		gCurFileState.totalSend = rate;

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
