/*
 * MotorDirConfigUI.h
 *
 *  Created on: May 19, 2020
 *      Author: shubin
 */

#ifndef USER_UI_WIDGETS_CONFIG_MOTORDIRCONFIGUI_H_
#define USER_UI_WIDGETS_CONFIG_MOTORDIRCONFIGUI_H_

#include "ConfigurationWidget.h"

typedef struct {
	UI_CHECK	x;
	UI_CHECK	y;
	UI_CHECK	z;
	UI_CHECK	e0;
	UI_CHECK	e1;
} MOTOR_DIR_CONFIG_UI_CONTROLS;

class MotorDirConfigUI: public ConfigurationWidget {
private:
	MOTOR_DIR_CONFIG_UI_CONTROLS ui;
protected:
	virtual const char * getTitle() {return lang_str.config_ui.axes_inversion;};
	virtual void on_button(UI_BUTTON hBtn);
	virtual void createControls();
public:
	MotorDirConfigUI() : ConfigurationWidget(MOTORDIR_UI, 1) {};
};

extern MotorDirConfigUI motor_dir_config_ui;

#endif /* USER_UI_WIDGETS_CONFIG_MOTORDIRCONFIGUI_H_ */
