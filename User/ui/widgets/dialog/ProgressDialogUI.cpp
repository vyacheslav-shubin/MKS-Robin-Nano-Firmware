//
// Created by shubin on 08.06.2020.
//

#include "ProgressDialogUI.h"
#include "ui_tools.h"

ProgressDialogUI progress_dialog_ui;

void ProgressDialogUI::createControls() {
    this->ui.progress = ui_create_std_progbar(40, DIALOG_HEIGHT/3 - 10, DIALOG_WIDTH - 80, 20, this->hWnd);
    this->ui.text = this->createTextF(20, DIALOG_HEIGHT/2, DIALOG_WIDTH - 40, DIALOG_HEIGHT/2 - 40, TEXT_CF_HCENTER | TEXT_CF_TOP, this->message);
    PROGBAR_SetValue(this->ui.progress, 0);
}

void ProgressDialogUI::setMessage(const char * message) {
    this->setText(this->ui.text, message);
}

void ProgressDialogUI::setProgress(unsigned char progress) {
    PROGBAR_SetValue(this->ui.progress, progress);
}
