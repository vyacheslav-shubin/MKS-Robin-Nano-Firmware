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
#include "../dialog/CalculatorDialogUI.h"

#define ui_invert_u8_flag(S) S=S==0 ? 1 : 0


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

typedef struct {
    UI_BUTTON text;
    UI_BUTTON button;
} UI_CONFIG_BUTTON;

typedef struct {
    UI_BUTTON text;
    UI_BUTTON button1;
    UI_BUTTON button2;
} UI_INPUT_DUAL;

typedef struct {
    UI_BUTTON text;
    UI_BUTTON button;
} UI_INPUT_SINGLE;



typedef UI_DOUBLE_BUTTON UI_RADIO;


typedef struct {
    UI_CHECK enabled;
    UI_INPUT_WITH_DEFAULT p;
    UI_INPUT_WITH_DEFAULT i;
    UI_INPUT_WITH_DEFAULT d;
} PID_UI_CONTROPLS;

typedef struct {
    UI_INPUT_WITH_DEFAULT p;
    UI_INPUT_WITH_DEFAULT i;
    UI_INPUT_WITH_DEFAULT d;
} PID_UI_CONTROPLS_1;

typedef struct {
    UI_INPUT_WITH_DEFAULT lo;
    UI_INPUT_WITH_DEFAULT hi;
} TEMP_INTERVAL_UI_CONTROLS;


#define ui_is_double_button(tst, db) ((tst==db.button) || (tst==db.text))

class ConfigurationWidget: public Widget {
protected:
	unsigned char page_count;
    unsigned char page;
	unsigned char dual_columns = 0;
	UI_NAVIGATOR navigator;
	UI_TEXT createLabel(int col, int row, const char* title);
    void createCheckPair(int col, int row, UI_CHECK * pair, const char* title, unsigned char state, BOOLEAN_LANG * lang = &lang_str.yes_no);
    void createRadio(int col, int row, UI_RADIO * radio, const char* title, unsigned char state);
    void updateRadio(UI_BUTTON radio, unsigned char state);

	void createArrowPair(int col, int row, UI_ARROW * pair, const char* title);
    void createInputWithDefault(int col, int row, UI_INPUT_WITH_DEFAULT * input, const char* title, const char* value, const char* dflt = 0);
    void createConfigButton(int col, int row, UI_CONFIG_BUTTON * button, const char* title);
    void createInputDial(int col, int row, UI_INPUT_DUAL * input, const char* title, const char* value1, const char* value2);
    void createInputSingle(int col, int row, UI_INPUT_SINGLE * input, const char* title, const char* value);
    UI_BUTTON createInput(int x, int y, const char* value);
    UI_BUTTON createDefaultSetButton(int x, int y, const char* value);
    UI_BUTTON createDefaultSetButtonAt(int col, int row, int offset, const char* value);
    virtual void createControls();
	virtual void on_button(UI_BUTTON hBtn);
public:
    virtual WIDGET_TYPE getType() {return WIDGET_CONFIG;};
    virtual void on_message(WM_MESSAGE * pMsg);
	virtual void show(Widget * caller = 0)  {Widget::show(caller); };
	ConfigurationWidget(DISP_STATE id,  unsigned char pages = 1) : Widget(id) {
		this->page_count = pages;
		this->page = 0;
	};
};

class ConfigurationWidgetWithCalc : public ConfigurationWidget, public CalculatorDialogCallback {
protected:
    virtual void setValue(unsigned char id, double value) = 0;
    void calculator(const char * label, const char * sub_label, double value, unsigned char id);
    void calculator(const char * label,  double value, unsigned char id) {
        this->calculator(label, 0, value, id);
    }
public:
    virtual void on_calculator(unsigned char action, double result, unsigned char dialog_id) {
        calculator_dialog_ui.hide();
        if (action==UI_BUTTON_OK)
            this->setValue(dialog_id, result);
        this->show();
    }
    ConfigurationWidgetWithCalc(DISP_STATE id,  unsigned char pages = 1) : ConfigurationWidget(id, pages) {};
};

#endif /* USER_UI_WIDGETS_CONFIGURATIONWIDGET_H_ */
