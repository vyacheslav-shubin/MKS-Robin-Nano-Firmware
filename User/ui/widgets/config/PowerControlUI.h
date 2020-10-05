//
// Created by shubin on 08.07.2020.
//

#ifndef WORK_POWERCONTROLUI_H
#define WORK_POWERCONTROLUI_H

#include "ConfigurationWidget.h"
typedef struct {
    UI_CHECK	presents;
    UI_CHECK	autoPowerOff;
    UI_CHECK	powerDet;
    UI_CHECK	tempCtrl;
    UI_CHECK	hwAsSw;
} POWER_CONTROL_UI_CONTROLS;

class PowerControlUI : public ConfigurationWidget {
private:
    POWER_CONTROL_UI_CONTROLS ui;
protected:
    virtual void on_button(UI_BUTTON hBtn);
    virtual void createControls();
public:
    virtual const char * getTitle() {return lang_str.ui_title_power_contol;};
    PowerControlUI() : ConfigurationWidget(POWER_CONTROL_UI, 2) {};
};

extern PowerControlUI power_control_ui;

#endif //WORK_POWERCONTROLUI_H
