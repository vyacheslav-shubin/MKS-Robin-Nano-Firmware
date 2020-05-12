/*
 * XyzUI.cpp
 *
 *  Created on: May 12, 2020
 *      Author: shubin
 */

#include "XyzUI.h"
#include "ui_tools.h"

void XyzUI::createControls() {
	memset(&this->ui, 0, sizeof(this->ui));
	this->ui.back = this->createButtonRet();
	this->ui.xp = this->createButtonAt(0, 0, img_x_plus, "X+");
	this->ui.xm = this->createButtonAt(0, 1, img_x_minus, "X-");
	this->ui.yp = this->createButtonAt(1, 0, img_y_plus, "Y+");
	this->ui.ym = this->createButtonAt(1, 1, img_y_minus, "Y-");
	this->ui.zp = this->createButtonAt(2, 0, img_z_plus, "Z+");
	this->ui.zm = this->createButtonAt(2, 1, img_z_minus, "Z-");
	this->ui.step = this->createButtonAt(3, 0, 0, 0);
	this->updateButtons();
}

void XyzUI::refresh_1s() {
	char * state = this->getStateString();
	if (state!=0) {
		GUI_SetColor(gCfgItems.background_color);
		GUI_FillRect(290, TITLE_YPOS, LCD_WIDTH, titleHeight);
		GUI_SetColor(gCfgItems.title_color);
		GUI_DispStringAt((const char *)state, 200, TITLE_YPOS);
	}
};


void XyzUI::updateButtons() {
	UI_STEP_INFO * step_info = this->getStepInfo();
	ui_update_std_button(this->ui.step, step_info->picture, step_info->title);
}

void XyzUI::on_button(WM_HWIN hBtn) {
	if (hBtn == this->ui.back) {
		this->hide();
		ui_app.back_ui();
	} else if (hBtn == this->ui.step) {
		if (++this->step >= this->step_count)
			this->step = 0;
		this->updateButtons();
	}
}
