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

static Widget * current_widget;

static void widget_callback(WM_MESSAGE * pMsg) {
	current_widget->on_message(pMsg);
}

void Widget::on_message(WM_MESSAGE * pMsg) {
	switch (pMsg->MsgId) {
    	case WM_NOTIFY_PARENT:
    		if(pMsg->Data.v == WM_NOTIFICATION_RELEASED)
    			this->on_button(pMsg->hWinSrc);
    		break;
	}
}


char * Widget::getTitle() {

}

void Widget::show(Widget * caller) {
	current_widget = this;
	ui_app.push(this);
	ui_init_page();
	this->hWnd = ui_std_window(widget_callback);
    this->createControls();
}

void Widget::hide() {
	ui_drop_window(this->hWnd);
	this->hWnd = 0;
	last_disp_state = this->id;
}

