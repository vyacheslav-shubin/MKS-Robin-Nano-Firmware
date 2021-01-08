//
// Created by shubin on 08.07.2020.
//

#ifndef WORK_POWEROFFDIALOGUI_H
#define WORK_POWEROFFDIALOGUI_H

#include "widgets/StdWidget.h"
#include "ActionDialog.h"

typedef struct {
    UI_BUTTON ok;
    UI_BUTTON cancel;
    UI_BUTTON reboot;
    UI_TEXT	text;
    UI_PROGRESS_BAR progress;
    STATE_BUTTON ext1;
    STATE_BUTTON ext2;
    STATE_BUTTON bed;
} POWER_OFF_DIALOG_UI_CONTROLS;

class PowerOffDialogUI:  public DialogWidget{
private:
    unsigned short timeout;
    unsigned short duration;
protected:
    POWER_OFF_DIALOG_UI_CONTROLS ui;
    void createControls();
    virtual void on_button(UI_BUTTON hBtn);
    virtual void refresh_1s();
    bool isTemperatureValid();
public:
    void show(unsigned short duration) {
        this->duration = duration==0?1:duration;
        DialogWidget::show();
    }
    PowerOffDialogUI(): DialogWidget(){};
};

extern PowerOffDialogUI power_off_dialog_ui;

#endif //WORK_POWEROFFDIALOGUI_H
