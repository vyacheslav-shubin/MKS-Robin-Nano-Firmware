//
// Created by shubin on 02.06.2020.
//

#ifndef WORK_MOTORSETTINGSUI_H
#define WORK_MOTORSETTINGSUI_H

#include "ConfigurationWidget.h"

typedef struct {
    UI_CHECK	x;
    UI_CHECK	y;
    UI_CHECK	z;
    UI_CHECK	e0;
    UI_CHECK	e1;
} _MOTOR_DIR_CONFIG_UI_CONTROLS;

typedef struct {
    UI_INPUT_WITH_DEFAULT 	x;
    UI_INPUT_WITH_DEFAULT	y;
    UI_INPUT_WITH_DEFAULT	z;
    UI_INPUT_WITH_DEFAULT	e0;
    UI_INPUT_WITH_DEFAULT	e1;
} MOTOR_STEPS_CONFIG_UI_CONTROLS;

typedef MOTOR_STEPS_CONFIG_UI_CONTROLS MOTOR_SPEED_CONFIG_UI_CONTROLS;

typedef struct {
    UI_INPUT_WITH_DEFAULT 	x;
    UI_INPUT_WITH_DEFAULT	y;
    UI_INPUT_WITH_DEFAULT	z;
    UI_INPUT_WITH_DEFAULT	e0;
} MOTOR_JERK_CONFIG_UI_CONTROLS;


typedef struct {
    UI_INPUT_WITH_DEFAULT 	print;
    UI_INPUT_WITH_DEFAULT 	retract;
    UI_INPUT_WITH_DEFAULT 	travel;
    UI_INPUT_WITH_DEFAULT 	x;
    UI_INPUT_WITH_DEFAULT	y;
    UI_INPUT_WITH_DEFAULT	z;
    UI_INPUT_WITH_DEFAULT	e0;
    UI_INPUT_WITH_DEFAULT	e1;
} MOTOR_ACCELERATIONS_CONFIG_UI_CONTROLS;

typedef struct {
    _MOTOR_DIR_CONFIG_UI_CONTROLS           dir;
    MOTOR_STEPS_CONFIG_UI_CONTROLS          steps;
    MOTOR_ACCELERATIONS_CONFIG_UI_CONTROLS  accel;
    MOTOR_SPEED_CONFIG_UI_CONTROLS          speed;
    MOTOR_JERK_CONFIG_UI_CONTROLS           jerk;
} MOTOR_SETTINGS_UI_CONTROLS;

class MotorSettingsUI : public ConfigurationWidgetWithCalc{
private:
    MOTOR_SETTINGS_UI_CONTROLS ui;
    static void _setValue(unsigned char index, float value);
    void updateControls();
protected:
    virtual void on_button(UI_BUTTON hBtn);
    virtual void createControls();
    virtual void setValue(unsigned char id, double value) {
        this->_setValue(id, value);
    }
public:
    virtual const char * getTitle() {
        switch (this->page) {
            case 0:
                return lang_str.config_ui.axes_inversion;
            case 1:
                return lang_str.config_ui.motor_steps_settings;
            case 2:
                return lang_str.config_ui.acceleration_settings;
            case 3:
                return lang_str.config_ui.maximum_speed_settings;
            case 4:
                return lang_str.config_ui.jerk_settings;
            default:
                return  lang_str.ui_title_motor_settings;
        }
    };
    MotorSettingsUI() : ConfigurationWidgetWithCalc(MOTOR_SETTINGS_UI, 5) {};
};


extern MotorSettingsUI motor_settings_ui;

#endif //WORK_MOTORSETTINGSUI_H
