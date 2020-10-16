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
	UI_CHECK	simpleMainUI;
	UI_CHECK	diplayBackLight;
	UI_INPUT_WITH_DEFAULT timeShift;
	UI_CONFIG_BUTTON restore;

} ADVANCED_CONFIG_UI_CONTROLS;

class AdvancedConfigUI  : public ConfigurationWidgetWithCalc, public ActionDialogCallback {
private:
	ADVANCED_CONFIG_UI_CONTROLS ui;
	void updateControls();
    virtual void _setValue(unsigned char id, short value);
protected:
    virtual void setValue(unsigned char id, double value) {
        _setValue(id, value);
    }
	virtual void on_button(UI_BUTTON hBtn);
	virtual void createControls();
public:
    virtual void on_action_dialog(u8 action, u8 dialog_id);
    virtual const char * getTitle() {return lang_str.ui_title_config_advanced;};
	AdvancedConfigUI() : ConfigurationWidgetWithCalc(ADVANCED_UI, 1) {};
};

extern AdvancedConfigUI advanced_config_ui;

#endif /* USER_UI_WIDGETS_CONFIG_ADVANCEDCONFIGUI_H_ */
