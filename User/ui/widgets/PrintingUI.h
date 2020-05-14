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
	UI_BUTTON pause;
	UI_BUTTON stop;
	UI_BUTTON tools;
	UI_BUTTON power_control;
	UI_PROGRESS_BAR progress;
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
	virtual void on_button(UI_BUTTON hBtn);
public:
	virtual void refresh();
	PrintingUI() : StdWidget(PRINTING_UI) {};
};

extern PrintingUI printing_ui;


#endif /* USER_UI_WIDGETS_PRINTINGUI_H_ */