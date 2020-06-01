//
// Created by shubin on 28.05.2020.
//

#ifndef WORK_HOMECONFIGUI_H
#define WORK_HOMECONFIGUI_H

#include "ConfigurationWidget.h"
#include "../dialog/CalculatorDialogUI.h"

typedef struct {
    UI_TEXT placement;
    UI_TEXT speed;
    UI_CHECK x_dir;
    UI_CHECK y_dir;
    UI_CHECK z_dir;
    UI_INPUT_WITH_DEFAULT xy_speed;
    UI_INPUT_WITH_DEFAULT z_speed;
    UI_TEXT pause;
    UI_INPUT_WITH_DEFAULT px;
    UI_INPUT_WITH_DEFAULT py;
    UI_INPUT_WITH_DEFAULT pz;
    UI_INPUT_WITH_DEFAULT pe_unload;
    UI_INPUT_WITH_DEFAULT pe_load;
} HOME_CONFIG_UI_CONTROLS;

class HomeConfigUI : public ConfigurationWidgetWithCalc {
private:
    HOME_CONFIG_UI_CONTROLS ui;
    void updateValues();
    void _setValue(unsigned char id, float value);
protected:
    virtual void on_button(UI_BUTTON hBtn);
    virtual void createControls();
    virtual void setValue(unsigned char id, double value) {
        this->_setValue(id, value);
    }
public:
    virtual const char * getTitle() {return lang_str.ui_title_config_home;};
    HomeConfigUI() : ConfigurationWidgetWithCalc(HOME_SETTINGS_UI, 2) {};
};


extern HomeConfigUI home_config_ui;

#endif //WORK_HOMECONFIGUI_H
