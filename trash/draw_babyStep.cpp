#include "GUI.h"
#include "BUTTON.h"
#include "ui_tools.h"
#include "draw_ui.h"
#include <math.h>
#include <stdint.h>
#include "stdint.h"
#include "Marlin.h"
#include "mks_reprint.h"
#include "UI.h"

static BUTTON_Handle buttonXI, buttonXD, buttonYI, buttonYD, buttonZI, buttonZD, buttonV,  buttonRet;
GUI_HWIN hMoveBabyStepWnd;

void babystep_update_v_button(void);
void babystep_update_z_offset_value(void);

#define BABY_STEP_INFO_COUNT 3

const STEP_INFO baby_step[BABY_STEP_INFO_COUNT] =  {
		{0.01, "bmp_moveStep1.bin"},
		{0.05, "bmp_moveStep2.bin"},
		{0.1, "bmp_moveStep3.bin"}
};

uint8_t current_info = 0;
static uint8_t has_adjust_z = 0; 

static void do_babystep(int direction, char* axe) {
	char baby_buf[30]={0};
	memset(baby_buf,0,sizeof(baby_buf));
	sprintf(baby_buf, "M290 %s%.3f", axe,  direction * baby_step[current_info].step);
	excute_m290(baby_buf);
}

static void cbBabyStepMotorWin(WM_MESSAGE * pMsg) {
	
	char baby_buf[30]={0};

	switch (pMsg->MsgId) {
		case WM_PAINT:
		case WM_TOUCH:
		case WM_TOUCH_CHILD:
			break;

		case WM_NOTIFY_PARENT:
			if(pMsg->Data.v == WM_NOTIFICATION_RELEASED) {
				if(pMsg->hWinSrc == buttonXI) {
					do_babystep(1, "X");
				} else if(pMsg->hWinSrc == buttonXD) {
					do_babystep(-1, "X");
				} else if(pMsg->hWinSrc == buttonYI) {
					do_babystep(1, "Y");
				} else if(pMsg->hWinSrc == buttonYD) {
					do_babystep(-1, "Y");
				} else if(pMsg->hWinSrc == buttonZI) {
					has_adjust_z = 1;
					do_babystep(1, "Z");
				} else if(pMsg->hWinSrc == buttonZD) {
					do_babystep(-1, "Z");
				} else if(pMsg->hWinSrc == buttonRet) {
					if(has_adjust_z == 1)
						excute_m500();
					has_adjust_z = 0;
					last_disp_state = BABY_STEP_UI;
					clear_babyStep();
					draw_return_ui();
				} else if(pMsg->hWinSrc == buttonV) {
					current_info++;
					if (current_info>=BABY_STEP_INFO_COUNT)
						current_info = 0;
					babystep_update_v_button();
				}
			}
			break;
		default:
			WM_DefaultProc(pMsg);
	}
}


void draw_babyStep() {
	babystep_ui.show();
	return;
	hMoveBabyStepWnd = ui_std_init_window(BABY_STEP_UI, cbBabyStepMotorWin);

	buttonXI = ui_std_button(0, 0, hMoveBabyStepWnd, "bmp_xAdd.bin", move_menu.x_add);
	buttonYI = ui_std_button(1, 0, hMoveBabyStepWnd, "bmp_yAdd.bin", move_menu.y_add);
	buttonZI = ui_std_button(2, 0, hMoveBabyStepWnd, "bmp_zAdd.bin", move_menu.z_add);

	buttonXD = ui_std_button(0, 1, hMoveBabyStepWnd, "bmp_xDec.bin", move_menu.x_dec);
	buttonYD = ui_std_button(1, 1, hMoveBabyStepWnd, "bmp_yDec.bin", move_menu.y_dec);
	buttonZD = ui_std_button(2, 1, hMoveBabyStepWnd, "bmp_zDec.bin", move_menu.z_dec);


	buttonV = ui_std_button(3, 0, hMoveBabyStepWnd, 0, 0);
	buttonRet = ui_std_button_return(hMoveBabyStepWnd);

	babystep_update_z_offset_value();
	babystep_update_v_button();
}

extern float zprobe_zoffset; // Initialized by settings.load()

void babystep_update_z_offset_value() {
	char buf[20];
	memset(buf,0,sizeof(buf));
	GUI_SetColor(gCfgItems.background_color);
	GUI_FillRect(290, TITLE_YPOS, LCD_WIDTH, titleHeight);
	GUI_SetColor(gCfgItems.title_color);
	sprintf(buf,"offset Z: %6.3f", zprobe_zoffset);
	GUI_DispStringAt((const char *)buf,290, TITLE_YPOS);
}

void babystep_update_v_button() {
	BUTTON_SetBmpFileName(buttonV, baby_step[current_info].pic,1);
	if(gCfgItems.multiple_language != 0) {
		char buf[20];
		sprintf(buf,"%1.2f", baby_step[current_info].step);
		BUTTON_SetText(buttonV,buf);
	}	
}

void refresh_babyStep() {
	babystep_ui.refresh();
	return;
	if (is_ui_timing(F_UI_TIMING_HALF_SEC)) {
		ui_timing_clear(F_UI_TIMING_HALF_SEC);
		babystep_update_z_offset_value();
	}
}

void clear_babyStep() {
	babystep_ui.hide();
	return;
	ui_drop_window(hMoveBabyStepWnd);
}

