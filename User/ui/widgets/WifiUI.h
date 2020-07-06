//
// Created by shubin on 05.06.2020.
//

#ifndef WORK_WIFIUI_H
#define WORK_WIFIUI_H

#include "StdWidget.h"

typedef struct {
    UI_TEXT ip;
    UI_TEXT name;
    UI_TEXT state;
    UI_BUTTON ret;
    UI_BUTTON reconnect;
    UI_BUTTON cloud;
} WIFI_UI_CONTROLS;

class WifiUI : public StdWidget, public ActionDialogCallback {
private:
    WIFI_UI_CONTROLS ui;
protected:
    virtual void on_button(UI_BUTTON hBtn);
    virtual void createControls();
    virtual void refresh_1s();
public:
    virtual void on_action_dialog(u8 action, u8 dialog_id);
    virtual const char * getTitle() {return "WiFi";};
    WifiUI() : StdWidget(WIFI_UI) {};
};


extern WifiUI wifi_ui;


#endif //WORK_WIFIUI_H
