//
// Created by shubin on 29.05.2020.
//

#ifndef WORK_LEVELINGAUTOCONFIGUI_H
#define WORK_LEVELINGAUTOCONFIGUI_H

#include "ConfigurationWidget.h"
#include "../dialog/CalculatorDialogUI.h"

typedef struct {
    UI_CHECK enable;
    UI_CHECK connector;
    UI_TEXT label;
    UI_TEXT offset_label;
    UI_INPUT_WITH_DEFAULT x_offset;
    UI_INPUT_WITH_DEFAULT y_offset;
    UI_INPUT_WITH_DEFAULT z_offset;
    UI_TEXT speed_label;
    UI_INPUT_WITH_DEFAULT xy_speed;
    UI_INPUT_WITH_DEFAULT z_speed_fast;
    UI_INPUT_WITH_DEFAULT z_speed_slow;
} LEVELING_AUTO_PROBE_UI_CONTROLS;

typedef struct {
    UI_CHECK autoleveling_enable;
    LEVELING_AUTO_PROBE_UI_CONTROLS probe;
} LEVELING_AUTO_CONFIG_UI_CONTROLS;


class LevelingAutoConfigUI  : public ConfigurationWidget, public CalculatorDialogCallback{
private:
    LEVELING_AUTO_CONFIG_UI_CONTROLS ui;
    void updateControls();
protected:
    virtual void on_button(UI_BUTTON hBtn);
    virtual void createControls();
public:
    virtual const char * getTitle() {return lang_str.ui_title_leveling_points;};
    void on_calculator(unsigned char action, double result, unsigned char dialog_id);
    LevelingAutoConfigUI() : ConfigurationWidget(LEVELING_PARA_UI, 2) {};
};

extern LevelingAutoConfigUI leveling_auto_config_ui;


#endif //WORK_LEVELINGAUTOCONFIGUI_H
