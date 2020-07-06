/*
 * MainUI.h
 *
 *  Created on: May 8, 2020
 *      Author: shubin
 */

#ifndef USER_UI_WIDGETS_MAINUI_H_
#define USER_UI_WIDGETS_MAINUI_H_

#include "StdWidget.h"
#include "ActionDialog.h"

typedef struct {
	UI_BUTTON tools;
    UI_BUTTON settings;
    UI_BUTTON heat_preset;
    UI_BUTTON power;
	UI_BUTTON print;
	UI_BUTTON home;
	UI_BUTTON move;
	UI_BUTTON preheat;
	UI_BUTTON leveling;
	UI_BUTTON filament;
    UI_BUTTON more;
    STATE_BUTTON ext1;
    STATE_BUTTON bed;
} MAIN_UI_CONTROLS;



class MainUI: public StdWidget, public ActionDialogCallback{
private:
    MAIN_UI_CONTROLS ui;
    char current_preheat_preset = 0;
protected:
	virtual void createControls();
	virtual void on_button(UI_BUTTON hBtn);
	virtual void refresh_1s();
public:
    virtual WIDGET_TYPE getType() {return WIDGET_ROOT_UI;};
    virtual void on_action_dialog(u8 action, u8 dialog_id);
	virtual const char * getTitle();
	MainUI() : StdWidget(PRINT_READY_UI){};
};

extern MainUI main_ui;

#endif /* USER_UI_WIDGETS_MAINUI_H_ */
