#include "../../trash/draw_change_speed.h"

#include "draw_ui.h"
#include "id_manage.h"
#include "Marlin.h"
#include "UI.h"
#include "planner.h"
#include "ui_tools.h"

#define STEPS_COUNT 3

const STEP_INFO percent_steps[STEPS_COUNT] = {
		{1, img_percent_step1},
		{5, img_percent_step5},
		{10, img_percent_step10},
};

static uint8_t current_percent_step;

GUI_HWIN hChangeSpeedWnd;
static TEXT_Handle textPrintSpeed,textSpeedVal;
static uint8_t speedType = 0;
static BUTTON_STRUCT buttonInc, buttonDec, buttonStep, buttonMove, buttonExtruct, buttonRet;

uint8_t move_speed_flg;

static void cbChangeSpeedWin(WM_MESSAGE * pMsg) {
	char buf[30] = {0};
	switch (pMsg->MsgId) {
		case WM_PAINT:
		case WM_TOUCH:
		case WM_TOUCH_CHILD:
			break;
			
		case WM_NOTIFY_PARENT:
			if(pMsg->Data.v == WM_NOTIFICATION_RELEASED) {
				if(pMsg->hWinSrc == buttonRet.btnHandle) {
					Clear_changeSpeed();
					draw_return_ui();
				} else if(pMsg->hWinSrc == buttonInc.btnHandle) {
					if(speedType == 0) {
						move_speed_flg = 1;
						if(feedrate_percentage  < MAX_EXT_SPEED_PERCENT - percent_steps[current_percent_step].step) {
							feedrate_percentage += percent_steps[current_percent_step].step;
						} else {
							feedrate_percentage = MAX_EXT_SPEED_PERCENT;
						}
					} else if(speedType == 1) {
						if(planner.flow_percentage[0]< MAX_EXT_SPEED_PERCENT - percent_steps[current_percent_step].step) {
							planner.flow_percentage[0] += percent_steps[current_percent_step].step;
							
						} else {
							planner.flow_percentage[0] = MAX_EXT_SPEED_PERCENT;
						}
                        planner.e_factor[0]= planner.flow_percentage[0]*0.01;
						planner.flow_percentage[1] = planner.flow_percentage[0];
                        planner.e_factor[1]= planner.flow_percentage[1]*0.01;
					}
					disp_print_speed();
				}
				else if(pMsg->hWinSrc == buttonDec.btnHandle) {
					if(speedType == 0) {
						if(feedrate_percentage > MIN_EXT_SPEED_PERCENT + percent_steps[current_percent_step].step) {
							feedrate_percentage -= percent_steps[current_percent_step].step;
						} else {
							feedrate_percentage = MIN_EXT_SPEED_PERCENT;
						}
					} else if(speedType == 1) {
						if(planner.flow_percentage[0] > MIN_EXT_SPEED_PERCENT + percent_steps[current_percent_step].step) {
							planner.flow_percentage[0] -= percent_steps[current_percent_step].step;
						} else {
							planner.flow_percentage[0] = MIN_EXT_SPEED_PERCENT;
						}
                        planner.e_factor[0]= planner.flow_percentage[0]*0.01;
						planner.flow_percentage[1] = planner.flow_percentage[0];
                        planner.e_factor[1]= planner.flow_percentage[1]*0.01;
					}
					disp_print_speed();
				} else if(pMsg->hWinSrc == buttonStep.btnHandle) {
					current_percent_step++;
					if (current_percent_step>=STEPS_COUNT)
						current_percent_step=0;
					disp_step_speed();
				} else if(pMsg->hWinSrc == buttonMove.btnHandle) {
					speedType = 0;
					disp_speed_type();
					disp_print_speed();
				} else if(pMsg->hWinSrc == buttonExtruct.btnHandle) {
					speedType = 1;
					disp_speed_type();
					disp_print_speed();
				}
				
			}
			break;
			
		default:
			WM_DefaultProc(pMsg);
		}
	}


void draw_changeSpeed() {
	speed_ui.show();
	return;
	hChangeSpeedWnd = ui_std_init_window(CHANGE_SPEED_UI, cbChangeSpeedWin);
	
	buttonInc.btnHandle = ui_std_button(0, 0, hChangeSpeedWnd, img_plus, speed_menu.add);
	buttonDec.btnHandle = ui_std_button(3, 0, hChangeSpeedWnd, img_minus, speed_menu.dec);
	buttonRet.btnHandle = ui_std_button_return(hChangeSpeedWnd);
	
	buttonMove.btnHandle = BUTTON_CreateEx(INTERVAL_V,  BTN_Y_PIXEL+INTERVAL_H,BTN_X_PIXEL, BTN_Y_PIXEL, hChangeSpeedWnd, BUTTON_CF_SHOW, 0, alloc_win_id());
	buttonExtruct.btnHandle = BUTTON_CreateEx(BTN_X_PIXEL+INTERVAL_V*2,  BTN_Y_PIXEL+INTERVAL_H,BTN_X_PIXEL, BTN_Y_PIXEL, hChangeSpeedWnd, BUTTON_CF_SHOW, 0, alloc_win_id());
	buttonStep.btnHandle = BUTTON_CreateEx(BTN_X_PIXEL*2+INTERVAL_V*3, BTN_Y_PIXEL+INTERVAL_H,BTN_X_PIXEL, BTN_Y_PIXEL, hChangeSpeedWnd, BUTTON_CF_SHOW, 0, alloc_win_id());

	textPrintSpeed= TEXT_CreateEx(BTN_X_PIXEL+INTERVAL_V*2,(BTN_Y_PIXEL-60)/2,BTN_X_PIXEL*2+INTERVAL_V,30, hChangeSpeedWnd, WM_CF_SHOW, GUI_TA_VCENTER|GUI_TA_HCENTER, alloc_win_id(),  " ");
	textSpeedVal= TEXT_CreateEx(BTN_X_PIXEL+INTERVAL_V*2,(BTN_Y_PIXEL-60)/2+30,BTN_X_PIXEL*2+INTERVAL_V,30, hChangeSpeedWnd, WM_CF_SHOW, GUI_TA_VCENTER|GUI_TA_HCENTER, alloc_win_id(),  " ");

	
	BUTTON_SetBitmapEx(buttonMove.btnHandle,0,&bmp_struct,BMP_PIC_X, BMP_PIC_Y);
	BUTTON_SetBitmapEx(buttonExtruct.btnHandle,0,&bmp_struct,BMP_PIC_X, BMP_PIC_Y);
	BUTTON_SetBitmapEx(buttonStep.btnHandle,0,&bmp_struct,BMP_PIC_X, BMP_PIC_Y);
	BUTTON_SetBitmapEx(buttonRet.btnHandle,0,&bmp_struct,BMP_PIC_X, BMP_PIC_Y);

	disp_print_speed();
	disp_speed_type();
	disp_step_speed();
}

void Clear_changeSpeed() {
	speed_ui.hide();
	return;
	ui_drop_window(hChangeSpeedWnd);
}

void disp_print_speed()
{

	char buf[30] = {0};
	char buf1[30] = {0};

	TEXT_SetTextColor(textPrintSpeed, gCfgItems.title_color);
	TEXT_SetBkColor(textPrintSpeed, gCfgItems.background_color);
	TEXT_SetTextColor(textSpeedVal, gCfgItems.title_color);
	TEXT_SetBkColor(textSpeedVal, gCfgItems.background_color);	
	
	if(speedType ==  0)	//move
	{
		sprintf(buf1, "%s",speed_menu.move_speed);
		sprintf(buf, "%d%%", feedrate_percentage);
		
	}
	else if(speedType ==  1)	// e1
	{
		sprintf(buf1, "%s",speed_menu.extrude_speed);	
		sprintf(buf, "%d%%", planner.flow_percentage[0]);	
	}
	TEXT_SetText(textPrintSpeed, buf1);
	TEXT_SetText(textSpeedVal, buf);

}

void disp_speed_type()
{
	switch(speedType)
	{
		case 1:
			BUTTON_SetBmpFileName(buttonExtruct.btnHandle, "bmp_extruct_sel.bin",1);	
			BUTTON_SetBmpFileName(buttonMove.btnHandle, "bmp_mov_changeSpeed.bin",1);			
			BUTTON_SetBitmapEx(buttonExtruct.btnHandle, 0, &bmp_struct, BMP_PIC_X, BMP_PIC_Y);
			BUTTON_SetBitmapEx(buttonMove.btnHandle, 0, &bmp_struct, BMP_PIC_X, BMP_PIC_Y);
							
			break;

		default:
			BUTTON_SetBmpFileName(buttonMove.btnHandle, "bmp_mov_sel.bin",1);
			BUTTON_SetBmpFileName(buttonExtruct.btnHandle, "bmp_speed_extruct.bin",1);
			BUTTON_SetBitmapEx(buttonMove.btnHandle, 0, &bmp_struct, BMP_PIC_X, BMP_PIC_Y);
			BUTTON_SetBitmapEx(buttonExtruct.btnHandle, 0, &bmp_struct, BMP_PIC_X, BMP_PIC_Y);		
			break;
	}
		
	if(gCfgItems.multiple_language != 0)
	{
		BUTTON_SetText(buttonExtruct.btnHandle, speed_menu.extrude);
		BUTTON_SetText(buttonMove.btnHandle, speed_menu.move);	
	}

}

void disp_step_speed() {
	BUTTON_SetBmpFileName(buttonStep.btnHandle, percent_steps[current_percent_step].pic, 1);
	sprintf(ui_buf1_20, "%.0f%%", percent_steps[current_percent_step].step);
	BUTTON_SetText(buttonStep.btnHandle, ui_buf1_20);
}


