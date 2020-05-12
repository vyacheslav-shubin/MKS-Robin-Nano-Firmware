/*
 * ToolsUI.cpp
 *
 *  Created on: May 9, 2020
 *      Author: shubin
 */

#include "ToolsUI.h"
#include "MotorMoveUI.h"
#include "ui_tools.h"

ToolsUI tools_ui;

void ToolsUI::createControls() {
	memset(&buttons, 0, sizeof(buttons));
	this->buttons.preheat = this->createButtonAt(0, 0, img_preheat, lang_str.preheat);
	this->buttons.extrude = this->createButtonAt(1, 0, img_extrusion, lang_str.extrusion);
	this->buttons.move = this->createButtonAt(2, 0, img_move, lang_str.move);
	this->buttons.home = this->createButtonAt(3, 0, img_home, lang_str.home);
	switch (gCfgItems.leveling_mode) {
		case 0:
			this->buttons.leveling = this->createButtonAt(0, 1, img_leveling_manual, lang_str.leveling);
			break;
		case 1:
			this->buttons.leveling = this->createButtonAt(0, 1, img_leveling_auto, lang_str.leveling);
			break;
		default:
			break;
	}
	this->buttons.filament = this->createButtonAt(1, 1, img_filament, lang_str.filament);
	this->buttons.more = this->createButtonAt(2, 1, img_more, lang_str.more);
	this->buttons.ret = this->createButtonRet();
}

#include "draw_more.h"
#include "draw_filamentchange.h"
#include "draw_pre_heat.h"
#include "draw_extrusion.h"
#include "draw_zero.h"

void ToolsUI::on_button(WM_HWIN hBtn) {
	if (hBtn==this->buttons.preheat) {
		this->hide();
		draw_preHeat();
	} else if (hBtn==this->buttons.extrude) {
		this->hide();
		draw_extrusion();
	} else if (hBtn==this->buttons.move) {
		this->hide();
		motor_move_ui.show(this);
	} else if (hBtn==this->buttons.home) {
		this->hide();
		draw_zero();
	} else if (hBtn==this->buttons.leveling) {
		this->action_leveling();
	} else  if (hBtn==this->buttons.filament) {
		this->hide();
		draw_FilamentChange();
	} else  if (hBtn==this->buttons.more) {
		this->hide();
		draw_More();
	} else  if (hBtn==this->buttons.ret) {
		this->action_back();
	}
}
