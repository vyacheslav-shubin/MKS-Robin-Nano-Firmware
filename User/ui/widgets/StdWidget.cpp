/*
 * StdWidget.cpp
 *
 *  Created on: May 8, 2020
 *      Author: shubin
 */

#include "StdWidget.h"
#include "integration.h"
#include "ui_tools.h"
#include "ManualLevelingUI.h"
#include "MeshLevelingUI.h"
#include "Application.h"
#include "Configuration.h"


void StdWidget::createStateButton(int x, int y, STATE_BUTTON * btn, const char * picture, const char * title) {
	btn->button = ui_create_state_button(x, y,this->hWnd, picture);
	btn->label = ui_create_std_text(x + STATE_PIC_X_PIXEL, y, 80, STATE_PIC_Y_PIXEL, this->hWnd, title);
}

void StdWidget::updateStateButton(STATE_BUTTON * btn, const char * img, const char * title) {
	if (img!=0)
		ui_update_state_button(btn->button, img);
	ui_set_text_value(btn->label, title);
}


UI_BUTTON StdWidget::createButtonAt(int phx, int phy, const char * picture, const char * title) {
	return this->createButton(ui_std_col(phx),  ui_std_row(phy), picture, title);
}

UI_BUTTON StdWidget::createButton(int x, int y, const char * picture, const char * title) {
	return ui_create_std_button(x, y, this->hWnd, picture, title);
}


UI_BUTTON StdWidget::createButtonRet() {
	return createButtonAt(3, 1, img_back, lang_str.back);
}


void  StdWidget::actionFilamentChangeParking() {
    sprintf(ui_buf1_100, "G91 G1 Z%.1f F%.1f", mksCfg.filament_change_z_add, mksCfg.homing_feedrate_z);
    shUI::pushGcode(ui_buf1_100);
    sprintf(ui_buf1_100, "G92 G1 X%.1f Y%.1f F%.1f", mksCfg.filament_change_x_pos, mksCfg.filament_change_y_pos, mksCfg.homing_feedrate_xy);
    shUI::pushGcode(ui_buf1_100);
}

void StdWidget::action_leveling() {
    switch (BED_LEVELING_METHOD) {
        case 0:
        case NULL_BED_LEVELING: {
            this->hide();
            manual_leveling_ui.show(this);
            break;
        }
        case MESH_BED_LEVELING: {
            this->hide();
            mesh_leveling_ui.show();
            break;
        }
        default: {
            shUI::doCustomLeveling();
            break;
        }
    }
}

void StdWidget::action_back() {
	this->hide();
	ui_app.back_ui();
}


void StdWidget::updateFanState(STATE_BUTTON * stateButton) {
	ui_update_fan_button(stateButton->button, stateButton->label);
}

