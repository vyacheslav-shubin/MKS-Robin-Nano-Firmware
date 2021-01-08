/*
 * PrintingToolsUI.cpp
 *
 *  Created on: May 13, 2020
 *      Author: shubin
 */

#include "PrintingToolsUI.h"
#include "integration.h"
#include "SpeedUI2.h"
#include "UI.h"

PrintingToolsUI printing_tools_ui;

void PrintingToolsUI::createControls() {
	memset(&this->ui, 0, sizeof(this->ui));
	this->ui.ret = this->createButtonRet();
	this->ui.preheat = this->createButtonAt(0, 0, img_preheat, lang_str.preheat);
	this->ui.fan = this->createButtonAt(1, 0, img_fan, lang_str.fan);
    this->ui.speed = this->createButtonAt(2, 0, img_speed, lang_str.speed);
    this->ui.speed1 = this->createButtonAt(2, 1, img_speed, "NEW!!!");
	if (shUI::isPaused()) {
		this->ui.move = this->createButtonAt(3, 0, img_move, lang_str.move);
		this->ui.filament = this->createButtonAt(0, 1, img_filament, lang_str.filament);
	} else {
		this->ui.babystep = this->createButtonAt(3, 0, img_babystep, lang_str.babysteps);
	}

}

void PrintingToolsUI::on_button(UI_BUTTON hBtn) {
	this->hide();
	if (hBtn==this->ui.ret) {
		ui_app.back_ui();
	} else if (hBtn==this->ui.preheat) {
		preheat_ui.show(this);
	} else if (hBtn==this->ui.fan) {
		fan_ui.show(this);
    } else if (hBtn==this->ui.speed) {
        speed_ui.show();
    } else if (hBtn==this->ui.speed1) {
        speed_ui_2.show();
	} else if (hBtn==this->ui.move) {
		motor_move_ui.show(this);
	} else if (hBtn==this->ui.babystep) {
		babystep_ui.show(this);
	} else if (hBtn==this->ui.filament) {
		filament_ui.show();
	}
}



