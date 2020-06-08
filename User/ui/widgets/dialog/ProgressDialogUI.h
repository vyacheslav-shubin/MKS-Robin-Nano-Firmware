//
// Created by shubin on 08.06.2020.
//

#ifndef WORK_PROGRESSDIALOGUI_H
#define WORK_PROGRESSDIALOGUI_H

#include "DialogWidget.h"
#include "ui_tools.h"

typedef struct {
    UI_TEXT	text;
    UI_PROGRESS_BAR progress;
} PROGRESS_DIALOG_UI_CONTROLS;

class ProgressDialogUI: public DialogWidget {
private:
    PROGRESS_DIALOG_UI_CONTROLS ui;
    const char * message;
protected:
    virtual void createControls();
public:
    void setMessage(const char * message);
    void setProgress(unsigned char progress);
    void show(const char * message, unsigned char progress) {
        this->message = message;
        DialogWidget::show();
    }
};


extern ProgressDialogUI progress_dialog_ui;

#endif //WORK_PROGRESSDIALOGUI_H
