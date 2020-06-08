//
// Created by shubin on 03.06.2020.
//

#include "WifiWaitInitDialogUI.h"
#include "ui_tools.h"

WifiWaitInitDialogUI wifi_wait_init_dialog_ui;

typedef enum {
    WIFI_SM_START = 0,
    WIFI_SM_LIST_READY,
    WIFI_SM_CONNECTED,
    WIFI_SM_FAIL
} WIFI_SM_JOIN;

void WifiWaitInitDialogUI::createControls() {
    memset(&this->ui, 0, sizeof(this->ui));
    this->createDialogDecoration(img_dialog_info, 0);
    this->ui.cancel = this->create96x80Button(DIALOG_WIDTH - (96 + 4) , DIALOG_HEIGHT - 84, img_cancel);
    this->ui.text = this->createTextF(10, 60, DIALOG_WIDTH - 20, DIALOG_HEIGHT - 60 - 80 - 10, TEXT_CF_HCENTER | TEXT_CF_VCENTER, 0);
    this->updateControls();
}

void WifiWaitInitDialogUI::updateControls() {
    switch (type) {
        case WIFI_DIALOG_LOOKUP: {
            switch (this->state_machine) {
                case WIFI_SM_START:
                    this->setText(this->ui.text, lang_str.wf.lookup);
                    break;
                case WIFI_SM_CONNECTED:
                    this->setText(this->ui.text, lang_str.wf.connected);
                    break;
                case WIFI_SM_LIST_READY:
                    this->setText(this->ui.text, lang_str.wf.lookup);
                    break;
            }
            break;
        }
        case WIFI_DIALOG_JOIN: {
            switch (this->state_machine) {
                case WIFI_SM_START: {
                    sprintf(ui_buf1_100, "%s [%s]", lang_str.wf.joining, wifi_list.wifi[wifi_list.selected].name);
                    this->setText(this->ui.text, ui_buf1_100);
                    break;
                }
                case WIFI_SM_CONNECTED: {
                    sprintf(ui_buf1_100, "%s [%s]", lang_str.wf.joined, wifi_list.wifi[wifi_list.selected].name);
                    this->setText(this->ui.text, ui_buf1_100);
                    break;
                }
                case WIFI_SM_FAIL: {
                    sprintf(ui_buf1_100, "%s [%s]", lang_str.wf.fail_join, wifi_list.wifi[wifi_list.selected].name);
                    this->setText(this->ui.text, ui_buf1_100);
                    break;
                }
            }
        }
    }
}


void WifiWaitInitDialogUI::on_button(UI_BUTTON hBtn) {
    if (hBtn==this->ui.cancel)
        this->doAction(UI_BUTTON_CANCEL);
}

void WifiWaitInitDialogUI::refresh_1s() {
    if (this->count_down!=0) {
        this->count_down--;
        return;
    }
    switch (this->state_machine) {
        case WIFI_SM_CONNECTED: {
            this->doAction(UI_ACTION_WIFI_CONNECTED);
            break;
        }
        case WIFI_SM_LIST_READY: {
            this->doAction(UI_ACTION_WIFI_LIST_READY);
            break;
        }
        case WIFI_SM_FAIL: {
            this->doAction(UI_ACTION_WIFI_FAIL);
            break;
        }
        default: {
            switch ((unsigned char)type) {
                case WIFI_DIALOG_LOOKUP: {
                    if (wifi_list_received_flag == 1) {
                        this->state_machine = (wifi_link_state == WIFI_CONNECTED && wifiPara.mode != 0x01)
                                              ? WIFI_SM_CONNECTED : WIFI_SM_LIST_READY;
                        this->count_down = 2;
                        this->updateControls();
                    }
                    break;
                }
                case  WIFI_DIALOG_JOIN: {
                    if(wifi_link_state == WIFI_CONNECTED && strcmp((const char *)wifi_list.wifiConnectedName,(const char *)wifi_list.wifi[wifi_list.selected].name) == 0) {
                        this->state_machine = WIFI_SM_CONNECTED;
                    } else {
                        if (this->join_count_down!=0) {
                            this->join_count_down--;
                            break;
                        } else {
                            this->state_machine = WIFI_SM_FAIL;
                        }
                    }
                    this->count_down = 2;
                    this->updateControls();
                    break;
                }
            }
        }
    }

}
