/*
 * SpeedUI.cpp
 *
 *  Created on: May 14, 2020
 *      Author: shubin
 */

#include "SpeedUI.h"
#include "integration.h"
#include "Application.h"


SpeedUI speed_ui;

#define SELECTOR_SIZE	3
#define STEPS_COUNT		3

static const UI_STEP_INFO speed_steps[STEPS_COUNT] = {
		{1, "1%", img_percent_step1},
		{5, "5%", img_percent_step5},
		{10, "10%", img_percent_step10},
};

static const UI_BUTTON_INFO selector_info[3] {
	{img_speed_selector_move, &lang_str.move},
	{img_speed_selector_ext1, &lang_str.extruder1},
	{img_speed_selector_ext2, &lang_str.extruder2}
};

void SpeedUI::refresh_1s() {
	sprintf(ui_buf1_80, "%d%%", shUI::getFeedratePercentage());
	this->updateStateButton(&this->ui.speed, 0, ui_buf1_80);

	sprintf(ui_buf1_80, "%d%%", shUI::getFlowPercentage(0));
	this->updateStateButton(&this->ui.ext1, 0, ui_buf1_80);

	if (shUI::isDualExtruders()) {
		sprintf(ui_buf1_80, "%d%%", shUI::getFlowPercentage(1));
		this->updateStateButton(&this->ui.ext2, 0, ui_buf1_80);
	}
}

#define COL(x) ui_std_col(x)
#define COL_T(x) COL(x) + STATE_PIC_X_PIXEL
#define ROW(y) (row_offset + (row_size*y))

void SpeedUI::createControls() {
	memset(&this->ui, 0, sizeof(this->ui));
	this->ui.ret = this->createButtonRet();
	this->ui.plus = ui_std_plus_button(2, 0);
	this->ui.minus = ui_std_minus_button(0, 0);
	this->ui.selector.button = this->createButtonAt(0, 1, 0, 0);
	this->ui.step.button = this->createButtonAt(3, 0, 0, 0);

	this->updateStepButton();
	this->updateSelectorButton();

	int row_offset;
	int row_size;
	if (shUI::isDualExtruders()) {
		row_offset = 0;
		row_size = 40;
		ui_std_ext2_state_button(COL(1), ROW(2), &this->ui.ext2);
	} else {
		row_offset = 20;
		row_size = 45;
	}
	ui_std_speed_state_button(COL(1), ROW(0), &this->ui.speed);
	ui_std_ext1_state_button(COL(1), ROW(1), &this->ui.ext1);

}

void SpeedUI::updateStepButton() {
	UI_STEP_INFO  * si = &speed_steps[this->ui.step.index];
	this->updateButton(this->ui.step.button, si->picture, si->title);
}

void SpeedUI::updateSelectorButton() {
	UI_BUTTON_INFO * bi = &selector_info[this->ui.selector.index];
	this->updateButton(this->ui.selector.button, bi->picture, *bi->title);
}

void SpeedUI::updateSpeed(char direction) {
	UI_STEP_INFO * si = &speed_steps[ui.step.index];
	switch(this->ui.selector.index) {
	case 0:
		shUI::addFeedratePercentage(si->size * direction);
		break;
	case 1:
		shUI::addFlowPercentage(0, si->size * direction);
		break;
	case 2:
		shUI::addFlowPercentage(1, si->size * direction);
		break;
	}
}


void SpeedUI::on_button(UI_BUTTON hBtn) {
	if (hBtn==this->ui.ret) {
		this->hide();
		ui_app.back_ui();
	}else if (hBtn==this->ui.selector.button) {
		this->ui.selector.index++;
		if (shUI::isDualExtruders()) {
			if (this->ui.selector.index>SELECTOR_SIZE - 1)
				this->ui.selector.index = 0;
		} else
			if (this->ui.selector.index>SELECTOR_SIZE - 2)
				this->ui.selector.index = 0;
		this->updateSelectorButton();
	} else if (hBtn==this->ui.step.button) {
		if (++this->ui.step.index > STEPS_COUNT - 1)
			this->ui.step.index = 0;
		this->updateStepButton();
	}else if (hBtn==this->ui.plus) {
		updateSpeed(1);
	}else if (hBtn==this->ui.minus) {
		updateSpeed(-1);
	}
}
