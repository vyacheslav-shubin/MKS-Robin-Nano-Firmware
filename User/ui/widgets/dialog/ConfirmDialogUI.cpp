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
static ui_dialog_callback dialog_cb;

void ConfirmDialogUI::on_button(UI_BUTTON hBtn) {
	if (dialog_cb!=0) {
		if (hBtn==this->ui.ok) {
			dialog_cb(UI_BUTTON_OK);
		} else if (hBtn==this->ui.cancel) {
			dialog_cb(UI_BUTTON_CANCEL);
		}
	}
}


void ConfirmDialogUI::show(char * message, ui_dialog_callback cb, Widget * caller = 0) {
	dialog_message = message;
	dialog_cb = cb;
	Widget::show(caller);
}

void ConfirmDialogUI::createControls() {
	memset(&this->ui, 0, sizeof(this->ui));
	this->createDialogDecoration(img_dialog_confirm, 0);
	this->ui.ok = this->create100x80Button(DIALOG_WIDTH - 100 - 4, DIALOG_HEIGHT - 84, img_ok);
	this->ui.cancel = this->create100x80Button(DIALOG_WIDTH - 200 - 8, DIALOG_HEIGHT - 84, img_cancel);
	this->ui.text = this->createTextF(10, 60, DIALOG_WIDTH - 20, DIALOG_HEIGHT - 60 - 80 - 10, TEXT_CF_HCENTER | TEXT_CF_VCENTER, dialog_message);
};

