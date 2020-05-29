//
// Created by shubin on 29.05.2020.
//

#ifndef WORK_MACHINETYPE_H
#define WORK_MACHINETYPE_H

#include "ConfigurationWidget.h"

typedef struct {
    UI_RADIO types[3];
} MACHINE_TYPE_CONFIG_UI_CONTROLS;

class MachineTypeConfigUI : public ConfigurationWidget{
private:
    MACHINE_TYPE_CONFIG_UI_CONTROLS ui;
    void updateControls();
protected:
    virtual void on_button(UI_BUTTON hBtn);
    virtual void createControls();
public:
    virtual const char * getTitle() {return lang_str.ui_title_machine_type_settings;};
    MachineTypeConfigUI() : ConfigurationWidget(MACHINETYPE_UI, 1) {};;
};

extern MachineTypeConfigUI machine_type_config_ui;

#endif //WORK_MACHINETYPE_H
