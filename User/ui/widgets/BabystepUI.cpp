/*
 * BabystepUI.cpp
 *
 *  Created on: May 12, 2020
 *      Author: shubin
 */

#include "BabystepUI.h"
#include "XyzUI.h"
#include "ui_tools.h"
#include "mks_reprint.h"
#include "Marlin.h"
#include "math.h"

BabystepUI babystep_ui;

static UI_STEP_INFO baby_step_info[3] = {
		{0.01, "0.01 mm", img_move_step_1},
		{0.05, "0.05 mm", img_move_step_2},
		{0.1, "0.10 mm", img_move_step_3}
};

UI_STEP_INFO * BabystepUI::getStepInfo() {
	return &baby_step_info[step];
}

void BabystepUI::doBabystep(int direction, char* axe) {
	UI_STEP_INFO * info = this->getStepInfo();
	sprintf(ui_buf1_80, "M290 %s%.3f", axe,  direction * info->size);
	excute_m290(ui_buf1_80);
}

void BabystepUI::createControls() {
	XyzUI::createControls();
	this->_has_adjust_z = 0;
	this->bs_x = 0;
	this->bs_y = 0;
}

char * BabystepUI::getStateString() {
	float ofs = abs(zprobe_zoffset);
	if (ofs<0.001)
		ofs = 0;
	sprintf(ui_buf1_80,"X:%3.2f Y:%3.2f Z:%3.2f", this->bs_x, this->bs_y, ofs);
	return ui_buf1_80;
}



void BabystepUI::on_button(WM_HWIN hBtn) {
	UI_STEP_INFO *si = this->getStepInfo();
	if (hBtn==this->ui.xp) {
		this->doBabystep(1, "X");
		this->bs_x+=si->size;
	} else if(hBtn==this->ui.xm) {
		this->doBabystep(-1, "X");
		this->bs_x-=si->size;
	} else if(hBtn == this->ui.yp) {
		this->doBabystep(1, "Y");
		this->bs_y+=si->size;
	} else if(hBtn == this->ui.ym) {
		this->doBabystep(-1, "Y");
		this->bs_y-=si->size;
	} else if(hBtn == this->ui.zp) {
		this->_has_adjust_z = 1;
		doBabystep(1, "Z");
	} else if(hBtn == this->ui.zm) {
		this->_has_adjust_z = 1;
		this->doBabystep(-1, "Z");
	} else if (hBtn == this->ui.back) {
		if(this->_has_adjust_z == 1)
			excute_m500();
		this->_has_adjust_z = 0;
		XyzUI::on_button(hBtn);
	} else
		XyzUI::on_button(hBtn);
}


