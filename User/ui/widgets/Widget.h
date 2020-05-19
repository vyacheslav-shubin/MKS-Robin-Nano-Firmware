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

class Widget {
protected:
	DISP_STATE id;
	GUI_HWIN hWnd = 0;
	virtual void createControls() {};
	virtual void on_button(UI_BUTTON hBtn) {};
	virtual void refresh_05() {};
	virtual void refresh_1s() {};
	void dropWindow();
	void buttonPreset(UI_BUTTON btn);
	void initPage();
public:
	Widget(DISP_STATE id) {this->id=id;};
	bool is_active();
	virtual void on_message(WM_MESSAGE * pMsg);
	virtual ~Widget() {};
	virtual char * getTitle();
	virtual void show(Widget * caller = 0);
	virtual void recreate();
	virtual void refresh();
	virtual void hide();

	friend Application;
};

#endif /* USER_UI_WIDGETS_WIDGET_H_ */
