/*
 * AdvancedConfigUI.cpp
 *
 *  Created on: May 21, 2020
 *      Author: shubin
 */

#include "../Src/sh_tools.h"
#include "AdvancedConfigUI.h"

#include "ili9320.h"
#include "Application.h"

AdvancedConfigUI advanced_config_ui;

void AdvancedConfigUI::on_button(UI_BUTTON hBtn) {
    if(hBtn == this->ui.simpleMainUI.button) {
		ui_invert_u8_flag(gCfgItems.display_style);
		this->updateCheckButton(ui.simpleMainUI.button, (char)gCfgItems.display_style);
		epr_write_data(EPR_SCREEN_DISPLAY_STYLE, (const unsigned char*)&gCfgItems.display_style, sizeof(gCfgItems.display_style));
	} else if(hBtn == this->ui.diplayBackLight.button) {
		ui_invert_u8_flag(gCfgItems.standby_mode);
		this->updateCheckButton(ui.diplayBackLight.button, (char)gCfgItems.standby_mode);
		epr_write_data(EPR_STANDBY_MODE, (const unsigned char*)&gCfgItems.standby_mode, sizeof(gCfgItems.standby_mode));
	} else if (hBtn == this->ui.timeShift.dflt) {
        gCfgItems.time_offset = 180;
        this->updateControls();
        epr_write_data(EPR_TIME_SHIFT, (const unsigned char*)&gCfgItems.time_offset, sizeof(gCfgItems.time_offset));
	} else if (hBtn == this->ui.timeShift.button) {
	    this->calculator(lang_str.config_ui.time_shift, gCfgItems.time_offset, 0);
    } else if (hBtn == this->ui.filamentDet.button) {
        gCfgItems.mask_det_Function ^= MASK_DETECTOR_FILAMENT;
        this->updateCheckButton(ui.filamentDet.button, !(gCfgItems.mask_det_Function & MASK_DETECTOR_FILAMENT));
        epr_write_data(EPR_MASK_DET_FUNCTION,(unsigned char *)&gCfgItems.mask_det_Function, sizeof(gCfgItems.mask_det_Function));
	} else
        ConfigurationWidget::on_button(hBtn);
}

void AdvancedConfigUI::_setValue(unsigned char id, short value) {
    gCfgItems.time_offset = value;
    epr_write_data(EPR_TIME_SHIFT, (const unsigned char*)&gCfgItems.time_offset, sizeof(gCfgItems.time_offset));
}

void AdvancedConfigUI::updateControls() {
    if (this->ui.timeShift.button) {
        sprintf(ui_buf1_100, "%d", gCfgItems.time_offset);
        this->setButtonText(this->ui.timeShift.button, ui_buf1_100);
    }
}

void AdvancedConfigUI::createControls() {
	ConfigurationWidget::createControls();
	memset(&this->ui, 0, sizeof(this->ui));
    this->dual_columns = 0;
    this->createCheckPair(0, 0, &this->ui.simpleMainUI, lang_str.config_ui.simple_main_ui, gCfgItems.display_style==1);
    this->createCheckPair(0, 1, &this->ui.filamentDet, lang_str.config_ui.filament_detector, !(gCfgItems.mask_det_Function & MASK_DETECTOR_FILAMENT));
    this->createCheckPair(0, 2, &this->ui.diplayBackLight, lang_str.config_ui.display_backlight_off, gCfgItems.standby_mode==1);
    this->createInputWithDefault(0, 3, &this->ui.timeShift, lang_str.config_ui.time_shift, 0);
    this->updateControls();
}

