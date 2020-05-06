#include "GUI.h"
#include "BUTTON.h"
#include "draw_ui.h"
#include "ui_tools.h"
#include "draw_ready_print.h"
#include "pic_manager.h"
#include "draw_print_file.h"
#include "draw_move_motor.h"
//#include "printer.h"
#include "draw_pre_heat.h"
#include "draw_extrusion.h"
#include "draw_zero.h"
#include "draw_set.h"
#include "draw_more.h"
#include "draw_fan.h"
#include "draw_more.h"
#include "draw_log_ui.h"
#include "draw_language.h"
#include "spi_flash.h"
#include "draw_manual_leveling.h"
#include "draw_tool.h"
#include "draw_meshleveling.h"
#include "marlin.h"
#include "temperature.h"
#ifndef GUI_FLASH
#define GUI_FLASH
#endif

static BUTTON_Handle  buttonTool, buttonPreHeat, buttonPrint, buttonExtruder, buttonMove, buttonFan, buttonMore, buttonZero, buttonSet,buttonLeveling;

static GUI_HWIN hReadyPrintWnd;
static TEXT_Handle textReadyTemp1,textReadyTemp2;
static TEXT_Handle textReadyBedTemp,textReadyFanSpeed;

int X_ADD=5,X_INTERVAL=10;   //**ͼƬ���
extern char cmd_code[201];
extern char x[1];
extern volatile char *codebufpoint;
extern volatile uint8_t  get_temp_flag;
extern uint8_t  Get_Temperature_Flg;
extern uint8_t leveling_first_time;

extern uint8_t disp_in_file_dir;


extern int8_t curSprayerTemp[10];
extern int8_t curBedTemp[10];

#define RETRO_ICON_INTEVAL  (LCD_WIDTH-4*50)/5
	


static void cbReadPrintWin(WM_MESSAGE * pMsg) {
	
	switch (pMsg->MsgId) {
		case WM_PAINT:
			break;
		case WM_NOTIFY_PARENT:
			if(pMsg->Data.v == WM_NOTIFICATION_RELEASED) {
				if(pMsg->hWinSrc == buttonPrint) {
					disp_in_file_dir = 1;
					last_disp_state = PRINT_READY_UI;
					Clear_ready_print();
					draw_print_file();
				} else if(pMsg->hWinSrc == buttonMove) {
					gCfgItems.getzpos_flg = 1;
					last_disp_state = PRINT_READY_UI;
					Clear_ready_print();
					draw_move_motor();			
				} else if(pMsg->hWinSrc == buttonPreHeat) {
					last_disp_state = PRINT_READY_UI;
					Clear_ready_print();
					draw_preHeat();
				} else if(pMsg->hWinSrc == buttonExtruder) {
					gCfgItems.extruSpeed_bak = feedrate_mm_s;
					last_disp_state = PRINT_READY_UI;
					Clear_ready_print();
					draw_extrusion();
				} else if(pMsg->hWinSrc == buttonZero) {
					last_disp_state = PRINT_READY_UI;
					Clear_ready_print();
					draw_zero();
				} else if(pMsg->hWinSrc == buttonSet) {
					last_disp_state = PRINT_READY_UI;
					Clear_ready_print();
					draw_Set();
				} else if(pMsg->hWinSrc == buttonMore) {
					last_disp_state = PRINT_READY_UI;
					Clear_ready_print();
					draw_More();
				} else if(pMsg->hWinSrc == buttonTool) {
					last_disp_state = PRINT_READY_UI;
					Clear_ready_print();
					draw_tool();
				} else if(pMsg->hWinSrc == buttonLeveling) {
					if(gCfgItems.leveling_mode == 1) {
						if(BED_LEVELING_METHOD & MESH_BED_LEVELING) {
                            last_disp_state = PRINT_READY_UI;
                            Clear_ready_print();
                            draw_meshleveling();
                        } else {
						    SPI_FLASH_BufferRead((u8 *)cmd_code,BUTTON_AUTOLEVELING_ADDR,201);
						    codebufpoint = cmd_code;		
                        }
					} else {
						leveling_first_time = 1;
						last_disp_state = PRINT_READY_UI;
						Clear_ready_print();
						draw_leveling();
					}
				}
			}
			break;
		default:
			WM_DefaultProc(pMsg);
	}
}





void draw_ready_print()
{
	int i;

	ui_reset_disp_stack(PRINT_READY_UI);
	ui_init_page();
	hReadyPrintWnd = ui_std_window(cbReadPrintWin);
	
	if(gCfgItems.display_style == 0) {
		buttonPreHeat = ui_std_button(0, 0, hReadyPrintWnd, "bmp_preHeat.bin", main_menu.preheat);
		buttonMove = 	ui_std_button(1, 0, hReadyPrintWnd, "bmp_mov.bin", main_menu.move);
		buttonZero = ui_std_button(2, 0, hReadyPrintWnd, "bmp_zero.bin", main_menu.home);
		buttonPrint = ui_std_button(3, 0, hReadyPrintWnd, "bmp_printing.bin", main_menu.print);

		buttonExtruder = ui_std_button(0, 1, hReadyPrintWnd, "bmp_extruct.bin", main_menu.extrude);

		uint8_t next_button_offset = 1;
		switch(gCfgItems.leveling_mode) {
		case 0:
			buttonSet  = ui_std_button(1, 1, hReadyPrintWnd, "bmp_leveling.bin",  main_menu.leveling);
			break;
		case 1:
			buttonSet  = ui_std_button(1, 1, hReadyPrintWnd, "bmp_autoleveling.bin",  main_menu.autoleveling);
			break;
		default:
			next_button_offset = 0;
			break;
		}
		buttonSet  = ui_std_button(1+next_button_offset, 1, hReadyPrintWnd, "bmp_set.bin",  main_menu.set);
		buttonMore = ui_std_button(2+next_button_offset, 1, hReadyPrintWnd, "bmp_more.bin",  main_menu.more);

	} else {
		buttonTool = ui_create_std_button(SIMPLE_FIRST_PAGE_GRAP+1,(LCD_HEIGHT-BTN_Y_PIXEL)/2-titleHeight, hReadyPrintWnd, "bmp_tool.bin", main_menu.tool);
		buttonSet = ui_create_std_button(BTN_X_PIXEL+SIMPLE_FIRST_PAGE_GRAP*2+1,(LCD_HEIGHT-BTN_Y_PIXEL)/2-titleHeight, hReadyPrintWnd, "bmp_set.bin", main_menu.set);
		buttonPrint = ui_create_std_button(BTN_X_PIXEL*2+SIMPLE_FIRST_PAGE_GRAP*3+1,(LCD_HEIGHT-BTN_Y_PIXEL)/2-titleHeight,hReadyPrintWnd, "bmp_printing.bin", main_menu.print);
	}
}

void disp_restro_state()
{
	char buf[30] = {0};

	memset(buf, 0, sizeof(buf));
	sprintf(buf, printing_menu.temp1, (int)thermalManager.current_temperature[0], (int)thermalManager.target_temperature[0]);
	TEXT_SetText(textReadyTemp1, buf);	
	
	if(mksCfg.extruders==2)
	{
		memset(buf,0,sizeof(buf));
		sprintf(buf, printing_menu.temp2, (int)thermalManager.current_temperature[1], (int)thermalManager.target_temperature[1]);
		TEXT_SetText(textReadyTemp2, buf);
	}
	if(mksCfg.has_temp_bed == 1)
        {
        	memset(buf, 0, sizeof(buf));
		sprintf(buf, printing_menu.bed_temp, (int)thermalManager.current_temperature_bed,  (int)thermalManager.target_temperature_bed);
		TEXT_SetText(textReadyBedTemp, buf);
        }
	memset(buf, 0, sizeof(buf));
	sprintf(buf, "%3d", fanSpeeds[0]);
	TEXT_SetText(textReadyFanSpeed, buf);
	
}

void Clear_ready_print()
{
	GUI_SetBkColor(gCfgItems.background_color);
	if(WM_IsWindow(hReadyPrintWnd))
	{
		WM_DeleteWindow(hReadyPrintWnd);
		//GUI_Exec();
	}
	
	//GUI_Clear();
}

