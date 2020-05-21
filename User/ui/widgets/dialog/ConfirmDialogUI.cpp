/*
 * ConfirmDialogUI.cpp
 *
 *  Created on: May 20, 2020
 *      Author: shubin
 */

#include "ConfirmDialogUI.h"
#include "ui_tools.h"

ConfirmDialogUI confirm_dialog_ui;

static char * dialog_message = 0;

void ConfirmDialogUI::on_button(UI_BUTTON hBtn) {
	if (this->callback != 0) {
		if (hBtn==this->ui.ok) {
			this->callback->on_confirm_dialog(UI_BUTTON_OK, this->id);
			//dialog_cb(UI_BUTTON_OK);
		} else if (hBtn==this->ui.cancel) {
			this->callback->on_confirm_dialog(UI_BUTTON_CANCEL, this->id);
			//dialog_cb(UI_BUTTON_CANCEL);
		}
	}
}

void ConfirmDialogUI::show(char * message, ConfirmDialogCallback * callback, u8 id = 0, Widget * caller=0) {
	this->show(message, callback, 0, id,  caller);
}

void ConfirmDialogUI::show(char * message, ConfirmDialogCallback * callback, u16 timeout, u8 id = 0, Widget * caller=0) {
	dialog_message = message;
	this->callback = callback;
	this->max_timeout = timeout;
	this->timeout = timeout;
	this->id = id;
	Widget::show(caller);
}

void ConfirmDialogUI::createControls() {
	memset(&this->ui, 0, sizeof(this->ui));
	this->createDialogDecoration(img_dialog_confirm, 0);
	this->ui.ok = this->create100x80Button(DIALOG_WIDTH - 100 - 4, DIALOG_HEIGHT - 84, img_ok);
	this->ui.cancel = this->create100x80Button(DIALOG_WIDTH - 200 - 8, DIALOG_HEIGHT - 84, img_cancel);
	this->ui.text = this->createTextF(10, 60, DIALOG_WIDTH - 20, DIALOG_HEIGHT - 60 - 80 - 10, TEXT_CF_HCENTER | TEXT_CF_VCENTER, dialog_message);
	if (this->timeout!=0) {
		this->ui.progress = ui_create_std_progbar(100, 20, DIALOG_WIDTH - 100 - 20, 20, this->hWnd);
		PROGBAR_SetValue(this->ui.progress, 0);
	}
};

void ConfirmDialogUI::refresh_1s() {
	if ((this->callback!=0) && (this->timeout != 0)) {
		PROGBAR_SetValue(this->ui.progress, (this->max_timeout - this->timeout) * 100 / this->max_timeout);
		if (--this->timeout==0)
			this->callback->on_confirm_dialog(UI_BUTTON_TIMEOUT, this->id);
	}
}


