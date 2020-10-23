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
#include "integration.h"
#include "Application.h"

void ui_update_bed_state_button(STATE_BUTTON * button) {
    shUI::BED_TEMP bt;
    shUI::getBedTemperature(&bt);
    sprintf(ui_buf1_80, "%d/%d°", bt.current,  bt.target);
    ui_set_text_value(button->label, ui_buf1_80);
}

void ui_update_ext_state_button(STATE_BUTTON * button, char index) {
    shUI::SPRAYER_TEMP st;
    shUI::getSprayerTemperature(index, &st);
    sprintf(ui_buf1_80, "%d/%d°", (int)st.current,  (int)st.target);
    ui_set_text_value(button->label, ui_buf1_80);
}

UI_BUTTON Widget::createCheckButton(int x, int y, unsigned char state, BOOLEAN_LANG * lang) {
    BUTTON_Handle btn = BUTTON_CreateEx(x, y, 90, 40, this->hWnd, BUTTON_CF_SHOW, 0, 0);
    this->buttonPreset(btn);
    BUTTON_SetTextAlign(btn, GUI_TA_HCENTER | GUI_TA_VCENTER);
    this->updateCheckButton(btn, state, lang);
    return btn;
}


void Widget::updateCheckButton(UI_BUTTON btn, unsigned char state, BOOLEAN_LANG * lang) {
    BUTTON_SetBmpFileName(btn, state ? img_check_on : img_check_off, 1);
    BUTTON_SetBitmapEx(btn,0,&bmp_struct90X30, 0, 5);
    if (lang==0)
        lang = &lang_str.yes_no;
    BUTTON_SetText(btn, state ? lang->tr : lang->fl);
}


void Widget::createStateButton(int x, int y, STATE_BUTTON * btn, const char * picture, const char * title) {
    btn->button = ui_create_state_button(x, y,this->hWnd, picture);
    btn->label = ui_create_std_text(x + STATE_PIC_X_PIXEL, y, 80, STATE_PIC_Y_PIXEL, this->hWnd, title);
}

void Widget::updateStateButton(STATE_BUTTON * btn, const char * img, const char * title) {
    if (img!=0)
        ui_update_state_button(btn->button, img);
    ui_set_text_value(btn->label, title);
}


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
    		if(pMsg->Data.v == WM_NOTIFICATION_RELEASED) {
                this->on_button((UI_BUTTON) pMsg->hWinSrc);
            } else if (pMsg->Data.v == WM_NOTIFICATION_CLICKED) {
                this->on_button_click((UI_BUTTON) pMsg->hWinSrc);
    		}
    		break;

	}
}

void Widget::createFrame() {
    GUI_Clear();
    ui_app.drawTitle();
    this->hWnd = WM_CreateWindow(0, titleHeight, LCD_WIDTH, LCD_HEIGHT - titleHeight, WM_CF_SHOW, widget_callback, 0);
}

void Widget::createWindow() {
	ui_app.current_ui = this;
    ui_app.defaultUI();
    this->createFrame();
    this->createControls();
}

void Widget::refresh() {
    if (is_ui_timing(F_UI_TIMING_QUARTER_SEC))
        this->refresh_025();
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
	ui_app.current_ui = 0;
}

void Widget::dropWindow(){
	//GUI_SetBkColor(gCfgItems.background_color);
	if ((this->hWnd!=0) && WM_IsWindow(this->hWnd))
		WM_DeleteWindow(this->hWnd);
    this->hWnd = 0;
}

void Widget::setButtonText(UI_BUTTON btn, const char * text) {
	BUTTON_SetText(btn, text);
}

TEXT_Handle Widget::createTextF(int x, int y, int w, int h, int flags, const char *text) {
	TEXT_Handle res = TEXT_CreateEx(x, y, w, h, this->hWnd, WM_CF_SHOW, flags,  GUI_ID_TEXT0, 0);
	TEXT_SetBkColor(res,  gCfgItems.background_color);
	TEXT_SetTextColor(res, gCfgItems.title_color);
	TEXT_SetText(res, text);
	return res;
}


UI_TEXT Widget::createText(int x, int y, int w, int h, const char * value) {
	return this->createTextF(x, y, w, h, TEXT_CF_LEFT|TEXT_CF_VCENTER, value);
}

void Widget::setText(UI_TEXT hText, const char * text) {
    TEXT_SetBkColor(hText, gCfgItems.background_color);
    TEXT_SetTextColor(hText, gCfgItems.title_color);
    TEXT_SetText(hText, text);
}

void Widget::updateButton(UI_BUTTON button, const char * picture, const char * title) {
	ui_update_std_button(button, picture, title);
}



UI_BUTTON Widget::create96x80Button(int x, int y, const char * picture) {
	return ui_create_96_80_button(x, y, this->hWnd, picture);
}

UI_BUTTON Widget::create70x50Button(int x, int y, const char * picture, const char * text) {
    BUTTON_Handle btn = BUTTON_CreateEx(x, y, 70, 60, this->hWnd, BUTTON_CF_SHOW, 0, 0);
    BUTTON_SetTextAlign(btn, GUI_TA_HCENTER|GUI_TA_VCENTER );
    BUTTON_SetBmpFileName(btn, picture, 1);
    BUTTON_SetBitmapEx(btn, 0, &bmp_struct_70x50, 0, 0);
    BUTTON_SetText(btn, text);
    return btn;

}


void Widget::drawXYZ() {
    GUI_SetColor(gCfgItems.background_color);
    GUI_FillRect(200, TITLE_YPOS, LCD_WIDTH, titleHeight);
    GUI_SetColor(gCfgItems.title_color);
    shUI::CURRENT_POSITION cp;
    shUI::getCurrentPosition(&cp);
    sprintf(ui_buf1_80,"X:%03.1f Y:%03.1f Z:%03.2f E:%04.1f", cp.x, cp.y, cp.z, cp.e);
    GUI_DispStringAt(ui_buf1_80, 200, TITLE_YPOS);
}

