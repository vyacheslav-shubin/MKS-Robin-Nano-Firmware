#include "GUI.h"
#include "BUTTON.h"
#include "draw_zero.h"
#include "draw_ui.h"
#include "ui_tools.h"
#include "fontLib.h"
#include "Marlin.h"

#ifndef GUI_FLASH
#define GUI_FLASH
#endif

static GUI_HWIN hZeroWnd;

static BUTTON_Handle buttonAllZero, buttonXZero, buttonYZero, buttonZZero, buttonRet, buttonDisable;

static void cbZeroWin(WM_MESSAGE * pMsg) {

	switch (pMsg->MsgId) {
		case WM_PAINT:
		case WM_TOUCH:
		case WM_TOUCH_CHILD: break;
			
		case WM_NOTIFY_PARENT:
			if(pMsg->Data.v == WM_NOTIFICATION_RELEASED) {
				if(pMsg->hWinSrc == buttonRet) {
					clear_zero();
					draw_return_ui();
				} else if(pMsg->hWinSrc == buttonAllZero) {
					enqueue_and_echo_commands_P(PSTR("G28"));
				}  else if(pMsg->hWinSrc == buttonXZero) {
					enqueue_and_echo_commands_P(PSTR("G28 X0"));
				} else if(pMsg->hWinSrc == buttonYZero) {
					enqueue_and_echo_commands_P(PSTR("G28 Y0"));
				} else if(pMsg->hWinSrc == buttonZZero) {
					enqueue_and_echo_commands_P(PSTR("G28 Z0"));
				} else if(pMsg->hWinSrc == buttonDisable) {
					enqueue_and_echo_commands_P(PSTR("M84"));
				}
			}
			break;
		default:
			WM_DefaultProc(pMsg);
	}
}


void draw_zero()
{	
	int i;
	ui_push_disp_stack(ZERO_UI);
	ui_clear_screen();
	
	hZeroWnd = ui_std_window(cbZeroWin);

	buttonAllZero = ui_std_button(0, 0, hZeroWnd, "bmp_zero.bin", home_menu.home_all);
	buttonXZero = ui_std_button(1, 0, hZeroWnd, "bmp_zeroX.bin", home_menu.home_x);
	buttonYZero = ui_std_button(2, 0, hZeroWnd, "bmp_zeroY.bin", home_menu.home_y);
	buttonZZero = ui_std_button(3, 0, hZeroWnd, "bmp_zeroZ.bin", home_menu.home_z);
	buttonRet = ui_std_button_return("bmp_return.bin");
	buttonDisable = ui_std_button(0, 1, hZeroWnd, "bmp_function1.bin", set_menu.motoroff);
}

void clear_zero()
{
	ui_drop_window(hZeroWnd);
}


