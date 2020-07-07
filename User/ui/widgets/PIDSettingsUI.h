//
// Created by shubin on 06.07.2020.
//

#ifndef WORK_PIDSETTINGSUI_H
#define WORK_PIDSETTINGSUI_H

#include "StdWidget.h"

typedef struct {
    UI_BUTTON back;
} PID_SETTINGS_UI_CONTROLS;

class PIDSettingsUI : public StdWidget {
private:
    PID_SETTINGS_UI_CONTROLS ui;
    unsigned short point;
protected:
    virtual void createControls();
    virtual void on_button(UI_BUTTON hBtn);
    virtual void refresh_05();
public:
    void on_message(WM_MESSAGE * pMsg);
    PIDSettingsUI() : StdWidget(PID_SETTINGS_UI) {};
};

extern PIDSettingsUI pid_settings_ui;

#endif //WORK_PIDSETTINGSUI_H
