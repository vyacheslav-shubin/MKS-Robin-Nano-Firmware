//
// Created by shubin on 03.06.2020.
//

#ifndef WORK_WIFIWAITINITDIALOGUI_H
#define WORK_WIFIWAITINITDIALOGUI_H

#include "ActionDialog.h"

typedef struct {
    UI_BUTTON cancel;
    UI_TEXT	text;
} WIFI_WAIT_INIT_DIALOG_UI_CONTROLS;

class WifiWaitInitDialogUI : public ActionDialog{
private:
    WIFI_WAIT_INIT_DIALOG_UI_CONTROLS ui;
protected:
    virtual void createControls();
    virtual void on_button(UI_BUTTON hBtn);
    virtual void refresh_05();
public:
    WifiWaitInitDialogUI(): ActionDialog() {};
};

extern WifiWaitInitDialogUI wifi_wait_init_dialog_ui;

#endif //WORK_WIFIWAITINITDIALOGUI_H
