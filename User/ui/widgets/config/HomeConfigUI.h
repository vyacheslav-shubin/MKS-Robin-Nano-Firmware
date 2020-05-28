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
} HOME_CONFIG_UI_CONTROLS;

class HomeConfigUI : public ConfigurationWidget, public CalculatorDialogCallback {
private:
    HOME_CONFIG_UI_CONTROLS ui;
    void updateValues();
protected:
    virtual void on_button(UI_BUTTON hBtn);
    virtual void createControls();
public:
    virtual const char * getTitle() {return lang_str.ui_title_config_home;};
    void on_calculator(unsigned char action, double result, unsigned char dialog_id);
    HomeConfigUI() : ConfigurationWidget(HOME_SETTINGS_UI, 1) {};
};


extern HomeConfigUI home_config_ui;

#endif //WORK_HOMECONFIGUI_H
