//
// Created by shubin on 22.05.2020.
//

#ifndef WORK_MACHINECONFIGURATIONUI_H
#define WORK_MACHINECONFIGURATIONUI_H

#include "ConfigurationWidget.h"
#include "config/MachineSizeUI.h"

typedef struct {
    UI_ARROW machine_type;
    UI_ARROW home_direction;
    UI_ARROW endstop_type;
    UI_ARROW filament_settings;
    UI_ARROW leveling_settings;
    UI_ARROW nozzle_settings;
    UI_ARROW hotbed_settings;
    UI_ARROW motor_settings;
    //page 2
    UI_ARROW advanced_settings;

    UI_ARROW language;
    UI_ARROW test1;
    UI_ARROW test2;

} MACHINE_CONFIG_UI_CONTROLS;

class MachineConfigurationUI : public ConfigurationWidget, public ActionDialogCallback{
private:
    MACHINE_CONFIG_UI_CONTROLS ui;
protected:
    virtual void createControls();
    virtual void on_button(UI_BUTTON hBtn);
public:
    virtual void on_action_dialog(u8 action, u8 dialog_id);
    virtual const char * getTitle() {return lang_str.ui_title_config;};
    MachineConfigurationUI() : ConfigurationWidget(MACHINE_SETTINGS_UI, 2) {}
};

extern MachineConfigurationUI machine_configuration_ui;

#endif //WORK_MACHINECONFIGURATIONUI_H
