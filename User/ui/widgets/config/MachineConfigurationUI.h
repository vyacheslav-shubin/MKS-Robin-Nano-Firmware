//
// Created by shubin on 22.05.2020.
//

#ifndef WORK_MACHINECONFIGURATIONUI_H
#define WORK_MACHINECONFIGURATIONUI_H

#include "ConfigurationWidget.h"

typedef struct {
    UI_ARROW machine_type;
    UI_ARROW machine_size;
    UI_ARROW home_direction;
    UI_ARROW endstop_type;
    UI_ARROW filament_settings;
    UI_ARROW manual_leveling_settings;
    UI_ARROW auto_leveling_settings;
    UI_ARROW nozzle_settings;
    UI_ARROW hotbed_settings;
    //page 2
    UI_ARROW motor_settings;
    UI_ARROW advanced_settings;

    UI_ARROW test_calc;

} MACHINE_CONFIG_UI_CONTROLS;

class MachineConfigurationUI : public ConfigurationWidget{
private:
    MACHINE_CONFIG_UI_CONTROLS ui;
protected:
    virtual void createControls();
    virtual void on_button(UI_BUTTON hBtn);
public:
    virtual const char * getTitle() {return lang_str.ui_title_config;};
    MachineConfigurationUI() : ConfigurationWidget(MACHINE_SETTINGS_UI, 2) {}
};

extern MachineConfigurationUI machine_configuration_ui;

#endif //WORK_MACHINECONFIGURATIONUI_H
