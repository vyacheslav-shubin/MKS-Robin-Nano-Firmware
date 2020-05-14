/*
 * PreheatUI.h
 *
 *  Created on: May 12, 2020
 *      Author: shubin
 */

#ifndef USER_UI_WIDGETS_PREHEATUI_H_
#define USER_UI_WIDGETS_PREHEATUI_H_

#include "StdWidget.h"

typedef struct{
	UI_BUTTON plus;
	UI_BUTTON minus;
	UI_BUTTON selector;
	UI_BUTTON temp_step;
	UI_BUTTON off;
	UI_BUTTON preset;
	UI_BUTTON back;

	STATE_BUTTON ext1;
	STATE_BUTTON ext2;
	STATE_BUTTON bed;

} PREHEAT_UI_CONTROLS;

class PreheatUI: public StdWidget {
private:
	PREHEAT_UI_CONTROLS ui;
	char current_preset = 0;
	char current_temp_step = 0;
	char selector = 0;
	void doAddTemperature(char sign);
	void updateSelector();
	void updatePreset();
	void updateTempStep();
protected:
	virtual void createControls();
	virtual void on_button(UI_BUTTON hBtn);
	virtual void refresh_1s();
public:
	PreheatUI(): StdWidget(PRE_HEAT_UI) {};
};

extern PreheatUI preheat_ui;

#endif /* USER_UI_WIDGETS_PREHEATUI_H_ */
