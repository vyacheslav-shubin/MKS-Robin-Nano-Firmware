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
    this->ui.minus = ui_std_minus_button(0, 0);
	this->ui.plus = ui_std_plus_button(2, 0);

	this->createStateButton(ui_std_col(1), _row(1), &ui.fan, FAN_STATES[0], 0);

	this->ui.run0 = this->createButtonAt(0, 1, img_fan_speed0, "0%");
	this->ui.run50 = this->createButtonAt(1, 1, img_fan_speed50, "50%");
	this->ui.run100 = this->createButtonAt(2, 1, img_fan_speed100, "100%");

	this->ui.back = this->createButtonRet();
	this->updateFanState(&this->ui.fan);
}

void FanUI::on_button(UI_BUTTON hBtn) {
    if (on_repeatable_button(hBtn)) {
    } else {
        if (hBtn == this->ui.back) {
            this->hide();
            ui_app.back_ui();
            return;
        } else if (hBtn == this->ui.run100) {
            shUI::fan_set_percent(100);
        } else if (hBtn == this->ui.run0) {
            shUI::fan_set_percent(0);
        } else if (hBtn == this->ui.run50) {
            shUI::fan_set_percent(50);
        } else if (hBtn == this->ui.plus) {
            shUI::fan_inc();
        } else if (hBtn == this->ui.minus) {
            shUI::fan_dec();
        } else if (hBtn == this->ui.fan.button) {
            this->calculator(lang_str.ui_title_fan, (double) shUI::fan_get_percent(), 0);
            return;
        }
    }
    this->updateFanState(&this->ui.fan);
}

unsigned char FanUI::on_repeatable_button(UI_BUTTON hBtn) {
    if(hBtn == this->ui.plus) {
        shUI::fan_inc();
        return 1;
    } else if(hBtn == this->ui.minus) {
        shUI::fan_dec();
        return 1;
    }
    return 0;
}

unsigned char FanUI::is_repeated_button(UI_BUTTON hBtn) {
    return (hBtn == this->ui.plus) || (hBtn == this->ui.minus);
};


void FanUI::refresh_05() {
    this->updateFanState(&this->ui.fan);
}

void FanUI::setValue(unsigned char id, double value) {
    shUI::fan_set_percent_double(value);
}
