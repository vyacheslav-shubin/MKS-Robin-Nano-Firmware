/*
 * MainUI.cpp
 *
 *  Created on: May 8, 2020
 *      Author: shubin
 */

#include "MainUI.h"
#include "ToolsUI.h"
#include "SettingsUI.h"
#include "MotorMoveUI.h"
#include "HomeUI.h"
#include "MoreUI.h"
#include "FilamentUI.h"
#include "PreheatUI.h"
#include "FileBrowserUI.h"
#include "ui_tools.h"
#include "integration.h"
#include "dialog/ConfirmDialogUI.h"

MainUI main_ui;

void MainUI::on_action_dialog(u8 action, u8 dialog_id) {
    confirm_dialog_ui.hide();
    if ((action==UI_BUTTON_OK) || (action==UI_ACTION_TIMEOUT)) {
        shUI::powerOff();
    } else {
        this->show();
    }
}


void MainUI::on_button(UI_BUTTON hBtn) {
	if (hBtn==this->ui.tools) {
		this->hide();
		tools_ui.show(this);
	} else if (hBtn==this->ui.settings) {
		this->hide();
		settings_ui.show(this);
	} else if (hBtn==this->ui.print) {
		this->hide();
		file_browser_ui.show(this);
	} else  if (hBtn==this->ui.preheat) {
		this->hide();
		preheat_ui.show(this);
	} else  if (hBtn==this->ui.more) {
		this->hide();
		more_ui.show(this);
	} else  if (hBtn==this->ui.move) {
		this->hide();
		motor_move_ui.show(this);
	} else  if (hBtn==this->ui.home) {
		this->hide();
		home_ui.show();
	} else  if (hBtn==this->ui.leveling) {
		this->action_leveling();
	} else  if (hBtn==this->ui.filament) {
		this->hide();
		filament_ui.show(this);
	} else  if (hBtn==this->ui.heat_preset) {
        const PREHEAT_PRESET * cp = &preset_preset[this->current_preheat_preset];
        shUI::setBedTemperature(cp->tbed);
        shUI::setSprayerTemperature(0, cp->tsprayer);
        if (++this->current_preheat_preset >= PREHEAT_PRESET_COUNT)
            this->current_preheat_preset = 0;
        ui_update_heatpreset_button(this->ui.heat_preset, this->current_preheat_preset);
	} else if (hBtn == this->ui.power) {
        this->hide();
        strcpy(ui_buf1_100, lang_str.power_off);
        strcat(ui_buf1_100, "?");
        confirm_dialog_ui.show(ui_buf1_100, this, 30, 0, this);
	}
}

void MainUI::createControls() {
	memset(&this->ui, 0, sizeof(this->ui));
	if(gCfgItems.display_style == 0) {
		this->ui.preheat = this->createButtonAt(0, 0, img_preheat, lang_str.preheat);
		this->ui.move = this->createButtonAt(1, 0, img_move, lang_str.move);
		this->ui.home = this->createButtonAt(2, 0, img_home_all, lang_str.home);
		this->ui.print = this->createButtonAt(3, 0, img_print, lang_str.print);
		this->ui.filament = this->createButtonAt(0, 1, img_filament, lang_str.filament);

        this->ui.leveling = this->createButtonAt(1, 1, img_leveling,  lang_str.leveling);
		this->ui.settings  = this->createButtonAt(2, 1, img_settings,  lang_str.settings);
		if (gCfgItems.MoreItem_pic_cnt)
			this->ui.more = this->createButtonAt(3, 1, img_more,  lang_str.more);
	} else {
		#define middle  ((LCD_HEIGHT-BTN_Y_PIXEL)/2-titleHeight)
		#define col(idx) SIMPLE_FIRST_PAGE_GRAP + 1 + (BTN_X_PIXEL+SIMPLE_FIRST_PAGE_GRAP) * idx
        this->ui.print = this->createButton(col(0), ui_std_row(0), img_print, lang_str.print);
		this->ui.tools = this->createButton(col(1), ui_std_row(0), img_tools, lang_str.tools);
        this->ui.settings = this->createButton(col(2), ui_std_row(0), img_settings, lang_str.settings);
        this->ui.heat_preset = this->createButton(col(1), ui_std_row(1), 0, 0);

        ui_std_ext1_state_button(col(0), ui_std_row(1) + 20, &this->ui.ext1);
        ui_std_bed_state_button(col(0), ui_std_row(1) + 65, &this->ui.bed);
        this->ui.power = this->createButton(col(2), ui_std_row(1), img_power, lang_str.power_off);
        ui_update_heatpreset_button(this->ui.heat_preset, this->current_preheat_preset);
        ui_update_bed_state_button(&this->ui.bed);
        ui_update_ext_state_button(&this->ui.ext1, 0);
	}
}

void MainUI::refresh_1s() {
    this->drawXYZ();
    if(gCfgItems.display_style == 0) {

    } else {
        ui_update_bed_state_button(&this->ui.bed);
        ui_update_ext_state_button(&this->ui.ext1, 0);
    }
}
