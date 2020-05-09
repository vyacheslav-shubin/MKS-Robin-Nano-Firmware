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
	BUTTON_Handle tools;
	BUTTON_Handle settings;
	BUTTON_Handle print;
	BUTTON_Handle home;
	BUTTON_Handle move;
	BUTTON_Handle preheat;
	BUTTON_Handle leveling;
	BUTTON_Handle extrude;
	BUTTON_Handle more;
} MAIN_UI_CONTROLS;


class MainUI: public StdWidget{
private:
	MAIN_UI_CONTROLS buttons;
protected:
	virtual void createControls();
	virtual void on_button(WM_HWIN hBtn);
public:
	MainUI() : StdWidget(PRINT_READY_UI){};
};

extern MainUI main_ui;

#endif /* USER_UI_WIDGETS_MAINUI_H_ */
