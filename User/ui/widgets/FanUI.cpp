/*
 * FanUI.cpp
 *
 *  Created on: May 12, 2020
 *      Author: shubin
 */

#include "FanUI.h"
#include "ui_tools.h"
#include "Marlin.h"
#include "tim.h"
#include "Application.h"


FanUI fan_ui;

#define _row(y) (40 * (y))

void FanUI::createControls() {
	memset(&this->ui, 0, sizeof(this->ui));
	this->ui.plus = ui_std_plus_button(0, 0);
	this->ui.minus = ui_std_minus_button(2, 0);

	this->createStateButton(ui_std_col(1), _row(1), &ui.fan, FAN_STATES[0], 0);

	this->ui.run0 = this->createButtonAt(0, 1, img_fan_speed0, "0%");
	this->ui.run50 = this->createButtonAt(1, 1, img_fan_speed50, "50%");
	this->ui.run100 = this->createButtonAt(2, 1, img_fan_speed100, "100%");

	this->ui.back = this->createButtonRet();
	this->updateFanState(&this->ui.fan);
}


void FanUI::commitFanState() {
	MKS_FAN_TIM = fanSpeeds[0]*10000/255;
	this->updateFanState(&this->ui.fan);
}

void FanUI::on_button(UI_BUTTON hBtn) {
	if(hBtn == this->ui.back) {
		this->hide();
		ui_app.back_ui();
		return;
	} else if(hBtn == this->ui.run100) {
		gCfgItems.fanOnoff = 1;
		fanSpeeds[0]=255;
	} else if(hBtn == this->ui.run0) {
		gCfgItems.fanOnoff = 0;
		gCfgItems.fanSpeed = 0;
		fanSpeeds[0] = 0;
	} else if(hBtn == this->ui.run50) {
		gCfgItems.fanOnoff = 1;
		fanSpeeds[0]=128;
	} else if(hBtn == this->ui.plus) {
		if(fanSpeeds[0]<255) {
			gCfgItems.fanOnoff = 1;
			fanSpeeds[0]++;
		} else {
			fanSpeeds[0]=255;
		}
	} else if(hBtn == this->ui.minus) {
		if(fanSpeeds[0]>0) {
			gCfgItems.fanOnoff = 1;
			fanSpeeds[0]--;
		} else {
			fanSpeeds[0]=0;
		}
	}
	this->commitFanState();
}

void FanUI::refresh_05() {
	this->updateFanState(&this->ui.fan);
}
