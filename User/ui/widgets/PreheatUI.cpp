/*
 * PreheatUI.cpp
 *
 *  Created on: May 12, 2020
 *      Author: shubin
 */

#include "PreheatUI.h"
#include "StdWidget.h"
#include "ui_tools.h"
#include "integration.h"

PreheatUI preheat_ui;


typedef struct {
	char * picture;
	float tsprayer;
	short tbed;
} PREHEAT_PRESET;

#define PRESET_COUNT 3

static const PREHEAT_PRESET preset[PRESET_COUNT] = {
		{img_preset_pla, 200, 60},
		{img_preset_sbs, 230, 90},
		{img_preset_petg, 240, 75},
};


#define STEPS_COUNT	3

static const UI_STEP_INFO temp_steps[STEPS_COUNT] = {
		{1, "1°C", img_degree_step1},
		{5, "5°C", img_degree_step5},
		{10, "10°C", img_degree_step10},
};

static const UI_BUTTON_INFO selector_info[3] {
	{img_heating_selector_bed, &lang_str.bed},
	{img_heating_selector_ext1, &lang_str.extruder1},
	{img_heating_selector_ext2, &lang_str.extruder2}
};

#define COL(x) ui_std_col(x)
#define COL_T(x) COL(x) + STATE_PIC_X_PIXEL
#define ROW(y) (row_offset + (row_size*y))

void PreheatUI::createControls() {
	memset(&this->ui, 0, sizeof(this->ui));
	this->ui.plus = ui_std_plus_button(0, 0);
	this->ui.minus = ui_std_minus_button(2, 0);
	this->ui.preset = this->createButtonAt(3, 0, 0, 0);
	this->ui.selector = this->createButtonAt(0, 1, 0, 0);
	this->ui.temp_step = this->createButtonAt(1, 1, 0, 0);
	this->ui.off = this->createButtonAt(2, 1, img_temperature_off, lang_str.off);
	this->ui.back = this->createButtonRet();

	int row_offset;
	int row_size;
	if (shUI::isDualExtruders()) {
		row_offset = 0;
		row_size = 40;
		ui_std_ext1_state_button(COL(1), ROW(0), &this->ui.ext1);
		ui_std_ext2_state_button(COL(1), ROW(1), &this->ui.ext2);
		ui_std_bed_state_button(COL(1), ROW(2), &this->ui.bed);
	} else {
		row_offset = 20;
		row_size = 45;
		ui_std_ext1_state_button(COL(1), ROW(0), &this->ui.ext1);
		ui_std_bed_state_button(COL(1), ROW(1), &this->ui.bed);
	}

	this->updateSelector();
	this->updatePreset();
	this->updateTempStep();
}

void PreheatUI::updateSelector() {
	this->updateButton(this->ui.selector, selector_info[this->selector].picture, *selector_info[this->selector].title);
}

void PreheatUI::updatePreset() {
	sprintf(ui_buf1_80, "%d/%d°C", (int)preset[this->current_preset].tsprayer, preset[this->current_preset].tbed);
	this->updateButton(this->ui.preset, preset[this->current_preset].picture, ui_buf1_80);
}

void PreheatUI::updateTempStep() {
	this->updateButton(this->ui.temp_step, temp_steps[this->current_temp_step].picture, temp_steps[this->current_temp_step].title);
}


void PreheatUI::doAddTemperature(char sign) {
	switch (this->selector) {
		case 0: shUI::addBedTemperature(sign * temp_steps[this->current_temp_step].size);break;
		case 1: shUI::addSprayerTemperature(0, sign * temp_steps[this->current_temp_step].size);break;
		case 2:shUI::addSprayerTemperature(1, sign * temp_steps[this->current_temp_step].size); break;
	}
}

void PreheatUI::on_button(UI_BUTTON hBtn) {
	if(hBtn == this->ui.selector) {
		this->selector++;
		if (shUI::isDualExtruders()) {
			if (this->selector>2)
				this->selector = 0;
		} else {
			if (this->selector>1)
				this->selector = 0;
		}
		this->updateSelector();
	} else if(hBtn == this->ui.plus) {
		doAddTemperature(1);
	} else if(hBtn == this->ui.minus) {
		doAddTemperature(-1);
	} else if(hBtn == this->ui.temp_step) {
		this->current_temp_step = this->current_temp_step + 1;
		if (this->current_temp_step>=STEPS_COUNT)
			this->current_temp_step = 0;
		this->updateTempStep();
	} else if(hBtn == this->ui.off) {
		switch (selector) {
			case 0: shUI::setBedTemperature(0); break;
			case 1: shUI::setSprayerTemperature(0, 0); break;
			case 2:shUI::setSprayerTemperature(1, 0); break;
		}
	} else if(hBtn == this->ui.preset) {
		PREHEAT_PRESET * cp = &preset[this->current_preset];
		shUI::setBedTemperature(cp->tbed);
		switch (selector) {
			case 0:
			case 1:
				shUI::setSprayerTemperature(0, cp->tsprayer);
				break;
			case 2:
				shUI::setSprayerTemperature(1, cp->tsprayer);
				break;
		}

		this->current_preset++;
		if (this->current_preset>=PRESET_COUNT)
			this->current_preset = 0;
		this->updatePreset();
	} else if(hBtn == this->ui.back) {
		this->hide();
		ui_app.back_ui();
	}

}

void PreheatUI::refresh_1s() {
	shUI::BED_TEMP bt;
	shUI::getBedTemperature(&bt);
	sprintf(ui_buf1_80, "%d/%d°", bt.current,  bt.target);
	this->updateStateButton(&this->ui.bed, 0, ui_buf1_80);

	shUI::SPRAYER_TEMP st;
	shUI::getSprayerTemperature(0, &st);
	sprintf(ui_buf1_80, "%d/%d°", (int)st.current,  (int)st.target);
	this->updateStateButton(&this->ui.ext1, 0, ui_buf1_80);

	if (shUI::isDualExtruders()) {
		shUI::getSprayerTemperature(1, &st);
		sprintf(ui_buf1_80, "%d/%d°", (int)st.current,  (int)st.target);
		this->updateStateButton(&this->ui.ext2, 0, ui_buf1_80);
	}
}

