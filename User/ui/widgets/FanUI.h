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
} FUN_UI_CONTROLS;

class FanUI: public StdWidget {
private:
	FUN_UI_CONTROLS ui;
	void commitFanState();
protected:
	virtual void createControls();
	virtual void on_button(UI_BUTTON hBtn);
	virtual void refresh_05();
public:
	FanUI() : StdWidget(FAN_UI) {};
};
extern FanUI fan_ui;

#endif /* USER_UI_WIDGETS_FANUI_H_ */
