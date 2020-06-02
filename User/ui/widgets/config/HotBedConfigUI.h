//
// Created by shubin on 01.06.2020.
//

#ifndef WORK_HOTBEDCONFIGUI_H
#define WORK_HOTBEDCONFIGUI_H

#include "ConfigurationWidget.h"

typedef struct {
    UI_CHECK enabled;
    TEMP_INTERVAL_UI_CONTROLS temp;
    PID_UI_CONTROPLS pid;
} HOTBED_CONFIG_UI_CONTROLS;

class HotBedConfigUI: public ConfigurationWidgetWithCalc {
private:
    HOTBED_CONFIG_UI_CONTROLS ui;
    void updateControls();
    void _setValue(unsigned char index, float value);
protected:
    virtual void on_button(UI_BUTTON hBtn);
    virtual void createControls();
    virtual void setValue(unsigned char id, double value) {
        this->_setValue(id, value);
    }
public:
    virtual const char * getTitle() {return lang_str.ui_title_hotbed_config;};
    HotBedConfigUI() : ConfigurationWidgetWithCalc(HOTBED_CONFIG_UI, 1) {};
};

extern HotBedConfigUI hotbed_config_ui;

#endif //WORK_HOTBEDCONFIGUI_H
