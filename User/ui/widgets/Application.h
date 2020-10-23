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
#define F_UI_TIMING_QUARTER_SEC		1<<2
#define F_UI_TIMING_HALF_SEC		1<<0
#define F_UI_TIMING_SEC				1<<1

#define is_ui_timing(FLAG) (ui_timing_flags & FLAG)
#define ui_timing_set(FLAG) (ui_timing_flags |= FLAG)
#define ui_timing_clear(FLAG) (ui_timing_flags &= (~FLAG))

class Widget;
class ProgressDialogUI;

typedef struct {
    unsigned char count;
} APP_BEEPER;


#define TEMP_STAT_COUNT     120

typedef struct {
    unsigned char ext0[TEMP_STAT_COUNT];
    unsigned char ext1[TEMP_STAT_COUNT];
    unsigned char bed[TEMP_STAT_COUNT];
    unsigned char cursor;
    unsigned char loop;
} TEMPSTAT_DATA;


class TempStat {
public:
    TEMPSTAT_DATA data;
    void stat();
    TempStat() {memset(&this->data, 0, sizeof(this->data));};
};

extern TempStat tempStat;

#define POWER_DETECTOR_COUNT_DOWN   1

class PowerDetector{
private:
    unsigned char active = 0;
    unsigned char count_down = POWER_DETECTOR_COUNT_DOWN;
public:
    void init();
    void refresh_05();
    PowerDetector() {};
};

extern PowerDetector powerDetector;

class ProgressUI : public ActionDialogCallback {
private:
    Widget * stored_last_ui = 0;
public:
    void on_action_dialog(u8 action, u8 dialog_id);
    void progress(const char * message, char progress);
	void complete(const char * message);
    void fail(const char * message);
    void done();
    ProgressUI() {};
    virtual ~ProgressUI() {};
};

class Application : public ActionDialogCallback {
private:
    APP_BEEPER beeper = {0};
    volatile u32 screenOffCountDown = 255;
	volatile u8 waitPenUp = 0;
    Widget * stored_last_ui;
    friend ProgressUI;
public:
	Widget * current_ui = 0;
	float storedFeedrate = 0;
	void drawTitle();
	const char * getTitle();
	void beep(unsigned char seconds) {this->beeper.count = seconds;};
	void start();
	void setup();
	void idle();
	void before_loop();
	void loop();
	void refresh();
	void refresh_05();
	void refresh_1s();
	void systick();
    char touch(u8 action);
	void defaultUI();
	void dropPreview();
    void startPrintFile(unsigned char savedPreview=0);
    void continuePrintFile(unsigned char reread_state_from_file = false);
	void terminatePrintFile();
	void drawLogo();
	void back_ui();
	void closeCurrentWidget();
	void push(Widget * widget);
	void pop();
	void showMainWidget();
	void resetStack(Widget * widget);

    void on_action_dialog(u8 action, u8 dialog_id);

    void power_off_dialog(unsigned short duration);

	Application() {};
	virtual ~Application() {};
};

extern Application ui_app;
extern ProgressUI progress_ui;

#endif /* USER_UI_WIDGETS_APPLICATION_H_ */
