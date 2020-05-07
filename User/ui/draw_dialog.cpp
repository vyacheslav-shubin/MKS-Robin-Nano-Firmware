#include "stdint.h"
#include "GUI.h"
#include "BUTTON.h"
#include "PROGBAR.h"
#include "draw_dialog.h"
#include "draw_ui.h"
#include "ui_tools.h"
#include "sh_tools.h"
#include "draw_printing.h"
#include "draw_ready_print.h"
#include "draw_pause_ui.h"
//#include "sdio_sdcard.h"
#include "sdio.h"       //skyblue 2016-12-13
//#include "mks_tft_com.h"
#include "ff.h"
#include "mks_cfg.h"
#include "usb_host.h"
#include "at24cxx.h"
//#include "sd_usr.h"

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

extern unsigned char codebuff[100];

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
extern volatile unsigned char BeeperCnt;
extern volatile unsigned long BeeperFreq;
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
extern uint8_t flash_preview_begin;
extern uint8_t from_flash_pic;
extern uint8_t default_preview_flg;

extern uint8_t pause_flag;

extern uint8_t filament_loading_time_flg;
extern uint32_t filament_loading_time_cnt;
extern uint8_t filament_loading_completed;
extern uint8_t filament_load_heat_flg;
extern uint8_t filament_unload_heat_flg;
extern uint8_t filament_unloading_time_flg;
extern uint32_t filament_unloading_time_cnt;
extern uint8_t filament_unloading_completed;

extern uint8_t filament_heat_completed_load;
extern uint8_t filament_heat_completed_unload;

extern uint8_t filamentchange_Process;

extern uint8_t disp_in_file_dir;

extern int upload_result ;

extern volatile WIFI_STATE wifi_link_state;
extern WIFI_PARA wifiPara;
extern uint8_t command_send_flag;

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
			ui_print_process.suicide.count_down = SUICIDE_WAIT;
	 	break;

		case WM_NOTIFY_PARENT:
			if(pMsg->Data.v == WM_NOTIFICATION_RELEASED) {
				if(pMsg->hWinSrc == buttonOk) {
					Clear_dialog();
					if(DialogType == DIALOG_TYPE_STOP) {
						stop_print_time();
						card.stopSDPrint();
						wait_for_heatup = false;
						mksReprint.mks_printer_state = MKS_STOP;
						gCfgItems.breakpoint_reprint_flg = 0;
						gCfgItems.breakpoint_z_pos = 0;
						gCfgItems.breakpoint_flg=0;
						reset_file_info();
						draw_ready_print();
					} else if(DialogType == DIALOG_TYPE_PRINT_FILE) {
						if(strlen(curFileName)>(100-1)) {
							draw_dialog(DIALOG_TYPE_MESSEGE_ERR1);
						} else {
							if (is_filament_fail()) {
								draw_dialog(DIALOG_TYPE_FILAMENT_NO_PRESS);
							} else {
								ui_start_print_file();
								draw_printing();
							}
						}
					} else if(DialogType == DIALOG_TYPE_REPRINT_NO_FILE) {
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
							draw_pause();
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
						}
				} else if(DialogType == DIALOG_TYPE_UNBIND) {
					cloud_unbind();
					draw_return_ui();
				} else if(DialogType == DIALOG_TYPE_M80_FAIL) {
					draw_ready_print();
				} else if(DialogType == DIALOG_TYPE_MESSEGE_ERR1) {
					draw_ready_print();
				} else if(DialogType == DIALOG_TYPE_FILAMENT_HEAT_LOAD_COMPLETED) {
					filament_heat_completed_load = 1;
				} else if(DialogType == DIALOG_TYPE_FILAMENT_HEAT_UNLOAD_COMPLETED) {
					filament_heat_completed_unload = 1;
				} else if(DialogType == DIALOG_TYPE_FINISH_PRINT) {
					draw_ready_print();
				} else if(DialogType == DIALOG_TYPE_FILAMENT_NO_PRESS) {
					if(last_disp_state==PRINTING_UI) {
						if(from_flash_pic == 1)
							flash_preview_begin = 1;
						else
							default_preview_flg = 1; 
					}
					draw_return_ui();
				} else {
					draw_return_ui();
				}
			} else if(pMsg->hWinSrc == buttonCancle) {
				unsigned int tmpFlag;
				Chk_close_machine_flg = 0;
                if((DialogType == DIALOG_TYPE_STOP)) {
					if(from_flash_pic == 1)
						flash_preview_begin = 1;
					else
						default_preview_flg = 1;
				}
				if(DialogType == DIALOG_TYPE_PRINT_FILE) {
					disp_in_file_dir = 1;
				}
				Clear_dialog();
				draw_return_ui();
				
				if((DialogType == DIALOG_TYPE_FILAMENT_LOAD_HEAT)
					||(DialogType == DIALOG_TYPE_FILAMENT_UNLOAD_HEAT)
					||(DialogType == DIALOG_TYPE_FILAMENT_HEAT_LOAD_COMPLETED)
					||(DialogType == DIALOG_TYPE_FILAMENT_UNLOAD_COMPLETED)) {
					thermalManager.target_temperature[gCfgItems.curSprayerChoose]= gCfgItems.desireSprayerTempBak;
				} if((DialogType == DIALOG_TYPE_FILAMENT_LOADING)
					||(DialogType == DIALOG_TYPE_FILAMENT_UNLOADING)) {
					enqueue_and_echo_commands_P(PSTR("M410"));
					filamentchange_Process = 0;
					filament_rate = 0;
					filament_loading_completed = 0;
					filament_unloading_completed = 0;
					filament_loading_time_flg = 0;
					filament_loading_time_cnt = 0;
					filament_unloading_time_flg = 0;
					filament_unloading_time_cnt = 0;
					thermalManager.target_temperature[gCfgItems.curSprayerChoose]= gCfgItems.desireSprayerTempBak;
				} if(DialogType == DIALOG_TYPE_REPRINT_NO_FILE) {
					mksReprint.mks_printer_state = MKS_IDLE;
                    if(gCfgItems.pwroff_save_mode != 1)
					    epr_write_data(EPR_SAV_FLAG, (uint8_t *)&mksReprint.mks_printer_state,sizeof(mksReprint.mks_printer_state));  //
                    Clear_dialog();
                    draw_ready_print();

                }
			} else if(pMsg->hWinSrc == buttonRePrint) {
				//TODO: Сделать отдельной ф-ей совмещается с buttonOk

				Clear_dialog();
				if(strlen(curFileName)>(100-1)) {
					draw_dialog(DIALOG_TYPE_MESSEGE_ERR1);
				} else {
					ui_start_print_file();
					draw_printing();
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
		} else if(DialogType == DIALOG_TYPE_FINISH_PRINT) {
			ui_set_text_value(printStopDlgText, print_file_dialog_menu.print_finish);

			buttonRePrint = ui_create_dialog_button((LCD_WIDTH-320)/2,(imgHeight-60)/2, hStopDlgWnd, print_file_dialog_menu.reprint);
			buttonOk = ui_create_dialog_button((LCD_WIDTH-320)/2+40+140,(imgHeight-60)/2,hStopDlgWnd, 0);

			printStopDlgText = ui_create_dialog_text(0,(imgHeight-40)/2-90, LCD_WIDTH, 30, hStopDlgWnd, 0);
			fileNameText = ui_create_dialog_text(0,(imgHeight-40)/2-90+30, LCD_WIDTH, 30, hStopDlgWnd, 0);

			memset(tmpCurFileStr,0,sizeof(tmpCurFileStr));
			strcat(tmpCurFileStr,print_file_dialog_menu.print_time);
			print_time_to_str(&print_time, &tmpCurFileStr[strlen(print_file_dialog_menu.print_time)]);
			ui_set_text_value(fileNameText, &curFileName[3]);
			if (ui_print_process.suicide.enabled) {
				progressBar = ui_create_std_progbar((LCD_WIDTH-400)/2, (imgHeight-60)/2 + 80, 400, 25, hStopDlgWnd);
				PROGBAR_SetValue(progressBar, 0);
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
					||(DialogType == DIALOG_TYPE_FILAMENT_LOAD_COMPLETED)
					||(DialogType == DIALOG_TYPE_FILAMENT_UNLOAD_COMPLETED)
			) {
				buttonOk= ui_create_dialog_button((LCD_WIDTH-140)/2,(imgHeight-40)/2, hStopDlgWnd, 0);
			} else if(DialogType == DIALOG_TYPE_FILAMENT_LOAD_HEAT) {
				buttonCancle= ui_create_dialog_button((LCD_WIDTH-140)/2,(imgHeight-40)/2, hStopDlgWnd, 0);
				filament_temper = ui_create_dialog_text(0,(imgHeight-40)/2-30, LCD_WIDTH, 30, hStopDlgWnd, 0);
			} else if(DialogType == DIALOG_TYPE_FILAMENT_UNLOAD_HEAT) {
				buttonCancle= BUTTON_CreateEx((LCD_WIDTH-140)/2,(imgHeight-40)/2,140, 50,hStopDlgWnd, BUTTON_CF_SHOW, 0, alloc_win_id());
				filament_temper = ui_create_std_text_f(0,(imgHeight-40)/2-30, LCD_WIDTH, 30, hStopDlgWnd, GUI_TA_TOP | GUI_TA_HCENTER, 0);
			} else if((DialogType == DIALOG_TYPE_FILAMENT_LOADING)||(DialogType == DIALOG_TYPE_FILAMENT_UNLOADING)) {
				progressBar = PROGBAR_CreateEx((LCD_WIDTH-400)/2, (imgHeight-40)/2-30, 400, 25, hStopDlgWnd, WM_CF_SHOW, 0, 0);
				PROGBAR_SetBarColor(progressBar, 0, GUI_GREEN);
				PROGBAR_SetValue(progressBar,filament_rate);
				PROGBAR_SetText(progressBar," ");
				buttonCancle = ui_create_dialog_button((LCD_WIDTH-140)/2,(imgHeight-40)/2, hStopDlgWnd, 0);
			} else if(DialogType == DIALOG_TYPE_FILAMENT_NO_PRESS) {
				buttonOk = ui_create_dialog_button((LCD_WIDTH-140)/2,(imgHeight-40)/2, hStopDlgWnd, 0);
			} else {
				buttonOk= ui_create_dialog_button((LCD_WIDTH-320)/2,(imgHeight-40)/2,hStopDlgWnd, 0);
				buttonCancle= ui_create_dialog_button((LCD_WIDTH-320)/2+40+140,(imgHeight-40)/2,hStopDlgWnd, 0);
			}


			if(DialogType == DIALOG_TYPE_STOP) {
				ui_set_text_value(printStopDlgText, print_file_dialog_menu.cancle_print);
			} else if(DialogType == DIALOG_TYPE_PRINT_FILE) {
				print_start_flg = 1;
				if(gCfgItems.breakpoint_reprint_flg == 1) {
					ui_set_text_value(printStopDlgText,  print_file_dialog_menu.print_from_breakpoint);
				} else {
					printStopDlgText = ui_create_dialog_text(0,(imgHeight-40)/2-120, LCD_WIDTH, 40, hStopDlgWnd, print_file_dialog_menu.print_file);
					printfilename = ui_create_std_text_f(0,(imgHeight-40)/2-60, LCD_WIDTH, 30, hStopDlgWnd, GUI_TA_TOP | GUI_TA_HCENTER, 0);
					/*
					memset(codebuff,0,sizeof(codebuff));
					strcpy((char*)codebuff,&curFileName[3]);
					*/
					ui_set_text_value(printfilename, &curFileName[3]);
				}
			} else if(DialogType == DIALOG_TYPE_REPRINT_NO_FILE) {
				TEXT_SetText(printStopDlgText, file_menu.no_file_and_check);
			} else if(DialogType == DIALOG_TYPE_M80_FAIL) {
				TEXT_SetText(printStopDlgText, print_file_dialog_menu.close_machine_error);
			} else if(DialogType == DIALOG_TYPE_UNBIND) {
				TEXT_SetText(printStopDlgText, common_menu.unbind_printer_tips);
			} else if(DialogType == DIALOG_TYPE_FILAMENT_LOAD_HEAT) {
				TEXT_SetText(printStopDlgText, filament_menu.filament_dialog_load_heat);
			} else if(DialogType == DIALOG_TYPE_FILAMENT_HEAT_LOAD_COMPLETED) {
				TEXT_SetText(printStopDlgText, filament_menu.filament_dialog_load_heat_confirm);
			} else if(DialogType == DIALOG_TYPE_FILAMENT_LOADING) {
				TEXT_SetText(printStopDlgText, filament_menu.filament_dialog_loading);
			} else if(DialogType == DIALOG_TYPE_FILAMENT_LOAD_COMPLETED) {
				TEXT_SetText(printStopDlgText, filament_menu.filament_dialog_load_completed);
			} else if(DialogType == DIALOG_TYPE_FILAMENT_UNLOAD_HEAT) {
				TEXT_SetText(printStopDlgText, filament_menu.filament_dialog_unload_heat);
			} else if(DialogType == DIALOG_TYPE_FILAMENT_HEAT_UNLOAD_COMPLETED) {
				TEXT_SetText(printStopDlgText, filament_menu.filament_dialog_unload_heat_confirm);
			} else if(DialogType == DIALOG_TYPE_FILAMENT_UNLOADING) {
				TEXT_SetText(printStopDlgText, filament_menu.filament_dialog_unloading);
			} else if(DialogType == DIALOG_TYPE_FILAMENT_UNLOAD_COMPLETED) {
				TEXT_SetText(printStopDlgText, filament_menu.filament_dialog_unload_completed);
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


void filament_setbar() {
	PROGBAR_SetBarColor(progressBar, 0, GUI_GREEN);
	PROGBAR_SetValue(progressBar,filament_rate);
}

void filament_sprayer_temp() {
	int8_t buf[50] = {0};
	int8_t buf1[30] = {0};
	TEXT_SetTextColor(filament_temper, gCfgItems.title_color);
	TEXT_SetBkColor(filament_temper, gCfgItems.background_color);
	sprintf((char*)buf,"E%d: ",gCfgItems.curSprayerChoose+1);
	sprintf((char *)buf1, filament_menu.stat_temp,(int)thermalManager.current_temperature[gCfgItems.curSprayerChoose],(int)thermalManager.target_temperature[gCfgItems.curSprayerChoose]);
	strcat((char*)buf,(char*)buf1);
	TEXT_SetText(filament_temper, (char *)buf);
}
void filament_dialog_handle() {
	if (temperature_change_frequency == 1) {
		temperature_change_frequency = 0;
		filament_sprayer_temp();
	}

	if(filament_heat_completed_load==1) {
		filament_heat_completed_load = 0;
		Clear_dialog();
		draw_dialog(DIALOG_TYPE_FILAMENT_LOADING);
		stepper.synchronize();
		filament_loading_time_flg = 1;
		filament_loading_time_cnt = 0;
		filamentchange_Process = 1;
		MYSERIAL.filamentchange();
	}

	if(filament_heat_completed_unload == 1)
	{
		filament_heat_completed_unload = 0;
		Clear_dialog();
		draw_dialog(DIALOG_TYPE_FILAMENT_UNLOADING);
		stepper.synchronize();
		filament_unloading_time_flg = 1;
		filament_unloading_time_cnt = 0;
		filamentchange_Process = 2;
		MYSERIAL.filamentchange();
	}
	
	if(((abs((int)((int)thermalManager.current_temperature[gCfgItems.curSprayerChoose] - gCfgItems.filament_load_limit_temper))<=1)
		||((int)thermalManager.current_temperature[gCfgItems.curSprayerChoose] > gCfgItems.filament_load_limit_temper))
		&&(filament_load_heat_flg==1))
	{
		filament_load_heat_flg = 0;
		Clear_dialog();
		draw_dialog(DIALOG_TYPE_FILAMENT_HEAT_LOAD_COMPLETED);				
	}
	
	if(filament_loading_completed==1)
	{
		filamentchange_Process = 0;
		filament_rate = 0;
		filament_loading_completed = 0;
		Clear_dialog();
		draw_dialog(DIALOG_TYPE_FILAMENT_LOAD_COMPLETED);
	}
	if(((abs((int)((int)thermalManager.current_temperature[gCfgItems.curSprayerChoose]- gCfgItems.filament_unload_limit_temper))<=1)
		||((int)thermalManager.current_temperature[gCfgItems.curSprayerChoose] > gCfgItems.filament_unload_limit_temper))
		&&(filament_unload_heat_flg==1))
	{
		filament_unload_heat_flg = 0;
		Clear_dialog();
		draw_dialog(DIALOG_TYPE_FILAMENT_HEAT_UNLOAD_COMPLETED);				
	}
	
	if(filament_unloading_completed==1)
	{
		filamentchange_Process = 0;
		filament_rate = 0;
		filament_unloading_completed = 0;
		Clear_dialog();
		draw_dialog(DIALOG_TYPE_FILAMENT_UNLOAD_COMPLETED);
	}

	if((DialogType == DIALOG_TYPE_FILAMENT_LOADING)||(DialogType == DIALOG_TYPE_FILAMENT_UNLOADING)) {
		filament_setbar();
	}
}

uint8_t command_send_flag;
void wifi_scan_handle() {
	char buf[6]={0};
	if(command_send_flag == 1) {
		if(wifi_link_state == WIFI_CONNECTED && wifiPara.mode != 0x01) {
			last_disp_state = PRINT_READY_UI;
			clear_ready_print();
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
		case DIALOG_TYPE_FILAMENT_LOAD_HEAT:
		case DIALOG_TYPE_FILAMENT_UNLOAD_HEAT:
			filament_dialog_handle();
			break;
		case WIFI_ENABLE_TIPS:
			wifi_scan_handle();
			break;
		case DIALOG_TYPE_FINISH_PRINT:
			if (ui_print_process.suicide.enabled) {
				if (is_ui_timing(F_UI_TIMING_SEC)) {
					ui_timing_clear(F_UI_TIMING_SEC);
					PROGBAR_SetValue(progressBar, 100 - ui_print_process.suicide.count_down*100/SUICIDE_WAIT);
					if (ui_print_process.suicide.count_down--==0) {
						ui_print_process.suicide.enabled=0;
						Clear_dialog();
						ui_clear_screen();
						enqueue_and_echo_commands_P(PSTR("M81"));
					}
				}
			}
			break;

	}
}
