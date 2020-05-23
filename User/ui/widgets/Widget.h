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

#include "Application.h"

typedef BUTTON_Handle 		UI_BUTTON;
typedef TEXT_Handle 		UI_TEXT;
typedef PROGBAR_Handle 		UI_PROGRESS_BAR;
typedef GUI_HWIN 			UI_WND;

void widget_callback(WM_MESSAGE * pMsg);

class Widget {
protected:
	DISP_STATE id;
	UI_WND hWnd = 0;
	virtual void createControls() {};
	virtual void on_button(UI_BUTTON hBtn) {};
	virtual void refresh_05() {};
	virtual void refresh_1s() {};
	void dropWindow();
	void buttonPreset(UI_BUTTON btn);
	virtual void createWindow();
    void drawXYZ();
	UI_BUTTON create96x80Button(int x, int y, const char * picture);
	void updateButton(UI_BUTTON button, const char * picture, const char * title);
	void setButtonText(UI_BUTTON btn, char * text);
	UI_TEXT createText(int x, int y, int w, int h, const char * value);
	UI_TEXT createTextF(int x, int y, int w, int h, int flags, const char *text);
	void setText(UI_TEXT hText, char * text);
public:
	Widget(DISP_STATE id) {this->id=id;};
	bool is_active();
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
