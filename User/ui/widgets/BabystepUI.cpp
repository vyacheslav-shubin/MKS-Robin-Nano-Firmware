/*
 * BabystepUI.cpp
 *
 *  Created on: May 12, 2020
 *      Author: shubin
 */

#include "BabystepUI.h"
#include "XyzUI.h"
#include "ui_tools.h"
#include "integration.h"

#define _do_babystep(axe, direction) shUI::babystep((axe), (direction) * this->getStepInfo()->size)

BabystepUI babystep_ui;

static UI_STEP_INFO baby_step_info[3] = {
		{0.01, "0.01 mm", img_move_step_1},
		{0.05, "0.05 mm", img_move_step_2},
		{0.1, "0.10 mm", img_move_step_3}
};

UI_STEP_INFO * BabystepUI::getStepInfo() {
	return &baby_step_info[step];
}

void BabystepUI::createControls() {
	XyzUI::createControls();
	this->_has_adjust_z = 0;
	this->bs_x = 0;
	this->bs_y = 0;
}

char * BabystepUI::getStateString() {
	sprintf(ui_buf1_80,"X:%3.2f Y:%3.2f Z:%3.2f", this->bs_x, this->bs_y, shUI::babystepGetZ());
	return ui_buf1_80;
}



void BabystepUI::on_button(UI_BUTTON hBtn) {
	UI_STEP_INFO *si = this->getStepInfo();
	if (hBtn==this->ui.xp) {
		_do_babystep("X", 1);
		this->bs_x+=si->size;
	} else if(hBtn==this->ui.xm) {
		_do_babystep("X", -1);
		this->bs_x-=si->size;
	} else if(hBtn == this->ui.yp) {
		_do_babystep("Y", 1);
		this->bs_y+=si->size;
	} else if(hBtn == this->ui.ym) {
		_do_babystep("Y", -1);
		this->bs_y-=si->size;
	} else if(hBtn == this->ui.zp) {
		this->_has_adjust_z = 1;
		_do_babystep("Z", 1);
	} else if(hBtn == this->ui.zm) {
		this->_has_adjust_z = 1;
		_do_babystep("Z", -1);
	} else if (hBtn == this->ui.back) {
		if(this->_has_adjust_z == 1)
			shUI::saveConfig();
		this->_has_adjust_z = 0;
		XyzUI::on_button(hBtn);
	} else
		XyzUI::on_button(hBtn);
}


