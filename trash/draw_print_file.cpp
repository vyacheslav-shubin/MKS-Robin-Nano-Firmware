//TODO: убрать диалог подтверждения перед печатью
#include "../../trash/draw_print_file.h"

#include "GUI.h"
#include "BUTTON.h"
#include "PROGBAR.h"
#include "draw_ui.h"
#include "fontLib.h"
#include "TEXT.h"
#include "UI.h"
#include "ui_tools.h"
#include "tim.h"
#include "ff.h"
#include "fatfs.h"
#include <string.h>
#include "sdio.h"
#include "sh_tools.h"

#include "Marlin.h"
#include "Configuration_adv.h"
#include "cardreader.h"
#include "draw_set.h"

#ifndef GUI_FLASH
#define GUI_FLASH
#endif
#define FILE_BTN_CNT    6
GUI_HWIN hPrintFileWnd, hAlertWin, hPrintFileDlgWnd;

uint8_t trig_browser_flag;
int8_t curDirLever = 0;
DIR_OFFSET dir_offset[10];

typedef struct{
	BUTTON_Handle handle;
	char* fileName;
	int preview_offset;
} FILE_BUTTON;

static int8_t pages;
static BUTTON_Handle buttonPu, buttonPd, buttonR;

static FILE_BUTTON buttonF[FILE_BTN_CNT];

uint8_t back_flg = 0;

void search_files();
void disp_udisk_files(int seq);
void draw_preview();

static void cbPrintFileWin(WM_MESSAGE * pMsg) {
	switch (pMsg->MsgId) {
		case WM_PAINT:
		case WM_TOUCH:
		case WM_TOUCH_CHILD:
			break;
		case WM_NOTIFY_PARENT:
			if(pMsg->Data.v == WM_NOTIFICATION_RELEASED) {
				if(pMsg->hWinSrc == buttonPd) {
					if(dir_offset[curDirLever].cur_page_last_offset > 0) {
						card.Sd_file_cnt = 0;
						card.gcodeFileList.index = 0;
						card.Sd_file_offset = dir_offset[curDirLever].cur_page_last_offset + 1;
						do {
							card.ShowSDFiles();
							if((card.gcodeFileList.listVaild == 1) && (card.gcodeFileList.index == 1))
								dir_offset[curDirLever].cur_page_first_offset = card.Sd_file_offset;
							if(card.gcodeFileList.index >= FILE_BTN_CNT) {
								dir_offset[curDirLever].cur_page_last_offset = card.Sd_file_offset;
								card.Sd_file_offset++;
								break;
							}
							card.Sd_file_offset++;
						} while(card.gcodeFileList.listVaild != 2);

						if(card.gcodeFileList.index != 0) {
							clear_print_file();
							disp_udisk_files(0);
							dir_offset[curDirLever].curPage++;
						}

						if(card.gcodeFileList.listVaild == 2)
							dir_offset[curDirLever].cur_page_last_offset = 0;
					}
				} else if(pMsg->hWinSrc == buttonPu) {
					if(dir_offset[curDirLever].curPage > 0) {
						card.Sd_file_cnt = 0;
						card.gcodeFileList.index = 0;
						if(dir_offset[curDirLever].cur_page_first_offset >= 5)
							card.Sd_file_offset = dir_offset[curDirLever].cur_page_first_offset - 1;
						do {
							card.ShowSDFiles();
				
							if((card.gcodeFileList.listVaild == 1) && (card.gcodeFileList.index == 1))
								dir_offset[curDirLever].cur_page_last_offset = card.Sd_file_offset;
							if(card.gcodeFileList.index >= FILE_BTN_CNT) {
								dir_offset[curDirLever].cur_page_first_offset = card.Sd_file_offset;
								if(card.Sd_file_offset)
									card.Sd_file_offset--;
								break;
							}
							if(card.Sd_file_offset)
								card.Sd_file_offset--;

						} while(card.gcodeFileList.listVaild != 2);
				
						if(card.gcodeFileList.index != 0) {
							clear_print_file();
							disp_udisk_files(1);
							dir_offset[curDirLever].curPage--;
						}
				
					}
				} else if(pMsg->hWinSrc == buttonR) {
					if(curDirLever == 0) {
						if(gCfgItems.breakpoint_reprint_flg == 1) {
							gCfgItems.breakpoint_reprint_flg = 0;
							last_disp_state = SET_UI;
							clear_print_file();
							card.Sd_file_offset = 0;
							card.Sd_file_cnt = 0;
							draw_return_ui();
						} else {
							last_disp_state = PRINT_FILE_UI;
							clear_print_file();
							card.Sd_file_offset = 0;
							card.Sd_file_cnt = 0;
							ui_app.showMainWidget();
						}
					} else {
						int8_t *ch = 0;
						ch =  (int8_t *)strrchr((char *)card.gCurDir, '/');
						if(ch != 0) {
							*ch = 0;
							clear_print_file();
							dir_offset[curDirLever].curPage = 0;
							dir_offset[curDirLever].cur_page_first_offset = 0;
							dir_offset[curDirLever].cur_page_last_offset = 0;
							curDirLever--;
							search_files();
						}
					}
				} else {
					int8_t i,j;
					for(i = 0; i < FILE_BTN_CNT; i++) {
						j = (back_flg == 1)?(FILE_BTN_CNT-1) - i:i;
						if(pMsg->hWinSrc == buttonF[i].handle) {
							if(card.gcodeFileList.fileName[j][0] != 0) {
								if(card.gcodeFileList.fileAttr[j] == 1) { //dir
									memset(card.gCurDir, 0, sizeof(card.gCurDir));
									strcpy((char *)card.gCurDir,  (const char *)card.gcodeFileList.fileName[j]);
									clear_print_file();
									curDirLever++;
									search_files();
								} else {
									memset(ui_print_process.file_name, 0, sizeof(ui_print_process.file_name));
									#if _LFN_UNICODE
									wcscpy((wchar_t *)ui_print_process.file_name, (const wchar_t*)card.gcodeFileList.fileName[j]);
									#else
									strcpy(ui_print_process.file_name, (const char *)card.gcodeFileList.fileName[j]);
									#endif

									if (is_filament_fail()) {
										clear_print_file();
										draw_dialog(DIALOG_TYPE_FILAMENT_NO_PRESS);
									} else {
										ui_app.startPrintFile();
									}


									//draw_dialog(DIALOG_TYPE_PRINT_FILE);
								}
								break;
							}
						}
					}
				}
			}
			break;
		default:
			WM_DefaultProc(pMsg);
	}
}

FATFS fs;

void draw_print_file(){
	int i;
	ui_push_disp_stack(PRINT_FILE_UI);
	ui_init_page();

	hPrintFileWnd = 0;
	curDirLever = 0;	
	dir_offset[curDirLever].curPage = 0;

	card.Sd_file_offset = 0;
	card.Sd_file_cnt = 0;
	memset(dir_offset, 0, sizeof(dir_offset));
	memset(card.gCurDir, 0, sizeof(card.gCurDir));
	#if _LFN_UNICODE
		card.gCurDir[0] = '1';
		card.gCurDir[1] = ':';
	#else
		if((gCfgItems.fileSysType == FILE_SYS_SD)) {
			if(SD_DET_IP == SD_DETECT_INVERTED)
				f_mount(&fs, (TCHAR const*)SD_Path, 0);
			strcpy((char *)card.gCurDir, "1:"); //skyblue-modify
		}
 	#endif
	search_files();
}

void search_files() {
	uint32_t tick1, tick2;
	GUI_DispStringAt(creat_title_text(), TITLE_XPOS, TITLE_YPOS);
	GUI_DispStringAt(file_menu.file_loading, (LCD_WIDTH-64)/2, imgHeight/2);

	tick1 = getTick();
	card.Sd_file_cnt = 0;
	card.gcodeFileList.listVaild = 0;
	card.gcodeFileList.index = 0;
	memset(card.gcodeFileList.fileAttr, 0, sizeof(card.gcodeFileList.fileAttr));
	memset(card.gcodeFileList.fileName, 0, sizeof(card.gcodeFileList.fileName));

	card.Sd_file_offset = dir_offset[curDirLever].cur_page_first_offset;
	do {
		card.ShowSDFiles();
		if((card.gcodeFileList.listVaild == 1) && (card.gcodeFileList.index == 1))
			dir_offset[curDirLever].cur_page_first_offset = card.Sd_file_offset;
		if(card.gcodeFileList.index >= FILE_BTN_CNT) {
			dir_offset[curDirLever].cur_page_last_offset = card.Sd_file_offset;
			card.Sd_file_offset++;
			break;
		}
		card.Sd_file_offset++;
	} while(card.gcodeFileList.listVaild != 2);

	if(card.gcodeFileList.index == 0) {
		GUI_ClearRect(80, 100, 480, 300);
		if(curDirLever == 0) {
			GUI_DispStringAt(file_menu.no_file_and_check, (LCD_WIDTH-200)/2, imgHeight/2);
		} else {
			GUI_DispStringAt(file_menu.no_file, (LCD_WIDTH-64)/2,imgHeight/2);
		}
		tick2 = getTick();
		while(1) {
			tick1 = getTick();
			if(getTickDiff(tick1, tick2) >= 3000) {
				if(curDirLever == 0) {
					if(gCfgItems.breakpoint_reprint_flg == 1) {
					    gCfgItems.breakpoint_reprint_flg = 0;
						last_disp_state = SET_UI;
						clear_print_file();
						draw_return_ui();
					} else {
						last_disp_state = PRINT_FILE_UI;
						clear_print_file();
						ui_app.showMainWidget();
					}
				} else {
					int8_t *ch = 0;
					ch =  (int8_t *)strrchr((const char *)card.gCurDir, '/');
					if(ch != 0) {
						*ch = 0;
						clear_print_file();
						dir_offset[curDirLever].curPage = 0;
						dir_offset[curDirLever].cur_page_first_offset = 0;
						dir_offset[curDirLever].cur_page_last_offset = 0;
						curDirLever--;
						search_files();
					}
				}
				break;
			}
		}
		return;
	}
	GUI_ClearRect(80, 100, 480, 300);
	disp_udisk_files(0);
}

void disp_udisk_files(int seq) {
	#if _LFN_UNICODE
		TCHAR tmpStr[100] = {0};
	#else
		int8_t tmpStr[100] = {0};
	#endif
	int8_t *tmpStr1;
	int8_t *tmpStr2;
	int8_t i, j;
	GUI_DispStringAt(creat_title_text(), TITLE_XPOS, TITLE_YPOS);
	hPrintFileWnd = WM_CreateWindow(0, titleHeight, LCD_WIDTH, imgHeight, WM_CF_SHOW, cbPrintFileWin, 0);
	buttonPu = ui_create_100_80_button(OTHER_BTN_XPIEL*3+INTERVAL_V*4,0,hPrintFileWnd, "bmp_page_up.bin");
	buttonPd = ui_create_100_80_button(OTHER_BTN_XPIEL*3+INTERVAL_V*4,OTHER_BTN_YPIEL+INTERVAL_H,hPrintFileWnd, "bmp_page_down.bin");
	buttonR = ui_create_100_80_button(OTHER_BTN_XPIEL*3+INTERVAL_V*4,OTHER_BTN_YPIEL*2+INTERVAL_H*2, hPrintFileWnd, "bmp_page_back.bin");
	GUI_Exec();

	GUI_UC_SetEncodeNone();
	char * file = 0;
	memset(buttonF, 0, sizeof(buttonF));

    for(i = 0; i < FILE_BTN_CNT; i++) {
    	if(seq) {
			j = (FILE_BTN_CNT-1) - i;
			back_flg = 1;
		} else {
			j = i;
			back_flg = 0;
		}
		if(i >= card.gcodeFileList.index)
			break;
	
		if(i < 3) {
			buttonF[i].handle = ui_std_button(i, 0, hPrintFileWnd, 0, 0);
		} else {
			buttonF[i].handle = ui_std_button(i-3, 1, hPrintFileWnd, 0, 0);
		}     
		BUTTON_SetFont(buttonF[i].handle,&GUI_FontHZ16);
		memset(tmpStr, 0, sizeof(tmpStr));
		#if _LFN_UNICODE
		cutFileName((TCHAR *)card.gcodeFileList.fileName[j], 16, 8,  tmpStr);
       	#else
		cutFileName((char *)card.gcodeFileList.fileName[j], 16, 8,  (char *)tmpStr);
        #endif        
		if(card.gcodeFileList.fileAttr[j] == 1) { //dir
			BUTTON_SetBmpFileName(buttonF[i].handle, "bmp_dir.bin",1);
			BUTTON_SetBitmapEx(buttonF[i].handle, 0, &bmp_struct,BMP_PIC_X, BMP_PIC_Y);
		} else {
			if(ui_file_with_preview(card.gcodeFileList.fileName[j], &buttonF[i].preview_offset)) {
				BUTTON_SetBmpFileName(buttonF[i].handle, "bmp_file_WPV.bin",1);
				BUTTON_SetBitmapEx(buttonF[i].handle, 0, &bmp_struct,BMP_PIC_X, BMP_PIC_Y);
				buttonF[i].fileName = card.gcodeFileList.fileName[j];
			} else {
				BUTTON_SetBmpFileName(buttonF[i].handle, "bmp_file.bin",1);
				BUTTON_SetBitmapEx(buttonF[i].handle, 0, &bmp_struct,BMP_PIC_X, BMP_PIC_Y);
			}
		}
		BUTTON_SetText(buttonF[i].handle, (char const *)(tmpStr));
	}
	GUI_Exec();

	draw_preview();
	ui_set_encoding();
}

void draw_preview() {
	for(uint8_t i=0;i<FILE_BTN_CNT;i++) {
		if (buttonF[i].fileName) {
			int x;
			int y;
			if (i<3) {
				x = ui_std_col(i);
				y = ui_std_row(0);
			} else {
				x = ui_std_col(i-3);
				y = ui_std_row(1);
			}
			x+=33;
			y+=28 + titleHeight;
			ui_gcode_small_preview(buttonF[i].fileName, buttonF[i].preview_offset, x, y);
		}
	}
}

void clear_print_file() {
	ui_drop_window(hPrintFileWnd);
	GUI_Clear();
}




