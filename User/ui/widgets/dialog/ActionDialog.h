//
// Created by shubin on 23.05.2020.
//

#ifndef WORK_ACTIONDIALOG_H
#define WORK_ACTIONDIALOG_H

#include "DialogWidget.h"

class ActionDialog : public DialogWidget {
protected:
    ActionDialogCallback * callback = 0;
    u8 id = 0;
    void doAction(u8 actionId);
public:
    void show(ActionDialogCallback * callback, u8 id, Widget * caller = 0) {
        this->callback = callback;
        this->id = id;
        DialogWidget::show(caller);
    }
    ActionDialog() : DialogWidget() {};
};


#endif //WORK_ACTIONDIALOG_H
