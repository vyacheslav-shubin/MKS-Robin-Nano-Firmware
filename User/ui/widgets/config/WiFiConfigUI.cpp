//
// Created by shubin on 19.10.2020.
//

#include "WiFiConfigUI.h"
#include "sh_tools.h"

WiFiConfigUI wifi_config_ui;

void WiFiConfigUI::updateControls() {
    sprintf(ui_buf1_100, "%d", gCfgItems.time_offset);
    this->setButtonText(this->ui.timeShift.button, ui_buf1_100);
}

void WiFiConfigUI::on_button(UI_BUTTON hBtn) {
    if (hBtn == this->ui.exists.button) {
        gCfgItems.wifi_flags ^= WIFI_MODULE_PRESENTS;
        this->updateCheckButton(this->ui.exists.button, gCfgItems.wifi_flags & WIFI_MODULE_PRESENTS);
        epr_write_data(EPR_WIFI_FLAGS, (const unsigned char*)&gCfgItems.wifi_flags, sizeof(gCfgItems.wifi_flags));
    } else if (hBtn == this->ui.ntp_support.button) {
        gCfgItems.wifi_flags ^= WIFI_MODULE_NTP_SUPPORT;
        this->updateCheckButton(this->ui.ntp_support.button, gCfgItems.wifi_flags & WIFI_MODULE_NTP_SUPPORT);
        epr_write_data(EPR_WIFI_FLAGS, (const unsigned char*)&gCfgItems.wifi_flags, sizeof(gCfgItems.wifi_flags));
    } else if (hBtn == this->ui.timeShift.button) {
        this->calculator(lang_str.config_ui.time_shift, gCfgItems.time_offset, 0);
    } else if (hBtn == this->ui.timeShift.dflt) {
        gCfgItems.time_offset = 180;
        this->updateControls();
        epr_write_data(EPR_TIME_SHIFT, (const unsigned char*)&gCfgItems.time_offset, sizeof(gCfgItems.time_offset));
    } else
        ConfigurationWidgetWithCalc::on_button(hBtn);
}

void WiFiConfigUI::createControls() {
    ConfigurationWidget::createControls();
    memset(&this->ui, 0, sizeof(this->ui));
    this->dual_columns = 0;
    switch (this->page) {
        case 0: {
            this->createCheckPair(0, 0, &this->ui.exists, lang_str.config_ui.module_exists, is_wifi_modue_presents());
            this->createCheckPair(0, 1, &this->ui.ntp_support, lang_str.config_ui.ntp, gCfgItems.wifi_flags & WIFI_MODULE_NTP_SUPPORT);
            this->createInputWithDefault(0, 2, &this->ui.timeShift, lang_str.config_ui.time_shift, 0);
            break;
        }
    }
    this->updateControls();

}

void WiFiConfigUI::on_action_dialog(u8 action, u8 dialog_id) {

}

void WiFiConfigUI::_setValue(unsigned char id, short value) {
    gCfgItems.time_offset = value;
    epr_write_data(EPR_TIME_SHIFT, (const unsigned char*)&gCfgItems.time_offset, sizeof(gCfgItems.time_offset));
}
