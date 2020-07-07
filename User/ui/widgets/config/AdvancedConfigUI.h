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
	UI_INPUT_WITH_DEFAULT timeShift;
    UI_CHECK	powerDet;
    UI_CHECK	filamentDet;
    //todo: доделать
    UI_CHECK	wifiScan;
} ADVANCED_CONFIG_UI_CONTROLS;

class AdvancedConfigUI  : public ConfigurationWidgetWithCalc {
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
	AdvancedConfigUI() : ConfigurationWidgetWithCalc(ADVANCED_UI, 2) {};
};

extern AdvancedConfigUI advanced_config_ui;

#endif /* USER_UI_WIDGETS_CONFIG_ADVANCEDCONFIGUI_H_ */
