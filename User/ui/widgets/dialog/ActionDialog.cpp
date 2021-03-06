//
// Created by shubin on 23.05.2020.
//

#include "ActionDialog.h"
#include "Application.h"

void ActionDialog::doAction(u8 actionId) {
    if (this->callback != 0)
        this->callback->on_action_dialog(actionId, this->id);
    else {
        this->hide();
        ui_app.showMainWidget();
    }
}
