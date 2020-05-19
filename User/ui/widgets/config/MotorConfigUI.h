/*
 * MotorConfigUI.h
 *
 *  Created on: May 19, 2020
 *      Author: shubin
 */

#ifndef USER_UI_WIDGETS_CONFIG_MOTORCONFIGUI_H_
#define USER_UI_WIDGETS_CONFIG_MOTORCONFIGUI_H_

#include "ConfigurationWidget.h"

typedef struct {
	UI_ARROW axes;
	UI_ARROW steps;
	UI_ARROW home_speed;
	UI_ARROW acceleration;
	UI_ARROW max_speed;
	UI_ARROW jerk;
} MOTOR_CONFIG_UI_CONTROLS;

class MotorConfigUI: public ConfigurationWidget {
private:
	MOTOR_CONFIG_UI_CONTROLS ui;
protected:
	virtual void on_button(UI_BUTTON hBtn);
	virtual void createControls();
public:
	virtual char * getTitle() {return lang_str.config_ui.simple_main_ui;};
	MotorConfigUI() : ConfigurationWidget(MOTOR_SETTINGS_UI, 1, 1) {};;
};

extern MotorConfigUI motor_config_ui;


#endif /* USER_UI_WIDGETS_CONFIG_MOTORCONFIGUI_H_ */
