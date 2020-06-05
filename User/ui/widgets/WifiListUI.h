//
// Created by shubin on 03.06.2020.
//

#ifndef WORK_WIFILISTUI_H
#define WORK_WIFILISTUI_H

#include "config/ConfigurationWidget.h"

typedef struct {
    UI_RADIO wifi[8];
} WIFI_LIST_UI_CONTROLS;

class WifiListUI : public ConfigurationWidget, public ActionDialogCallback {
private:
    WIFI_LIST_UI_CONTROLS ui;
protected:
    virtual void on_button(UI_BUTTON hBtn);
    virtual void createControls();
public:
    virtual void on_action_dialog(u8 action, u8 dialog_id);
    virtual const char * getTitle() {return lang_str.ui_title_wifi_list;};
    WifiListUI() : ConfigurationWidget(WIFI_LIST_UI, 1) {};
};

extern WifiListUI wifi_list_ui;


#endif //WORK_WIFILISTUI_H
