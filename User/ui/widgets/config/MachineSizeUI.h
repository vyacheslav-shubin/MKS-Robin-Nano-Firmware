//
// Created by shubin on 26.05.2020.
//

#ifndef WORK_MACHINESIZEUI_H
#define WORK_MACHINESIZEUI_H

#include "ConfigurationWidget.h"
#include "../dialog/CalculatorDialogUI.h"

typedef struct {
    UI_INPUT_WITH_DEFAULT 	xmin;
    UI_INPUT_WITH_DEFAULT 	xmax;
    UI_INPUT_WITH_DEFAULT 	ymin;
    UI_INPUT_WITH_DEFAULT 	ymax;
    UI_INPUT_WITH_DEFAULT 	zmin;
    UI_INPUT_WITH_DEFAULT 	zmax;
} MACHINE_SIZE_UI_CONTROLS;

class MachineSizeUI : public ConfigurationWidgetWithCalc{
private:
    MACHINE_SIZE_UI_CONTROLS ui;
    void updateValues();
    void _setValue(unsigned char value_id, float value);
protected:
    virtual void setValue(unsigned char id, double value) {
        this->_setValue(id, value);
    }
    virtual void on_button(UI_BUTTON hBtn);
    virtual void createControls();
public:
    virtual const char * getTitle() {return lang_str.ui_title_config_machine_size;};
    MachineSizeUI() : ConfigurationWidgetWithCalc(STROKE_UI, 1) {};
};

extern MachineSizeUI machine_size_ui;


#endif //WORK_MACHINESIZEUI_H
