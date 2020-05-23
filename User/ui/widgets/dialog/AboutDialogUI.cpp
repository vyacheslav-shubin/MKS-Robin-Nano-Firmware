/*
 * AboutDialogUI.cpp
 *
 *  Created on: May 22, 2020
 *      Author: shubin
 */

#include "AboutDialogUI.h"
#include "../../ui_tools.h"

AboutDialogUI about_dialog_ui;

void AboutDialogUI::on_message(WM_MESSAGE * pMsg) {
	switch (pMsg->MsgId) {
    	case WM_TOUCH:
    	case WM_TOUCH_CHILD:
            this->doAction(UI_BUTTON_OK);
    		break;
	}
}

void AboutDialogUI::createControls() {
    memset(&this->ui, 0, sizeof(this->ui));
	this->createDialogDecoration(img_dialog_info, 0);
	this->ui.ok = this->create96x80Button(DIALOG_WIDTH - 100 - 4, DIALOG_HEIGHT - 84, img_ok);
	this->ui.powered_by = this->createTextF(10, 60, DIALOG_WIDTH - 20, DIALOG_HEIGHT - 60 - 80 - 10, TEXT_CF_HCENTER | TEXT_CF_VCENTER, lang_str.dialog.refactored_by);
#ifdef BUILD_DATE
    this->ui.date = this->createText(10, DIALOG_HEIGHT - 40, DIALOG_WIDTH - 120, 38, BUILD_DATE);
#endif

}

