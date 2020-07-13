//
// Created by shubin on 03.06.2020.
//

#include "WifiListUI.h"
#include "KeyboardUI.h"
#include "wifi_module.h"
#include "WifiUI.h"
#include "WifiWaitInitDialogUI.h"
#include "Application.h"



WifiListUI wifi_list_ui;

static char password_buffer[17];

typedef enum {
    DIALOG_ID_PASSWORD,
    DIALOG_ID_WIFI_WAIT
};

void WifiListUI::on_action_dialog(u8 action, u8 dialog_id) {
    switch (dialog_id) {
        case DIALOG_ID_PASSWORD: {
            keyboard_ui.hide();
            if (action==UI_BUTTON_OK) {
                switch (dialog_id) {
                    case DIALOG_ID_PASSWORD: {
                        memset((void *)gCfgItems.wifi_ap, 0, sizeof(gCfgItems.wifi_ap));
                        memcpy((void *)gCfgItems.wifi_ap, wifi_list.wifi[wifi_list.selected].name, 32);
                        memset((void *)gCfgItems.wifi_key, 0, sizeof(gCfgItems.wifi_key));
                        memcpy((void *)gCfgItems.wifi_key, password_buffer, sizeof(password_buffer));
                        gCfgItems.wifi_mode_sel = 2;

                        package_to_wifi(WIFI_PARA_SET, (char *)0, 0);

                        ui_buf1_100[0] = 0xA5;
                        ui_buf1_100[1] = 0x09;
                        ui_buf1_100[2] = 0x01;
                        ui_buf1_100[3] = 0x00;
                        ui_buf1_100[4] = 0x01;
                        ui_buf1_100[5] = 0xFC;
                        ui_buf1_100[6] = 0x00;
                        raw_send_to_wifi(ui_buf1_100, 6);
                        this->show();
                        GUI_Exec();
                        this->hide();
                        wifi_wait_init_dialog_ui.show(WIFI_DIALOG_JOIN, this, DIALOG_ID_WIFI_WAIT);
                        break;
                    }
                }
            } else {
                this->show();
            }
            break;
        }
        case DIALOG_ID_WIFI_WAIT: {
            switch (action) {
                case UI_ACTION_WIFI_FAIL:
                    break;
                case UI_BUTTON_CANCEL: {
                    wifi_wait_init_dialog_ui.hide();
                    this->show();
                    break;
                }
                case UI_ACTION_WIFI_CONNECTED: {
                    wifi_wait_init_dialog_ui.hide();
                    ui_app.pop();
                    wifi_ui.show();
                    break;
                }
            }
        }
    }
}


void WifiListUI::on_button(UI_BUTTON hBtn) {
    for (unsigned char i = 0; i < 8;i++) {
        if (this->ui.wifi[i].button) {
            if (hBtn==this->ui.wifi[i].button) {
                wifi_list.selected = this->page * 8 + i;
                this->hide();
                if (wifi_link_state == WIFI_CONNECTED && strcmp(wifi_list.wifiConnectedName, wifi_list.wifi[wifi_list.selected].name) == 0) {
                    ui_app.pop();
                    wifi_ui.show(this);
                } else {
                    memset(password_buffer, 0, sizeof(password_buffer));
                    keyboard_ui.show("Password:", password_buffer, sizeof(password_buffer) - 1, this, DIALOG_ID_PASSWORD, this);
                }
                return;
            }
        } else
            break;
    }
    ConfigurationWidget::on_button(hBtn);
}

void WifiListUI::createControls() {
    this->page_count = (wifi_list.count / 8) + 1;
    ConfigurationWidget::createControls();
    memset(&this->ui, 0, sizeof(this->ui));
    this->dual_columns = 1;
    for (unsigned char i = 0; i < 8;i++) {
        unsigned char wi = this->page * 8 + i;
        if (i < wifi_list.count) {
            char eq = strncmp((const char *)gCfgItems.wifi_ap,(const char *)wifi_list.wifi[wi].name, sizeof(wifi_list.wifi[wi].name));
            this->createRadio(i / 4, i % 4, &this->ui.wifi[i], wifi_list.wifi[wi].name, !eq);
        }
    }
}
