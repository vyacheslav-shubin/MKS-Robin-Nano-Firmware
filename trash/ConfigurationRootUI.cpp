/*
 * MachineSettingsRootUI.cpp
 *
 *  Created on: May 18, 2020
 *      Author: shubin
 */

#include "ConfigurationRootUI.h"
#include "MotorConfigUI.h"
#include "AdvancedConfigUI.h"
#include "MachineConfigurationUI.h"

ConfigurationRootUI configuration_root_ui;

void ConfigurationRootUI::createControls() {
	memset(&this->ui, 0, sizeof(this->ui));
	this->ui.back = this->createButtonRet();
	this->ui.machine = this->createButtonAt(0, 0, img_machine_settings, lang_str.machine);
	this->ui.motors = this->createButtonAt(1, 0, img_motor_settings, lang_str.motors);
	this->ui.advanced = this->createButtonAt(2, 0, img_advanced_settings, lang_str.advanced);
}

void ConfigurationRootUI::on_button(UI_BUTTON hBtn) {
	this->hide();
	if (hBtn==this->ui.back) {
		ui_app.back_ui();
	} else if (hBtn==this->ui.machine) {
		machine_configuration_ui.show(this);
	} else if (hBtn==this->ui.motors) {
        motor_config_ui.show(this);
	} else if (hBtn==this->ui.advanced) {
        advanced_config_ui.show(this);
	}
}
