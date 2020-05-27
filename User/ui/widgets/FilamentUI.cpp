/*
 * FilamentUI.cpp
 *
 *  Created on: May 13, 2020
 *      Author: shubin
 */

#include "FilamentUI.h"
#include "PreheatUI.h"
#include "integration.h"

#define HYSTERESIS 2

FilamentUI filament_ui;

static const UI_BUTTON_INFO extruder_selector_info[2] {
	{img_heating_selector_ext1, &lang_str.extruder1},
	{img_heating_selector_ext2, &lang_str.extruder2}
};

#define STEPS_COUNT	4
#define SPEED_COUNT	3

static const UI_STEP_INFO filament_steps[STEPS_COUNT] = {
		{1, "1 mm", img_filament_step1},
		{5, "5 mm", img_filament_step2},
		{10, "10 mm", img_filament_step3},
		{0, 0, img_filament_change},
};

static const UI_STEP_INFO filament_speed[SPEED_COUNT] = {
		{100, "F100", img_speed1},
		{500, "F500", img_speed2},
		{1000, "F1000", img_speed3},
};

#define COL(x) ui_std_col(x)
#define COL_T(x) COL(x) + STATE_PIC_X_PIXEL
#define ROW(y) (row_offset + (row_size*y))


void FilamentUI::createControls() {
	memset(&this->ui, 0, sizeof(this->ui));
	unsigned char row_offset;
	unsigned char row_size;
	if (shUI::isDualExtruders()) {
		row_offset = 0;
		row_size = 40;
		ui_std_ext2_state_button(COL(1), ROW(1), &this->ui.ext2);
	} else {
		this->current_extruder=0;
		row_offset = 20;
		row_size = 45;
	}
	ui_std_ext1_state_button(COL(1), ROW(0), &this->ui.ext1);

	this->ui.back = this->createButtonRet();
	this->ui.selector = this->createButtonAt(0, 1, 0, 0);
	this->ui.load = this->createButtonAt(0, 0, img_filament_load, lang_str.load);
	this->ui.unload = this->createButtonAt(2, 0, img_filament_unload, lang_str.unload);
	this->ui.size = this->createButtonAt(3, 0, 0, 0);
	this->ui.speed = this->createButtonAt(1, 1, 0, 0);
	this->ui.preheat = this->createButtonAt(2, 1, img_preheat, lang_str.preheat);
	this->updateExtruderSelector();
	this->updateSpeedSelector();
	this->updateStepSelector();
}

void FilamentUI::on_button(UI_BUTTON hBtn) {
	if (hBtn == this->ui.back) {
		this->hide();
		ui_app.back_ui();
	} if (hBtn == this->ui.selector) {
		if (shUI::isDualExtruders()) {
			this->current_extruder++;
			if (this->current_extruder>1)
				this->current_extruder = 0;
		} else {
			this->current_extruder=0;
		}
		this->updateExtruderSelector();
	} if (hBtn==this->ui.size) {
		this->current_step++;
		if (this->current_step>=STEPS_COUNT)
			this->current_step = 0;
		this->updateStepSelector();
	} if (hBtn==this->ui.speed) {
		this->current_speed++;
		if (this->current_speed>=SPEED_COUNT)
			this->current_speed = 0;
		this->updateSpeedSelector();
	} else if (hBtn==this->ui.preheat) {
		this->hide();
		preheat_ui.show();
	} else if (hBtn==this->ui.load) {
		doFilament(1);
	} else if (hBtn==this->ui.unload) {
		doFilament(-1);
	}
}

short FilamentUI::getWantedTemperature(char direction) {
	if (this->current_step==3)
		return shUI::getFilamentLoadUnloadMinTempereture(direction);
	else
		return shUI::getFilamentExtrudeMinTempereture();
}

void _execute_load_unload(unsigned char extruder, char direction) {
    shUI::MOVE_DISTANCE dis;
    shUI::getFilamentLoadUnloadDistance(direction, &dis);
    shUI::doFilamentMove(
            extruder,
            (int) dis.distance,
            (int) dis.speed
    );
}

void FilamentUI::on_action_dialog(u8 action, u8 dialog_id) {
    confirm_dialog_ui.hide();
    this->show();
    if (action==UI_BUTTON_OK)
        this->doFilament(1, 0);
}


void FilamentUI::doFilament(char direction, unsigned char confirm) {
	shUI::SPRAYER_TEMP st;
	shUI::getSprayerTemperature(this->current_extruder, &st);
	int wanted = this->getWantedTemperature(direction);
	if (wanted < st.current + HYSTERESIS) {
		if (this->current_step==3) {
			if ((direction==1) && confirm){
			    this->hide();
                confirm_dialog_ui.show(lang_str.dialog.confirm_filament_load, this, 0, this);
			} else {
                _execute_load_unload(this->current_extruder, direction);
            }
		} else {
			shUI::doFilamentMove(
				this->current_extruder,
				(int)(direction * filament_steps[this->current_step].size),
				(int)(filament_speed[this->current_speed].size)
			);
		}
	} else if (wanted>st.target)
        shUI::setSprayerTemperature(this->current_extruder, wanted);
}

void FilamentUI::updateExtruderSelector() {
	UI_BUTTON_INFO * info = &extruder_selector_info[this->current_extruder];
	this->updateButton(this->ui.selector, info->picture, *info->title);
}

void FilamentUI::updateStepSelector() {
	UI_STEP_INFO * sp = &filament_steps[this->current_step];
	this->updateButton(this->ui.size, sp->picture, (sp->title==0)?lang_str.change:sp->title);
}

void FilamentUI::updateSpeedSelector() {
	UI_STEP_INFO * sp = &filament_speed[this->current_speed];
	this->updateButton(this->ui.speed, sp->picture, sp->title);
}

void FilamentUI::refresh_05() {
	shUI::SPRAYER_TEMP st;
	shUI::getSprayerTemperature(this->current_extruder, &st);
	int wanted = this->getWantedTemperature(1);
	if (wanted > st.current - HYSTERESIS) {
		STATE_BUTTON * btn;
		switch (this->current_extruder) {
			case 0:
				btn = &this->ui.ext1; break;
			default:
				btn = &this->ui.ext2; break;
		}
		this->updateStateButton(btn, 0, "");
	}
}


void FilamentUI::refresh_1s() {
	shUI::SPRAYER_TEMP st;
	shUI::getSprayerTemperature(0, &st);
	sprintf(ui_buf1_80, "%d/%d°", (int)st.current,  (int)st.target);
	this->updateStateButton(&this->ui.ext1, 0, ui_buf1_80);

	if (shUI::isDualExtruders()) {
		shUI::getSprayerTemperature(1, &st);
		sprintf(ui_buf1_80, "%d/%d°", (int)st.current,  (int)st.target);
		this->updateStateButton(&this->ui.ext2, 0, ui_buf1_80);
	}
	this->drawXYZ();
}
