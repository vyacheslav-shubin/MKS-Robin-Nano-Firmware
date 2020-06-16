#include "stdint.h"
#include "GUI.h"
#include "UI.h"
#include "BUTTON.h"
#include "PROGBAR.h"
#include "draw_dialog.h"
#include "draw_ui.h"
#include "ui_tools.h"
#include "sh_tools.h"
#include "UI.h"
#include "sdio.h"       //skyblue 2016-12-13
#include "ff.h"
#include "mks_cfg.h"
#include "usb_host.h"
#include "at24cxx.h"

#include "mks_cfg.h"
#include "Marlin.h"
#include "cardreader.h"
#include "mks_reprint.h"
#include "planner.h"
#include "planner.h"
#include "fatfs.h"
#include "draw_ui.h"
#include "wifi_module.h"

#ifndef GUI_FLASH
#define GUI_FLASH
#endif
//extern GUI_CONST_STORAGE GUI_FONT GUI_FontFont24_MS;

extern FATFS fs;

extern FIL fp_reprint_rw;

extern unsigned char path_bak[15];
extern unsigned char *path_reprint;


extern uint8_t print_start_flg;

extern uint8_t  mksPrinterStatusFlag;

static GUI_HWIN hStopDlgWnd;

extern int8_t curFilePath[30];

uint32_t rePrintOffset;

extern volatile int16_t logo_time;

extern GUI_FLASH const GUI_FONT GUI_FontHZ_fontHz18;
extern int X_ADD,X_INTERVAL;   //**ͼƬ���

extern uint8_t  Get_Temperature_Flg;
//extern PR_STATUS printerStaus;
extern volatile uint8_t get_temp_flag;
extern unsigned char SendOneTime;
//int8_t  gCurDir[100];
uint8_t Chk_close_machine_flg;
extern unsigned char breakpoint_homeFlag;

static TEXT_Handle printStopDlgText,filament_temper,printfilename,fileNameText;

static BUTTON_Handle buttonOk, buttonCancle, buttonRePrint;

static PROGBAR_Handle progressBar;
uint32_t filament_rate;

uint8_t DialogType;
extern uint8_t print_start_flg;

extern uint8_t pause_flag;

extern int upload_result ;

extern volatile WIFI_STATE wifi_link_state;
extern WIFI_PARA wifiPara;

static void cbDlgWin(WM_MESSAGE * pMsg) {
	int8_t sel_item;
	uint32_t i;
	uint8_t i2c_cnt =  0;
	volatile uint8_t tmp = 0xff;
	uint8_t BakfilePathLen;

	switch (pMsg->MsgId) {
		case WM_PAINT:
		 	break;
		case WM_TOUCH:
		case WM_TOUCH_CHILD:
	 	break;

		case WM_NOTIFY_PARENT:
			if(pMsg->Data.v == WM_NOTIFICATION_RELEASED) {
                Clear_dialog();
                draw_return_ui();
            }
	}
}

void draw_dialog(uint8_t type)
{
	TCHAR tmpCurFileStr[40];
	int i;
	
	ui_push_disp_stack(DIALOG_UI);
	//ui_initialize_screen_gui();
	ui_clear_screen();

	buttonOk = 0;
	buttonCancle = 0;
	DialogType = type;

	hStopDlgWnd = 0;

	if(disp_state_stack._disp_index > 1)
		GUI_DispStringAt(creat_title_text(), TITLE_XPOS, TITLE_YPOS);

    {
		hStopDlgWnd = ui_std_window(cbDlgWin);

		if(DialogType == DIALOG_TYPE_UPLOAD_FILE) {

			printStopDlgText = TEXT_CreateEx(0,0, LCD_WIDTH, imgHeight/2, hStopDlgWnd, WM_CF_SHOW, TEXT_CF_LEFT,  alloc_win_id(), "Uploading......");
			TEXT_SetBkColor(printStopDlgText, gCfgItems.background_color);
			TEXT_SetTextColor(printStopDlgText, gCfgItems.title_color);
			TEXT_SetTextAlign(printStopDlgText, GUI_TA_VCENTER | GUI_TA_HCENTER);
			if(upload_result == 1) {
				TEXT_SetText(printStopDlgText, DIALOG_UPLOAD_ING_EN);
			} else if(upload_result == 2) {
				buttonOk = BUTTON_CreateEx((LCD_WIDTH-DIALOG_BTN_XPIEL)/2,(imgHeight)/2, DIALOG_BTN_XPIEL, DIALOG_BTN_YPIEL,hStopDlgWnd, BUTTON_CF_SHOW, 0, alloc_win_id());
				TEXT_SetText(printStopDlgText, DIALOG_UPLOAD_ERROR_EN);
			} else if(upload_result == 3) {
				//TODO: upload_size, upload_time - отдельной структурой
				buttonOk= BUTTON_CreateEx((LCD_WIDTH-140)/2,(imgHeight)/2, 140, 50,hStopDlgWnd, BUTTON_CF_SHOW, 0, alloc_win_id());
				TEXT_SetText(printStopDlgText, DIALOG_UPLOAD_FINISH_EN);
			}
		} else {

			printStopDlgText = ui_create_dialog_text(0,(imgHeight-40)/2-90, LCD_WIDTH, 70, hStopDlgWnd, 0);


			buttonOk= ui_create_dialog_button((LCD_WIDTH-320)/2,(imgHeight-40)/2,hStopDlgWnd, 0);
			buttonCancle= ui_create_dialog_button((LCD_WIDTH-320)/2+40+140,(imgHeight-40)/2,hStopDlgWnd, 0);



		}
	}
	if (buttonOk)
		BUTTON_SetText(buttonOk, print_file_dialog_menu.confirm);
	if (buttonCancle)
		BUTTON_SetText(buttonCancle, print_file_dialog_menu.cancle);
}


void Clear_dialog() {
	ui_drop_window(hStopDlgWnd);
}

void refresh_dialog() {
}
