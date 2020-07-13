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
#include "Application.h"



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
		this->page++;
		if (this->page>=this->page_count)
			this->page = 0;
		this->recreate();
	} else if(hBtn == this->navigator.previous) {
		if (this->page!=0)
			this->page--;
		else
			this->page = this->page_count - 1;
		this->recreate();
	}
}



#define X_RADIO_SZ 	30
#define X_CHECK_BOX_SZ 	110
#define X_ARROW_SZ 	10

#define ROW(idx) (10+50*idx)
#define TEXT_X_OFFSET 20

void ConfigurationWidget::createRadio(int col, int row, UI_RADIO * radio, const char* title, unsigned char state) {
    int x = (this->dual_columns ? 240 : 0) * col + TEXT_X_OFFSET;
    int w = (this->dual_columns ? 240 - TEXT_X_OFFSET : (480 * (2 - TEXT_X_OFFSET))) - X_RADIO_SZ - 10;

    radio->text = BUTTON_CreateEx(x, ROW(row), w, 40, this->hWnd, BUTTON_CF_SHOW, 0, 0);
    BUTTON_SetTextAlign(radio->text, GUI_TA_LEFT | GUI_TA_VCENTER);
    BUTTON_SetText(radio->text, title);

    radio->button = BUTTON_CreateEx(x + w, ROW(row), 40, 40, this->hWnd, BUTTON_CF_SHOW, 0, 0);
    this->updateRadio(radio->button, state);
}

void ConfigurationWidget::updateRadio(UI_BUTTON radio, unsigned char state) {
    BUTTON_SetBmpFileName(radio, state ? img_radio_sel : img_radio, 1);
    BUTTON_SetBitmapEx(radio, 0, &bmp_struct_30x30,5, 5);
}


void ConfigurationWidget::createCheckPair(int col, int row, UI_CHECK * pair, const char* title, unsigned char state, BOOLEAN_LANG * lang) {
	int x = (this->dual_columns ? 240 * (col+1) : 240 * 2) - X_CHECK_BOX_SZ;
    pair->button = this->createCheckButton(x, ROW(row), state, lang);
    x = (this->dual_columns ? 240 * col : 0) + TEXT_X_OFFSET;
    int w = (this->dual_columns ? (240 - X_CHECK_BOX_SZ) : 240 * 2 - X_CHECK_BOX_SZ) - TEXT_X_OFFSET;
    pair->text = BUTTON_CreateEx(x, ROW(row), w, 40, this->hWnd, BUTTON_CF_SHOW, 0, 0);
    BUTTON_SetTextAlign(pair->text, GUI_TA_LEFT | GUI_TA_VCENTER);
    BUTTON_SetText(pair->text, title);
}

void ConfigurationWidget::createControls() {
    if (this->page>= this->page_count)
        this->page = 0;
	this->navigator.back = BUTTON_CreateEx(400, 230, 70, 40, this->hWnd, BUTTON_CF_SHOW, 0, 0);
    BUTTON_SetBmpFileName(this->navigator.back, img_navigator_back,1);
    BUTTON_SetBitmapEx(this->navigator.back, 0, &bmp_struct70X40,0, 0);
    this->navigator.next = 0;
    this->navigator.previous = 0;

    if (this->page_count>1) {
    	if (this->page < this->page_count-1)
    		this->navigator.next = BUTTON_CreateEx(320, 230, 70, 40, this->hWnd, BUTTON_CF_SHOW, 0, 0);
    	if (this->page > 0) {
    		this->navigator.previous = BUTTON_CreateEx(this->navigator.next ? 240 : 320, 230, 70, 40, this->hWnd, BUTTON_CF_SHOW, 0, 0);
    	}
    }

	if (this->navigator.next!=0) {
		BUTTON_SetBmpFileName(this->navigator.next, img_navigator_next,1);
		BUTTON_SetBitmapEx(this->navigator.next, 0, &bmp_struct70X40,0, 0);
	}

	if (this->navigator.previous!=0) {
		BUTTON_SetBmpFileName(this->navigator.previous, img_navigator_prev,1);
		BUTTON_SetBitmapEx(this->navigator.previous, 0, &bmp_struct70X40,0, 0);
	}
}

void ConfigurationWidget::createArrowPair(int col, int row, UI_ARROW * pair, const char* title) {
    int x = (this->dual_columns ? 240 * (col+1) : 240 * 2) - X_ARROW_SZ;
    pair->button = BUTTON_CreateEx(x, ROW(row), 20, 40, this->hWnd, BUTTON_CF_SHOW, 0, 0);
    BUTTON_SetBmpFileName(pair->button, img_arrow, 1);
    BUTTON_SetBitmapEx(pair->button, 0, &bmp_struct10x17, 0, 0);

    x = (this->dual_columns ? 240 * col : 0) + TEXT_X_OFFSET;
    int w = (this->dual_columns? (240 - X_ARROW_SZ) : 240 * 2 - X_ARROW_SZ) - TEXT_X_OFFSET;
	pair->text = BUTTON_CreateEx(x, ROW(row), w, 40, this->hWnd, BUTTON_CF_SHOW, 0, 0);
    BUTTON_SetTextAlign(pair->text,GUI_TA_LEFT|GUI_TA_VCENTER);
    BUTTON_SetText(pair->text, title);
}

UI_TEXT ConfigurationWidget::createLabel(int col, int row, const char* title) {
	int text_w = (this->dual_columns ? 240 : 480) - TEXT_X_OFFSET - 10;
	int x = (this->dual_columns ? 240 * col : 0) + TEXT_X_OFFSET;
	return this->createText(x, ROW(row), text_w, 40, title);
}


UI_BUTTON ConfigurationWidget::createInput(int x, int y, const char* value) {
    UI_BUTTON button = BUTTON_CreateEx(x, y, 70, 28, this->hWnd, BUTTON_CF_SHOW, 0, 0);
    BUTTON_SetBmpFileName(button, img_value_blank, 1);
    BUTTON_SetBitmapEx(button, 0, &bmp_struct_70x28, 0, 0);
    BUTTON_SetText(button, value);
    BUTTON_SetTextAlign(button, GUI_TA_CENTER | GUI_TA_VCENTER);
    BUTTON_SetTextColor(button, BUTTON_CI_PRESSED, 0x0);
    BUTTON_SetTextColor(button, BUTTON_CI_UNPRESSED, 0x0);
    BUTTON_SetBkColor(button, BUTTON_CI_PRESSED, 0xe3ebca & 0xF8FAF8);
    BUTTON_SetBkColor(button, BUTTON_CI_UNPRESSED, 0xe3ebca & 0xF8FAF8); //caebe3
    return button;
}

UI_BUTTON ConfigurationWidget::createDefaultSetButtonAt(int col, int row, int offset, const char* value) {
    int x = (this->dual_columns ? 240 * col : 0);
    return this->createDefaultSetButton(x + TEXT_X_OFFSET + offset, ROW(row) + 6, value);
}


UI_BUTTON ConfigurationWidget::createDefaultSetButton(int x, int y, const char* value) {
    UI_BUTTON dflt = BUTTON_CreateEx(x, y, 70, 28, this->hWnd, BUTTON_CF_SHOW, 0, 0);
    BUTTON_SetBmpFileName(dflt, img_value_default, 1);
    BUTTON_SetBitmapEx(dflt, 0, &bmp_struct_70x28, 0, 0);
    BUTTON_SetTextAlign(dflt,GUI_TA_CENTER | GUI_TA_VCENTER);
    BUTTON_SetTextColor(dflt, BUTTON_CI_PRESSED, 0xFFFFFF);
    BUTTON_SetTextColor(dflt, BUTTON_CI_UNPRESSED, 0xFFFFFF);
    BUTTON_SetBkColor(dflt, BUTTON_CI_PRESSED, 0xf8845d & 0xF8FAF8);
    BUTTON_SetBkColor(dflt, BUTTON_CI_UNPRESSED, 0xf8845d & 0xF8FAF8);
    BUTTON_SetText(dflt, value);
    return dflt;

}

void ConfigurationWidget::createInputWithDefault(int col, int row, UI_INPUT_WITH_DEFAULT * input, const char* title, const char* value,  const char* dflt) {
	int x = (this->dual_columns ? 240 * col : 0);
	int text_w = (this->dual_columns ? 240 : 480)- (70 * 2) - 8 - TEXT_X_OFFSET - 10;

    input->button = this->createInput(TEXT_X_OFFSET + x +  text_w + 4, ROW(row) + 6, value);
    input->dflt = this->createDefaultSetButton(TEXT_X_OFFSET + x + text_w + 4 + 70 + 4, ROW(row) + 6, dflt);

	input->text = BUTTON_CreateEx(TEXT_X_OFFSET + x , ROW(row),  text_w, 40, this->hWnd, BUTTON_CF_SHOW, 0, 0);
    BUTTON_SetTextAlign(input->text,GUI_TA_LEFT | GUI_TA_VCENTER);
    BUTTON_SetText(input->text, title);
}

void ConfigurationWidget::createInputSingle(int col, int row, UI_INPUT_SINGLE * input, const char* title, const char* value) {
    int x = (this->dual_columns ? 240 * col : 0);
    int text_w = (this->dual_columns ? 240 : 480)- 70  - 4 - TEXT_X_OFFSET - 10;

    input->button = this->createInput(TEXT_X_OFFSET + x +  text_w + 4, ROW(row) + 6, value);

    input->text = BUTTON_CreateEx(TEXT_X_OFFSET + x , ROW(row),  text_w, 40, this->hWnd, BUTTON_CF_SHOW, 0, 0);
    BUTTON_SetTextAlign(input->text,GUI_TA_LEFT | GUI_TA_VCENTER);
    BUTTON_SetText(input->text, title);
}


void ConfigurationWidget::createInputDial(int col, int row, UI_INPUT_DUAL * input, const char* title, const char* value1, const char* value2) {
    int x = (this->dual_columns ? 240 * col : 0);
    int text_w = (this->dual_columns ? 240 : 480)- (70 * 2) - 8 - TEXT_X_OFFSET - 10;

    input->button1 = this->createInput(TEXT_X_OFFSET + x +  text_w + 4, ROW(row) + 6, value1);
    input->button2 = this->createInput(TEXT_X_OFFSET + x + text_w + 4 + 70 + 4, ROW(row) + 6, value2);

    input->text = BUTTON_CreateEx(TEXT_X_OFFSET + x , ROW(row),  text_w, 40, this->hWnd, BUTTON_CF_SHOW, 0, 0);
    BUTTON_SetTextAlign(input->text,GUI_TA_LEFT | GUI_TA_VCENTER);
    BUTTON_SetText(input->text, title);
}


void ConfigurationWidgetWithCalc::calculator(const char * label, const char * sub_label, double value, unsigned char id) {
    this->hide();
    const char * title;
    if (sub_label) {
        if (ui_buf1_100!=label)
            strcpy(ui_buf1_100, label);
        strcat(ui_buf1_100, sub_label);
        title = ui_buf1_100;
    } else
        title = label;
    calculator_dialog_ui.show(title, value, id, this, this);
}
