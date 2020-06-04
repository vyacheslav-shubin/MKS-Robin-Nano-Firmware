//
// Created by shubin on 03.06.2020.
//

#include "WifiWaitInitDialogUI.h"
#include "ui_tools.h"

WifiWaitInitDialogUI wifi_wait_init_dialog_ui;

void WifiWaitInitDialogUI::createControls() {
    memset(&this->ui, 0, sizeof(this->ui));
    this->createDialogDecoration(img_dialog_info, 0);
    this->ui.cancel = this->create96x80Button(DIALOG_WIDTH - (96 + 4) , DIALOG_HEIGHT - 84, img_cancel);
    this->ui.text = this->createTextF(10, 60, DIALOG_WIDTH - 20, DIALOG_HEIGHT - 60 - 80 - 10, TEXT_CF_HCENTER | TEXT_CF_VCENTER, "wifi initialization");
}

void WifiWaitInitDialogUI::on_button(UI_BUTTON hBtn) {
    if (hBtn==this->ui.cancel)
        this->doAction(UI_BUTTON_CANCEL);
}

void WifiWaitInitDialogUI::refresh_05() {
    if(wifi_list_received_flag == 1) {
        if(wifi_link_state == WIFI_CONNECTED && wifiPara.mode != 0x01) {
            this->doAction(UI_ACTION_WIFI_CONNECTED);
        } else {
            this->doAction(UI_ACTION_WIFI_LIST_READY);
        }
    }
}
