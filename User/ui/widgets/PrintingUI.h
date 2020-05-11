/*
 * PrintingUI.h
 *
 *  Created on: May 10, 2020
 *      Author: shubin
 */

#ifndef USER_UI_WIDGETS_PRINTINGUI_H_
#define USER_UI_WIDGETS_PRINTINGUI_H_

#include "StdWidget.h"

typedef struct{
	STATE_BUTTON ext1;
	STATE_BUTTON ext2;
	STATE_BUTTON fan;
	STATE_BUTTON z;
	STATE_BUTTON bed;
	STATE_BUTTON time;
	BUTTON_Handle pause;
	BUTTON_Handle stop;
	BUTTON_Handle tools;
	BUTTON_Handle power_control;
	PROGBAR_Handle progress;
	char preview_done;
} PRINTING_UI_CONTROLS;

class PrintingUI : public StdWidget{
private:
	PRINTING_UI_CONTROLS ui;
	void createStateButtonAt(char col, char row, STATE_BUTTON * btn, const char * img, const char * title);
	void updatePowerControlButton();
	void updatePauseButton();
	void updateStateButtons();
	void updateProgress();
	void doFinishPrint();
protected:
	virtual void refresh_05();
	virtual void refresh_1s();
	virtual void createControls();
	virtual void on_button(WM_HWIN hBtn);
public:
	virtual void refresh();
	PrintingUI() : StdWidget(PRINTING_UI) {};
};

extern PrintingUI printing_ui;


#endif /* USER_UI_WIDGETS_PRINTINGUI_H_ */
