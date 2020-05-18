#include "ui_tools.h"
#include "GUI.h"
#include "Application.h"
#include "draw_ui.h"
#include "Marlin.h"
#include "planner.h"
#include "cardreader.h"
#include "tim.h"
#include "ff.h"
#include "ili9320.h"

extern CardReader card;
FATFS fs;

char ui_buf1_100[100];

UPLOAD_INFO upload_file_info = {0, 0};

UI_PRINT_PROCESS ui_print_process = {0, {0, 0, SUICIDE_WAIT}, 0, {0}};

const char* FAN_STATES[3] {img_fan_state0, img_fan_state1, img_fan_state2};

//не нужна!!
void reset_file_info() {
	ui_print_process.rate = 0;
}

void print_time_to_str(PRINT_TIME * pt, char * buf) {
	sprintf(buf, "%d%d:%d%d:%d%d", pt->hours/10, pt->hours%10, pt->minutes/10, pt->minutes%10,  pt->seconds/10, pt->seconds%10);
}

void ui_start_print_process(void) {
	if(card.openFile(ui_print_process.file_name, true)) {
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

void ui_start_print_file() {
	reset_print_time();
	start_print_time();
	ui_print_process.rate = 0;
	ui_print_process.preview_state_flags = 0;
	if(gCfgItems.breakpoint_reprint_flg == 1)
		gCfgItems.breakpoint_z_pos = current_position[Z_AXIS];
	char has_preview = 0;
	epr_write_data(EPR_PREVIEW_FROM_FLASH, &has_preview,1);
	ui_start_print_process();
}

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


void ui_pop_disp_stack(void) {
	disp_state_stack._disp_index--;
	disp_state = disp_state_stack._disp_state[disp_state_stack._disp_index];
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


WM_HWIN ui_std_init_window(DISP_STATE ui_id, WM_CALLBACK* cb) {
	ui_push_disp_stack(ui_id);
	ui_init_page();
	return ui_std_window(cb);
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

void ui_update_std_button(BUTTON_Handle btn, const char* file, char* title) {
	ui_buttonpreset(btn);
	if (file!=0) {
		BUTTON_SetBmpFileName(btn, file, 1);
		BUTTON_SetBitmapEx(btn, 0, &bmp_struct, BMP_PIC_X, BMP_PIC_Y);
	}
	BUTTON_SetText(btn, title);
}



BUTTON_Handle ui_create_150_80_button(int x, int y, WM_HWIN hWinParent, const char *pFile, const char* text) {
	BUTTON_Handle btn = BUTTON_CreateEx(x, y, 150, 80, hWinParent, BUTTON_CF_SHOW, 0, alloc_win_id());
	ui_buttonpreset(btn);
	BUTTON_SetBmpFileName(btn, pFile,1);
	BUTTON_SetBitmapEx(btn, 0, &bmp_struct_150, 0, 0);
	BUTTON_SetTextAlign(btn, GUI_TA_VCENTER | GUI_CUSTOM_POS);
	BUTTON_SetText(btn, text);
	return btn;
}

GUI_BITMAP bmp_struct_100x80 = { 100, 80, 160, 16, (unsigned char *)bmp_public_buf,  0, GUI_DRAW_BMPM565};


BUTTON_Handle ui_create_100_80_button(int x, int y, WM_HWIN hWinParent, const char *pFile) {
	BUTTON_Handle btn = BUTTON_CreateEx(x, y, 100, 80, hWinParent, BUTTON_CF_SHOW, 0, alloc_win_id());
	ui_buttonpreset(btn);
	BUTTON_SetBmpFileName(btn, pFile,1);
	BUTTON_SetBitmapEx(btn, 0, &bmp_struct_100x80, 0, 0);
	return btn;
}

BUTTON_Handle ui_create_check_button(int x, int y, WM_HWIN hWinParent, uint8_t state) {
	BUTTON_Handle btn = BUTTON_CreateEx(x, y, 90, 40, hWinParent, BUTTON_CF_SHOW, 0, alloc_win_id());
	ui_buttonpreset(btn);
	BUTTON_SetTextAlign(btn,GUI_TA_HCENTER|GUI_TA_VCENTER);
	ui_update_check_button(btn, state);
    return btn;
}


void ui_update_check_button(BUTTON_Handle  btn, uint8_t state) {
	BUTTON_SetBmpFileName(btn, state?"bmp_enable.bin":"bmp_disable.bin", 1);
    BUTTON_SetBitmapEx(btn,0,&bmp_struct90X30, 0, 5);
	BUTTON_SetText(btn, state?machine_menu.enable:machine_menu.disable);
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

TEXT_Handle ui_create_std_text_f(int x, int y, int w, int h, WM_HWIN hWinParent, int flags, char *text) {
	TEXT_Handle res = TEXT_CreateEx(x, y, w, h, hWinParent, WM_CF_SHOW, flags,  GUI_ID_TEXT0, 0);
	TEXT_SetBkColor(res,  gCfgItems.background_color);
	TEXT_SetTextColor(res, gCfgItems.title_color);
	return res;
}

TEXT_Handle ui_create_dialog_text(int x, int y, int w, int h, WM_HWIN hWinParent, char *text) {
	return ui_create_std_text_f(x, y, w, h, hWinParent, TEXT_CF_HCENTER|TEXT_CF_VCENTER, text);
}

TEXT_Handle ui_create_std_text(int x, int y, int w, int h, WM_HWIN hWinParent, char *text) {
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

void ui_timings(void) {
	//TODO: добавить разрешение таймингов, тогда можно будет все завязать на этот механизм

	if(!(TimeIncrease * TICK_CYCLE % 500))	// 0.5 sec
		ui_timing_set(F_UI_TIMING_HALF_SEC);

	if(!(TimeIncrease * TICK_CYCLE % 1000)) { //1 sec
		ui_timing_set(F_UI_TIMING_SEC);
		if(print_time.start == 1) {
			print_time.seconds++;
			if(print_time.seconds >= 60) {
				print_time.seconds = 0;
				print_time.minutes++;
				if(print_time.minutes >= 60) {
					print_time.minutes = 0;
					print_time.hours++;
				}
			}
		}
	}

	//TODO: К херам (!!) достаточно секундных обновлений
	temperature_change_frequency_cnt++;
	if((temperature_change_frequency_cnt>=2000) && (temperature_change_frequency!=1)) {
		temperature_change_frequency_cnt = 0;
		temperature_change_frequency = 1;
	}

}

void ui_make_page_navigator(WM_HWIN hWin, UI_PAGE_NAVIGATOR * navigator) {
	navigator->button_back = BUTTON_CreateEx(400, 230, 70, 40, hWin, BUTTON_CF_SHOW, 0, alloc_win_id());
    BUTTON_SetBmpFileName(navigator->button_back, "bmp_back70x40.bin",1);
    BUTTON_SetBitmapEx(navigator->button_back, 0, &bmp_struct70X40,0, 0);
    navigator->button_next = 0;
    navigator->button_previous = 0;

    if (navigator->page_count>1) {
    	if (navigator->page < navigator->page_count-1)
    		navigator->button_next = BUTTON_CreateEx(320, 230, 70, 40, hWin, BUTTON_CF_SHOW, 0, alloc_win_id());
    	if (navigator->page > 0) {
    		navigator->button_previous = BUTTON_CreateEx(navigator->button_next?240:320, 230, 70, 40, hWin, BUTTON_CF_SHOW, 0, alloc_win_id());
    	}
    }

	if (navigator->button_next!=0) {
		BUTTON_SetBmpFileName(navigator->button_next, "bmp_next70x40.bin",1);
		BUTTON_SetBitmapEx(navigator->button_next, 0, &bmp_struct70X40,0, 0);
	}

	if (navigator->button_previous!=0) {
		BUTTON_SetBmpFileName(navigator->button_previous, "bmp_prev70x40.bin",1);
		BUTTON_SetBitmapEx(navigator->button_previous, 0, &bmp_struct70X40,0, 0);
	}
}

#define ROW(idx) (10+50*idx)
#define X_TEXT 10
#define X_RADIO 370
#define X_ARROW 460

void ui_make_check_pair(int row, WM_HWIN hWin, UI_CHECK_PAIR * pair, char* title, uint8_t state) {
	pair->button_text = BUTTON_CreateEx(X_TEXT, ROW(row), 240, 40, hWin, BUTTON_CF_SHOW, 0, alloc_win_id());
    BUTTON_SetTextAlign(pair->button_text,GUI_TA_LEFT|GUI_TA_VCENTER);
    BUTTON_SetText(pair->button_text, title);
    pair->button_check = ui_create_check_button(X_RADIO, ROW(row), hWin, state);
}

void ui_make_half_row_check_pair(int col, int row, WM_HWIN hWin, UI_CHECK_PAIR * pair, char* title, uint8_t state) {
	pair->button_text = BUTTON_CreateEx(X_TEXT + 240 * col , ROW(row), 240-60, 40, hWin, BUTTON_CF_SHOW, 0, alloc_win_id());
    BUTTON_SetTextAlign(pair->button_text,GUI_TA_LEFT|GUI_TA_VCENTER);
    BUTTON_SetText(pair->button_text, title);
    pair->button_check = ui_create_check_button(240*(col+1)-110, ROW(row), hWin, state);
}


void ui_make_arrow_pair(int row, WM_HWIN hWin, UI_ARROW_PAIR * pair, char* title) {
	pair->button_text = BUTTON_CreateEx(X_TEXT, ROW(row), 450, 40, hWin, BUTTON_CF_SHOW, 0, alloc_win_id());
    BUTTON_SetTextAlign(pair->button_text,GUI_TA_LEFT|GUI_TA_VCENTER);
    BUTTON_SetText(pair->button_text, title);
    pair->button_arrow = BUTTON_CreateEx(X_ARROW,ROW(row),10,40,hWin, BUTTON_CF_SHOW, 0, alloc_win_id());
    BUTTON_SetBmpFileName(pair->button_arrow, "bmp_arrow.bin",1);
    BUTTON_SetBitmapEx(pair->button_arrow,0,&bmp_struct10x17,BMP_PIC_X,12);
}

void ui_draw_config_lines() {
	GUI_SetColor(0xff5449);
	GUI_FillRect(10, 50, 470, 50);
	GUI_FillRect(10, 100, 470, 100);
	GUI_FillRect(10, 150, 470, 150);
	GUI_FillRect(10, 200, 470, 200);
}

extern void ui_draw_config_half_lines() {
	GUI_SetColor(0xff5449);
	GUI_FillRect(10, 50, 230, 50);
	GUI_FillRect(10, 100, 230, 100);
	GUI_FillRect(10, 150, 230, 150);
	GUI_FillRect(10, 200, 230, 200);

	GUI_FillRect(250, 50, 470, 50);
	GUI_FillRect(250, 100, 470, 100);
	GUI_FillRect(250, 150, 470, 150);
	GUI_FillRect(250, 200, 470, 200);
}


uint8_t ui_file_with_preview(char *path, int *withoffset) {
	#define PREPERD_SIZE 512
	FIL file;
	if (f_open(&file, path, FA_OPEN_EXISTING | FA_READ) != FR_OK)
		return 0;
	memset(bmp_public_buf, 0, PREPERD_SIZE+1);
	UINT readed;
	f_read(&file,bmp_public_buf,PREPERD_SIZE,&readed);
	if (readed != PREPERD_SIZE)
		return 0;
	f_close(&file);
	char * pos = strstr(bmp_public_buf,";simage:");
	if (pos==0)
		return 0;
	else {
		*withoffset = (int)pos-(int)bmp_public_buf;
		return 1;
	}
}

void ui_gcode_small_preview(char *file_name, int offset, int xpos_pixel,int ypos_pixel) {
	FIL file;
	int res = f_open(&file, file_name, FA_OPEN_EXISTING | FA_READ);
	if(res != FR_OK)
		return;
	f_lseek(&file, offset);
	LCD_setWindowArea(xpos_pixel, ypos_pixel, 50, 50);
	LCD_WriteRAM_Prepare();
	for (uint8_t i=0;i<50;i++) {
		UINT read;
		f_read(&file, bmp_public_buf, 209, &read);
		if (read!=209)
			break;
		for(uint8_t j=8; j<208; j+=4) {
			uint16_t color = (ascii2dec(bmp_public_buf[j+2])<<12) | (ascii2dec(bmp_public_buf[j+3])<<8) | (ascii2dec(bmp_public_buf[j])<<4) | (ascii2dec(bmp_public_buf[j+1]));
			LCD_WriteRAM(color);
		}
	}
	f_close(&file);
}

void ui_update_fan_button(BUTTON_Handle button, TEXT_Handle text) {
	static uint8_t fan_state = 0;
	if (fanSpeeds[0]>1) {
		fan_state++;
		if (fan_state>2)
			fan_state = 0;
		ui_update_state_button(button, FAN_STATES[fan_state]);
	}
	long fs = fanSpeeds[0] * 100;
	uint8_t pr=fs/255;
	if ((pr==0) && (fanSpeeds[0]>0))
		pr = 1;
	sprintf(ui_buf1_20, "%d/%d%%", fanSpeeds[0], pr);
	ui_set_text_value(text, ui_buf1_20);

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

