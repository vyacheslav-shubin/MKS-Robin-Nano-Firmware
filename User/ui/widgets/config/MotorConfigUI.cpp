/*
 * MotorConfigUI.cpp
 *
 *  Created on: May 19, 2020
 *      Author: shubin
 */

#include "MotorConfigUI.h"
#include "MotorDirConfigUI.h"

MotorConfigUI motor_config_ui;

void MotorConfigUI::on_button(UI_BUTTON hBtn) {
	if (ui_is_double_button(hBtn, this->ui.axes)) {
		this->hide();
		motor_dir_config_ui.show(this);
	} else if (ui_is_double_button(hBtn, this->ui.steps)) {
		this->hide();
        draw_Steps();
	} else if (ui_is_double_button(hBtn, this->ui.acceleration)) {
		this->hide();
		draw_Acceleration();
	} else if (ui_is_double_button(hBtn, this->ui.max_speed)) {
		this->hide();
		draw_MaxFeedRate();
	} else if (ui_is_double_button(hBtn, this->ui.jerk)) {
		this->hide();
        draw_Jerk();
	} else
		ConfigurationWidget::on_button(hBtn);
}

void MotorConfigUI::createControls() {
	ConfigurationWidget::createControls();
	memset(&this->ui, 0, sizeof(this->ui));
	this->dual_columns = 1;
	this->createArrowPair(0, 0, &this->ui.axes, lang_str.config_ui.axes_inversion);
	this->createArrowPair(0, 1, &this->ui.steps, lang_str.config_ui.motor_steps_settings);
	this->createArrowPair(0, 2, &this->ui.acceleration, lang_str.config_ui.acceleration_settings);
	this->createArrowPair(0, 3, &this->ui.max_speed, lang_str.config_ui.maximum_speed_settings);
	this->createArrowPair(1, 0, &this->ui.jerk, lang_str.config_ui.jerk_settings);
}

