#include "../../trash/draw_fan.h"

#include "GUI.h"
#include "BUTTON.h"
#include "PROGBAR.h"
#include "draw_ui.h"
#include "ui_tools.h"
#include "TEXT.h"
#include "Marlin.h"
#include "tim.h"
#include "UI.h"

GUI_HWIN hFanWnd;

static TEXT_Handle textFan;
static BUTTON_Handle buttonAdd, buttonDec, buttonOpen, buttonHalf, buttonOff,  buttonRet, buttonFan;

static void update_fan_state(void);

static void cbFanWin(WM_MESSAGE * pMsg) {
	char buf[30] = {0};
	switch (pMsg->MsgId) {
		case WM_PAINT:
		case WM_TOUCH:
		case WM_TOUCH_CHILD:
			break;
		case WM_NOTIFY_PARENT:
			if(pMsg->Data.v == WM_NOTIFICATION_RELEASED) {
				if(pMsg->hWinSrc == buttonRet) {
					last_disp_state = FAN_UI;
					clear_fan();
					draw_return_ui();
				} else if(pMsg->hWinSrc == buttonOpen) {
					gCfgItems.fanOnoff = 1;
					fanSpeeds[0]=255;
					update_fan_state();
				}
				else if(pMsg->hWinSrc == buttonOff) {
					gCfgItems.fanOnoff = 0;
					gCfgItems.fanSpeed = 0;
					fanSpeeds[0] = 0;
					update_fan_state();
				} else if(pMsg->hWinSrc == buttonHalf) {
					gCfgItems.fanOnoff = 1;
					fanSpeeds[0]=128;
					update_fan_state();
				} else if(pMsg->hWinSrc == buttonAdd) {
					if(fanSpeeds[0]<255) {
						gCfgItems.fanOnoff = 1;
						fanSpeeds[0]++;
					} else {
						fanSpeeds[0]=255;
					}
					update_fan_state();
				}
				else if(pMsg->hWinSrc == buttonDec) {
					if(fanSpeeds[0]>0) {
						gCfgItems.fanOnoff = 1;
						fanSpeeds[0]--;
					} else {
						fanSpeeds[0]=0;
					}
					update_fan_state();
				}                
			}
			break;
		default:
			WM_DefaultProc(pMsg);
	}
}

#define COL(x) ui_std_col(x)
#define COL_T(x) COL(x) + STATE_PIC_X_PIXEL
#define ROW(y) (40 * (y))
#define COL_INDEX 1
#define ROW_INDEX 1
#define TEXT_L() ui_create_std_text(COL_T(COL_INDEX)+10, ROW(ROW_INDEX), 80, STATE_PIC_Y_PIXEL, hFanWnd, 0)
#define BUTTON_L(file) ui_create_state_button(COL(COL_INDEX)+10, ROW(ROW_INDEX), hFanWnd, file);

void draw_fan() {
	fan_ui.show();
	return;

	hFanWnd = ui_std_init_window(FAN_UI, cbFanWin);

	buttonAdd = ui_std_button(0, 0, hFanWnd, "bmp_Add.bin", fan_menu.add);
	buttonDec = ui_std_button(2, 0, hFanWnd, "bmp_Dec.bin", fan_menu.dec);
	buttonOff = ui_std_button(2, 1, hFanWnd, "bmp_speed0.bin", "0%");
	buttonOpen = ui_std_button(0, 1, hFanWnd, "bmp_speed255.bin", "100%");
	buttonHalf = ui_std_button(1, 1, hFanWnd, "bmp_speed127.bin", "50%");
	buttonRet = ui_std_button_return(hFanWnd);
	buttonFan = BUTTON_L(FAN_STATES[0]);
	textFan = TEXT_L();
	ui_update_fan_button(buttonFan, textFan);
}

void update_fan_state(void) {
	MKS_FAN_TIM = fanSpeeds[0]*10000/255;
	ui_update_fan_button(buttonFan, textFan);
}


void refresh_fan() {
	fan_ui.refresh();
	return;
	if (is_ui_timing(F_UI_TIMING_HALF_SEC)) {
		ui_timing_clear(F_UI_TIMING_HALF_SEC);
		ui_update_fan_button(buttonFan, textFan);
	}
}

void clear_fan() {
	fan_ui.hide();
	return;
	ui_drop_window(hFanWnd);
}



