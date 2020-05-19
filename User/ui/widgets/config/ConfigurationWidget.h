/*
 * ConfigurationWidget.h
 *
 *  Created on: May 18, 2020
 *      Author: shubin
 */

#ifndef USER_UI_WIDGETS_CONFIGURATIONWIDGET_H_
#define USER_UI_WIDGETS_CONFIGURATIONWIDGET_H_

#include "Widget.h"
#include "ui_tools.h"
#include "mks_cfg.h"

typedef struct {
	UI_BUTTON text;
	UI_BUTTON button;
} UI_DOUBLE_BUTTON;

typedef UI_DOUBLE_BUTTON UI_CHECK;
typedef UI_DOUBLE_BUTTON UI_ARROW;

typedef struct {
	UI_BUTTON next;
	UI_BUTTON previous;
	UI_BUTTON back;
	unsigned char page;
	unsigned char count;
} UI_NAVIGATOR;


#define ui_is_double_button(tst, db) ((tst==db.button) || (tst==db.text))


class ConfigurationWidget: public Widget {
protected:
	unsigned char dual_columns;
	UI_NAVIGATOR navigator;
	UI_BUTTON createCheckButton(int x, int y, unsigned char state);
	void updateCheckButton(UI_BUTTON btn, unsigned char state);
	void createCheckPair(int col, int row, UI_CHECK * pair, char* title, unsigned char state);
	void createArrowPair(int col, int row, UI_ARROW * pair, char* title);
	virtual void createControls();
	virtual void on_button(UI_BUTTON hBtn);
public:
	virtual void on_message(WM_MESSAGE * pMsg);
	virtual void show(Widget * caller = 0)  {this->navigator.page = 0; Widget::show(caller); };
	ConfigurationWidget(DISP_STATE id,  unsigned char pages,  unsigned char dual_columns = 0) : Widget(id) {
		this->navigator.count = pages;
		this->navigator.page = 0;
		this->dual_columns = dual_columns;
	};
};

#endif /* USER_UI_WIDGETS_CONFIGURATIONWIDGET_H_ */
