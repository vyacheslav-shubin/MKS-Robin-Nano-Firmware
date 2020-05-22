/*
 * AdvancedConfigUI.cpp
 *
 *  Created on: May 21, 2020
 *      Author: shubin
 */

#include "AdvancedConfigUI.h"

#include "ili9320.h"

AdvancedConfigUI advanced_config_ui;

void AdvancedConfigUI::on_button(UI_BUTTON hBtn) {
	if(hBtn == this->ui.autoPowerOff.button) {
		ui_invert_u8_flag(gCfgItems.print_finish_close_machine_flg);
		this->updateCheckButton(ui.autoPowerOff.button, (char)gCfgItems.print_finish_close_machine_flg);
		epr_write_data(EPR_AUTO_CLOSE_MACHINE, &gCfgItems.print_finish_close_machine_flg, sizeof(gCfgItems.print_finish_close_machine_flg));
	} else if(hBtn == this->ui.simpleMainUI.button) {
		ui_invert_u8_flag(gCfgItems.display_style);
		this->updateCheckButton(ui.simpleMainUI.button, (char)gCfgItems.display_style);
		epr_write_data(EPR_SCREEN_DISPLAY_STYLE, &gCfgItems.display_style, sizeof(gCfgItems.display_style));
	} else if(hBtn == this->ui.diplayBackLight.button) {
		ui_invert_u8_flag(gCfgItems.standby_mode);
		this->updateCheckButton(ui.diplayBackLight.button, (char)gCfgItems.standby_mode);
		epr_write_data(EPR_STANDBY_MODE, &gCfgItems.standby_mode, sizeof(gCfgItems.standby_mode));
	} else
		ConfigurationWidget::on_button(hBtn);
}

void AdvancedConfigUI::createControls() {
	ConfigurationWidget::createControls();
    this->dual_columns = 0;
	this->createCheckPair(0, 0, &this->ui.autoPowerOff, lang_str.config_ui.power_off_after_print, gCfgItems.print_finish_close_machine_flg==1);
	this->createCheckPair(0, 1, &this->ui.simpleMainUI, lang_str.config_ui.simple_main_ui, gCfgItems.display_style==1);
	this->createCheckPair(0, 2, &this->ui.diplayBackLight, lang_str.config_ui.display_backlight_off, gCfgItems.standby_mode==1);

}

