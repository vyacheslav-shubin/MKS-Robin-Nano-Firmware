//
// Created by shubin on 19.10.2020.
//

#ifndef WORK_WIFICONFIGUI_H
#define WORK_WIFICONFIGUI_H


#include "ConfigurationWidget.h"

typedef struct {
    UI_CHECK	exists;
    UI_CHECK	ntp_support;
    UI_INPUT_WITH_DEFAULT timeShift;
} WIFI_CONFIG_UI_CONTROLS;

class WiFiConfigUI : public ConfigurationWidgetWithCalc {
private:
    WIFI_CONFIG_UI_CONTROLS ui;
    void updateControls();
protected:
    virtual void on_button(UI_BUTTON hBtn);
    virtual void createControls();
    virtual void _setValue(unsigned char id, short value);
public:
    virtual void setValue(unsigned char id, double value) {
        _setValue(id, value);
    }
    virtual void on_action_dialog(u8 action, u8 dialog_id);
    virtual const char * getTitle() {return lang_str.wifi;};
    WiFiConfigUI() : ConfigurationWidgetWithCalc(WIFI_CONFIG_UI, 1) {};
};

extern WiFiConfigUI wifi_config_ui;

#endif //WORK_WIFICONFIGUI_H
