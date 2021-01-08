/*
 * Widget.h
 *
 *  Created on: May 8, 2020
 *      Author: shubin
 */

#ifndef USER_UI_WIDGETS_WIDGET_H_
#define USER_UI_WIDGETS_WIDGET_H_

#include "GUI.h"
#include "BUTTON.h"
#include "TEXT.h"
#include "draw_ui.h"

class Application;

typedef BUTTON_Handle 		UI_BUTTON;
typedef TEXT_Handle 		UI_TEXT;
typedef PROGBAR_Handle 		UI_PROGRESS_BAR;
typedef GUI_HWIN 			UI_WND;

void widget_callback(WM_MESSAGE * pMsg);

typedef enum {
    WIDGET_UI, WIDGET_ROOT_UI, WIDGET_DIALOG, WIDGET_CONFIG
} WIDGET_TYPE;
typedef struct {
    UI_BUTTON text;
    UI_BUTTON button;
} UI_DOUBLE_BUTTON;

typedef UI_DOUBLE_BUTTON UI_CHECK;
typedef UI_DOUBLE_BUTTON UI_ARROW;

typedef struct {
    UI_BUTTON 		button;
    unsigned char	index;
} UI_SELECTOR;

typedef struct {
    float size;
    const char * title;
    const char * picture;
} UI_STEP_INFO;

typedef struct {
    const char * picture;
    const char ** title;
} UI_BUTTON_INFO;

typedef struct {
    UI_BUTTON button;
    UI_TEXT	label;
} STATE_BUTTON;

extern void ui_update_heatpreset_button(UI_BUTTON button, char index);
extern void ui_update_bed_state_button(STATE_BUTTON * button);
extern void ui_update_ext_state_button(STATE_BUTTON * button, char index);


class ActionDialogCallback {
protected:
public:
    virtual void on_action_dialog(u8 action, u8 dialog_id) = 0;
    virtual ~ActionDialogCallback() {};
};

class Widget {
protected:
	DISP_STATE id;
	UI_WND hWnd = 0;
	virtual void createControls() {};
	virtual void on_button(UI_BUTTON hBtn) {};
    virtual void on_button_click(UI_BUTTON hBtn) {};
    virtual void refresh_025() {};
	virtual void refresh_05() {};
	virtual void refresh_1s() {};
	void dropWindow();
	void buttonPreset(UI_BUTTON btn);
    virtual void createFrame();
	virtual void createWindow();
    void drawXYZ();
    UI_BUTTON create90x60Button(int x, int y, const char * picture);
    UI_BUTTON create96x80Button(int x, int y, const char * picture);
    UI_BUTTON create70x50Button(int x, int y, const char * picture, const char * text);
	void updateButton(UI_BUTTON button, const char * picture, const char * title);
	void setButtonText(UI_BUTTON btn, const char * text);
	UI_TEXT createText(int x, int y, int w, int h, const char * value);
	UI_TEXT createTextF(int x, int y, int w, int h, int flags, const char *text);
	void setText(UI_TEXT hText, const char * text);
    void createStateButton(int x, int y, STATE_BUTTON * btn, const char * picture, const char * title);
    void updateStateButton(STATE_BUTTON * btn, const char * img, const char * title);
    UI_BUTTON createCheckButton(int x, int y, unsigned char state, BOOLEAN_LANG * lang = &lang_str.yes_no);
    void updateCheckButton(UI_BUTTON btn, unsigned char state, BOOLEAN_LANG * lang = &lang_str.yes_no);
public:
	Widget(DISP_STATE id) {this->id=id;};
	bool is_active();
    virtual WIDGET_TYPE getType() {return WIDGET_UI;};
	virtual void on_message(WM_MESSAGE * pMsg);
	virtual ~Widget() {};
	virtual const char * getTitle() {return 0; };
	virtual void show(Widget * caller = 0);
	virtual void recreate();
	virtual void refresh();
	virtual void hide();

	friend Application;
};

#endif /* USER_UI_WIDGETS_WIDGET_H_ */
