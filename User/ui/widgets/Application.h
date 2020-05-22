/*
 * Application.h
 *
 *  Created on: May 8, 2020
 *      Author: shubin
 */

#ifndef USER_UI_WIDGETS_APPLICATION_H_
#define USER_UI_WIDGETS_APPLICATION_H_

#include "Widget.h"
#include "wifi_module.h"

extern volatile u8 ui_timing_flags;
#define F_UI_TIMING_HALF_SEC		1<<0
#define F_UI_TIMING_SEC				1<<1

#define is_ui_timing(FLAG) (ui_timing_flags & FLAG)
#define ui_timing_set(FLAG) (ui_timing_flags |= FLAG)
#define ui_timing_clear(FLAG) (ui_timing_flags &= (~FLAG))

class Widget;

class Application {
private:
	volatile u32 screenOffCountDown = 255;
	volatile u8 waitPenUp = 0;
public:
	Widget * current_ui = 0;
	float storedFeedrate = 0;
	void drawTitle();
	const char * getTitle();
	void start();
	void setup();
	void loop();
	void refresh();
	void refresh_05();
	void refresh_1s();
	void systick();
	char touch(u8 action);
	void defaultUI();
	void dropPreview();
	void startPrintFile(unsigned char savedPreview=0);
	void terminatePrintFile();
	void drawLogo();
	void back_ui();
	void closeCurrentWidget();
	void push(Widget * widget);
	void pop();
	void showMainWidget();
	void reset_stack(Widget * widget);
	Application() {};
	virtual ~Application() {};
};

extern Application ui_app;

#endif /* USER_UI_WIDGETS_APPLICATION_H_ */
