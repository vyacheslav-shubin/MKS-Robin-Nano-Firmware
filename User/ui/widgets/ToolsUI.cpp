/*
 * ToolsUI.cpp
 *
 *  Created on: May 9, 2020
 *      Author: shubin
 */

#include "ToolsUI.h"
#include "MotorMoveUI.h"
#include "PreheatUI.h"
#include "HomeUI.h"
#include "FilamentUI.h"
#include "integration.h"
#include "ui_tools.h"

ToolsUI tools_ui;

void ToolsUI::createControls() {
	memset(&this->ui, 0, sizeof(this->ui));
	this->ui.preheat = this->createButtonAt(0, 0, img_preheat, lang_str.preheat);
	this->ui.filament = this->createButtonAt(1, 0, img_filament, lang_str.filament);
	this->ui.move = this->createButtonAt(2, 0, img_move, lang_str.move);
	this->ui.home = this->createButtonAt(3, 0, img_home, lang_str.home);
	switch (gCfgItems.leveling_mode) {
		case 0:
			this->ui.leveling = this->createButtonAt(0, 1, img_leveling_manual, lang_str.leveling);
			break;
		case 1:
			this->ui.leveling = this->createButtonAt(0, 1, img_leveling_auto, lang_str.leveling);
			break;
		default:
			break;
	}
	this->ui.more = this->createButtonAt(2, 1, img_more, lang_str.more);
	this->ui.ret = this->createButtonRet();
}

#include "draw_more.h"

void ToolsUI::on_button(UI_BUTTON hBtn) {
	if (hBtn==this->ui.preheat) {
		this->hide();
		preheat_ui.show(this);
	} else if (hBtn==this->ui.move) {
		this->hide();
		motor_move_ui.show(this);
	} else if (hBtn==this->ui.home) {
		this->hide();
		home_ui.show();
	} else if (hBtn==this->ui.leveling) {
		this->action_leveling();
	} else  if (hBtn==this->ui.filament) {
		this->hide();
		filament_ui.show(this);
	} else  if (hBtn==this->ui.more) {
		this->hide();
		draw_More();
	} else  if (hBtn==this->ui.ret) {
		this->action_back();
	}
}
