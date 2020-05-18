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

static void widget_callback(WM_MESSAGE * pMsg) {
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

void Widget::refresh() {
	if (is_ui_timing(F_UI_TIMING_HALF_SEC)) {
		ui_timing_clear(F_UI_TIMING_HALF_SEC);
		this->refresh_05();
	}
	if (is_ui_timing(F_UI_TIMING_SEC)) {
		ui_timing_clear(F_UI_TIMING_SEC);
		this->refresh_1s();
	}
}

char * Widget::getTitle() {

}

void Widget::recreate() {
	this->dropWindow();
	this->initPage();
	this->hWnd = ui_std_window(widget_callback);
    this->createControls();
}


void Widget::show(Widget * caller) {
	ui_app.current_ui = this;
	ui_app.push(this);
	this->initPage();
	this->hWnd = ui_std_window(widget_callback);
    this->createControls();
}

void Widget::hide() {
	this->dropWindow();
	last_disp_state = this->id;
	ui_app.current_ui = 0;
}

void Widget::dropWindow(){
	GUI_SetBkColor(gCfgItems.background_color);
	if(WM_IsWindow(this->hWnd)) {
		WM_DeleteWindow(this->hWnd);
		this->hWnd = 0;
	}
}

void Widget::initPage() {
	ui_clear_screen();
	ui_set_encoding();
	ui_app.drawTitle();
}
