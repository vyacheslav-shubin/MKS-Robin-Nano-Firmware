/*
 * SettingsUI.cpp
 *
 *  Created on: May 22, 2020
 *      Author: shubin
 */

#include "SettingsUI.h"
#include "dialog/AboutDialogUI.h"
#include "MachineConfigurationUI.h"
#include "dialog/WifiWaitInitDialogUI.h"
#include "WifiListUI.h"
#include "WifiUI.h"

SettingsUI settings_ui;

typedef enum{
    DIALOG_ID_ABOUT,
    DIALOG_ID_INIT_WIFI
} ;

void SettingsUI::createControls() {
	memset(&this->ui, 0, sizeof(this->ui));
	this->ui.configuration = this->createButtonAt(0, 0, img_machine_settings_root, lang_str.machine_settings);
	this->ui.wifi = this->createButtonAt(1, 0, img_wifi, lang_str.wifi);
    this->ui.about = this->createButtonAt(3, 0, img_about, lang_str.about);
    //this->ui.reset = this->createButtonAt(0, 1, img_about, lang_str.about);
	this->ui.ret = this->createButtonRet();
}

void SettingsUI::action_wifi() {
	if(gCfgItems.wifi_scan == 1) {
		if(wifi_link_state == WIFI_CONNECTED && wifiPara.mode != 0x01) {
			this->hide();
            wifi_ui.show(this);
		} else {
            wifi_list_received_flag = 0;
            get_wifi_list_command_send();
            this->hide();
            wifi_wait_init_dialog_ui.show(WIFI_DIALOG_LOOKUP, this, DIALOG_ID_INIT_WIFI, this);
		}
	} else {
		this->hide();
        wifi_ui.show(this);
	}
}

extern "C" void Reset_Handler(void);

void SettingsUI::on_button(UI_BUTTON hBtn) {
	if (hBtn==this->ui.ret) {
		this->action_back();
	}  else if (hBtn==this->ui.configuration) {
		this->hide();
		machine_configuration_ui.show(this);
	}  else if (hBtn==this->ui.wifi) {
		this->action_wifi();
	}  else if (hBtn==this->ui.about) {
		this->hide();
		about_dialog_ui.show(this, DIALOG_ID_ABOUT, this);
	}  else if (hBtn==this->ui.reset) {
        Reset_Handler();
	}
}

void SettingsUI::on_action_dialog(u8 action, u8 dialog_id) {
    if (dialog_id==DIALOG_ID_ABOUT) {
        about_dialog_ui.hide();
        this->show();
    } else if (dialog_id==DIALOG_ID_INIT_WIFI) {
        wifi_wait_init_dialog_ui.hide();
        switch(action) {
            case UI_BUTTON_CANCEL:
                this->show();
                break;
            case UI_ACTION_WIFI_LIST_READY:
                wifi_list_ui.show();
                //draw_Wifi_list();
                break;
            case UI_ACTION_WIFI_CONNECTED:
                wifi_ui.show(this);
                break;
        };
    }
}
