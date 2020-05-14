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

void Widget::show(Widget * caller) {
	ui_app.current_ui = this;
	ui_app.push(this);
	ui_init_page();
	this->hWnd = ui_std_window(widget_callback);
    this->createControls();
}

void Widget::hide() {
	ui_drop_window(this->hWnd);
	this->hWnd = 0;
	last_disp_state = this->id;
	ui_app.current_ui = 0;
}

