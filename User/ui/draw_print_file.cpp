//TODO: убрать диалог подтверждения перед печатью
#include "GUI.h"
#include "BUTTON.h"
#include "PROGBAR.h"
#include "draw_print_file.h"
#include "draw_ui.h"
#include "fontLib.h"
#include "TEXT.h"
#include "ui_tools.h"
#include "tim.h"
#include "ff.h"
#include "fatfs.h"
#include <string.h>
#include <draw_set.h>
#include "sdio.h"

#include "draw_printing.h"
#include "sh_tools.h"

#include "Marlin.h"
#include "configuration_adv.h"
#include "cardreader.h"

#ifndef GUI_FLASH
#define GUI_FLASH
#endif
#define FILE_BTN_CNT    6
GUI_HWIN hPrintFileWnd, hAlertWin, hPrintFileDlgWnd;

uint8_t trig_browser_flag;
int8_t curDirLever = 0;
DIR_OFFSET dir_offset[10];

#if _LFN_UNICODE
TCHAR curFileName[100] = "notValid";
#else
char curFileName[100] = "notValid";
#endif

extern GUI_FLASH const GUI_FONT GUI_FontHZ_fontHz18;
extern int X_ADD,X_INTERVAL;

static int8_t pages;
static TEXT_Handle printDlgAskText;
static BUTTON_STRUCT buttonPu, buttonPd, buttonR, buttonF[FILE_BTN_CNT], buttonOk, buttonCancle;

uint8_t back_flg = 0;	
uint8_t disp_in_file_dir;


static void cbPrintFileWin(WM_MESSAGE * pMsg) {
	switch (pMsg->MsgId) {
		case WM_PAINT:
		case WM_TOUCH:
		case WM_TOUCH_CHILD:
			break;
		case WM_NOTIFY_PARENT:
			if(pMsg->Data.v == WM_NOTIFICATION_RELEASED) {
				if(pMsg->hWinSrc == buttonPd.btnHandle) {
					if((gCfgItems.language != LANG_SIMPLE_CHINESE)&&(gCfgItems.language != LANG_COMPLEX_CHINESE))
						GUI_UC_SetEncodeUTF8();
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
							Clear_print_file();
							disp_udisk_files(0);
							dir_offset[curDirLever].curPage++;
						}

						if(card.gcodeFileList.listVaild == 2)
							dir_offset[curDirLever].cur_page_last_offset = 0;
					}
				} else if(pMsg->hWinSrc == buttonPu.btnHandle) {
					if((gCfgItems.language != LANG_SIMPLE_CHINESE)&&(gCfgItems.language != LANG_COMPLEX_CHINESE))
						GUI_UC_SetEncodeUTF8();
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
							Clear_print_file();
							disp_udisk_files(1);
							dir_offset[curDirLever].curPage--;
						}
				
					}
				} else if(pMsg->hWinSrc == buttonR.btnHandle) {
					if(curDirLever == 0) {
						if((gCfgItems.language == LANG_SIMPLE_CHINESE)||(gCfgItems.language == LANG_COMPLEX_CHINESE)) {
							GUI_SetFont(&GUI_FontHZ16);
							BUTTON_SetDefaultFont(&GUI_FontHZ16);
							TEXT_SetDefaultFont(&GUI_FontHZ16);
							GUI_UC_SetEncodeNone();
						} else {
							GUI_SetFont(&FONT_TITLE);
							BUTTON_SetDefaultFont(&FONT_TITLE);
							TEXT_SetDefaultFont(&FONT_TITLE);
							GUI_UC_SetEncodeUTF8();
						}
						if(gCfgItems.breakpoint_reprint_flg == 1) {
							gCfgItems.breakpoint_reprint_flg = 0;
							last_disp_state = SET_UI;
							Clear_print_file();
							card.Sd_file_offset = 0;
							card.Sd_file_cnt = 0;
							draw_return_ui();
						} else {
							last_disp_state = PRINT_FILE_UI;
							Clear_print_file();
							card.Sd_file_offset = 0;
							card.Sd_file_cnt = 0;
							draw_ready_print();
						}
						disp_in_file_dir = 0;
					} else {
						int8_t *ch = 0;
						ch =  (int8_t *)strrchr((char *)card.gCurDir, '/');
						if(ch != 0) {
							*ch = 0;
							Clear_print_file();
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
						if(pMsg->hWinSrc == buttonF[i].btnHandle) {
							if(card.gcodeFileList.fileName[j][0] != 0) {
								if(card.gcodeFileList.fileAttr[j] == 1) { //dir
									memset(card.gCurDir, 0, sizeof(card.gCurDir));
									strcpy((char *)card.gCurDir,  (const char *)card.gcodeFileList.fileName[j]);
									Clear_print_file();
									curDirLever++;
									search_files();
								} else {
									memset(curFileName, 0, sizeof(curFileName));
									#if _LFN_UNICODE
									wcscpy((wchar_t *)curFileName, (const wchar_t*)card.gcodeFileList.fileName[j]);
									#else
									strcpy(curFileName, (const char *)card.gcodeFileList.fileName[j]);
									#endif
									if((gCfgItems.language != LANG_SIMPLE_CHINESE)&&(gCfgItems.language != LANG_COMPLEX_CHINESE))
										GUI_UC_SetEncodeUTF8();
									Clear_print_file();
									disp_in_file_dir = 0;

									if (is_filament_fail()) {
										draw_dialog(DIALOG_TYPE_FILAMENT_NO_PRESS);
									} else {
										ui_start_print_file();
										draw_printing();
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
	ui_clear_screen();

	GUI_DispStringAt(creat_title_text(),  TITLE_XPOS, TITLE_YPOS);
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
	if((gCfgItems.language != LANG_SIMPLE_CHINESE)&&(gCfgItems.language != LANG_COMPLEX_CHINESE))
		GUI_UC_SetEncodeUTF8();

	GUI_SetColor(gCfgItems.title_color);
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
					disp_in_file_dir = 0;
					if(gCfgItems.breakpoint_reprint_flg == 1) {
					    gCfgItems.breakpoint_reprint_flg = 0;
						last_disp_state = SET_UI;
						Clear_print_file();
						draw_return_ui();
					} else {
						last_disp_state = PRINT_FILE_UI;
						Clear_print_file();
						draw_ready_print();
					}
				} else {
					int8_t *ch = 0;
					ch =  (int8_t *)strrchr((const char *)card.gCurDir, '/');
					if(ch != 0) {
						*ch = 0;
						Clear_print_file();
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
	buttonPu.btnHandle = ui_create_100_80_button(OTHER_BTN_XPIEL*3+INTERVAL_V*4,0,hPrintFileWnd, "bmp_page_up.bin", 0);
	buttonPd.btnHandle = ui_create_100_80_button(OTHER_BTN_XPIEL*3+INTERVAL_V*4,OTHER_BTN_YPIEL+INTERVAL_H,hPrintFileWnd, "bmp_page_down.bin", 0);
	buttonR.btnHandle = ui_create_100_80_button(OTHER_BTN_XPIEL*3+INTERVAL_V*4,OTHER_BTN_YPIEL*2+INTERVAL_H*2, hPrintFileWnd, "bmp_page_back.bin", 0);
	GUI_Exec();

	GUI_UC_SetEncodeNone();
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
			buttonF[i].btnHandle = BUTTON_CreateEx(BTN_X_PIXEL*i+INTERVAL_V*(i+1),  0, BTN_X_PIXEL, BTN_Y_PIXEL,hPrintFileWnd, BUTTON_CF_SHOW, 0, 103 + i);
		} else {
			buttonF[i].btnHandle = BUTTON_CreateEx(BTN_X_PIXEL*(i-3)+INTERVAL_V*((i-3)+1),  BTN_Y_PIXEL+INTERVAL_H, BTN_X_PIXEL, BTN_Y_PIXEL,hPrintFileWnd, BUTTON_CF_SHOW, 0, 107 + i);
		}     
		BUTTON_SetFont(buttonF[i].btnHandle,&GUI_FontHZ16);
		memset(tmpStr, 0, sizeof(tmpStr));
		#if _LFN_UNICODE
		cutFileName((TCHAR *)card.gcodeFileList.fileName[j], 16, 8,  tmpStr);
       	#else
		cutFileName((char *)card.gcodeFileList.fileName[j], 16, 8,  (char *)tmpStr);
        #endif        
		if(card.gcodeFileList.fileAttr[j] == 1) { //dir
			BUTTON_SetBmpFileName(buttonF[i].btnHandle, "bmp_dir.bin",1);
			BUTTON_SetBitmapEx(buttonF[i].btnHandle, 0, &bmp_struct,BMP_PIC_X, BMP_PIC_Y);
		} else {
			BUTTON_SetBmpFileName(buttonF[i].btnHandle, "bmp_file.bin",1);
			BUTTON_SetBitmapEx(buttonF[i].btnHandle, 0, &bmp_struct,BMP_PIC_X, BMP_PIC_Y);
		}
		BUTTON_SetText(buttonF[i].btnHandle, (char const *)(tmpStr));
	}
	GUI_Exec();
	if((gCfgItems.language != LANG_SIMPLE_CHINESE)&&(gCfgItems.language != LANG_COMPLEX_CHINESE))
	GUI_UC_SetEncodeUTF8();
}

void Clear_print_file() {
	ui_drop_window(hPrintFileWnd);
	GUI_Clear();
}




