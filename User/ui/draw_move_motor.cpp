#include "draw_move_motor.h"
#include "GUI.h"
#include "BUTTON.h"
#include "draw_ui.h"
#include "ui_tools.h"
#include "draw_print_file.h"
#include "fontLib.h"
#include <math.h>
#include <stdint.h>
#include "stdint.h"
#include "Marlin.h"

#ifndef GUI_FLASH
#define GUI_FLASH
#endif


static BUTTON_Handle buttonXI, buttonXD, buttonYI, buttonYD, buttonZI, buttonZD, buttonV,  buttonRet;
GUI_HWIN hMoveMotorWnd;

void disp_move_dist(void);

static int move_step_index = 0;

struct move_step_info{
	float step;
	char* pic;
};

#define STEPS_COUNT 4

const struct move_step_info move_steps[STEPS_COUNT] = {
		{0.1,"bmp_moveStep1.bin"},
		{1,"bmp_moveStep2.bin"},
		{10, "bmp_moveStep3.bin"},
		{50, "bmp_moveStepHuge.bin"}
};

static void move_motor_exec(int direction, char* axe) {
	char buf[30] = {0};
	enqueue_and_echo_commands_P(PSTR("G91"));
	sprintf(buf, "G1 %s%3.1f F%d\n", axe, move_steps[move_step_index].step * direction, gCfgItems.moveSpeed);
	enqueue_and_echo_commands_P(PSTR(buf));
	enqueue_and_echo_commands_P(PSTR("G90"));
}

static void cbMoveMotorWin(WM_MESSAGE * pMsg) {
	
	char buf[30] = {0};

	switch (pMsg->MsgId) {
		case WM_PAINT:
		case WM_TOUCH:
		case WM_TOUCH_CHILD:
			break;

		case WM_NOTIFY_PARENT:
			if(pMsg->Data.v == WM_NOTIFICATION_RELEASED) {
				if(pMsg->hWinSrc == buttonXI) {
					move_motor_exec(1, "X");
				} else if(pMsg->hWinSrc == buttonXD) {
					move_motor_exec(-1, "X");
				} else if(pMsg->hWinSrc == buttonYI) {
					move_motor_exec(1, "Y");
				} else if(pMsg->hWinSrc == buttonYD) {
					move_motor_exec(-1, "Y");
				} else if(pMsg->hWinSrc == buttonZI) {
					move_motor_exec(1, "Z");
				} else if(pMsg->hWinSrc == buttonZD) {
					move_motor_exec(-1, "Z");
				} else if(pMsg->hWinSrc == buttonRet) {
					feedrate_mm_s = gCfgItems.moveSpeed_bak;
					last_disp_state = MOVE_MOTOR_UI;
					clear_move_motor();
					draw_return_ui();
				} else if(pMsg->hWinSrc == buttonV) {
					move_step_index++;
					if (move_step_index>=STEPS_COUNT)
						move_step_index = 0;
					disp_move_dist();
				}
			}
			break;
		default:
			WM_DefaultProc(pMsg);
	}
}


void draw_move_motor() {
	
  	char buffer_z[15]={0};

	ui_push_disp_stack(MOVE_MOTOR_UI);
	ui_clear_screen();

	GUI_DispStringAt(creat_title_text(),  TITLE_XPOS, TITLE_YPOS);

	hMoveMotorWnd = ui_std_window(cbMoveMotorWin);

	buttonXI = ui_std_button(0, 0, hMoveMotorWnd, "bmp_xAdd.bin", move_menu.x_add);
	buttonYI = ui_std_button(1, 0, hMoveMotorWnd, "bmp_yAdd.bin", move_menu.y_add);
	buttonZI = ui_std_button(2, 0, hMoveMotorWnd, "bmp_zAdd.bin", move_menu.z_add);
	
	buttonXD = ui_std_button(0, 1, hMoveMotorWnd, "bmp_xDec.bin", move_menu.x_dec);
	buttonYD = ui_std_button(1, 1, hMoveMotorWnd, "bmp_yDec.bin", move_menu.y_dec);
	buttonZD = ui_std_button(2, 1, hMoveMotorWnd, "bmp_zDec.bin", move_menu.z_dec);

	buttonV = ui_std_button(3, 0, hMoveMotorWnd, 0, 0);
	buttonRet = ui_std_button_return(hMoveMotorWnd);

	disp_move_dist();
}

void disp_move_dist() {
	char buf[30] = {0};
	BUTTON_SetBmpFileName(buttonV, move_steps[move_step_index].pic ,1);
	if(gCfgItems.multiple_language != 0)
	{
		sprintf(buf, "%3.1f mm", move_steps[move_step_index].step);
		BUTTON_SetText(buttonV,buf);
	}	
}


void clear_move_motor() {
	ui_drop_window(hMoveMotorWnd);
}

