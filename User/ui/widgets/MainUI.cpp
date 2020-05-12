/*
 * MainUI.cpp
 *
 *  Created on: May 8, 2020
 *      Author: shubin
 */

#include "MainUI.h"
#include "ToolsUI.h"
#include "MotorMoveUI.h"
#include "ui_tools.h"

#include "draw_print_file.h"
#include "draw_set.h"

MainUI main_ui;


#include "draw_more.h"
#include "draw_filamentchange.h"
#include "draw_pre_heat.h"
#include "draw_extrusion.h"
#include "draw_zero.h"

void MainUI::on_button(WM_HWIN hBtn) {
	if (hBtn==this->buttons.tools) {
		this->hide();
		tools_ui.show(this);
	} else if (hBtn==this->buttons.settings) {
		this->hide();
		draw_Set();
	} else if (hBtn==this->buttons.print) {
		this->hide();
		draw_print_file();
	} else  if (hBtn==this->buttons.preheat) {
		this->hide();
		draw_preHeat();
	} else  if (hBtn==this->buttons.more) {
		this->hide();
		draw_More();
	} else  if (hBtn==this->buttons.move) {
		this->hide();
		motor_move_ui.show(this);
	} else  if (hBtn==this->buttons.home) {
		this->hide();
		draw_zero();
	} else  if (hBtn==this->buttons.leveling) {
		this->action_leveling();
	} else  if (hBtn==this->buttons.extrude) {
		this->hide();
		draw_extrusion();
	}
}

void MainUI::createControls() {
	memset(&this->buttons, 0, sizeof(this->buttons));
	if(gCfgItems.display_style == 0) {
		this->buttons.preheat = this->createButtonAt(0, 0, img_preheat, lang_str.preheat);
		this->buttons.move = this->createButtonAt(1, 0, img_move, lang_str.move);
		this->buttons.home = this->createButtonAt(2, 0, img_home, lang_str.home);
		this->buttons.print = this->createButtonAt(3, 0, img_print, lang_str.print);
		this->buttons.extrude = this->createButtonAt(0, 1, img_extrusion, lang_str.extrusion);

		char next_button_offset = 1;
		switch(gCfgItems.leveling_mode) {
		case 0:
			this->buttons.leveling = this->createButtonAt(1, 1, img_leveling_manual,  lang_str.leveling);
			break;
		case 1:
			this->buttons.leveling = this->createButtonAt(1, 1, img_leveling_auto,  lang_str.leveling);
			break;
		default:
			next_button_offset = 0;
			break;
		}
		this->buttons.more = this->createButtonAt(2+next_button_offset, 1, img_more,  lang_str.more);
		this->buttons.settings  = this->createButtonAt(1+next_button_offset, 1, img_settings,  lang_str.settings);
	} else {
		#define middle  ((LCD_HEIGHT-BTN_Y_PIXEL)/2-titleHeight)
		#define col(idx) SIMPLE_FIRST_PAGE_GRAP + 1 + (BTN_X_PIXEL+SIMPLE_FIRST_PAGE_GRAP) * idx
		this->buttons.tools = this->createButton(col(0), middle, img_tools, lang_str.tools);
		this->buttons.settings = this->createButton(col(1), middle, img_settings, lang_str.settings);
		this->buttons.print = this->createButton(col(2), middle, img_print, lang_str.print);
	}
}

