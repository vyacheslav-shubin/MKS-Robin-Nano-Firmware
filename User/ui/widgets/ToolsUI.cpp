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
#include "MoreUI.h"
#include "FilamentUI.h"
#include "FileBrowserUI.h"
#include "ConfirmDialogUI.h"
#include "integration.h"
#include "ui_tools.h"
#include "FanUI.h"
ToolsUI tools_ui;

void ToolsUI::createControls() {
	memset(&this->ui, 0, sizeof(this->ui));
	this->ui.preheat = this->createButtonAt(0, 0, img_preheat, lang_str.preheat);
	this->ui.filament = this->createButtonAt(1, 0, img_filament, lang_str.filament);
	this->ui.move = this->createButtonAt(2, 0, img_move, lang_str.move);
	this->ui.home = this->createButtonAt(3, 0, img_home_all, lang_str.home);
	this->ui.leveling = this->createButtonAt(0, 1, img_leveling, lang_str.leveling);
	this->ui.fan = this->createButtonAt(1, 1, img_fan, lang_str.fan);

	if (gCfgItems.MoreItem_pic_cnt)
		this->ui.more = this->createButtonAt(2, 1, img_more, lang_str.more);
	else
		this->ui.files = this->createButtonAt(2, 1, img_print, lang_str.print);
	this->ui.ret = this->createButtonRet();
}

void ToolsUI::refresh_1s() {
    this->drawXYZ();
}


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
		more_ui.show(this);
	} else  if (hBtn==this->ui.ret) {
		this->action_back();
	} else if (hBtn==this->ui.fan) {
		this->hide();
		fan_ui.show();
	} else if (hBtn==this->ui.files) {
		this->hide();
		file_browser_ui.show();
	}
}
