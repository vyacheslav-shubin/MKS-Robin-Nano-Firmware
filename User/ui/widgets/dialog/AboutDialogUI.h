/*
 * AboutDialogUI.h
 *
 *  Created on: May 22, 2020
 *      Author: shubin
 */

#ifndef USER_UI_WIDGETS_DIALOG_ABOUTDIALOGUI_H_
#define USER_UI_WIDGETS_DIALOG_ABOUTDIALOGUI_H_

#include "DialogWidget.h"

typedef struct {
	UI_BUTTON ok;
    UI_TEXT	date;
    UI_TEXT	powered_by;
} ABOUT_DIALOG_UI_CONTROLS;

class AboutDialogUI : public DialogWidget{
private:
	ABOUT_DIALOG_UI_CONTROLS ui;
	ActionDialogCallback * callback = 0;
protected:
	virtual void createControls();
public:
	virtual void on_message(WM_MESSAGE * pMsg);
	void show(ActionDialogCallback * callback, Widget * caller = 0);
	AboutDialogUI() : DialogWidget() {};
};

extern AboutDialogUI about_dialog_ui;

#endif /* USER_UI_WIDGETS_DIALOG_ABOUTDIALOGUI_H_ */
