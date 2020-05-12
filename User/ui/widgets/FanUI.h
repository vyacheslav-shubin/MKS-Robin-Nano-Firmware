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
	BUTTON_Handle plus;
	BUTTON_Handle minus;
	BUTTON_Handle run100;
	BUTTON_Handle run50;
	BUTTON_Handle run0;
	BUTTON_Handle back;
	STATE_BUTTON fan;
} FUN_UI_CONTROLS;

class FanUI: public StdWidget {
private:
	FUN_UI_CONTROLS ui;
	void commitFanState();
protected:
	virtual void createControls();
	virtual void on_button(WM_HWIN hBtn);
	virtual void refresh_05();
public:
	FanUI() : StdWidget(FAN_UI) {};
};
extern FanUI fan_ui;

#endif /* USER_UI_WIDGETS_FANUI_H_ */
