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
#include "wifi_module.h"

class Application {
public:
	Widget * current_ui = 0;
	float storedFeedrate = 0;
	void drawTitle();
	char * getTitle();
	void start();
	void loop();
	void dropPreview();
	void startPrintFile(unsigned char savedPreview=0);
	void drawLogo();
	void back_ui();
	void closeCurrentWidget();
	void push(Widget * widget);
	void showMainWidget();
	void reset_stack(Widget * widget);
	Application() {};
	virtual ~Application() {};
};

extern Application ui_app;

#endif /* USER_UI_WIDGETS_APPLICATION_H_ */
