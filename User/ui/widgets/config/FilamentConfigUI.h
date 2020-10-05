//
// Created by shubin on 27.05.2020.
//

#ifndef WORK_FILAMENTCONFIGUI_H
#define WORK_FILAMENTCONFIGUI_H

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
    UI_CHECK	filamentDet;
    UI_CHECK	e1;
    UI_CHECK	e2;
} FILAMET_CONFIG_UI_CONTROLS;


class FilamentConfigUI : public ConfigurationWidgetWithCalc{
private:
    FILAMET_CONFIG_UI_CONTROLS ui;
    void updateValues();
    void createSet(FILAMET_CHANGE_UI_CONTROLS_SET * set, unsigned char col);
    unsigned char checkButtonSet(UI_BUTTON hBtn, unsigned char index);
    void _setValue(unsigned char value_id, u32 value);
protected:
    virtual void on_button(UI_BUTTON hBtn);
    virtual void createControls();
    void setValue(unsigned char value_id, double value) {
        this->_setValue(value_id, value);
    }
public:
    virtual const char * getTitle() {return lang_str.ui_title_config_filament;};
    FilamentConfigUI() : ConfigurationWidgetWithCalc(FILAMENT_SETTINGS_UI, 2) {};
};

extern FilamentConfigUI filament_config_ui;

#endif //WORK_FILAMENTCONFIGUI_H
