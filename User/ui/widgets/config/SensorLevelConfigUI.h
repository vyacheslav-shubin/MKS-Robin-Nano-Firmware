//
// Created by shubin on 28.05.2020.
//

#ifndef WORK_SENSORLEVELCONFIGUI_H
#define WORK_SENSORLEVELCONFIGUI_H

#include "ConfigurationWidget.h"

typedef struct {
    UI_CHECK	mn;
    UI_CHECK	mx;
} UI_MIN_MAX_CHECK;

typedef struct {
    UI_MIN_MAX_CHECK	x;
    UI_MIN_MAX_CHECK	y;
    UI_MIN_MAX_CHECK	z;
    UI_CHECK	lvl;
    UI_CHECK	e1;
    UI_CHECK	e2;
} SENSOR_LEVEL_CONFIG_UI_CONTROLS;

class SensorLevelConfigUI : public ConfigurationWidget {
private:
    SENSOR_LEVEL_CONFIG_UI_CONTROLS ui;
protected:
    virtual const char *getTitle() { return lang_str.ui_title_sensor_levels; };
    virtual void on_button(UI_BUTTON hBtn);
    virtual void createControls();
public:
    SensorLevelConfigUI() : ConfigurationWidget(ENDSTOP_TYPE_UI, 1) {};
};

extern SensorLevelConfigUI sensor_level_config_ui;


#endif //WORK_SENSORLEVELCONFIGUI_H
