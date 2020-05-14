#include "../../trash/draw_operate.h"

#include "GUI.h"
#include "BUTTON.h"
#include "draw_change_speed.h"
#include "draw_printing_moremenu.h"
#include "PROGBAR.h"
#include "draw_ui.h"
#include "fontLib.h"
#include "LISTBOX.h"
#include "TEXT.h"
#include "UI.h"
#include "mks_cfg.h"

#include "Marlin.h"
#include "mks_reprint.h"
#ifndef GUI_FLASH
#define GUI_FLASH
#endif

extern float feedrate_mm_s;


static GUI_HWIN hOperateWnd, hStopDlgWnd;

extern int8_t curFilePath[30];
extern void reset_tx_fifo();

extern GUI_FLASH const GUI_FONT GUI_FontHZ_fontHz18;
extern int X_ADD, X_INTERVAL;

static PROGBAR_Handle printingBar;

static TEXT_Handle textPrintTemp1, textPrintTemp2, printStopDlgText,Fill_State_BK;
static BUTTON_STRUCT buttonPause, buttonStop, buttonTemperature, buttonSpeed, buttonMore_printing, buttonRet, buttonOk, buttonCancle,buttonFan;
static BUTTON_STRUCT buttonFilament,buttonAuto_Off,buttonExtrude,buttonMove,buttonBabystep;

uint8_t pause_flag;

uint8_t IsChooseAutoShutdown = 0;

//void Autoshutdown_display();

static void cbOperateWin(WM_MESSAGE * pMsg) {

switch (pMsg->MsgId)
{
	case WM_PAINT:
		break;
	case WM_TOUCH:
	 	
		break;
	case WM_TOUCH_CHILD:
		
		break;
		
	case WM_NOTIFY_PARENT:
		#if 1
		if(pMsg->Data.v == WM_NOTIFICATION_RELEASED) {
		    if(pMsg->hWinSrc == buttonRet.btnHandle)
			{
				last_disp_state = OPERATE_UI;
				Clear_operate();
				printing_ui.show();
			}
			else if(pMsg->hWinSrc == buttonTemperature.btnHandle)
			{
				if(mksCfg.extruders==2)
				{
					gCfgItems.curSprayerChoose_bak= active_extruder;
				}			
				last_disp_state = OPERATE_UI;
				Clear_operate();
				preheat_ui.show();
			}
	            #if defined(TFT35)
				else if(pMsg->hWinSrc == buttonFan.btnHandle)
				{
					last_disp_state = OPERATE_UI;
					Clear_operate();
					fan_ui.show();
				}
	            #endif
			else if(pMsg->hWinSrc == buttonFilament.btnHandle)
			{
				if(mksCfg.extruders==2)
				{
					gCfgItems.curSprayerChoose_bak= active_extruder;
					gCfgItems.moveSpeed_bak = feedrate_mm_s;
				}

				if(mksReprint.mks_printer_state == MKS_WORKING)
				{
					stop_print_time();
					last_disp_state = OPERATE_UI;
					//MX_I2C1_Init();
      				card.pauseSDPrint();
      				print_job_timer.pause();					
					mksReprint.mks_printer_state = MKS_PAUSING;				
					Clear_operate();
					gCfgItems.desireSprayerTempBak = thermalManager.target_temperature[active_extruder];
					filament_ui.show();
				}
				else
				{
					last_disp_state = OPERATE_UI;
					Clear_operate();
                    gCfgItems.desireSprayerTempBak = thermalManager.target_temperature[active_extruder];
					filament_ui.show();
				}

			}			
			else if (pMsg->hWinSrc == buttonMore_printing.btnHandle)
			{
				last_disp_state = OPERATE_UI;
				Clear_operate();
				draw_printmore();	
			}
            #if defined(MKS_ROBIN2) ||defined(MKS_ROBIN_NANO)
			else if(pMsg->hWinSrc == buttonSpeed.btnHandle)
			{
				last_disp_state = OPERATE_UI;
				Clear_operate();
				//if(gCfgItems.sprayerNum==2)
				//{
				//	gCfgItems.curSprayerChoose_bak= gCfgItems.curSprayerChoose;
				//}
				draw_changeSpeed();

			}
			else if(pMsg->hWinSrc == buttonExtrude.btnHandle)
			{
				last_disp_state = OPERATE_UI;
				Clear_operate();
				if(mksCfg.extruders==2)
				{
					gCfgItems.curSprayerChoose_bak= active_extruder;
					gCfgItems.moveSpeed_bak = feedrate_mm_s;
				}
				gCfgItems.extruSpeed_bak = feedrate_mm_s;
				filament_ui.show();
			}	
			else if(pMsg->hWinSrc == buttonMove.btnHandle)
			{
				gCfgItems.moveSpeed_bak = feedrate_mm_s;
				
				last_disp_state = OPERATE_UI;
				Clear_operate();

				motor_move_ui.show();
			}
			#if 1
			else if(pMsg->hWinSrc == buttonAuto_Off.btnHandle)
			{
				if(IsChooseAutoShutdown == 1)
				{
						IsChooseAutoShutdown = 0;		
						Autoshutdown_display2();
				}
				else
				{
						IsChooseAutoShutdown = 1;
						Autoshutdown_display2();
				}
			}	
			#endif
			else if(pMsg->hWinSrc == buttonBabystep.btnHandle)
			{
				last_disp_state = OPERATE_UI;
				Clear_operate();
				babystep_ui.show();
			}
			
            #endif
		}
		#endif
		break;
		
	default:
		WM_DefaultProc(pMsg);
	}
}


void draw_operate()
{
	printing_tools_ui.show();
	return;
	int i;

	if(disp_state_stack._disp_state[disp_state_stack._disp_index] != OPERATE_UI)
	{
		disp_state_stack._disp_index++;
		disp_state_stack._disp_state[disp_state_stack._disp_index] = OPERATE_UI;
	}
	
	disp_state = OPERATE_UI;
		
	GUI_SetBkColor(gCfgItems.background_color);
	GUI_SetColor(gCfgItems.title_color);
	GUI_Clear();

	
	GUI_DispStringAt(creat_title_text(), TITLE_XPOS, TITLE_YPOS);
	
	hOperateWnd = WM_CreateWindow(0, titleHeight, LCD_WIDTH, imgHeight, WM_CF_SHOW, cbOperateWin, 0);

	buttonTemperature.btnHandle = BUTTON_CreateEx(INTERVAL_V,0,BTN_X_PIXEL, BTN_Y_PIXEL, hOperateWnd, BUTTON_CF_SHOW, 0, 301);
	buttonFan.btnHandle = BUTTON_CreateEx(BTN_X_PIXEL+INTERVAL_V*2,0,BTN_X_PIXEL, BTN_Y_PIXEL, hOperateWnd, BUTTON_CF_SHOW, 0, 302);
	buttonFilament.btnHandle = BUTTON_CreateEx(BTN_X_PIXEL*2+INTERVAL_V*3,0,BTN_X_PIXEL,BTN_Y_PIXEL, hOperateWnd, BUTTON_CF_SHOW, 0, 303);

	if((mksReprint.mks_printer_state == MKS_PAUSING)||(mksReprint.mks_printer_state == MKS_PAUSED)||(mksReprint.mks_printer_state ==MKS_REPRINTING)||(mksReprint.mks_printer_state ==MKS_REPRINTED))
	{
		buttonSpeed.btnHandle = 0;
		buttonAuto_Off.btnHandle = 0;
		
		buttonExtrude.btnHandle = BUTTON_CreateEx(BTN_X_PIXEL*3+INTERVAL_V*4,0,BTN_X_PIXEL,BTN_Y_PIXEL,hOperateWnd, BUTTON_CF_SHOW, 0, 106);
		buttonMove.btnHandle = BUTTON_CreateEx(INTERVAL_V,BTN_Y_PIXEL+INTERVAL_H,BTN_X_PIXEL, BTN_Y_PIXEL,hOperateWnd, BUTTON_CF_SHOW, 0, 106);		
		buttonMore_printing.btnHandle = BUTTON_CreateEx(BTN_X_PIXEL+INTERVAL_V*2, BTN_Y_PIXEL+INTERVAL_H,BTN_X_PIXEL,BTN_Y_PIXEL,hOperateWnd, BUTTON_CF_SHOW, 0, 302);

		if(gCfgItems.print_finish_close_machine_flg == 1)
		{
			buttonAuto_Off.btnHandle = BUTTON_CreateEx(BTN_X_PIXEL*2+INTERVAL_V*3,BTN_Y_PIXEL+INTERVAL_H,BTN_X_PIXEL,BTN_Y_PIXEL,hOperateWnd, BUTTON_CF_SHOW, 0, 107);
		}

	}
	else //if(mksReprint.mks_printer_state ==MKS_WORKING)
	{
			buttonExtrude.btnHandle = 0;
			buttonMove.btnHandle = 0;
			
			buttonSpeed.btnHandle = BUTTON_CreateEx(BTN_X_PIXEL*3+INTERVAL_V*4,0,BTN_X_PIXEL, BTN_Y_PIXEL,hOperateWnd, BUTTON_CF_SHOW, 0, 106);
			buttonMore_printing.btnHandle = BUTTON_CreateEx(INTERVAL_V, BTN_Y_PIXEL+INTERVAL_H,BTN_X_PIXEL, BTN_Y_PIXEL,hOperateWnd, BUTTON_CF_SHOW, 0, 302);
		
			if(gCfgItems.print_finish_close_machine_flg == 1)
			{
				buttonAuto_Off.btnHandle = BUTTON_CreateEx(BTN_X_PIXEL+INTERVAL_V*2,BTN_Y_PIXEL+INTERVAL_H,BTN_X_PIXEL,BTN_Y_PIXEL,hOperateWnd, BUTTON_CF_SHOW, 0, 302);
			}
			buttonBabystep.btnHandle = BUTTON_CreateEx(BTN_X_PIXEL*2+INTERVAL_V*3,BTN_Y_PIXEL+INTERVAL_H,BTN_X_PIXEL,BTN_Y_PIXEL,hOperateWnd, BUTTON_CF_SHOW, 0, alloc_win_id());
			BUTTON_SetBmpFileName(buttonBabystep.btnHandle, "bmp_mov.bin",1);
			BUTTON_SetBitmapEx(buttonBabystep.btnHandle, 0, &bmp_struct, BMP_PIC_X, BMP_PIC_Y);
			if(gCfgItems.multiple_language != 0)
			{
				BUTTON_SetText(buttonBabystep.btnHandle, operation_menu.babystep);
			}
	}

	
	buttonRet.btnHandle = BUTTON_CreateEx(BTN_X_PIXEL*3+INTERVAL_V*4, BTN_Y_PIXEL+INTERVAL_H,BTN_X_PIXEL, BTN_Y_PIXEL, hOperateWnd, BUTTON_CF_SHOW, 0, 306);

	BUTTON_SetBmpFileName(buttonTemperature.btnHandle, "bmp_temp.bin",1);
	BUTTON_SetBmpFileName(buttonSpeed.btnHandle, "bmp_speed.bin",1);
	BUTTON_SetBmpFileName(buttonFan.btnHandle, "bmp_fan.bin",1);
	BUTTON_SetBmpFileName(buttonFilament.btnHandle, "bmp_filamentchange.bin",1);
	BUTTON_SetBmpFileName(buttonExtrude.btnHandle, "bmp_extrude_opr.bin",1);
	BUTTON_SetBmpFileName(buttonMove.btnHandle, "bmp_move_opr.bin",1);
	BUTTON_SetBmpFileName(buttonMore_printing.btnHandle, "bmp_more_opr.bin",1);
	
	if(gCfgItems.print_finish_close_machine_flg == 1)
	{
		if(IsChooseAutoShutdown == 1)
		{
			BUTTON_SetBmpFileName(buttonAuto_Off.btnHandle, "bmp_auto_off.bin",1);
			
			if(gCfgItems.multiple_language != 0)
			{	
				BUTTON_SetText(buttonAuto_Off.btnHandle,operation_menu.auto_off);
			}

		}
		else
		{
			BUTTON_SetBmpFileName(buttonAuto_Off.btnHandle, "bmp_manual_off.bin",1);
				
			if(gCfgItems.multiple_language != 0)
			{	
				BUTTON_SetText(buttonAuto_Off.btnHandle,operation_menu.manual_off);
			}			
		}
		BUTTON_SetBitmapEx(buttonAuto_Off.btnHandle,0, &bmp_struct, BMP_PIC_X, BMP_PIC_Y);

		
	}
	BUTTON_SetBmpFileName(buttonRet.btnHandle, "bmp_return.bin",1);
	
	BUTTON_SetBitmapEx(buttonTemperature.btnHandle, 0, &bmp_struct, BMP_PIC_X, BMP_PIC_Y);
	BUTTON_SetBitmapEx(buttonSpeed.btnHandle, 0, &bmp_struct, BMP_PIC_X, BMP_PIC_Y);
	BUTTON_SetBitmapEx(buttonFan.btnHandle, 0, &bmp_struct, BMP_PIC_X, BMP_PIC_Y);
	BUTTON_SetBitmapEx(buttonFilament.btnHandle, 0, &bmp_struct, BMP_PIC_X, BMP_PIC_Y);
	BUTTON_SetBitmapEx(buttonExtrude.btnHandle, 0, &bmp_struct, BMP_PIC_X, BMP_PIC_Y);
	BUTTON_SetBitmapEx(buttonMove.btnHandle, 0, &bmp_struct, BMP_PIC_X, BMP_PIC_Y);
	{
		BUTTON_SetBitmapEx(buttonMore_printing.btnHandle, 0, &bmp_struct, BMP_PIC_X, BMP_PIC_Y);
	}
	
	BUTTON_SetBitmapEx(buttonRet.btnHandle, 0, &bmp_struct, BMP_PIC_X, BMP_PIC_Y);
	if(gCfgItems.multiple_language != 0)
	{
		BUTTON_SetText(buttonFan.btnHandle, operation_menu.fan);
		BUTTON_SetText(buttonFilament.btnHandle, operation_menu.filament);
		BUTTON_SetText(buttonTemperature.btnHandle, operation_menu.temp);
		BUTTON_SetText(buttonMore_printing.btnHandle, operation_menu.more);
		BUTTON_SetText(buttonSpeed.btnHandle, operation_menu.speed);
		BUTTON_SetText(buttonExtrude.btnHandle, operation_menu.extr);
		BUTTON_SetText(buttonMove.btnHandle, operation_menu.move);
		BUTTON_SetText(buttonRet.btnHandle, common_menu.text_back);
	}

	
}


void Autoshutdown_display2()
{
	if(IsChooseAutoShutdown == 1)
	{
		BUTTON_SetBmpFileName(buttonAuto_Off.btnHandle, "bmp_auto_off.bin",0);
		BUTTON_SetBitmapEx(buttonAuto_Off.btnHandle, 0, &bmp_struct, BMP_PIC_X, BMP_PIC_Y);
	}
	else
	{
		BUTTON_SetBmpFileName(buttonAuto_Off.btnHandle, "bmp_manual_off.bin",0);
		BUTTON_SetBitmapEx(buttonAuto_Off.btnHandle, 0, &bmp_struct, BMP_PIC_X, BMP_PIC_Y);
	}
	if(gCfgItems.multiple_language != 0)
	{
		if(IsChooseAutoShutdown == 1)
			BUTTON_SetText(buttonAuto_Off.btnHandle,printing_more_menu.auto_close);
		else
			BUTTON_SetText(buttonAuto_Off.btnHandle,printing_more_menu.manual);
	}

}

void Clear_operate()
{
	printing_tools_ui.hide();
	return;
	GUI_SetBkColor(gCfgItems.background_color);
	if(WM_IsWindow(hOperateWnd))
	{
		WM_DeleteWindow(hOperateWnd);
		//GUI_Exec();
	}
	
	//GUI_Clear();
}
