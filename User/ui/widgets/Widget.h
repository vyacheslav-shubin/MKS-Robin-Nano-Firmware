/*
 * Widget.h
 *
 *  Created on: May 8, 2020
 *      Author: shubin
 */

#ifndef USER_UI_WIDGETS_WIDGET_H_
#define USER_UI_WIDGETS_WIDGET_H_

#include "GUI.h"
#include "draw_ui.h"

class Application;

#include "Application.h"

class Widget {
protected:
	DISP_STATE id;
	GUI_HWIN hWnd = 0;
	virtual void createControls() {};
	virtual void on_button(WM_HWIN hBtn) {};
	virtual void refresh_05() {};
	virtual void refresh_1s() {};
public:
	Widget(DISP_STATE id) {this->id=id;};
	virtual void on_message(WM_MESSAGE * pMsg);
	virtual ~Widget() {};
	virtual char * getTitle();
	virtual void show(Widget * caller);
	void show() {this->show(0);};
	virtual void refresh();
	void hide();

	friend Application;
};

#endif /* USER_UI_WIDGETS_WIDGET_H_ */
