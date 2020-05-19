/*
 * MachineSettingsRootUI.cpp
 *
 *  Created on: May 18, 2020
 *      Author: shubin
 */

#include "ConfigurationRootUI.h"
#include "MotorConfigUI.h"

ConfigurationRootUI configuration_root_ui;

void ConfigurationRootUI::createControls() {
	memset(&this->ui, 0, sizeof(this->ui));
	this->ui.back = this->createButtonRet();
	this->ui.machine = this->createButtonAt(0, 0, img_machine_settings, lang_str.machine);
	this->ui.temperature = this->createButtonAt(1, 0, img_temperature_settings, lang_str.temperature);
	this->ui.motors = this->createButtonAt(2, 0, img_motor_settings, lang_str.motors);
	this->ui.advanced = this->createButtonAt(3, 0, img_advanced_settings, lang_str.advanced);
}

#include "draw_machine_settings.h"
#include "draw_temperature_settings.h"
#include "draw_advanced.h"

void ConfigurationRootUI::on_button(UI_BUTTON hBtn) {
	this->hide();
	if (hBtn==this->ui.back) {
		ui_app.back_ui();
	} else if (hBtn==this->ui.machine) {
		draw_MachineSettings();
	} else if (hBtn==this->ui.temperature) {
        draw_TemperatureSettings();
	} else if (hBtn==this->ui.motors) {
        motor_config_ui.show(this);
	} else if (hBtn==this->ui.advanced) {
        draw_Advanced();
	}
}
