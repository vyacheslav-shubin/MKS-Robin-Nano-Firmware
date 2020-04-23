#include "ui_tools.h"
#include "draw_ui.h"
#include "marlin.h"
#include "planner.h"
#include "cardreader.h"
#include "tim.h"

extern uint8_t	ui_suicide_enabled = 0;
extern CardReader card;
UPLOAD_INFO upload_file_info = {0, 0};

uint8_t once_flag = 0; //printing


void ui_start_print_process(void) {
	if(card.openFile(curFileName, true)) {
		feedrate_percentage = 100;
		saved_feedrate_percentage = feedrate_percentage;
		planner.flow_percentage[0] = 100;
		planner.e_factor[0]= planner.flow_percentage[0]*0.01;
		if(mksCfg.extruders==2) {
			planner.flow_percentage[1] = 100;
			planner.e_factor[1]= planner.flow_percentage[1]*0.01;
		}
		card.startFileprint();
		once_flag = 0;
	}
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
		//GUI_Exec();
	}
}

void ui_initialize_screen_gui(void) {
	GUI_UC_SetEncodeNone();
	GUI_SetFont(&GUI_FontHZ16);
	GUI_DispStringAt(creat_title_text(),  TITLE_XPOS, TITLE_YPOS);
    if((gCfgItems.language == LANG_SIMPLE_CHINESE)||(gCfgItems.language == LANG_COMPLEX_CHINESE))
    {
      GUI_SetFont(&GUI_FontHZ16);
      BUTTON_SetDefaultFont(&GUI_FontHZ16);
      TEXT_SetDefaultFont(&GUI_FontHZ16);
      GUI_UC_SetEncodeNone();
    }
    else
    {
      GUI_SetFont(&FONT_TITLE);
      BUTTON_SetDefaultFont(&FONT_TITLE);
      TEXT_SetDefaultFont(&FONT_TITLE);
      GUI_UC_SetEncodeUTF8();
    }
}


BUTTON_Handle ui_create_std_button(int x, int y, WM_HWIN hWinParent, char *pFile, const char* text) {
	BUTTON_Handle btn = BUTTON_CreateEx(x, y, BTN_X_PIXEL, BTN_Y_PIXEL, hWinParent, BUTTON_CF_SHOW, 0, alloc_win_id());
	BUTTON_SetBmpFileName(btn, pFile, 1);
	BUTTON_SetBitmapEx(btn, 0, &bmp_struct, BMP_PIC_X, BMP_PIC_Y);
	if(gCfgItems.multiple_language != 0)
		BUTTON_SetText(btn, text);
	return btn;
}

BUTTON_Handle ui_create_150_80_button(int x, int y, WM_HWIN hWinParent, char *pFile, const char* text) {
	BUTTON_Handle btn = BUTTON_CreateEx(x, y, 150, 80, hWinParent, BUTTON_CF_SHOW, 0, alloc_win_id());
	BUTTON_SetBmpFileName(btn, pFile,1);
	BUTTON_SetBkColor(btn, BUTTON_CI_PRESSED, gCfgItems.btn_color);
	BUTTON_SetBkColor(btn, BUTTON_CI_UNPRESSED, gCfgItems.btn_color);
	BUTTON_SetTextColor(btn, BUTTON_CI_PRESSED, gCfgItems.btn_textcolor);
	BUTTON_SetTextColor(btn, BUTTON_CI_UNPRESSED, gCfgItems.btn_textcolor);
	BUTTON_SetBitmapEx(btn, 0, &bmp_struct_150, 0, 0);
	BUTTON_SetTextAlign(btn, GUI_TA_VCENTER | GUI_CUSTOM_POS);
	if(gCfgItems.multiple_language != 0)
		BUTTON_SetText(btn, text);
	return btn;
}

BUTTON_Handle ui_create_state_button_id(int x, int y, WM_HWIN hWinParent, char *pFile, uint32_t id) {
	BUTTON_Handle btn = BUTTON_CreateEx(x, y, STATE_PIC_X_PIXEL, STATE_PIC_Y_PIXEL, hWinParent, BUTTON_CF_SHOW, 0, id);
	BUTTON_SetBmpFileName(btn, pFile,0);
	BUTTON_SetBitmapEx(btn, 0, &bmp_struct_50, 0, 0);
	return btn;
}


BUTTON_Handle ui_create_state_button(int x, int y, WM_HWIN hWinParent, char *pFile) {
	return ui_create_state_button_id(x, y, hWinParent, pFile, alloc_win_id());
}

TEXT_Handle ui_create_std_text(int x, int y, int w, int h, WM_HWIN hWinParent, char *text) {
	TEXT_Handle res;
	res = TEXT_CreateEx(x, y, w, h, hWinParent, WM_CF_SHOW, TEXT_CF_LEFT|TEXT_CF_VCENTER,  GUI_ID_TEXT0, text?text:" ");
	TEXT_SetBkColor(res,  gCfgItems.background_color);
	TEXT_SetTextColor(res, gCfgItems.title_color);
	return res;
}


void ui_set_text_value(TEXT_Handle handle, char* val) {
	TEXT_SetBkColor(handle,gCfgItems.background_color);
	TEXT_SetTextColor(handle,gCfgItems.title_color);
	TEXT_SetText(handle, val);
}

void ui_timings(void) {
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

	//TODO: К херам (!!)
	temperature_change_frequency_cnt++;
	if((temperature_change_frequency_cnt>=2000) && (temperature_change_frequency!=1)) {
		temperature_change_frequency_cnt = 0;
		temperature_change_frequency = 1;
	}

}

