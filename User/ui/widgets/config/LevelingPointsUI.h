//
// Created by shubin on 27.05.2020.
//

#ifndef WORK_LEVELINGPOINTSUI_H
#define WORK_LEVELINGPOINTSUI_H

#include "ConfigurationWidget.h"
#include "../dialog/CalculatorDialogUI.h"


typedef struct {
    UI_INPUT_DUAL points[5];
    UI_TEXT x_info;
    UI_TEXT y_info;
    UI_BUTTON deflt;
} LEVELING_POINTS_CONFIG_UI_CONTROLS;

class LevelingPointsUI : public ConfigurationWidget, public CalculatorDialogCallback{
private:
    LEVELING_POINTS_CONFIG_UI_CONTROLS ui;
    void updateValues();
    unsigned char checkButtonSet(UI_BUTTON hBtn, unsigned char index);
protected:
    virtual void on_button(UI_BUTTON hBtn);
    virtual void createControls();
public:
    virtual const char * getTitle() {return lang_str.ui_title_leveling_points;};
    void on_calculator(unsigned char action, double result, unsigned char dialog_id);
    LevelingPointsUI() : ConfigurationWidget(XYZ_LEVELING_PARA_UI, 1) {};
};


extern LevelingPointsUI leveling_points_ui;

#endif //WORK_LEVELINGPOINTSUI_H
