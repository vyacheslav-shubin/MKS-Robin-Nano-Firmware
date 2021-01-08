/*
 * HomeUI.cpp
 *
 *  Created on: May 13, 2020
 *      Author: shubin
 */

#include "HomeUI.h"
#include "integration.h"
#include "Application.h"


HomeUI home_ui;


void HomeUI::createControls() {
	memset(&this->ui, 0, sizeof(this->ui));
	this->ui.back = this->createButtonRet();
	this->ui.all = this->createButtonAt(0, 0, img_home_all, "XYZ");
    sprintf(ui_buf1_80,"X=>%.3f",mksCfg.x_home_dir==1?mksCfg.x_max_pos:mksCfg.x_min_pos);
	this->ui.x= this->createButtonAt(1, 0, img_home_x, ui_buf1_80);
    sprintf(ui_buf1_80,"Y=>%.3f",mksCfg.y_home_dir==1?mksCfg.y_max_pos:mksCfg.y_min_pos);
	this->ui.y= this->createButtonAt(2, 0, img_home_y, ui_buf1_80);
    sprintf(ui_buf1_80,"Z=>%.3f",mksCfg.z_home_dir==1?mksCfg.z_max_pos:mksCfg.z_min_pos);
    this->ui.z= this->createButtonAt(3, 0, img_home_z, ui_buf1_80);
    this->ui.t= this->createButtonAt(2, 1, img_home_t, lang_str.filament);
    sprintf(ui_buf1_80,"Z=>%.3f",mksCfg.bed_homind_z);
    this->ui.b= this->createButtonAt(1, 1, img_home_b, ui_buf1_80);
	this->ui.stop = this->createButtonAt(0, 1, img_stop_motor, lang_str.stop);
}

void HomeUI::on_button(UI_BUTTON hBtn) {
	if (hBtn==this->ui.back) {
		this->hide();
		ui_app.back_ui();
	} else if (hBtn==this->ui.all) {
		shUI::pushGcode("G28");
	} else if (hBtn==this->ui.x) {
		shUI::pushGcode("G28 X0");
	} else if (hBtn==this->ui.y) {
		shUI::pushGcode("G28 Y0");
	} else if (hBtn==this->ui.z) {
		shUI::pushGcode("G28 Z0");
    } else if (hBtn==this->ui.t) {
	    this->actionFilamentChangeParking();
    } else if (hBtn==this->ui.b) {
        this->actionBedParking();
	} else if (hBtn==this->ui.stop) {
		shUI::pushGcode("M84");
	}
}

void HomeUI::refresh_1s() {
	this->drawXYZ();
}
