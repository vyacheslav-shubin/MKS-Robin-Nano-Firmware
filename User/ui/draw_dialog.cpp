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

#include "draw_bind.h"
#include "mks_cfg.h"
#include "Marlin.h"
#include "cardreader.h"
#include "mks_reprint.h"
#include "planner.h"
#include "planner.h"
#include "fatfs.h"
#include "draw_ui.h"
#include "wifi_module.h"
#include "draw_wifi_list.h"
#include "draw_wifi.h"

#ifndef GUI_FLASH
#define GUI_FLASH
#endif
//extern GUI_CONST_STORAGE GUI_FONT GUI_FontFont24_MS;

extern FATFS fs;

extern FIL fp_reprint_rw;

extern unsigned char path_bak[15];
extern unsigned char *path_reprint;


extern FIL *srcfp;
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
				if(pMsg->hWinSrc == buttonOk) {
					Clear_dialog();
					if(DialogType == DIALOG_TYPE_PRINT_FILE) {
						if(strlen(ui_print_process.file_name)>(100-1)) {
							draw_dialog(DIALOG_TYPE_MESSEGE_ERR1);
						} else {
							if (is_filament_fail()) {
								draw_dialog(DIALOG_TYPE_FILAMENT_NO_PRESS);
							} else {
								ui_app.startPrintFile();
							}
						}
					} else if(DialogType == DIALOG_TYPE_REPRINT_NO_FILE) {
						//TODO: этот код из mks_contiuePrint_UI
						ui_print_process.preview_state_flags = 0;
						f_mount(&fs, (TCHAR const*)SD_Path, 0);
						card.openFile(mksReprint.filename, true);
						if(!card.isFileOpen()) {
							disp_state_stack._disp_index = 0;
							memset(disp_state_stack._disp_state, 0, sizeof(disp_state_stack._disp_state));
							disp_state_stack._disp_state[disp_state_stack._disp_index] = PRINT_READY_UI;
							draw_dialog(DIALOG_TYPE_REPRINT_NO_FILE);
						} else {
							if(gCfgItems.pwroff_save_mode != 1)
                                mks_ReadFromFile();
							epr_write_data(EPR_SAV_FILENAME, (uint8_t *)&mksReprint.filename[0],sizeof(mksReprint.filename)); 
							card.sdprinting = 0;
							if(mksReprint.resume == MKS_RESUME_PWDWN) 
								mks_getPositionXYZE();
							if(gCfgItems.pwrdn_mtrdn_level_flg != 1)
								card.setIndex(mksReprint.sdpos);
							else
								card.setIndex(mksReprint.sdpos_from_epr);
							current_position[X_AXIS] = mksReprint.current_position[0];
							current_position[Y_AXIS] = mksReprint.current_position[1];
							current_position[Z_AXIS] = mksReprint.current_position[2];
							mks_clearDir();
							printing_ui.show();
						}
				} else if(DialogType == DIALOG_TYPE_UNBIND) {
					cloud_unbind();
					draw_return_ui();
				} else if(DialogType == DIALOG_TYPE_M80_FAIL) {
					ui_app.showMainWidget();
				} else if(DialogType == DIALOG_TYPE_MESSEGE_ERR1) {
					ui_app.showMainWidget();
				} else if(DialogType == DIALOG_TYPE_FILAMENT_NO_PRESS) {
					draw_return_ui();
				} else {
					draw_return_ui();
				}
			} else if(pMsg->hWinSrc == buttonCancle) {
				unsigned int tmpFlag;
				Chk_close_machine_flg = 0;
				Clear_dialog();
				draw_return_ui();
				
				if(DialogType == DIALOG_TYPE_REPRINT_NO_FILE) {
					mksReprint.mks_printer_state = MKS_IDLE;
                    if(gCfgItems.pwroff_save_mode != 1)
					    epr_write_data(EPR_SAV_FLAG, (uint8_t *)&mksReprint.mks_printer_state,sizeof(mksReprint.mks_printer_state));  //
                    Clear_dialog();
                    ui_app.showMainWidget();
                }
			} else if(pMsg->hWinSrc == buttonRePrint) {
				//TODO: Сделать отдельной ф-ей совмещается с buttonOk

				if(strlen(ui_print_process.file_name)>(100-1)) {
					Clear_dialog();
					draw_dialog(DIALOG_TYPE_MESSEGE_ERR1);
				} else {
					ui_app.startPrintFile();
				}
			}
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

	if(DialogType == DIALOG_TYPE_UPDATE_ESP_FIRMARE) {
		GUI_DispStringAt(DIALOG_UPDATE_WIFI_FIRMWARE_EN, 40, 120);
	} else if(DialogType == DIALOG_TYPE_UPDATE_ESP_DATA) {
		GUI_DispStringAt(DIALOG_UPDATE_WIFI_WEB_EN, 40, 100);
	} else {
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
				char buf[200];
				int _index = 0;
				memset(buf,0,sizeof(200));
				buttonOk= BUTTON_CreateEx((LCD_WIDTH-140)/2,(imgHeight)/2, 140, 50,hStopDlgWnd, BUTTON_CF_SHOW, 0, alloc_win_id());
				strcpy(buf, DIALOG_UPLOAD_FINISH_EN);
				_index = strlen(buf);
				buf[_index] = '\n';
				_index++;
				strcat(buf, DIALOG_UPLOAD_SIZE_EN);
				_index = strlen(buf);
				buf[_index] = ':';
				_index++;
				sprintf(&buf[_index], " %.1d KBytes\n", upload_file_info.size / 1024);
				strcat(buf, DIALOG_UPLOAD_TIME_EN);
				_index = strlen(buf);
				buf[_index] = ':';
				_index++;
				sprintf(&buf[_index], " %d s\n", upload_file_info.time);
				strcat(buf, DIALOG_UPLOAD_SPEED_EN);
				_index = strlen(buf);
				buf[_index] = ':';
				_index++;
				sprintf(&buf[_index], " %d KBytes/s\n", upload_file_info.size / upload_file_info.time / 1024);
				TEXT_SetText(printStopDlgText, buf);
			}
		} else if(DialogType == WIFI_ENABLE_TIPS) {
			buttonCancle= BUTTON_CreateEx((LCD_WIDTH-120)/2,(imgHeight-60)/2,120,60,hStopDlgWnd, BUTTON_CF_SHOW, 0, alloc_win_id());
			printStopDlgText = TEXT_CreateEx(0,(imgHeight-40)/2-90, LCD_WIDTH, 60, hStopDlgWnd, WM_CF_SHOW, GUI_TA_VCENTER | GUI_TA_HCENTER,	alloc_win_id(), " ");
			TEXT_SetBkColor(printStopDlgText, gCfgItems.background_color);
			TEXT_SetTextColor(printStopDlgText, gCfgItems.title_color);
			TEXT_SetText(printStopDlgText, print_file_dialog_menu.wifi_enable_tips);
		} else {

			printStopDlgText = ui_create_dialog_text(0,(imgHeight-40)/2-90, LCD_WIDTH, 70, hStopDlgWnd, 0);

			if(
					(DialogType == DIALOG_TYPE_M80_FAIL)
			) {
				buttonOk= ui_create_dialog_button((LCD_WIDTH-140)/2,(imgHeight-40)/2, hStopDlgWnd, 0);
			} else if(DialogType == DIALOG_TYPE_FILAMENT_NO_PRESS) {
				buttonOk = ui_create_dialog_button((LCD_WIDTH-140)/2,(imgHeight-40)/2, hStopDlgWnd, 0);
			} else {
				buttonOk= ui_create_dialog_button((LCD_WIDTH-320)/2,(imgHeight-40)/2,hStopDlgWnd, 0);
				buttonCancle= ui_create_dialog_button((LCD_WIDTH-320)/2+40+140,(imgHeight-40)/2,hStopDlgWnd, 0);
			}


			if(DialogType == DIALOG_TYPE_PRINT_FILE) {
				print_start_flg = 1;
				if(gCfgItems.breakpoint_reprint_flg == 1) {
					ui_set_text_value(printStopDlgText,  print_file_dialog_menu.print_from_breakpoint);
				} else {
					printStopDlgText = ui_create_dialog_text(0,(imgHeight-40)/2-120, LCD_WIDTH, 40, hStopDlgWnd, print_file_dialog_menu.print_file);
					printfilename = ui_create_std_text_f(0,(imgHeight-40)/2-60, LCD_WIDTH, 30, hStopDlgWnd, GUI_TA_TOP | GUI_TA_HCENTER, 0);
					ui_set_text_value(printfilename, &ui_print_process.file_name[3]);
				}
			} else if(DialogType == DIALOG_TYPE_REPRINT_NO_FILE) {
				TEXT_SetText(printStopDlgText, file_menu.no_file_and_check);
			} else if(DialogType == DIALOG_TYPE_M80_FAIL) {
				TEXT_SetText(printStopDlgText, print_file_dialog_menu.close_machine_error);
			} else if(DialogType == DIALOG_TYPE_UNBIND) {
				TEXT_SetText(printStopDlgText, common_menu.unbind_printer_tips);
			} else if(DialogType == DIALOG_TYPE_FILAMENT_NO_PRESS) {
				TEXT_SetText(printStopDlgText, print_file_dialog_menu.filament_no_press);
			}
		}
	}
	if (buttonOk)
		BUTTON_SetText(buttonOk, print_file_dialog_menu.confirm);
	if (buttonCancle)
		BUTTON_SetText(buttonCancle, print_file_dialog_menu.cancle);
}


void wifi_scan_handle() {
	char buf[6]={0};
	if(wifi_list_received_flag == 1) {
		if(wifi_link_state == WIFI_CONNECTED && wifiPara.mode != 0x01) {
			last_disp_state = PRINT_READY_UI;
			main_ui.hide();
			draw_Wifi();
		} else {
			last_disp_state = DIALOG_UI;
			Clear_dialog();
			draw_Wifi_list();
		}
	}
}


void Clear_dialog() {
	ui_drop_window(hStopDlgWnd);
}

void refresh_dialog() {
	switch (DialogType) {
		case WIFI_ENABLE_TIPS:
			wifi_scan_handle();
			break;
	}
}
