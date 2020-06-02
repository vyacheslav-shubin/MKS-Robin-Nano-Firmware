//
// Created by shubin on 22.05.2020.
//

#include "SensorLevelConfigUI.h"
#include "MachineConfigurationUI.h"
#include "MachineTypeConfigUI.h"
#include "LevelingAutoConfigUI.h"
#include "AdvancedConfigUI.h"
#include "MotorConfigUI.h"
#include "dialog/CalculatorDialogUI.h"
#include "LevelingPointsUI.h"
#include "FilamentChangeConfigUI.h"
#include "HomeConfigUI.h"
#include "HotBedConfigUI.h"
#include "NozzleConfigUI.h"

MachineConfigurationUI machine_configuration_ui;

void MachineConfigurationUI::createControls() {
    ConfigurationWidget::createControls();
    this->dual_columns = 1;
    memset(&this->ui, 0, sizeof(this->ui));
    switch (this->page) {
        case 0: {
            this->dual_columns = 1;
            this->createArrowPair(0, 0, &this->ui.machine_type, lang_str.config_ui.machine_type);
            this->createArrowPair(0, 1, &this->ui.machine_size, lang_str.config_ui.machine_size);
            this->createArrowPair(0, 2, &this->ui.home_direction, lang_str.config_ui.parking);
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
            //this->createArrowPair(0, 2, &this->ui.test_calc, "test calc");

            break;
        }
    }
}

void MachineConfigurationUI::on_button(UI_BUTTON hBtn) {
    if (ui_is_double_button(hBtn, this->ui.machine_type)) {
        this->hide();
        machine_type_config_ui.show(this);
    } else if (ui_is_double_button(hBtn, this->ui.machine_size)) {
        this->hide();
        machine_size_ui.show(this);
    } else if (ui_is_double_button(hBtn, this->ui.home_direction)) {
        this->hide();
        home_config_ui.show(this);
    } else if (ui_is_double_button(hBtn, this->ui.endstop_type)) {
        this->hide();
        sensor_level_config_ui.show(this);
    } else if (ui_is_double_button(hBtn, this->ui.filament_settings)) {
        this->hide();
        filament_change_config_ui.show(this);
    } else if (ui_is_double_button(hBtn, this->ui.manual_leveling_settings)) {
        this->hide();
        leveling_points_ui.show(this);
    } else if (ui_is_double_button(hBtn, this->ui.auto_leveling_settings)) {
        this->hide();
        leveling_auto_config_ui.show(this);
    } else if (ui_is_double_button(hBtn, this->ui.nozzle_settings)) {
        this->hide();
        nozzle_config_ui.show();
    } else if (ui_is_double_button(hBtn, this->ui.hotbed_settings)) {
        this->hide();
        hotbed_config_ui.show(this);
        //draw_HotbedConfig();
    } else if (ui_is_double_button(hBtn, this->ui.motor_settings)) {
        this->hide();
        motor_config_ui.show(this);
    } else if (ui_is_double_button(hBtn, this->ui.advanced_settings)) {
        this->hide();
        advanced_config_ui.show(this);
    //} else if (ui_is_double_button(hBtn, this->ui.test_calc)) {
        //this->hide();
        //calculator_dialog_ui.show(this);
    } else {
        ConfigurationWidget::on_button(hBtn);
    }
}
