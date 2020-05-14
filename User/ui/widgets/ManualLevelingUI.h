/*
 * ManualLevelingUI.h
 *
 *  Created on: May 9, 2020
 *      Author: shubin
 */

#ifndef USER_UI_WIDGETS_MANUALLEVELINGUI_H_
#define USER_UI_WIDGETS_MANUALLEVELINGUI_H_

#include "StdWidget.h"

typedef struct {
	UI_BUTTON leveling1;
	UI_BUTTON leveling2;
	UI_BUTTON leveling3;
	UI_BUTTON leveling4;
	UI_BUTTON leveling5;
	UI_BUTTON home;
	UI_BUTTON homeZ;
	UI_BUTTON back;
} MANUAL_LEVELING_UI_CONTROLS;

class ManualLevelingUI: public StdWidget  {
private:
	MANUAL_LEVELING_UI_CONTROLS buttons;
protected:
	virtual void createControls();
	virtual void on_button(UI_BUTTON hBtn);
	virtual void refresh_05();
public:
	ManualLevelingUI() : StdWidget(LEVELING_UI) {};
};

extern ManualLevelingUI manual_leveling_ui;

#endif /* USER_UI_WIDGETS_MANUALLEVELINGUI_H_ */
