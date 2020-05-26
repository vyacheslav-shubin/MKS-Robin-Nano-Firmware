//
// Created by shubin on 26.05.2020.
//

#ifndef WORK_MACHINESIZEUI_H
#define WORK_MACHINESIZEUI_H

#include "ConfigurationWidget.h"

class MachineSizeUI : public ConfigurationWidget{
protected:
    virtual void on_button(UI_BUTTON hBtn);
    virtual void createControls();
public:
    MachineSizeUI() : ConfigurationWidget(STROKE_UI, 1) {};
};

extern MachineSizeUI machine_size_ui;


#endif //WORK_MACHINESIZEUI_H
