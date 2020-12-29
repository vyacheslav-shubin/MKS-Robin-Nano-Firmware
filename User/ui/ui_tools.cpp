#include "ui_tools.h"
#include "GUI.h"
#include "Application.h"
#include "draw_ui.h"
#include "Marlin.h"
#include "planner.h"
#include "cardreader.h"
#include "ff.h"
#include "fatfs.h"
#include "ili9320.h"
#include "integration.h"

extern CardReader card;
FATFS fs;

char ui_buf1_100[100];

UPLOAD_INFO upload_file_info = {0, 0};

UI_PRINT_PROCESS ui_print_process = {0, 0, 0, {0}};

const char* FAN_STATES[3] {img_fan_state0, img_fan_state1, img_fan_state2};

//не нужна!!
void reset_file_info() {
	ui_print_process.rate = 0;
}

void print_time_to_str(PRINT_TIME * pt, char * buf) {
	sprintf(buf, "%d%d:%d%d:%d%d", pt->hours/10, pt->hours%10, pt->minutes/10, pt->minutes%10,  pt->seconds/10, pt->seconds%10);
}

/*
void ui_start_print_process(void) {
	if(card.openFile(ui_print_process.file_name, true)) {
        filament_counter = 0;
	    feedrate_percentage = 100;
		saved_feedrate_percentage = feedrate_percentage;
		planner.flow_percentage[0] = 100;
		planner.e_factor[0]= planner.flow_percentage[0]*0.01;
		if(mksCfg.extruders==2) {
			planner.flow_percentage[1] = 100;
			planner.e_factor[1]= planner.flow_percentage[1]*0.01;
		}
		card.startFileprint();
		ui_print_process.once = 0;
	}
}
*/

void ui_push_disp_stack(DISP_STATE ui_id) {
    if(disp_state_stack._disp_state[disp_state_stack._disp_index] != ui_id)
    {
        disp_state_stack._disp_index++;
        disp_state_stack._disp_state[disp_state_stack._disp_index] = ui_id;
    }
    disp_state = ui_id;
}

void ui_reset_disp_stack(DISP_STATE ui_id) {
	disp_state_stack._disp_index = 0;
	memset(disp_state_stack._disp_state, 0, sizeof(disp_state_stack._disp_state));
	disp_state_stack._disp_state[disp_state_stack._disp_index] = ui_id;
	disp_state = ui_id;
}


void ui_clear_screen(void) {
	GUI_SetBkColor(gCfgItems.background_color);
	GUI_SetColor(gCfgItems.title_color);
	GUI_Clear();
}

void ui_drop_window(WM_HWIN wnd) {
	GUI_SetBkColor(gCfgItems.background_color);
	if(WM_IsWindow(wnd)) {
		WM_DeleteWindow(wnd);
	}
}

void ui_set_encoding(void) {
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
}


void ui_init_page(void) {
	ui_clear_screen();
	ui_set_encoding();
	ui_app.drawTitle();
}



void ui_buttonpreset(BUTTON_Handle btn) {
	BUTTON_SetBkColor(btn, BUTTON_CI_PRESSED, gCfgItems.btn_color);
	BUTTON_SetBkColor(btn, BUTTON_CI_UNPRESSED, gCfgItems.btn_color);
	BUTTON_SetTextColor(btn, BUTTON_CI_PRESSED, gCfgItems.btn_textcolor);
	BUTTON_SetTextColor(btn, BUTTON_CI_UNPRESSED, gCfgItems.btn_textcolor);
}

BUTTON_Handle ui_create_std_button(int x, int y, WM_HWIN hWinParent, const char *pFile, const char* text) {
	BUTTON_Handle btn = BUTTON_CreateEx(x, y, BTN_X_PIXEL, BTN_Y_PIXEL, hWinParent, WM_CF_SHOW, 0, alloc_win_id());
	ui_update_std_button(btn, pFile, text);
	return btn;
}

void ui_update_std_button(BUTTON_Handle btn, const char* file, const char* title) {
	ui_buttonpreset(btn);
	if (file!=0) {
		BUTTON_SetBmpFileName(btn, file, 1);
		BUTTON_SetBitmapEx(btn, 0, &bmp_struct, BMP_PIC_X, BMP_PIC_Y);
	}
	BUTTON_SetText(btn, title);
}


GUI_BITMAP bmp_struct_96x80 = { 96, 80, 96*2, 16, (unsigned char *)bmp_public_buf,  0, GUI_DRAW_BMPM565};
GUI_BITMAP bmp_struct_50x50 = { 50, 50, 100, 16, (unsigned char *)bmp_public_buf,  0, GUI_DRAW_BMPM565};
GUI_BITMAP bmp_struct_70x50 = { 70, 50, 140, 16, (unsigned char *)bmp_public_buf,  0, GUI_DRAW_BMPM565};
GUI_BITMAP bmp_struct_70x28 = { 70, 28, 140, 16, (unsigned char *)bmp_public_buf,  0, GUI_DRAW_BMPM565};
GUI_BITMAP bmp_struct_30x30 = { 30, 30, 60, 16, (unsigned char *)bmp_public_buf,  0, GUI_DRAW_BMPM565};

BUTTON_Handle ui_create_96_80_button(int x, int y, WM_HWIN hWinParent, const char *pFile) {
	BUTTON_Handle btn = BUTTON_CreateEx(x, y, 96, 80, hWinParent, BUTTON_CF_SHOW, 0, alloc_win_id());
	ui_buttonpreset(btn);
	BUTTON_SetBmpFileName(btn, pFile,1);
	BUTTON_SetBitmapEx(btn, 0, &bmp_struct_96x80, 0, 0);
	return btn;
}


void ui_update_state_button(BUTTON_Handle btn, const char *pFile) {
	BUTTON_SetBmpFileName(btn, pFile, 0);
	BUTTON_SetBitmapEx(btn, 0, &bmp_struct_50, 0, 0);
}


BUTTON_Handle ui_create_state_button(int x, int y, WM_HWIN hWinParent, const char *pFile) {
	BUTTON_Handle btn = BUTTON_CreateEx(x, y, STATE_PIC_X_PIXEL, STATE_PIC_Y_PIXEL, hWinParent, BUTTON_CF_SHOW, 0, alloc_win_id());
	ui_update_state_button(btn, pFile);
	return btn;
}


BUTTON_Handle ui_create_dialog_button(int x, int y, WM_HWIN hWinParent, const char* text) {
	BUTTON_Handle btn = BUTTON_CreateEx(x, y, 140,50, hWinParent, BUTTON_CF_SHOW, 0, alloc_win_id());
	BUTTON_SetText(btn, text);
	BUTTON_SetBkColor(btn, BUTTON_CI_UNPRESSED, gCfgItems.dialog_btn_color);
	BUTTON_SetBkColor(btn, BUTTON_CI_PRESSED, gCfgItems.dialog_btn_color);
	BUTTON_SetTextColor(btn, BUTTON_CI_UNPRESSED, gCfgItems.dialog_btn_textcolor);
	BUTTON_SetTextColor(btn, BUTTON_CI_PRESSED, gCfgItems.dialog_btn_textcolor);
	BUTTON_SetTextAlign(btn, GUI_TA_VCENTER | GUI_TA_HCENTER);
	BUTTON_SetBmpFileName(btn, 0, 1);
	return btn;
}

TEXT_Handle ui_create_std_text_f(int x, int y, int w, int h, WM_HWIN hWinParent, int flags, const char *text) {
	TEXT_Handle res = TEXT_CreateEx(x, y, w, h, hWinParent, WM_CF_SHOW, flags,  GUI_ID_TEXT0, 0);
	TEXT_SetBkColor(res,  gCfgItems.background_color);
	TEXT_SetTextColor(res, gCfgItems.title_color);
	TEXT_SetText(res, text);
	return res;
}

TEXT_Handle ui_create_dialog_text(int x, int y, int w, int h, WM_HWIN hWinParent, const char *text) {
	return ui_create_std_text_f(x, y, w, h, hWinParent, TEXT_CF_HCENTER|TEXT_CF_VCENTER, text);
}

TEXT_Handle ui_create_std_text(int x, int y, int w, int h, WM_HWIN hWinParent, const char *text) {
	return ui_create_std_text_f(x, y, w, h, hWinParent, TEXT_CF_LEFT|TEXT_CF_VCENTER, text);
}

extern PROGBAR_Handle ui_create_std_progbar(int x, int y, int w, int h, WM_HWIN hWndParent) {
	PROGBAR_Handle bar = PROGBAR_CreateEx(x, y, w, h, hWndParent, WM_CF_SHOW, 0, 0);
	PROGBAR_SetBarColor(bar, 0, gCfgItems.printing_bar_color_left);
	PROGBAR_SetBarColor(bar, 1, gCfgItems.printing_bar_color_right);
	PROGBAR_SetTextColor(bar, 0, gCfgItems.printing_bar_text_color_left);
	PROGBAR_SetTextColor(bar, 1, gCfgItems.printing_bar_text_color_right);
	PROGBAR_SetFont(bar, &FONT_TITLE);
	return bar;
}


void ui_set_text_value(TEXT_Handle handle, char* val) {
	TEXT_SetBkColor(handle,gCfgItems.background_color);
	TEXT_SetTextColor(handle,gCfgItems.title_color);
	TEXT_SetText(handle, val);
}


#define ROW(idx) (10+50*idx)

void ui_file_check_preview(char *path, PREVIEW_META *meta) {
    #define PREPERD_SIZE 1024
    FIL file;
    meta->mode = PREVIEW_NONE;
    int res = f_open(&file, path, FA_OPEN_EXISTING | FA_READ);
    if (res == FR_OK) {
        memset(bmp_public_buf, 0, PREPERD_SIZE+1);
        UINT readed;
        f_read(&file, bmp_public_buf, PREPERD_SIZE, &readed);
        f_close(&file);
        if (readed == PREPERD_SIZE) {
            char * pos = strstr(bmp_public_buf,";simage:");
            if (pos != 0) {
                meta->offset = (unsigned long)pos-(unsigned long)bmp_public_buf;
                unsigned short i = meta->offset;
                pos = strchr(&bmp_public_buf[meta->offset], '\r');
                if (pos != 0) {
                    unsigned int len = (unsigned long)pos - (unsigned long)(&bmp_public_buf) - meta->offset;
                    if (len==208) {
                        meta->mode = PREVIEW_50;
                    } else if (len==408) {
                        meta->mode = PREVIEW_100;
                    } else
                        meta->mode = PREVIEW_NONE;
                } else {
                    meta->mode = PREVIEW_NONE;
                }
            }
        }
    } else {
        SERIAL_ECHOLNPAIR("PREVIEW FILE OPEN ERROR:", path);
        SERIAL_ECHOLNPAIR("ERROR:", res);
    }
}


unsigned char ui_file_with_preview(char *path, int *withoffset) {
	#define PREPERD_SIZE 512
	FIL file;
	if (f_open(&file, path, FA_OPEN_EXISTING | FA_READ) == FR_OK) {
		memset(bmp_public_buf, 0, PREPERD_SIZE+1);
		UINT readed;
		f_read(&file, bmp_public_buf, PREPERD_SIZE, &readed);
		f_close(&file);
		if (readed == PREPERD_SIZE) {
			char * pos = strstr(bmp_public_buf,";simage:");
			if (pos!=0) {
				*withoffset = (unsigned long)pos-(unsigned long)bmp_public_buf;
				return 1;
			}
		}
	}
	return 0;
}

void ui_gcode_small_preview(char *file_name, PREVIEW_META * meta, int xpos_pixel,int ypos_pixel) {
	FIL file;
	if (f_open(&file, file_name, FA_OPEN_EXISTING | FA_READ) == FR_OK) {
		f_lseek(&file, meta->offset);
		if (meta->mode==PREVIEW_50) {
            LCD_setWindowArea(xpos_pixel, ypos_pixel, 50, 50);
            LCD_WriteRAM_Prepare();
            for (u8 i=0; i< 50; i++) {
                UINT read;
                f_read(&file, bmp_public_buf, 209, &read);
                if (read!=209)
                    break;
                for(u8 j=8; j<208; j+=4) {
                    u16 color = (ascii2dec(bmp_public_buf[j+2])<<12) | (ascii2dec(bmp_public_buf[j+3])<<8) | (ascii2dec(bmp_public_buf[j])<<4) | (ascii2dec(bmp_public_buf[j+1]));
                    LCD_WriteRAM(color);
                }
            }
		} else if (meta->mode==PREVIEW_100) {
            LCD_setWindowArea(xpos_pixel, ypos_pixel, 100, 100);
            LCD_WriteRAM_Prepare();
            for (unsigned char i=0; i < 100; i++) {
                UINT read;
                f_read(&file, bmp_public_buf, 409, &read);
                if (read!=409)
                    break;
                unsigned char k = 0;
                for(unsigned short j=8; j<408; j+=4, k++) {
                    //if (((i & 1)==0) && ((k & 1)==0)) {
                        u16 color = (ascii2dec(bmp_public_buf[j + 2]) << 12) | (ascii2dec(bmp_public_buf[j + 3]) << 8) |
                                    (ascii2dec(bmp_public_buf[j]) << 4) | (ascii2dec(bmp_public_buf[j + 1]));
                        LCD_WriteRAM(color);
                    //}
                }
            }
		}
		f_close(&file);
	}
}



void ui_update_fan_button_text(TEXT_Handle text) {
    //sprintf(ui_buf1_20, "%d/%d%%", fanSpeeds[0], (int)get_fan_percent());
    sprintf(ui_buf1_20, "%d%%", (int)shUI::fan_get_percent());
    ui_set_text_value(text, ui_buf1_20);
}

void ui_update_fan_button(BUTTON_Handle button, TEXT_Handle text) {
	static uint8_t fan_state = 0;
	if (fanSpeeds[0]>1) {
		fan_state++;
		if (fan_state>2)
			fan_state = 0;
		ui_update_state_button(button, FAN_STATES[fan_state]);
	}
    ui_update_fan_button_text(text);
}


char ascii2dec(char ascii) {
	int result = 0;
	if(ascii == 0)
		return 0;
	if(ascii >= '0' && ascii <= '9')
		result = ascii - '0';
	else if(ascii >= 'a' && ascii <= 'f')
		result = ascii - 'a' + 0x0a;
	else if(ascii >= 'A' && ascii <= 'F')
		result = ascii - 'A' + 0x0a;
	else
		return 0;
	return result;
}

