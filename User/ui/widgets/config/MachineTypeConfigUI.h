//
// Created by shubin on 29.05.2020.
//

#ifndef WORK_MACHINETYPE_H
#define WORK_MACHINETYPE_H

#include "ConfigurationWidget.h"

typedef struct {
    UI_RADIO types[3];
} MECH_TYPE_CONFIG_UI_CONTROLS;


typedef struct {
    UI_INPUT_WITH_DEFAULT 	xmin;
    UI_INPUT_WITH_DEFAULT 	xmax;
    UI_INPUT_WITH_DEFAULT 	ymin;
    UI_INPUT_WITH_DEFAULT 	ymax;
    UI_INPUT_WITH_DEFAULT 	zmin;
    UI_INPUT_WITH_DEFAULT 	zmax;
} MACHINE_SIZE_UI_CONTROLS1;

typedef struct {
    MECH_TYPE_CONFIG_UI_CONTROLS mech;
    MACHINE_SIZE_UI_CONTROLS1   size;
} MACHINE_TYPE_CONFIG_UI_CONTROLS;



class MachineTypeConfigUI : public ConfigurationWidgetWithCalc{
private:
    MACHINE_TYPE_CONFIG_UI_CONTROLS ui;
    void updateControls();
    void _setValue(unsigned char value_id, float value);
protected:
    virtual void setValue(unsigned char id, double value) {
        this->_setValue(id, value);
    }
    virtual void on_button(UI_BUTTON hBtn);
    virtual void createControls();
public:
    virtual const char * getTitle() {
        switch (page) {
            case 0:
                return lang_str.ui_title_machine_type_settings;
            case 1:
                return lang_str.ui_title_config_machine_size;
        }
    };
    MachineTypeConfigUI() : ConfigurationWidgetWithCalc(MACHINETYPE_UI, 2) {};;
};

extern MachineTypeConfigUI machine_type_config_ui;

#endif //WORK_MACHINETYPE_H
