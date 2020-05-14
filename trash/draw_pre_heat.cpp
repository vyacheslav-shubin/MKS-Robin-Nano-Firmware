#include "../../trash/draw_pre_heat.h"

#include "GUI.h"
#include "BUTTON.h"
#include "UI.h"
#include "draw_ui.h"
#include "ui_tools.h"
#include "fontLib.h"
#include "TEXT.h"
#include "mks_cfg.h"

#include "temperature.h"
#include "mks_reprint.h"

#ifndef GUI_FLASH
#define GUI_FLASH
#endif

GUI_HWIN hPreHeatWnd;

static BUTTON_Handle buttonInc, buttonDec, buttonSelector, buttonStepSize, buttonOff, buttonRet, buttonPreset;

static BUTTON_Handle buttonExt1, buttonExt2, buttonBed;
static TEXT_Handle textExt1, textExt2, textBed;

typedef struct {
	char * pic;
	int	tsprayer;
	int tbed;
} PREHEAT_PRESET;

#define PRESET_COUNT 3

const PREHEAT_PRESET preset[PRESET_COUNT] = {
		{img_preset_pla, 200, 60},
		{img_preset_sbs, 230, 90},
		{img_preset_petg, 240, 75},
};

uint8_t current_preset = 0;

#define STEPS_COUNT	3

const STEP_INFO temp_steps[STEPS_COUNT] = {
		{1,img_degree_step1},
		{5,img_degree_step5},
		{10, img_degree_step10},
};

uint8_t current_temp_step = 0;

const BUTTON_META selector_meta[3] {
	{"bmp_bed.bin", &preheat_menu.hotbed},
	{"bmp_extru1.bin", &preheat_menu.ext1},
	{"bmp_extru2.bin", &preheat_menu.ext2},
};

uint8_t heater_selector = 0;


void disp_update_preset(void);
void disp_update_temp(void);
void disp_update_selector(void);
void disp_update_step(void);

static void inc_sprayer(uint8_t index) {
	thermalManager.target_temperature[index] += temp_steps[current_temp_step].step;
	if((int)thermalManager.target_temperature[index] > (mksCfg.heater_0_maxtemp - (WATCH_TEMP_INCREASE + TEMP_HYSTERESIS + 1)))
		thermalManager.target_temperature[index] = (float)mksCfg.heater_0_maxtemp - (WATCH_TEMP_INCREASE + TEMP_HYSTERESIS + 1);
	thermalManager.start_watching_heater(index);
}

static void dec_sprayer(uint8_t index) {
	if((int)thermalManager.target_temperature[index] > temp_steps[current_temp_step].step) {
		thermalManager.target_temperature[index] -= temp_steps[current_temp_step].step;
	} else {
		thermalManager.target_temperature[index] = (float)0;
	}
	thermalManager.start_watching_heater(index);
}


static void cbPreHeatWin(WM_MESSAGE * pMsg) {
	char  buf[50] = {0};
	switch (pMsg->MsgId) {
		case WM_PAINT:
		case WM_TOUCH:
		case WM_TOUCH_CHILD:
			break;
		case WM_NOTIFY_PARENT:
			if(pMsg->Data.v == WM_NOTIFICATION_RELEASED) {
				if(pMsg->hWinSrc == buttonSelector) {
					heater_selector++;
					if (is_dual_extruders()) {
						if (heater_selector>2)
							heater_selector = 0;
					} else {
						if (heater_selector>1)
							heater_selector = 0;
					}
					disp_update_selector();
				} else if(pMsg->hWinSrc == buttonInc) {
					switch (heater_selector) {
						case 0: {
							thermalManager.target_temperature_bed += temp_steps[current_temp_step].step;
							if((int)thermalManager.target_temperature_bed > mksCfg.bed_maxtemp- (WATCH_BED_TEMP_INCREASE + TEMP_BED_HYSTERESIS + 1))
								thermalManager.target_temperature_bed = (float)mksCfg.bed_maxtemp - (WATCH_BED_TEMP_INCREASE + TEMP_BED_HYSTERESIS + 1);
							thermalManager.start_watching_bed();
						}
						break;
						case 1: inc_sprayer(0); break;
						case 2: inc_sprayer(1); break;
					}
					disp_update_temp();
				} else if(pMsg->hWinSrc == buttonDec) {
					switch (heater_selector) {
						case 0: {
							if((int)thermalManager.target_temperature_bed > temp_steps[current_temp_step].step) {
								thermalManager.target_temperature_bed -= temp_steps[current_temp_step].step;
							} else {
								thermalManager.target_temperature_bed = (float)0;
							}
							thermalManager.start_watching_bed();
						}
						break;
						case 1: dec_sprayer(0); break;
						case 2: dec_sprayer(1); break;
					}
					disp_update_temp();
				} else if(pMsg->hWinSrc == buttonStepSize) {
					current_temp_step = current_temp_step + 1;
					if (current_temp_step>=STEPS_COUNT)
						current_temp_step = 0;
					disp_update_step();
				} else if(pMsg->hWinSrc == buttonOff) {
					switch (heater_selector) {
						case 0: {
							thermalManager.target_temperature_bed = (float)0;
							thermalManager.start_watching_bed();
							break;
						}
						case 1: {
							thermalManager.target_temperature[0] = (float)0;
							thermalManager.start_watching_heater(0);
						}
						case 2: {
							thermalManager.target_temperature[1] = (float)0;
							thermalManager.start_watching_heater(1);
						}
					}
					disp_update_temp();
				} else if(pMsg->hWinSrc == buttonPreset) {
					thermalManager.target_temperature[0] = preset[current_preset].tsprayer;
					thermalManager.target_temperature_bed = preset[current_preset].tbed;
					thermalManager.start_watching_heater(0);
					thermalManager.start_watching_bed();
					current_preset++;
					if (current_preset>=PRESET_COUNT)
						current_preset = 0;
					disp_update_preset();
					disp_update_temp();
				} else  if(pMsg->hWinSrc == buttonRet) {
					clear_preHeat();
					draw_return_ui();
				}
			}
			break;
		default:
			WM_DefaultProc(pMsg);
	}
}

#define COL(x) ui_std_col(x)
#define COL_T(x) COL(x) + STATE_PIC_X_PIXEL
#define ROW(y) (row_offset + (row_size*y))
#define COL_INDEX 1
#define TEXT_L(phy) ui_create_std_text(COL_T(COL_INDEX)+10, ROW(phy), 80, STATE_PIC_Y_PIXEL, hPreHeatWnd, 0)
#define BUTTON_L(phy, file) ui_create_state_button(COL(COL_INDEX)+10, ROW(phy), hPreHeatWnd, file);

void draw_preHeat() {
	preheat_ui.show();
	return;
	int i;
	hPreHeatWnd = ui_std_init_window(PRE_HEAT_UI, cbPreHeatWin);

	int row_offset;
	int row_size;
	if (is_dual_extruders()) {
		row_offset = 0;
		row_size = 40;
	} else {
		row_offset = 20;
		row_size = 45;
	}
	

	buttonInc = ui_std_button(0, 0, hPreHeatWnd, img_plus, preheat_menu.add);
	buttonDec = ui_std_button(2, 0, hPreHeatWnd, img_minus, preheat_menu.dec);
	buttonPreset =  ui_std_button(3, 0, hPreHeatWnd, 0, 0);

	buttonSelector = ui_std_button(0, 1, hPreHeatWnd, 0, 0);
	buttonStepSize = ui_std_button(1, 1, hPreHeatWnd, 0, 0);
	buttonOff =  ui_std_button(2, 1, hPreHeatWnd, "bmp_speed0.bin", preheat_menu.off);
	buttonRet = ui_std_button_return(hPreHeatWnd);



	buttonExt1 = BUTTON_L(0, img_state_extruder1);
	textExt1 = TEXT_L(0);

	if (is_dual_extruders()) {
		buttonExt2 = BUTTON_L(1, img_state_extruder2);
		textExt2 = TEXT_L(1);

		buttonBed = BUTTON_L(2, img_state_bed);
		textBed = TEXT_L(2);
	} else {
		buttonBed = BUTTON_L(1, img_state_bed);
		textBed = TEXT_L(1);
	}

	if(gCfgItems.singleNozzle == 1)
		gCfgItems.curSprayerChoose = 0;
	else
		gCfgItems.curSprayerChoose = active_extruder;

	disp_update_temp();
	disp_update_selector();

	disp_update_step();
	disp_update_preset();

}

void clear_preHeat() {
	ui_drop_window(hPreHeatWnd);
}


void disp_update_step() {
	char buf[10];
	BUTTON_SetBmpFileName(buttonStepSize, temp_steps[current_temp_step].pic,1);
	memset(buf,0,sizeof(buf));
	sprintf(buf, "%.0f°C", temp_steps[current_temp_step].step);
	BUTTON_SetText(buttonStepSize,buf);
}

void disp_update_preset() {
	char buf[10];
	BUTTON_SetBmpFileName(buttonPreset, preset[current_preset].pic,1);
	BUTTON_SetBitmapEx(buttonPreset, 0, &bmp_struct, BMP_PIC_X, BMP_PIC_Y);
	memset(buf,0,sizeof(buf));
	sprintf(buf, "%d/%d°C", preset[current_preset].tsprayer, preset[current_preset].tbed);
	BUTTON_SetText(buttonPreset,buf);
}

void disp_update_temp() {
	char buf[20] = {0};
	memset(buf,0,(sizeof(buf)-1));
	sprintf(buf, "%d/%d°", (int)thermalManager.current_temperature_bed,  (int)thermalManager.target_temperature_bed);
	ui_set_text_value(textBed, buf);
	memset(buf,0,(sizeof(buf)-1));
	sprintf(buf, "%d/%d°", (int)thermalManager.current_temperature[0],  (int)thermalManager.target_temperature[0]);
	ui_set_text_value(textExt1, buf);
	if (is_dual_extruders()) {
		memset(buf,0,(sizeof(buf)-1));
		sprintf(buf, "%d/%d°", (int)thermalManager.current_temperature[1],  (int)thermalManager.target_temperature[1]);
		ui_set_text_value(textExt2, buf);
	}
}


void disp_update_selector() {
	ui_update_std_button(buttonSelector, selector_meta[heater_selector].pic, *(selector_meta[heater_selector].title));
}

void refresh_preHeat(void) {
	if (is_ui_timing(F_UI_TIMING_SEC)) {
		ui_timing_clear(F_UI_TIMING_SEC);
		disp_update_temp();
	}
}
