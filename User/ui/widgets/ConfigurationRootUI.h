/*
 * MachineSettingsRootUI.h
 *
 *  Created on: May 18, 2020
 *      Author: shubin
 */

#ifndef USER_UI_WIDGETS_CONFIGURATIONROOTUI_H_
#define USER_UI_WIDGETS_CONFIGURATIONROOTUI_H_

#include "StdWidget.h"

typedef struct {
	UI_BUTTON machine;
	UI_BUTTON temperature;
	UI_BUTTON motors;
	UI_BUTTON advanced;
	UI_BUTTON back;
} MACHINE_SETTINGS_ROOT_UI_CONTROLS;

class ConfigurationRootUI : public StdWidget {
private:
	MACHINE_SETTINGS_ROOT_UI_CONTROLS ui;
protected:
	virtual void createControls();
	virtual void on_button(UI_BUTTON hBtn);
public:
	virtual char * getTitle() {return lang_str.ui_title_config;};
	ConfigurationRootUI() : StdWidget(MACHINE_PARA_UI) {};
};

extern ConfigurationRootUI configuration_root_ui;

#endif /* USER_UI_WIDGETS_CONFIGURATIONROOTUI_H_ */
