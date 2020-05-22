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

MainUI main_ui;

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
	}
}

void MainUI::createControls() {
	memset(&this->ui, 0, sizeof(this->ui));
	if(gCfgItems.display_style == 0) {
		this->ui.preheat = this->createButtonAt(0, 0, img_preheat, lang_str.preheat);
		this->ui.move = this->createButtonAt(1, 0, img_move, lang_str.move);
		this->ui.home = this->createButtonAt(2, 0, img_home, lang_str.home);
		this->ui.print = this->createButtonAt(3, 0, img_print, lang_str.print);
		this->ui.filament = this->createButtonAt(0, 1, img_filament, lang_str.filament);

		char next_button_offset = 1;
		switch(gCfgItems.leveling_mode) {
		case 0:
			this->ui.leveling = this->createButtonAt(1, 1, img_leveling_manual,  lang_str.leveling);
			break;
		case 1:
			this->ui.leveling = this->createButtonAt(1, 1, img_leveling_auto,  lang_str.leveling);
			break;
		default:
			next_button_offset = 0;
			break;
		}
		this->ui.settings  = this->createButtonAt(1+next_button_offset, 1, img_settings,  lang_str.settings);
		if (gCfgItems.MoreItem_pic_cnt)
			this->ui.more = this->createButtonAt(2+next_button_offset, 1, img_more,  lang_str.more);
	} else {
		#define middle  ((LCD_HEIGHT-BTN_Y_PIXEL)/2-titleHeight)
		#define col(idx) SIMPLE_FIRST_PAGE_GRAP + 1 + (BTN_X_PIXEL+SIMPLE_FIRST_PAGE_GRAP) * idx
        this->ui.print = this->createButton(col(0), middle, img_print, lang_str.print);
		this->ui.tools = this->createButton(col(1), middle, img_tools, lang_str.tools);
        this->ui.settings = this->createButton(col(2), middle, img_settings, lang_str.settings);
	}
}

