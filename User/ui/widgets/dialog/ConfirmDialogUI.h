/*
 * ConfirmDialogUI.h
 *
 *  Created on: May 20, 2020
 *      Author: shubin
 */

#ifndef USER_UI_WIDGETS_DIALOG_CONFIRMDIALOGUI_H_
#define USER_UI_WIDGETS_DIALOG_CONFIRMDIALOGUI_H_

#include "DialogWidget.h"
#include "ActionDialog.h"

typedef struct {
	UI_BUTTON ok;
	UI_BUTTON cancel;
	UI_TEXT	text;
	UI_PROGRESS_BAR progress;
} CONFIRM_DIALOG_UI_CONTROLS;

class ConfirmDialogUI;

#define CONFIRM_DIALOG_OK_BUTTON                ((u8)(1<<0))
#define CONFIRM_DIALOG_CANCEL_BUTTON            ((u8)(1<<1))
#define CONFIRM_DIALOG_PROGRESS                 ((u8)(1<<2))
#define CONFIRM_DIALOG_ALL                      ((u8)0xFF)


class ConfirmDialogUI: public ActionDialog {
private:
	CONFIRM_DIALOG_UI_CONTROLS ui;
	u16 timeout;
	u16 max_timeout;
	u8 flags;
    const char * dialog_message = 0;
protected:
	virtual void createControls();
	virtual void on_button(UI_BUTTON hBtn);
	virtual void refresh_1s();
public:
    virtual void setMessage(const char * message);
    void showEx(const char * message, ActionDialogCallback * callback, u8 id, u16 timeout, u8 flags) {
        this->dialog_message = message;
        this->max_timeout = timeout;
        this->timeout = timeout;
        this->flags = flags;
        ActionDialog::show(callback, id, 0);
    }
	void show(const char * message, ActionDialogCallback * callback, u8 id = 0, Widget * caller = 0) {
        this->show(message, callback, 0, id,  caller);
    }
	void show(const char * message, ActionDialogCallback * callback, u16 timeout, u8 id, Widget * caller = 0) {
        this->showEx(message, callback, id, timeout, CONFIRM_DIALOG_ALL);
    }
	ConfirmDialogUI() : ActionDialog() {};
};

extern ConfirmDialogUI confirm_dialog_ui;

#endif /* USER_UI_WIDGETS_DIALOG_CONFIRMDIALOGUI_H_ */
