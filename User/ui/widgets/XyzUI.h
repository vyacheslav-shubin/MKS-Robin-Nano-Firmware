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
	UI_BUTTON xp;
	UI_BUTTON xm;
	UI_BUTTON yp;
	UI_BUTTON ym;
	UI_BUTTON zp;
	UI_BUTTON zm;
	UI_BUTTON back;
	UI_BUTTON step;
} XYZ_UI_CONTROLS;


class XyzUI: public StdWidget {
protected:
	XYZ_UI_CONTROLS ui;
	char step = 0;
	char step_count = 0;
	virtual void on_button(UI_BUTTON hBtn);
	virtual void createControls();
	virtual void updateButtons();
	virtual UI_STEP_INFO * getStepInfo() = 0;
	virtual char * getStateString() {return 0;};
	virtual void refresh_1s();
public:
	XyzUI(DISP_STATE id) : StdWidget(id) {};
};

#endif /* USER_UI_WIDGETS_XYZUI_H_ */
