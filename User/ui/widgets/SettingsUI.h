/*
 * SettingsUI.h
 *
 *  Created on: May 22, 2020
 *      Author: shubin
 */

#ifndef USER_UI_WIDGETS_SETTINGSUI_H_
#define USER_UI_WIDGETS_SETTINGSUI_H_

#include "StdWidget.h"
#include "dialog/DialogWidget.h"

typedef struct{
	UI_BUTTON wifi;
	UI_BUTTON configuration;
	UI_BUTTON lang;
	UI_BUTTON about;
	UI_BUTTON ret;
} SETTINGS_UI_CONTROLS;

class SettingsUI : public StdWidget, public ActionDialogCallback{
private:
	SETTINGS_UI_CONTROLS ui;
	void action_wifi();
protected:
	virtual void createControls();
	virtual void on_button(UI_BUTTON hBtn);
	virtual void on_action_dialog(u8 action, u8 dialog_id);
public:
	SettingsUI() : StdWidget(SET_UI) {};
};

extern SettingsUI settings_ui;


#endif /* USER_UI_WIDGETS_SETTINGSUI_H_ */
