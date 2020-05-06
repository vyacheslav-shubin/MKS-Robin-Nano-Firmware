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

typedef struct{
	BUTTON_Handle  tool, preHeat, print, extruder, move, more, zero, set, leveling;
} BUTTONS_DEF;

static BUTTONS_DEF btn;

static GUI_HWIN hReadyPrintWnd;

int X_ADD=5,X_INTERVAL=10;
extern char cmd_code[201];
extern char x[1];
extern volatile char *codebufpoint;
extern uint8_t leveling_first_time;

extern uint8_t disp_in_file_dir;


#define RETRO_ICON_INTEVAL  (LCD_WIDTH-4*50)/5

static void cbReadPrintWin(WM_MESSAGE * pMsg) {
	
	switch (pMsg->MsgId) {
		case WM_PAINT:
			break;
		case WM_NOTIFY_PARENT:
			if(pMsg->Data.v == WM_NOTIFICATION_RELEASED) {
				if(pMsg->hWinSrc == btn.print) {
					disp_in_file_dir = 1;
					last_disp_state = PRINT_READY_UI;
					clear_ready_print();
					draw_print_file();
				} else if(pMsg->hWinSrc == btn.move) {
					gCfgItems.getzpos_flg = 1;
					last_disp_state = PRINT_READY_UI;
					clear_ready_print();
					draw_move_motor();			
				} else if(pMsg->hWinSrc == btn.preHeat) {
					last_disp_state = PRINT_READY_UI;
					clear_ready_print();
					draw_preHeat();
				} else if(pMsg->hWinSrc == btn.extruder) {
					gCfgItems.extruSpeed_bak = feedrate_mm_s;
					last_disp_state = PRINT_READY_UI;
					clear_ready_print();
					draw_extrusion();
				} else if(pMsg->hWinSrc == btn.zero) {
					last_disp_state = PRINT_READY_UI;
					clear_ready_print();
					draw_zero();
				} else if(pMsg->hWinSrc == btn.set) {
					last_disp_state = PRINT_READY_UI;
					clear_ready_print();
					draw_Set();
				} else if(pMsg->hWinSrc == btn.more) {
					last_disp_state = PRINT_READY_UI;
					clear_ready_print();
					draw_More();
				} else if(pMsg->hWinSrc == btn.tool) {
					last_disp_state = PRINT_READY_UI;
					clear_ready_print();
					draw_tool();
				} else if(pMsg->hWinSrc == btn.leveling) {
					if(gCfgItems.leveling_mode == 1) {
						if(BED_LEVELING_METHOD & MESH_BED_LEVELING) {
                            last_disp_state = PRINT_READY_UI;
                            clear_ready_print();
                            draw_meshleveling();
                        } else {
						    SPI_FLASH_BufferRead((u8 *)cmd_code,BUTTON_AUTOLEVELING_ADDR,201);
						    codebufpoint = cmd_code;		
                        }
					} else {
						leveling_first_time = 1;
						last_disp_state = PRINT_READY_UI;
						clear_ready_print();
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

	memset(&btn, 0, sizeof(btn));
	ui_reset_disp_stack(PRINT_READY_UI);
	ui_init_page();
	hReadyPrintWnd = ui_std_window(cbReadPrintWin);
	
	if(gCfgItems.display_style == 0) {
		btn.preHeat = ui_std_button(0, 0, hReadyPrintWnd, "bmp_preHeat.bin", main_menu.preheat);
		btn.move = 	ui_std_button(1, 0, hReadyPrintWnd, "bmp_mov.bin", main_menu.move);
		btn.zero = ui_std_button(2, 0, hReadyPrintWnd, "bmp_zero.bin", main_menu.home);
		btn.print = ui_std_button(3, 0, hReadyPrintWnd, "bmp_printing.bin", main_menu.print);

		btn.extruder = ui_std_button(0, 1, hReadyPrintWnd, "bmp_extruct.bin", main_menu.extrude);

		uint8_t next_button_offset = 1;
		switch(gCfgItems.leveling_mode) {
		case 0:
			btn.leveling  = ui_std_button(1, 1, hReadyPrintWnd, "bmp_leveling.bin",  main_menu.leveling);
			break;
		case 1:
			btn.leveling  = ui_std_button(1, 1, hReadyPrintWnd, "bmp_autoleveling.bin",  main_menu.autoleveling);
			break;
		default:
			next_button_offset = 0;
			break;
		}
		btn.set  = ui_std_button(1+next_button_offset, 1, hReadyPrintWnd, "bmp_set.bin",  main_menu.set);
		btn.more = ui_std_button(2+next_button_offset, 1, hReadyPrintWnd, "bmp_more.bin",  main_menu.more);

	} else {
		btn.tool = ui_create_std_button(SIMPLE_FIRST_PAGE_GRAP+1,(LCD_HEIGHT-BTN_Y_PIXEL)/2-titleHeight, hReadyPrintWnd, "bmp_tool.bin", main_menu.tool);
		btn.set = ui_create_std_button(BTN_X_PIXEL+SIMPLE_FIRST_PAGE_GRAP*2+1,(LCD_HEIGHT-BTN_Y_PIXEL)/2-titleHeight, hReadyPrintWnd, "bmp_set.bin", main_menu.set);
		btn.print = ui_create_std_button(BTN_X_PIXEL*2+SIMPLE_FIRST_PAGE_GRAP*3+1,(LCD_HEIGHT-BTN_Y_PIXEL)/2-titleHeight,hReadyPrintWnd, "bmp_printing.bin", main_menu.print);
	}
}

void clear_ready_print()
{
	ui_drop_window(hReadyPrintWnd);
}

