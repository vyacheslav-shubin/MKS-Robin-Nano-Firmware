/*
 * MainUI.h
 *
 *  Created on: May 8, 2020
 *      Author: shubin
 */

#ifndef USER_UI_WIDGETS_MAINUI_H_
#define USER_UI_WIDGETS_MAINUI_H_

#include "StdWidget.h"

typedef struct {
	UI_BUTTON tools;
	UI_BUTTON settings;
	UI_BUTTON print;
	UI_BUTTON home;
	UI_BUTTON move;
	UI_BUTTON preheat;
	UI_BUTTON leveling;
	UI_BUTTON filament;
	UI_BUTTON more;
} MAIN_UI_CONTROLS;


class MainUI: public StdWidget{
private:
	MAIN_UI_CONTROLS ui;
protected:
	virtual void createControls();
	virtual void on_button(UI_BUTTON hBtn);
public:
	MainUI() : StdWidget(PRINT_READY_UI){};
};

extern MainUI main_ui;

#endif /* USER_UI_WIDGETS_MAINUI_H_ */
