/*
 * DialogWidget.cpp
 *
 *  Created on: May 20, 2020
 *      Author: shubin
 */

#include "DialogWidget.h"
#include "ui_tools.h"

DialogWidget dialog_widget;

void DialogWidget::createFrame() {
#define ax1	DIALOG_X
#define ay1	DIALOG_Y
#define ax2	(DIALOG_X+DIALOG_WIDTH)
#define ay2	(DIALOG_Y+DIALOG_HEIGHT)


	GUI_SetColor(gCfgItems.background_color);
	GUI_FillRect(ax1, ay1, ax2, ay2);
	GUI_SetColor(0xff5449);
	GUI_FillRect(ax1, ay1, ax2, ay1+2);
	GUI_FillRect(ax1, ay2-2, ax2, ay2);
	GUI_FillRect(ax1, ay1, ax1+2, ay2);
	GUI_FillRect(ax2-2, ay1, ax2, ay2);
	ui_app.defaultUI();
	this->hWnd = WM_CreateWindow(DIALOG_X, DIALOG_Y, DIALOG_WIDTH, DIALOG_HEIGHT, WM_CF_SHOW, widget_callback, 0);
}

void DialogWidget::createDialogDecoration(const char * img, const char * title) {
	UI_BUTTON btn = BUTTON_CreateEx(10, 10, 50, 50, this->hWnd, BUTTON_CF_SHOW, 0, 0);
	BUTTON_SetBmpFileName(btn, img, 1);
	BUTTON_SetBitmapEx(btn, 0, &bmp_struct_50x50, 0, 0);
}

void DialogWidget::hide() {
	Widget::hide();
	ui_app.pop();
}

