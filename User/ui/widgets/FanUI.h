/*
 * FanUI.h
 *
 *  Created on: May 12, 2020
 *      Author: shubin
 */

#ifndef USER_UI_WIDGETS_FANUI_H_
#define USER_UI_WIDGETS_FANUI_H_

#include "StdWidget.h"

typedef struct {
	UI_BUTTON plus;
	UI_BUTTON minus;
	UI_BUTTON run100;
	UI_BUTTON run50;
	UI_BUTTON run0;
	UI_BUTTON back;
	STATE_BUTTON fan;
    char auto_change;
    unsigned char auto_delay;
} FUN_UI_CONTROLS;

class FanUI: public StdWidgetWithCalc {
private:
	FUN_UI_CONTROLS ui;
protected:
    virtual void setValue(unsigned char id, double value);
	virtual void createControls();
	virtual void on_button(UI_BUTTON hBtn);
    virtual void on_button_click(UI_BUTTON hBtn);
    virtual void refresh_025();
	virtual void refresh_05();
public:
	virtual const char * getTitle() {return lang_str.ui_title_fan;};
	FanUI() : StdWidgetWithCalc(FAN_UI) {};
};
extern FanUI fan_ui;

#endif /* USER_UI_WIDGETS_FANUI_H_ */
