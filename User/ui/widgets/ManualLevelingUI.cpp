/*
 * ManualLevelingUI.cpp
 *
 *  Created on: May 9, 2020
 *      Author: shubin
 */

#include "ManualLevelingUI.h"
#include "ui_tools.h"
#include "Marlin.h"

ManualLevelingUI manual_leveling_ui;

char * _button_title(char point) {
	sprintf(ui_buf1_20,"X:%d Y:%d",gCfgItems.leveling_points[point].x, gCfgItems.leveling_points[point].y);
	return ui_buf1_20;
}

extern float current_position[XYZE];

char * _home_z_button_title() {
	sprintf(ui_buf1_20,"Z:%.3f",current_position[Z_AXIS]);
	return ui_buf1_20;
}

void manual_leveling(char point) {
	sprintf(ui_buf1_80,"G91 G1 Z10 F%d\n",gCfgItems.leveling_z_speed);
	enqueue_and_echo_commands_P(PSTR(ui_buf1_80));
	sprintf(ui_buf1_80,"G90 G1 X%d Y%d F%d\n", gCfgItems.leveling_points[point].x, gCfgItems.leveling_points[point].y, gCfgItems.leveling_xy_speed);
	enqueue_and_echo_commands_P(PSTR(ui_buf1_80));
	enqueue_and_echo_commands_P(PSTR("G1 Z0"));
}

void ManualLevelingUI::createControls() {
	memset(&this->buttons, 0, sizeof(this->buttons));
	buttons.leveling1 = this->createButtonAt(0, 1, img_leveling_point1, _button_title(0));
	buttons.leveling2 = this->createButtonAt(1, 1, img_leveling_point2, _button_title(1));
	buttons.leveling3 = this->createButtonAt(1, 0, img_leveling_point3, _button_title(3));
	buttons.leveling4 = this->createButtonAt(0, 0, img_leveling_point4, _button_title(2));
	buttons.leveling5 = this->createButtonAt(2, 1, img_leveling_point5, _button_title(4));
	buttons.home = this->createButtonAt(3, 0, img_home, lang_str.home);
	buttons.homeZ = this->createButtonAt(2, 0, img_home_z, _home_z_button_title());
	buttons.back = this->createButtonRet();
}

void ManualLevelingUI::on_button(UI_BUTTON hBtn) {
	if (hBtn==buttons.back) {
		this->action_back();
	} else if (hBtn==buttons.leveling1) {
		manual_leveling(0);
	} else if (hBtn==buttons.leveling2) {
		manual_leveling(1);
	} else if (hBtn==buttons.leveling3) {
		manual_leveling(2);
	} else if (hBtn==buttons.leveling4) {
		manual_leveling(3);
	} else if (hBtn==buttons.leveling5) {
		manual_leveling(4);
	} else if (hBtn==buttons.home) {
		enqueue_and_echo_commands_P(PSTR("G28"));
	} else if (hBtn==buttons.homeZ) {
		enqueue_and_echo_commands_P(PSTR("G28 Z0"));
	}
}

void ManualLevelingUI::refresh_05() {
	BUTTON_SetText(buttons.homeZ, _home_z_button_title());
	this->drawXYZ();
}


