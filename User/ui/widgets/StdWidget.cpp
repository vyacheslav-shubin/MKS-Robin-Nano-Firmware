/*
 * StdWidget.cpp
 *
 *  Created on: May 8, 2020
 *      Author: shubin
 */

#include "StdWidget.h"
#include "Configuration.h"
#include "ui_tools.h"
#include "Marlin.h"
#include "ManualLevelingUI.h"

//#define ui_std_col(ph_x) (INTERVAL_H + (BTN_X_PIXEL+INTERVAL_H)*(ph_x))
//#define ui_std_row(ph_y) (INTERVAL_V + (BTN_Y_PIXEL + INTERVAL_V) * (ph_y))

BUTTON_Handle StdWidget::createButtonAt(int phx, int phy, const char * picture, const char * title) {
	return this->createButton(ui_std_col(phx),  ui_std_row(phy), picture, title);
}

BUTTON_Handle StdWidget::createButton(int x, int y, const char * picture, const char * title) {
	return ui_create_std_button(x, y, this->hWnd, picture, title);
}

BUTTON_Handle StdWidget::createButtonRet() {
	return createButtonAt(3, 1, img_back, lang_str.back);
}

void StdWidget::action_leveling() {
	if(gCfgItems.leveling_mode == 1) {
		if(BED_LEVELING_METHOD & MESH_BED_LEVELING) {
	    	this->hide();
	    	draw_meshleveling();
		} else {
		    //SPI_FLASH_BufferRead((u8 *)cmd_code,BUTTON_AUTOLEVELING_ADDR,201);
		    //codebufpoint = cmd_code;
	    }
	} else {
    	this->hide();
    	manual_leveling_ui.show(this);
	}
}

void StdWidget::action_back() {
	this->hide();
	ui_app.back_ui();
}


void StdWidget::draw_xyz() {
	GUI_SetColor(gCfgItems.background_color);
	GUI_FillRect(200, TITLE_YPOS, LCD_WIDTH, titleHeight);
	GUI_SetColor(gCfgItems.title_color);
	sprintf(ui_buf1_20,"X:%1.2f Y:%1.2f Z:%1.2f", current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS]);
	GUI_DispStringAt(ui_buf1_20,200, TITLE_YPOS);
}
