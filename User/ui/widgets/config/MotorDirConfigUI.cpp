/*
 * MotorDirConfigUI.cpp
 *
 *  Created on: May 19, 2020
 *      Author: shubin
 */

#include "MotorDirConfigUI.h"
#include "stepper.h"

MotorDirConfigUI motor_dir_config_ui;

#define fix_stepper(axis) {stepper.last_direction_bits = ((stepper.last_direction_bits & (1<<axis)) == 0) ? stepper.last_direction_bits | (1<<axis) : stepper.last_direction_bits & (~(1<<axis));}


void MotorDirConfigUI::on_button(UI_BUTTON hBtn) {
	if (hBtn==this->ui.x.button) {
		mksCfg.invert_x_dir = mksCfg.invert_x_dir!=0 ? 0 : 1;
		epr_write_data(EPR_INVERT_X_DIR, &mksCfg.invert_x_dir,1);
		fix_stepper(X_AXIS);
		this->updateCheckButton(this->ui.x.button, mksCfg.invert_x_dir);
	} else if (hBtn==this->ui.y.button) {
		mksCfg.invert_y_dir = mksCfg.invert_y_dir!=0 ? 0 : 1;
		epr_write_data(EPR_INVERT_Y_DIR, &mksCfg.invert_y_dir,1);
		fix_stepper(Y_AXIS);
		this->updateCheckButton(this->ui.y.button, mksCfg.invert_y_dir);
	} else if (hBtn==this->ui.z.button) {
		mksCfg.invert_z_dir = mksCfg.invert_z_dir!=0 ? 0 : 1;
		epr_write_data(EPR_INVERT_Z_DIR, &mksCfg.invert_z_dir,1);
		fix_stepper(Z_AXIS);
		this->updateCheckButton(this->ui.z.button, mksCfg.invert_z_dir);
	} else if (hBtn==this->ui.e0.button) {
		mksCfg.invert_e0_dir = mksCfg.invert_e0_dir!=0 ? 0 : 1;
		epr_write_data(EPR_INVERT_E0_DIR, &mksCfg.invert_e0_dir,1);
		fix_stepper(E_AXIS);
		this->updateCheckButton(this->ui.e0.button, mksCfg.invert_e0_dir);
	} else if (hBtn==this->ui.e1.button) {
		mksCfg.invert_e1_dir = mksCfg.invert_e1_dir!=0 ? 0 : 1;
		epr_write_data(EPR_INVERT_E1_DIR, &mksCfg.invert_e1_dir,1);
		fix_stepper(E_AXIS);
		this->updateCheckButton(this->ui.e1.button, mksCfg.invert_e1_dir);
	} else {
		ConfigurationWidget::on_button(hBtn);
	}
}


void MotorDirConfigUI::createControls() {
	ConfigurationWidget::createControls();
	memset(&this->ui, 0, sizeof(this->ui));
    this->dual_columns = 1;
	this->createCheckPair(0, 0, &this->ui.x, "X:", mksCfg.invert_x_dir);
	this->createCheckPair(0, 1, &this->ui.y, "Y:", mksCfg.invert_y_dir);
	this->createCheckPair(0, 2, &this->ui.z, "Z:", mksCfg.invert_z_dir);
	this->createCheckPair(1, 0, &this->ui.e0, "E0:", mksCfg.invert_e0_dir);
	this->createCheckPair(1, 1, &this->ui.e1, "E1:", mksCfg.invert_e1_dir);

}


//ui_make_half_row_check_pair(0, 0, hMotorDirWnd, &ui.x, "X:", mksCfg.invert_x_dir);
//ui_make_half_row_check_pair(0, 1, hMotorDirWnd, &ui.y, "Y:", mksCfg.invert_y_dir);
//ui_make_half_row_check_pair(0, 2, hMotorDirWnd, &ui.z, "Z:", mksCfg.invert_z_dir);
//ui_make_half_row_check_pair(1, 0, hMotorDirWnd, &ui.e0, "E0:", mksCfg.invert_e0_dir);
//ui_make_half_row_check_pair(1, 1, hMotorDirWnd, &ui.e1, "E1:", mksCfg.invert_e1_dir);
