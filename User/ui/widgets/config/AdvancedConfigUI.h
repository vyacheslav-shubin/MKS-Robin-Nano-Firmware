/*
 * AdvancedConfigUI.h
 *
 *  Created on: May 21, 2020
 *      Author: shubin
 */

#ifndef USER_UI_WIDGETS_CONFIG_ADVANCEDCONFIGUI_H_
#define USER_UI_WIDGETS_CONFIG_ADVANCEDCONFIGUI_H_

#include "ConfigurationWidget.h"

typedef struct {
	UI_CHECK	autoPowerOff;
	UI_CHECK	simpleMainUI;
	UI_CHECK	diplayBackLight;
    //todo: доделать
    UI_CHECK	wifiScan;
} ADVANCED_CONFIG_UI_CONTROLS;

class AdvancedConfigUI  : public ConfigurationWidget {
private:
	ADVANCED_CONFIG_UI_CONTROLS ui;
protected:
	virtual void on_button(UI_BUTTON hBtn);
	virtual void createControls();
public:
	AdvancedConfigUI() : ConfigurationWidget(ADVANCED_UI) {};
};

extern AdvancedConfigUI advanced_config_ui;

#endif /* USER_UI_WIDGETS_CONFIG_ADVANCEDCONFIGUI_H_ */
