//
// Created by shubin on 02.06.2020.
//

#ifndef WORK_NOZZLECONFIGUI_H
#define WORK_NOZZLECONFIGUI_H

#include "ConfigurationWidget.h"

typedef struct {
    UI_TEXT label;
    TEMP_INTERVAL_UI_CONTROLS temp;
    PID_UI_CONTROPLS_1 pid;
} SINGLE_NOZZLE_CONFIG_UI_CONTROLS;

typedef struct {
    UI_INPUT_SINGLE nozzles;
    UI_INPUT_SINGLE  e0sensorType;
    UI_CHECK pid_enabled;
    SINGLE_NOZZLE_CONFIG_UI_CONTROLS nozzle[2];
} NOZZLE_CONFIG_UI_CONTROLS;

class NozzleConfigUI : public ConfigurationWidgetWithCalc {
private:
    NOZZLE_CONFIG_UI_CONTROLS ui;
    void updateControls();
    void updatePidControls(unsigned char index);
    void _setValue(unsigned char index, float value);
    void makeNozzleControls(unsigned char index);
    unsigned char on_input_button(UI_BUTTON hBtn, unsigned char index);
    unsigned char on_default_button(UI_BUTTON hBtn, unsigned char index);
protected:
    virtual void on_button(UI_BUTTON hBtn);
    virtual void createControls();
    virtual void setValue(unsigned char id, double value) {
        this->_setValue(id, value);
    }
public:
    virtual const char * getTitle() {return lang_str.ui_title_extruders_config;};
    NozzleConfigUI() : ConfigurationWidgetWithCalc(NOZZLE_CONFIG_UI, 3) {};
};

extern NozzleConfigUI nozzle_config_ui;


#endif //WORK_NOZZLECONFIGUI_H
