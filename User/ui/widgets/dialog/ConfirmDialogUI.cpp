/*
 * ConfirmDialogUI.cpp
 *
 *  Created on: May 20, 2020
 *      Author: shubin
 */

#include "ConfirmDialogUI.h"
#include "ui_tools.h"

ConfirmDialogUI confirm_dialog_ui;


void ConfirmDialogUI::on_button(UI_BUTTON hBtn) {
	if (this->callback != 0) {
		if (hBtn==this->ui.ok) {
			this->doAction(UI_BUTTON_OK);
		} else if (hBtn==this->ui.cancel) {
			this->doAction(UI_BUTTON_CANCEL);
		}
	}
}

void ConfirmDialogUI::setMessage(const char * message) {
    this->setText(this->ui.text, message);
}

void ConfirmDialogUI::createControls() {
	memset(&this->ui, 0, sizeof(this->ui));
	this->createDialogDecoration(img_dialog_confirm, 0);
	if (this->flags & CONFIRM_DIALOG_OK_BUTTON)
        this->ui.ok = this->create96x80Button(DIALOG_WIDTH - (96 + 4), DIALOG_HEIGHT - 84, img_ok);
    if (this->flags & CONFIRM_DIALOG_CANCEL_BUTTON)
	    this->ui.cancel = this->create96x80Button(DIALOG_WIDTH - (96 + 4) * 2, DIALOG_HEIGHT - 84, img_cancel);
	if ((this->flags & CONFIRM_DIALOG_PROGRESS) && this->timeout) {
		this->ui.progress = ui_create_std_progbar(100, 20, DIALOG_WIDTH - 100 - 20, 20, this->hWnd);
		PROGBAR_SetValue(this->ui.progress, 0);
	}
    this->ui.text = this->createTextF(10, 60, DIALOG_WIDTH - 20, DIALOG_HEIGHT - 60 - 80 - 10, TEXT_CF_HCENTER | TEXT_CF_VCENTER, this->dialog_message);
};

void ConfirmDialogUI::refresh_1s() {

	if (this->ui.progress && this->timeout) {
		PROGBAR_SetValue(this->ui.progress, (this->max_timeout - this->timeout) * 100 / this->max_timeout);
		if (--this->timeout==0)
			this->doAction(UI_ACTION_TIMEOUT);
	}
}


