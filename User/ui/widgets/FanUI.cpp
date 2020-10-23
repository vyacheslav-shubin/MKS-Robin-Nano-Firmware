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
#include "integration.h"

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


void FanUI::on_button_click(UI_BUTTON hBtn) {
    if (hBtn == this->ui.plus) {
        this->ui.auto_change = 1;
        this->ui.auto_delay = 6;
    } else if (hBtn == this->ui.minus) {
        this->ui.auto_change = -1;
        this->ui.auto_delay = 6;
    } else {
        this->ui.auto_change = 0;
    }
}


void FanUI::on_button(UI_BUTTON hBtn) {
    this->ui.auto_change = 0;
	if(hBtn == this->ui.back) {
		this->hide();
		ui_app.back_ui();
		return;
	} else if(hBtn == this->ui.run100) {
	    shUI::fan_set_percent(100);
	} else if(hBtn == this->ui.run0) {
        shUI::fan_set_percent(0);
	} else if(hBtn == this->ui.run50) {
        shUI::fan_set_percent(50);
	} else if(hBtn == this->ui.plus) {
        shUI::fan_inc();
	} else if(hBtn == this->ui.minus) {
        shUI::fan_dec();
	} else if (hBtn == this->ui.fan.button) {
	    this->calculator(lang_str.ui_title_fan, (double)shUI::fan_get_percent(), 0);
	    return;
	}
    this->updateFanState(&this->ui.fan);
}

void FanUI::refresh_025() {
    if (this->ui.auto_change) {
        if (this->ui.auto_delay==0) {
            if (this->ui.auto_change==1) {
                shUI::fan_inc();
            } else {
                shUI::fan_dec();
            }
            ui_update_fan_button_text(this->ui.fan.label);
        } else
            this->ui.auto_delay--;
    }
}

void FanUI::refresh_05() {
    this->updateFanState(&this->ui.fan);
}

void FanUI::setValue(unsigned char id, double value) {
    shUI::fan_set_percent_double(value);
}
