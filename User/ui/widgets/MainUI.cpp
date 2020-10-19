/*
 * MainUI.cpp
 *
 *  Created on: May 8, 2020
 *      Author: shubin
 */

#include "sh_tools.h"
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
#include "Application.h"

MainUI main_ui;

void MainUI::on_action_dialog(u8 action, u8 dialog_id) {
    confirm_dialog_ui.hide();
    if ((action==UI_BUTTON_OK) || (action==UI_ACTION_TIMEOUT)) {
        shUI::powerOff();
    } else {
        this->show();
    }
}

const char * MainUI::getTitle() {
    if (is_ntp_support() && (shUI::hasTime())) {
        shUI::getTimeStr(ui_buf1_100);
        return ui_buf1_100;
    } else {
        return lang_str.ui_title_main;
    }
};


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
        if (++this->current_preheat_preset >= PREHEAT_PRESET_COUNT)
            this->current_preheat_preset = 0;
        const PREHEAT_PRESET * cp = &preset_preset[this->current_preheat_preset];
        shUI::setBedTemperature(cp->tbed);
        shUI::setSprayerTemperature(0, cp->tsprayer);
        ui_update_heatpreset_button(this->ui.heat_preset, this->current_preheat_preset);
	} else if (hBtn == this->ui.power) {
        this->hide();
        ui_app.power_off_dialog(SUICIDE_WAIT);
    } else if(hBtn == this->ui.bed.button) {
        shUI::BED_TEMP bt;
        shUI::getBedTemperature(&bt);
        this->calculator(lang_str.bed, bt.target, PREHEAT_CALC_ID_BED);
    } else if(hBtn == this->ui.ext1.button) {
        shUI::SPRAYER_TEMP st;
        shUI::getSprayerTemperature(0, &st);
        this->calculator(lang_str.extruder1, st.target, PREHEAT_CALC_ID_SPR1);
    } else if(hBtn == this->ui.ext2.button) {
        shUI::SPRAYER_TEMP st;
        shUI::getSprayerTemperature(1, &st);
        this->calculator(lang_str.extruder2, st.target, PREHEAT_CALC_ID_SPR2);
    }
}

void MainUI::updateStateButtons() {
    ui_update_bed_state_button(&this->ui.bed);
    ui_update_ext_state_button(&this->ui.ext1, 0);
    if (this->ui.ext2.button)
        ui_update_ext_state_button(&this->ui.ext2, 1);
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
        #define state_row(y) (ui_std_row(1) + row_offset + (row_size*y))
        this->ui.print = this->createButton(col(0), ui_std_row(0), img_print, lang_str.print);
		this->ui.tools = this->createButton(col(1), ui_std_row(0), img_tools, lang_str.tools);
        this->ui.settings = this->createButton(col(2), ui_std_row(0), img_settings, lang_str.settings);
        this->ui.heat_preset = this->createButton(col(1), ui_std_row(1), 0, 0);
        int row_offset;
        int row_size;
        if (is_dual_extruders()) {
            row_offset = 0;
            row_size = 40;
            ui_std_ext1_state_button(col(0), state_row(0), &this->ui.ext1);
            ui_std_ext2_state_button(col(0), state_row(1), &this->ui.ext2);
            ui_std_bed_state_button(col(0), state_row(2), &this->ui.bed);
        } else {
            row_offset = 20;
            row_size = 45;
            ui_std_ext1_state_button(col(0), state_row(0), &this->ui.ext1);
            ui_std_bed_state_button(col(0), state_row(1), &this->ui.bed);
        }
        if (is_power_control_presents())
            this->ui.power = this->createButton(col(2), ui_std_row(1), img_power, lang_str.power_off);

        ui_update_heatpreset_button(this->ui.heat_preset, this->current_preheat_preset);
        this->updateStateButtons();
	}
}

void MainUI::refresh_1s() {
    this->drawXYZ();
    if (shUI::hasTime())
        ui_app.drawTitle();
    if(gCfgItems.display_style == 0) {

    } else
        this->updateStateButtons();
}

void MainUI::setValue(unsigned char id, double value) {
    preheat_set_calc_value((PREHEAT_CALC_ID)id, value);
}
