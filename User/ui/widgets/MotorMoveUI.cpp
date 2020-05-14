/*
 * MotorMoveUI.cpp
 *
 *  Created on: May 12, 2020
 *      Author: shubin
 */

#include "MotorMoveUI.h"
#include "XyzUI.h"
#include "ui_tools.h"
#include "Marlin.h"
#include "Application.h"

MotorMoveUI motor_move_ui;

static UI_STEP_INFO motor_move_info[4] {
	{0.1, "0.1 mm", img_move_step_1},
	{1, "1 mm", img_move_step_2},
	{10, "10 mm", img_move_step_3},
	{50, "50 mm", img_move_step_huge}
};

void MotorMoveUI::doMoveMotor(int direction, char* axe) {
	UI_STEP_INFO * info = this->getStepInfo();
	enqueue_and_echo_commands_P(PSTR("G91"));
	sprintf(ui_buf1_80, "G1 %s%3.1f F%d\n", axe, info->size * direction, gCfgItems.moveSpeed);
	enqueue_and_echo_commands_P(PSTR(ui_buf1_80));
	enqueue_and_echo_commands_P(PSTR("G90"));
}


UI_STEP_INFO * MotorMoveUI::getStepInfo() {
	return &motor_move_info[this->step];
}

void MotorMoveUI::createControls() {
	XyzUI::createControls();
	ui_app.storedFeedrate = feedrate_mm_s;
}


void MotorMoveUI::on_button(UI_BUTTON hBtn) {
	UI_STEP_INFO *si = this->getStepInfo();
	if (hBtn==this->ui.xp) {
		this->doMoveMotor(1, "X");
	} else if(hBtn==this->ui.xm) {
		this->doMoveMotor(-1, "X");
	} else if(hBtn == this->ui.yp) {
		this->doMoveMotor(1, "Y");
	} else if(hBtn == this->ui.ym) {
		this->doMoveMotor(-1, "Y");
	} else if(hBtn == this->ui.zp) {
		this->doMoveMotor(1, "Z");
	} else if(hBtn == this->ui.zm) {
		this->doMoveMotor(-1, "Z");
	} else if (hBtn == this->ui.back) {
		feedrate_mm_s = ui_app.storedFeedrate;
		XyzUI::on_button(hBtn);
	} else
		XyzUI::on_button(hBtn);
}

char * MotorMoveUI::getStateString() {
	sprintf(ui_buf1_80,"X:%1.2f Y:%1.2f Z:%1.2f", current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS]);
	return ui_buf1_80;
}
