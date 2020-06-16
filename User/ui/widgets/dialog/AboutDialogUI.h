/*
 * AboutDialogUI.h
 *
 *  Created on: May 22, 2020
 *      Author: shubin
 */

#ifndef USER_UI_WIDGETS_DIALOG_ABOUTDIALOGUI_H_
#define USER_UI_WIDGETS_DIALOG_ABOUTDIALOGUI_H_

#include "DialogWidget.h"
#include "ActionDialog.h"

typedef struct {
	UI_BUTTON ok;
    UI_TEXT	date;
    UI_TEXT	powered_by;
} ABOUT_DIALOG_UI_CONTROLS;

class AboutDialogUI : public ActionDialog{
private:
	ABOUT_DIALOG_UI_CONTROLS ui;
protected:
	virtual void createControls();
    virtual void on_button(UI_BUTTON hBtn);
public:
	AboutDialogUI() : ActionDialog() {};
};

extern AboutDialogUI about_dialog_ui;

#endif /* USER_UI_WIDGETS_DIALOG_ABOUTDIALOGUI_H_ */
