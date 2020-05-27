//
// Created by shubin on 27.05.2020.
//

#ifndef WORK_FILAMENTCHANGECONFIGUI_H
#define WORK_FILAMENTCHANGECONFIGUI_H

#include "ConfigurationWidget.h"
#include "../dialog/CalculatorDialogUI.h"

typedef struct {
    UI_INPUT_WITH_DEFAULT 	termerature;
    UI_INPUT_WITH_DEFAULT 	speed;
    UI_INPUT_WITH_DEFAULT 	length;
} FILAMET_CHANGE_UI_CONTROLS_SET;

typedef struct {
    FILAMET_CHANGE_UI_CONTROLS_SET 	load;
    FILAMET_CHANGE_UI_CONTROLS_SET 	unload;
} FILAMET_CHANGE_UI_CONTROLS;


class FilamentChangeConfigUI : public ConfigurationWidget, public CalculatorDialogCallback{
private:
    FILAMET_CHANGE_UI_CONTROLS ui;
    void updateValues();
    void createSet(FILAMET_CHANGE_UI_CONTROLS_SET * set, unsigned char col);
    unsigned char checkButtonSet(UI_BUTTON hBtn, unsigned char index);
protected:
    virtual void on_button(UI_BUTTON hBtn);
    virtual void createControls();
public:
    virtual const char * getTitle() {return lang_str.ui_title_config_filament_change;};
    void on_calculator(unsigned char action, double result, unsigned char dialog_id);
    FilamentChangeConfigUI() : ConfigurationWidget(FILAMENT_SETTINGS_UI, 1) {};
};

extern FilamentChangeConfigUI filament_change_config_ui;

#endif //WORK_FILAMENTCHANGECONFIGUI_H
