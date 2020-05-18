/*
 * MotorMoveUI.h
 *
 *  Created on: May 12, 2020
 *      Author: shubin
 */

#ifndef USER_UI_WIDGETS_MOTORMOVEUI_H_
#define USER_UI_WIDGETS_MOTORMOVEUI_H_

#include "XyzUI.h"

class MotorMoveUI : public XyzUI {
private:
	void doMoveMotor(int direction, char* axe);
protected:
	virtual void createControls();
	virtual UI_STEP_INFO * getStepInfo();
	virtual void on_button(UI_BUTTON hBtn);
	virtual char * getStateString();
public:
	virtual char * getTitle() {return lang_str.ui_title_move;};
	MotorMoveUI() : XyzUI(MOVE_MOTOR_UI) {this->step_count = 4;};
};

extern MotorMoveUI motor_move_ui;

#endif /* USER_UI_WIDGETS_MOTORMOVEUI_H_ */
