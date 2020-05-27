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

class MachineSizeUI : public ConfigurationWidget, public CalculatorDialogCallback{
private:
    MACHINE_SIZE_UI_CONTROLS ui;
    void updateValues();
protected:
    virtual void on_button(UI_BUTTON hBtn);
    virtual void createControls();
public:
    virtual const char * getTitle() {return lang_str.ui_title_config_machine_size;};
    void on_calculator(unsigned char action, double result, unsigned char dialog_id);
    MachineSizeUI() : ConfigurationWidget(STROKE_UI, 1) {};
};

extern MachineSizeUI machine_size_ui;


#endif //WORK_MACHINESIZEUI_H
