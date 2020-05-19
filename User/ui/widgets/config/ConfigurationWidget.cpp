/*
 * ConfigurationWidget.cpp
 *
 *  Created on: May 18, 2020
 *      Author: shubin
 */

#include "ConfigurationWidget.h"
#include "GUI.h"
#include "BUTTON.h"
#include "ui_tools.h"


void ConfigurationWidget::on_message(WM_MESSAGE * pMsg) {
    switch (pMsg->MsgId) {
    	case WM_PAINT: {
			GUI_SetColor(0xff5449);
    		if (this->dual_columns) {
    			GUI_FillRect(10, 50, 230, 50);
    			GUI_FillRect(10, 100, 230, 100);
    			GUI_FillRect(10, 150, 230, 150);
    			GUI_FillRect(10, 200, 230, 200);

    			GUI_FillRect(250, 50, 470, 50);
    			GUI_FillRect(250, 100, 470, 100);
    			GUI_FillRect(250, 150, 470, 150);
    			GUI_FillRect(250, 200, 470, 200);
    		} else {
    			GUI_FillRect(10, 50, 470, 50);
    			GUI_FillRect(10, 100, 470, 100);
    			GUI_FillRect(10, 150, 470, 150);
    			GUI_FillRect(10, 200, 470, 200);
    		}

    	}
    }
    Widget::on_message(pMsg);
}


void ConfigurationWidget::on_button(UI_BUTTON hBtn) {
	if(hBtn == this->navigator.back) {
		this->hide();
		ui_app.back_ui();
	} else if(hBtn == this->navigator.next) {
		this->navigator.page++;
		if (this->navigator.page>=this->navigator.count)
			this->navigator.page = 0;
		this->recreate();
	} else if(hBtn == this->navigator.previous) {
		if (this->navigator.page!=0)
			this->navigator.page--;
		else
			this->navigator.page = this->navigator.count - 1;
		this->recreate();
	}
}


void ConfigurationWidget::updateCheckButton(UI_BUTTON btn, unsigned char state) {
	BUTTON_SetBmpFileName(btn, state ? img_check_on : img_check_off, 1);
    BUTTON_SetBitmapEx(btn,0,&bmp_struct90X30, 0, 5);
	BUTTON_SetText(btn, state ? lang_str.yes : lang_str.no);
}


UI_BUTTON ConfigurationWidget::createCheckButton(int x, int y, unsigned char state) {
	BUTTON_Handle btn = BUTTON_CreateEx(x, y, 90, 40, this->hWnd, BUTTON_CF_SHOW, 0, alloc_win_id());
	this->buttonPreset(btn);
	BUTTON_SetTextAlign(btn, GUI_TA_HCENTER | GUI_TA_VCENTER);
	this->updateCheckButton(btn, state);
	return btn;
}

#define X_TEXT 		10
#define X_RADIO 	370
#define X_RADIO_SZ 	110
#define X_ARROW_SZ 	10

#define ROW(idx) (10+50*idx)
#define TEXT_X_OFFSET 20

void ConfigurationWidget::createCheckPair(int col, int row, UI_CHECK * pair, char* title, unsigned char state) {
	int x = (this->dual_columns ? 240 * (col+1) : 240 * 2) - X_RADIO_SZ;
    pair->button = this->createCheckButton(x, ROW(row), state);
    x = (this->dual_columns ? 240 * col : 0) + TEXT_X_OFFSET;
    int w = (this->dual_columns? (240 - X_RADIO_SZ) : 240 * 2 - X_RADIO_SZ) - TEXT_X_OFFSET;
    pair->text = BUTTON_CreateEx(x, ROW(row), w, 40, this->hWnd, BUTTON_CF_SHOW, 0, alloc_win_id());
    BUTTON_SetTextAlign(pair->text, GUI_TA_LEFT | GUI_TA_VCENTER);
    BUTTON_SetText(pair->text, title);
}

void ConfigurationWidget::createControls() {
	this->navigator.back = BUTTON_CreateEx(400, 230, 70, 40, this->hWnd, BUTTON_CF_SHOW, 0, alloc_win_id());
    BUTTON_SetBmpFileName(this->navigator.back, "bmp_back70x40.bin",1);
    BUTTON_SetBitmapEx(this->navigator.back, 0, &bmp_struct70X40,0, 0);
    this->navigator.next = 0;
    this->navigator.previous = 0;

    if (this->navigator.count>1) {
    	if (this->navigator.page < this->navigator.count-1)
    		this->navigator.next = BUTTON_CreateEx(320, 230, 70, 40, this->hWnd, BUTTON_CF_SHOW, 0, alloc_win_id());
    	if (this->navigator.page > 0) {
    		this->navigator.previous = BUTTON_CreateEx(this->navigator.next ? 240 : 320, 230, 70, 40, this->hWnd, BUTTON_CF_SHOW, 0, alloc_win_id());
    	}
    }

	if (this->navigator.next!=0) {
		BUTTON_SetBmpFileName(this->navigator.next, "bmp_next70x40.bin",1);
		BUTTON_SetBitmapEx(this->navigator.next, 0, &bmp_struct70X40,0, 0);
	}

	if (this->navigator.previous!=0) {
		BUTTON_SetBmpFileName(this->navigator.previous, "bmp_prev70x40.bin",1);
		BUTTON_SetBitmapEx(this->navigator.previous, 0, &bmp_struct70X40,0, 0);
	}
}

void ConfigurationWidget::createArrowPair(int col, int row, UI_ARROW * pair, char* title) {
    int x = (this->dual_columns ? 240 * (col+1) : 240 * 2) - X_ARROW_SZ;
    pair->button = BUTTON_CreateEx(x, ROW(row), 20, 40, this->hWnd, BUTTON_CF_SHOW, 5, 12);
    BUTTON_SetBmpFileName(pair->button, "bmp_arrow.bin",1);
    BUTTON_SetBitmapEx(pair->button, 0, &bmp_struct10x17, 0, 0);

    x = (this->dual_columns ? 240 * col : 0) + TEXT_X_OFFSET;
    int w = (this->dual_columns? (240 - X_ARROW_SZ) : 240 * 2 - X_ARROW_SZ) - TEXT_X_OFFSET;
	pair->text = BUTTON_CreateEx(x, ROW(row), w, 40, this->hWnd, BUTTON_CF_SHOW, 0, 0);
    BUTTON_SetTextAlign(pair->text,GUI_TA_LEFT|GUI_TA_VCENTER);
    BUTTON_SetText(pair->text, title);
}

