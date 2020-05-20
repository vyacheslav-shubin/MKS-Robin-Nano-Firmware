/*
 * ConfirmDialogUI.h
 *
 *  Created on: May 20, 2020
 *      Author: shubin
 */

#ifndef USER_UI_WIDGETS_DIALOG_CONFIRMDIALOGUI_H_
#define USER_UI_WIDGETS_DIALOG_CONFIRMDIALOGUI_H_

#include "DialogWidget.h"

typedef struct {
	UI_BUTTON ok;
	UI_BUTTON cancel;
	UI_TEXT	text;
	UI_PROGRESS_BAR progress;
} CONFIRM_DIALOG_UI_CONTROLS;

class ConfirmDialogUI: public DialogWidget {
private:
	CONFIRM_DIALOG_UI_CONTROLS ui;
	u16 timeout;
	u16 max_timeout;
protected:
	virtual void createControls();
	virtual void on_button(UI_BUTTON hBtn);
	virtual void refresh_1s();
public:
	void show(char * message, ui_dialog_callback cb, Widget * caller = 0);
	void show(char * message, ui_dialog_callback cb, u16 timeout, Widget * caller = 0);
	ConfirmDialogUI() : DialogWidget() {};
};

extern ConfirmDialogUI confirm_dialog_ui;

#endif /* USER_UI_WIDGETS_DIALOG_CONFIRMDIALOGUI_H_ */
