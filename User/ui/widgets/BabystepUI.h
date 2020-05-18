/*
 * BabystepUI.h
 *
 *  Created on: May 12, 2020
 *      Author: shubin
 */

#ifndef USER_UI_WIDGETS_BABYSTEPUI_H_
#define USER_UI_WIDGETS_BABYSTEPUI_H_

#include "XyzUI.h"

class BabystepUI: public XyzUI {
private:
	char _has_adjust_z=0;
	float bs_x = 0;
	float bs_y = 0;
protected:
	virtual void createControls();
	virtual UI_STEP_INFO * getStepInfo();
	virtual void on_button(UI_BUTTON hBtn);
	virtual char * getStateString();
public:
	virtual char * getTitle() {return lang_str.ui_title_babysteps;};
	BabystepUI() : XyzUI(BABY_STEP_UI) {this->step_count = 3;} ;
};

extern BabystepUI babystep_ui;

#endif /* USER_UI_WIDGETS_BABYSTEPUI_H_ */
