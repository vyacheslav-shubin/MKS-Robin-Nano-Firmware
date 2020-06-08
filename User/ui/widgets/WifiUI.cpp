//
// Created by shubin on 05.06.2020.
//

#include "WifiUI.h"
#include "WifiListUI.h"
#include "ui_tools.h"
#include "wifi_module.h"

WifiUI wifi_ui;

void WifiUI::on_button(UI_BUTTON hBtn) {
    if (hBtn==this->ui.ret) {
        this->action_back();
    } else if (hBtn==this->ui.reconnect) {
        ui_buf1_100[0] = 0xA5;
        ui_buf1_100[1] = 0x07;
        ui_buf1_100[2] = 0x00;
        ui_buf1_100[3] = 0x00;
        ui_buf1_100[4] = 0xFC;
        raw_send_to_wifi(ui_buf1_100, 5);
        this->hide();
        ui_app.pop();
        wifi_list_ui.show();
    } else if (hBtn==this->ui.cloud) {

    }
}

void WifiUI::createControls() {
    memset(&this->ui, 0, sizeof(this->ui));
    this->ui.ret = this->createButtonRet();
    this->ui.reconnect = this->createButtonAt(2, 1, img_wifi, lang_str.wifi_reconnect);
    if((gCfgItems.wifi_type == 0x02) && (gCfgItems.cloud_enable == 1))
        this->ui.cloud = this->createButtonAt(3, 0, img_cloud, lang_str.cloud);
    this->ui.name = this->createText(10, 30, LCD_WIDTH / 2, 20, 0);
    this->ui.ip = this->createText(10, 30 + 20, LCD_WIDTH / 2, 20, 0);;
    this->ui.state = this->createText(10, 30 + 40, LCD_WIDTH / 2, 20, 0);;

}
void WifiUI::refresh_1s() {
    sprintf(ui_buf1_100, "IP: %d.%d.%d.%d / %s",
            ipPara.ip[0], ipPara.ip[1], ipPara.ip[2], ipPara.ip[3],
            wifiPara.mode == 0x01 ? "AP" : "STA"
            );
    this->setText(this->ui.ip, ui_buf1_100);
    strcpy(ui_buf1_100, "WiFi: ");
    strcat(ui_buf1_100, wifiPara.ap_name);
    this->setText(this->ui.name, ui_buf1_100);
    const char * msg;
    if(wifi_link_state == WIFI_CONNECTED)
        msg = lang_str.wf.connected;
    else if(wifi_link_state == WIFI_NOT_CONFIG)
        msg = lang_str.wf.disconnected;
    else
        msg = lang_str.wf.fail_join;
    sprintf(ui_buf1_100, "%s: %s", lang_str.wf.state, msg);
    this->setText(this->ui.state, ui_buf1_100);
}