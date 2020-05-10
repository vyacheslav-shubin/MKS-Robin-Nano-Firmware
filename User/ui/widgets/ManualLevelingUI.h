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
	BUTTON_Handle leveling1;
	BUTTON_Handle leveling2;
	BUTTON_Handle leveling3;
	BUTTON_Handle leveling4;
	BUTTON_Handle leveling5;
	BUTTON_Handle home;
	BUTTON_Handle homeZ;
	BUTTON_Handle back;
} MANUAL_LEVELING_UI_CONTROLS;

class ManualLevelingUI: public StdWidget  {
private:
	MANUAL_LEVELING_UI_CONTROLS buttons;
protected:
	virtual void createControls();
	virtual void on_button(WM_HWIN hBtn);
	virtual void refresh_05();
public:
	ManualLevelingUI() : StdWidget(LEVELING_UI) {};
};

extern ManualLevelingUI manual_leveling_ui;

#endif /* USER_UI_WIDGETS_MANUALLEVELINGUI_H_ */
