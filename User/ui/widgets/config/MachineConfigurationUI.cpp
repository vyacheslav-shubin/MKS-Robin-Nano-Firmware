//
// Created by shubin on 22.05.2020.
//

#include "MachineConfigurationUI.h"
#include "AdvancedConfigUI.h"
#include "MotorConfigUI.h"

MachineConfigurationUI machine_configuration_ui;

void MachineConfigurationUI::createControls() {
    ConfigurationWidget::createControls();
    memset(&this->ui, 0, sizeof(this->ui));
    switch (this->page) {
        case 0: {
            this->dual_columns = 1;
            this->createArrowPair(0, 0, &this->ui.machine_type, lang_str.config_ui.machine_type);
            this->createArrowPair(0, 1, &this->ui.machine_size, lang_str.config_ui.machine_size);
            this->createArrowPair(0, 2, &this->ui.home_direction, lang_str.config_ui.home_direction);
            this->createArrowPair(0, 3, &this->ui.endstop_type, lang_str.config_ui.endtop_type);
            this->createArrowPair(0, 4, &this->ui.filament_settings, lang_str.config_ui.filament_settings);
            this->createArrowPair(1, 0, &this->ui.manual_leveling_settings, lang_str.config_ui.manual_leveling_settings);
            this->createArrowPair(1, 1, &this->ui.auto_leveling_settings, lang_str.config_ui.auto_leveling_settings);
            this->createArrowPair(1, 2, &this->ui.nozzle_settings, lang_str.config_ui.nozzle_settings);
            this->createArrowPair(1, 3, &this->ui.hotbed_settings, lang_str.config_ui.hotbed_settings);
            break;
        }
        case 1: {
            this->dual_columns = 1;
            this->createArrowPair(0, 0, &this->ui.motor_settings, lang_str.motors);
            this->createArrowPair(0, 1, &this->ui.advanced_settings, lang_str.advanced);
            break;
        }
    }
}

void MachineConfigurationUI::on_button(UI_BUTTON hBtn) {
    if (ui_is_double_button(hBtn, this->ui.machine_type)) {
        this->hide();
        draw_MachineType();
    } else if (ui_is_double_button(hBtn, this->ui.machine_size)) {
        this->hide();
        draw_Stroke();
    } else if (ui_is_double_button(hBtn, this->ui.home_direction)) {
        this->hide();
        draw_HomeDir();
    } else if (ui_is_double_button(hBtn, this->ui.endstop_type)) {
        this->hide();
        draw_EndstopType();
    } else if (ui_is_double_button(hBtn, this->ui.filament_settings)) {
        this->hide();
        draw_FilamentSettings();
    } else if (ui_is_double_button(hBtn, this->ui.manual_leveling_settings)) {
        this->hide();
        draw_XYZLevelPara();
    } else if (ui_is_double_button(hBtn, this->ui.auto_leveling_settings)) {
        this->hide();
        draw_LevelingPara();
    } else if (ui_is_double_button(hBtn, this->ui.nozzle_settings)) {
        this->hide();
        draw_NozzleConfig();
    } else if (ui_is_double_button(hBtn, this->ui.hotbed_settings)) {
        this->hide();
        draw_HotbedConfig();
    } else if (ui_is_double_button(hBtn, this->ui.motor_settings)) {
        this->hide();
        motor_config_ui.show(this);
    } else if (ui_is_double_button(hBtn, this->ui.advanced_settings)) {
        this->hide();
        advanced_config_ui.show(this);
    } else {
        ConfigurationWidget::on_button(hBtn);
    }
}