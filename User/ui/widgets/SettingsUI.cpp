/*
 * SettingsUI.cpp
 *
 *  Created on: May 22, 2020
 *      Author: shubin
 */

#include "SettingsUI.h"
#include "dialog/AboutDialogUI.h"
#include "MachineConfigurationUI.h"

SettingsUI settings_ui;

void SettingsUI::createControls() {
	memset(&this->ui, 0, sizeof(this->ui));
	this->ui.configuration = this->createButtonAt(0, 0, img_machine_settings_root, lang_str.machine_settings);
	this->ui.wifi = this->createButtonAt(1, 0, img_wifi, lang_str.wifi);
	this->ui.about = this->createButtonAt(3, 0, img_about, lang_str.about);
	this->ui.ret = this->createButtonRet();
}

#include "draw_wifi_list.h"
#include "draw_wifi.h"

void SettingsUI::action_wifi() {
	if(gCfgItems.wifi_scan == 1) {
		if(wifi_link_state == WIFI_CONNECTED && wifiPara.mode != 0x01) {
			this->hide();
			draw_Wifi();
		} else {
			if(command_send_flag == 1) {
				ui_buf1_100[0] = 0xA5;
				ui_buf1_100[1] = 0x07;
				ui_buf1_100[2] = 0x00;
				ui_buf1_100[3] = 0x00;
				ui_buf1_100[4] = 0xFC;
				raw_send_to_wifi(ui_buf1_100, 5);
				last_disp_state = SET_UI;
				this->hide();
				draw_Wifi_list();
			} else {
				this->hide();
				//Диалог ожидания. Так же можно реализовать на обратном вызове
				draw_dialog(WIFI_ENABLE_TIPS);
			}
		}
	} else {
		this->hide();
		draw_Wifi();
	}
}

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
		about_dialog_ui.show(this, 0, this);
	}
}

void SettingsUI::on_action_dialog(u8 action, u8 dialog_id) {
	about_dialog_ui.hide();
	this->show();
}
