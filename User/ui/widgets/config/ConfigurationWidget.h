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

#define ui_invert_u8_flag(S) S=S==0 ? 1 : 0

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
} UI_NAVIGATOR;

typedef struct {
    UI_BUTTON text;
    UI_BUTTON button;
    UI_BUTTON dflt;
} UI_INPUT_WITH_DEFAULT;




#define ui_is_double_button(tst, db) ((tst==db.button) || (tst==db.text))


class ConfigurationWidget: public Widget {
private:
    unsigned char count;
protected:
    unsigned char page;
	unsigned char dual_columns = 0;
	UI_NAVIGATOR navigator;
	UI_BUTTON createCheckButton(int x, int y, unsigned char state);
	UI_TEXT createLabel(int col, int row, const char* title);
	void updateCheckButton(UI_BUTTON btn, unsigned char state);
	void createCheckPair(int col, int row, UI_CHECK * pair, const char* title, unsigned char state);
	void createArrowPair(int col, int row, UI_ARROW * pair, const char* title);
	void createInputWithDefault(int col, int row, UI_INPUT_WITH_DEFAULT * input, const char* title, const char* value, const char* dflt = 0);
	virtual void createControls();
	virtual void on_button(UI_BUTTON hBtn);
public:
	virtual void on_message(WM_MESSAGE * pMsg);
	virtual void show(Widget * caller = 0)  {Widget::show(caller); };
	ConfigurationWidget(DISP_STATE id,  unsigned char pages = 1) : Widget(id) {
		this->count = pages;
		this->page = 0;
	};
};

#endif /* USER_UI_WIDGETS_CONFIGURATIONWIDGET_H_ */
