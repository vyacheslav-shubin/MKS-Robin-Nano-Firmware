//
// Created by shubin on 03.06.2020.
//

#ifndef WORK_WIFIWAITINITDIALOGUI_H
#define WORK_WIFIWAITINITDIALOGUI_H

#include "ActionDialog.h"

typedef enum {
    WIFI_DIALOG_LOOKUP,
    WIFI_DIALOG_JOIN,
    WIFI_DIALOG_NET_LIST,
} WIFI_DIALOG_TYPE;

typedef struct {
    UI_BUTTON cancel;
    UI_TEXT	text;
} WIFI_WAIT_INIT_DIALOG_UI_CONTROLS;

class WifiWaitInitDialogUI : public ActionDialog{
private:
    WIFI_WAIT_INIT_DIALOG_UI_CONTROLS ui;
    WIFI_DIALOG_TYPE type;
    unsigned char count_down;
    unsigned char join_count_down;
    unsigned char state_machine;
    void updateControls();
protected:
    virtual void createControls();
    virtual void on_button(UI_BUTTON hBtn);
    virtual void refresh_1s();
public:
    void show(WIFI_DIALOG_TYPE type, ActionDialogCallback * callback, u8 id, Widget * caller = 0) {
        this->type = type;
        this->state_machine = 0;
        this->count_down = 2;
        this->join_count_down = 30;
        ActionDialog::show(callback, id, caller);
    }
    WifiWaitInitDialogUI(): ActionDialog() {};
};

extern WifiWaitInitDialogUI wifi_wait_init_dialog_ui;

#endif //WORK_WIFIWAITINITDIALOGUI_H
