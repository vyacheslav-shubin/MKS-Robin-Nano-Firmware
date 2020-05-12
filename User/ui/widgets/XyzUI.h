/*
 * XyzUI.h
 *
 *  Created on: May 12, 2020
 *      Author: shubin
 */

#ifndef USER_UI_WIDGETS_XYZUI_H_
#define USER_UI_WIDGETS_XYZUI_H_

#include "StdWidget.h"

typedef struct {
	BUTTON_Handle xp;
	BUTTON_Handle xm;
	BUTTON_Handle yp;
	BUTTON_Handle ym;
	BUTTON_Handle zp;
	BUTTON_Handle zm;
	BUTTON_Handle back;
	BUTTON_Handle step;
} XYZ_UI_CONTROLS;

typedef struct {
	float size;
	const char * title;
	const char * picture;
} UI_STEP_INFO;


class XyzUI: public StdWidget {
protected:
	XYZ_UI_CONTROLS ui;
	char step = 0;
	char step_count = 0;
	virtual void on_button(WM_HWIN hBtn);
	virtual void createControls();
	virtual void updateButtons();
	virtual UI_STEP_INFO * getStepInfo() = 0;
	virtual char * getStateString() {return 0;};
	virtual void refresh_1s();
public:
	XyzUI(DISP_STATE id) : StdWidget(id) {};
};

#endif /* USER_UI_WIDGETS_XYZUI_H_ */
