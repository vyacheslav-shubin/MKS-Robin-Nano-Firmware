/*
 * Widget.cpp
 *
 *  Created on: May 8, 2020
 *      Author: shubin
 */

#include "Widget.h"
#include "GUI.h"
#include "ui_tools.h"
#include "WM.h"
#include "BUTTON.h"

void Widget::buttonPreset(UI_BUTTON btn) {
	BUTTON_SetBkColor(btn, BUTTON_CI_PRESSED, gCfgItems.btn_color);
	BUTTON_SetBkColor(btn, BUTTON_CI_UNPRESSED, gCfgItems.btn_color);
	BUTTON_SetTextColor(btn, BUTTON_CI_PRESSED, gCfgItems.btn_textcolor);
	BUTTON_SetTextColor(btn, BUTTON_CI_UNPRESSED, gCfgItems.btn_textcolor);
}

void widget_callback(WM_MESSAGE * pMsg) {
	ui_app.current_ui->on_message(pMsg);
}

bool Widget::is_active() {
	return this==ui_app.current_ui;
}

void Widget::on_message(WM_MESSAGE * pMsg) {
	switch (pMsg->MsgId) {
    	case WM_NOTIFY_PARENT:
    		if(pMsg->Data.v == WM_NOTIFICATION_RELEASED)
    			this->on_button((UI_BUTTON)pMsg->hWinSrc);
    		break;
	}
}

void Widget::createWindow() {
	ui_app.current_ui = this;
    ui_app.defaultUI();
	GUI_Clear();
    ui_app.drawTitle();
    this->hWnd = WM_CreateWindow(0, titleHeight, LCD_WIDTH, LCD_HEIGHT - titleHeight, WM_CF_SHOW, widget_callback, 0);
    this->createControls();
}

void Widget::refresh() {
	if (is_ui_timing(F_UI_TIMING_HALF_SEC))
		this->refresh_05();
	if (is_ui_timing(F_UI_TIMING_SEC))
		this->refresh_1s();
}

void Widget::recreate() {
	this->dropWindow();
	this->createWindow();
}


void Widget::show(Widget * caller) {
	ui_app.push(this);
	this->createWindow();
}

void Widget::hide() {
	this->dropWindow();
	last_disp_state = this->id;
	ui_app.current_ui = 0;
}

void Widget::dropWindow(){
	//GUI_SetBkColor(gCfgItems.background_color);
	if ((this->hWnd!=0) && WM_IsWindow(this->hWnd)) {
		WM_DeleteWindow(this->hWnd);
		this->hWnd = 0;
	}
}

void Widget::setButtonText(UI_BUTTON btn, char * text) {
	BUTTON_SetText(btn, text);
}

TEXT_Handle Widget::createTextF(int x, int y, int w, int h, int flags, char *text) {
	TEXT_Handle res = TEXT_CreateEx(x, y, w, h, this->hWnd, WM_CF_SHOW, flags,  GUI_ID_TEXT0, 0);
	TEXT_SetBkColor(res,  gCfgItems.background_color);
	TEXT_SetTextColor(res, gCfgItems.title_color);
	TEXT_SetText(res, text);
	return res;
}


UI_TEXT Widget::createText(int x, int y, int w, int h, const char * value) {
	return this->createTextF(x, y, w, h, TEXT_CF_LEFT|TEXT_CF_VCENTER, value);
}

void Widget::setText(UI_TEXT hText, char * text) {
	ui_set_text_value(hText, text);
}

void Widget::updateButton(UI_BUTTON button, const char * picture, const char * title) {
	ui_update_std_button(button, picture, title);
}



UI_BUTTON Widget::create100x80Button(int x, int y, const char * picture) {
	return ui_create_100_80_button(x, y, this->hWnd, picture);
}

