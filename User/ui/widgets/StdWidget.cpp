/*
 * StdWidget.cpp
 *
 *  Created on: May 8, 2020
 *      Author: shubin
 */

#include "StdWidget.h"
#include "integration.h"
#include "ui_tools.h"
#include "ManualLevelingUI.h"

void StdWidget::createStateButton(int x, int y, STATE_BUTTON * btn, const char * picture, const char * title) {
	btn->button = ui_create_state_button(x, y,this->hWnd, picture);
	btn->label = ui_create_std_text(x + STATE_PIC_X_PIXEL, y, 80, STATE_PIC_Y_PIXEL, this->hWnd, title);
}

void StdWidget::updateStateButton(STATE_BUTTON * btn, const char * img, const char * title) {
	if (img!=0)
		ui_update_state_button(btn->button, img);
	ui_set_text_value(btn->label, title);
}

void StdWidget::setButtonText(UI_BUTTON btn, char * text) {
	BUTTON_SetText(btn, text);
}


UI_BUTTON StdWidget::createButtonAt(int phx, int phy, const char * picture, const char * title) {
	return this->createButton(ui_std_col(phx),  ui_std_row(phy), picture, title);
}

UI_BUTTON StdWidget::createButton(int x, int y, const char * picture, const char * title) {
	return ui_create_std_button(x, y, this->hWnd, picture, title);
}

UI_TEXT StdWidget::createText(int x, int y, int w, int h, const char * value) {
	return ui_create_std_text(x, y, w, h, this->hWnd, value);
}

void StdWidget::setText(UI_TEXT hText, char * text) {
	ui_set_text_value(hText, text);
}

void StdWidget::updateButton(UI_BUTTON button, const char * picture, const char * title) {
	ui_update_std_button(button, picture, title);
}


UI_BUTTON StdWidget::createButtonRet() {
	return createButtonAt(3, 1, img_back, lang_str.back);
}

UI_BUTTON StdWidget::create100x80Button(int x, int y, const char * picture) {
	return ui_create_100_80_button(x, y, this->hWnd, picture);
}


void StdWidget::action_leveling() {
	if(shUI::isManualLeveling()) {
    	this->hide();
    	manual_leveling_ui.show(this);
	} else {
		if(shUI::isMeshLeveling()) {
	    	this->hide();
	    	draw_meshleveling();
		} else {
			shUI::doCustomLeveling();
	    }
	}
}

void StdWidget::action_back() {
	this->hide();
	ui_app.back_ui();
}


void StdWidget::drawXYZ() {
	GUI_SetColor(gCfgItems.background_color);
	GUI_FillRect(200, TITLE_YPOS, LCD_WIDTH, titleHeight);
	GUI_SetColor(gCfgItems.title_color);
	shUI::CURRENT_POSITION cp;
	shUI::getCurrentPosition(&cp);
	sprintf(ui_buf1_80,"X:%03.1f Y:%03.1f Z:%03.2f E:%04.1f", cp.x, cp.y, cp.z, cp.e);
	GUI_DispStringAt(ui_buf1_80, 200, TITLE_YPOS);
}

void StdWidget::updateFanState(STATE_BUTTON * stateButton) {
	ui_update_fan_button(stateButton->button, stateButton->label);
}

