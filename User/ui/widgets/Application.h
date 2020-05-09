/*
 * Application.h
 *
 *  Created on: May 8, 2020
 *      Author: shubin
 */

#ifndef USER_UI_WIDGETS_APPLICATION_H_
#define USER_UI_WIDGETS_APPLICATION_H_

class Widget;

#include "Widget.h"

class Application {
public:
	void back_ui();
	void push(Widget * widget);
	void showMainWidget();
	void reset_stack(Widget * widget);
	Application() {};
	virtual ~Application() {};
};

extern Application ui_app;

#endif /* USER_UI_WIDGETS_APPLICATION_H_ */
