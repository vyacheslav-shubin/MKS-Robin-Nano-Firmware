//
// Created by shubin on 23.05.2020.
//

#ifndef WORK_PRINTINGINFODIALOG_H
#define WORK_PRINTINGINFODIALOG_H

#include "DialogWidget.h"
#include "ActionDialog.h"

typedef struct {
    UI_BUTTON ok;
    UI_TEXT file_size;
    UI_TEXT layers;
    UI_TEXT time;
    UI_TEXT filament;
    UI_TEXT mmx;
    UI_TEXT mmy;
    UI_TEXT mmz;
} PRINTING_INFO_DIALOG_UI_CONTROLS;

class PrintingInfoDialogUI: public ActionDialog {
private:
    PRINTING_INFO_DIALOG_UI_CONTROLS ui;
    void updateLabels();
protected:
    virtual void createControls();
    virtual void on_button(UI_BUTTON hBtn);
    virtual void refresh_1s();
public:
    PrintingInfoDialogUI(): ActionDialog() {};
};


extern PrintingInfoDialogUI printing_info_dialog_ui;

#endif //WORK_PRINTINGINFODIALOG_H
