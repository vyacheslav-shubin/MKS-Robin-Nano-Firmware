/*
 * SpeedUI.h
 *
 *  Created on: May 14, 2020
 *      Author: shubin
 */

#ifndef USER_UI_WIDGETS_SPEEDUI_H_
#define USER_UI_WIDGETS_SPEEDUI_H_

#include "StdWidget.h"

typedef struct{
	STATE_BUTTON ext1;
	STATE_BUTTON ext2;
	STATE_BUTTON speed;
	UI_BUTTON plus;
	UI_BUTTON minus;
	UI_BUTTON ret;
	UI_SELECTOR selector;
	UI_SELECTOR step;
} SPEED_UI_CONTROLS;

class SpeedUI: public StdWidget {
private:
	SPEED_UI_CONTROLS ui;
	void updateStepButton();
	void updateSelectorButton();
	void updateSpeed(char direction);
protected:
	virtual void createControls();
	virtual void on_button(UI_BUTTON hBtn);
	virtual void refresh_1s();
public:
	virtual char * getTitle() {return lang_str.ui_title_speed;};
	SpeedUI() : StdWidget(CHANGE_SPEED_UI) {};
};

extern SpeedUI speed_ui;

#endif /* USER_UI_WIDGETS_SPEEDUI_H_ */
