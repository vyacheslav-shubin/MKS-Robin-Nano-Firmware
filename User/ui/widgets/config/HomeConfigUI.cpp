//
// Created by shubin on 28.05.2020.
//

#include "HomeConfigUI.h"
#include "mks_reprint.h"
#include "../dialog/CalculatorDialogUI.h"

HomeConfigUI home_config_ui;

typedef enum {
    SPEED_XY, SPEED_Z_FAST, PAUSE_X, PAUSE_Y, PAUSE_dZ, PAUSE_dE
} VALUES;

void HomeConfigUI::updateValues() {
    if (this->ui.xy_speed.button!=0) {
        sprintf(ui_buf1_100, "%.1f", mksCfg.homing_feedrate_xy);
        this->setButtonText(this->ui.xy_speed.button, ui_buf1_100);
    }
    if (this->ui.z_speed.button!=0) {
        sprintf(ui_buf1_100, "%.1f", mksCfg.homing_feedrate_z);
        this->setButtonText(this->ui.z_speed.button, ui_buf1_100);
    }
    if (this->ui.px.button!=0) {
        sprintf(ui_buf1_100, "%.1f", mksCfg.filament_change_x_pos);
        this->setButtonText(this->ui.px.button, ui_buf1_100);
    }
    if (this->ui.py.button!=0) {
        sprintf(ui_buf1_100, "%.1f", mksCfg.filament_change_y_pos);
        this->setButtonText(this->ui.py.button, ui_buf1_100);
    }
    if (this->ui.pz.button!=0) {
        sprintf(ui_buf1_100, "%.1f", mksCfg.filament_change_z_add);
        this->setButtonText(this->ui.pz.button, ui_buf1_100);
    }
}

void HomeConfigUI::_setValue(unsigned char id, float value) {
    switch (id) {
        case SPEED_XY:
            mksCfg.homing_feedrate_xy = value;
            epr_write_data(EPR_HOMING_FEEDRATE_XY,(uint8_t *)&value, sizeof(value));
            break;
        case SPEED_Z_FAST:
            mksCfg.homing_feedrate_z = value;
            epr_write_data(EPR_HOMING_FEEDRATE_Z,(uint8_t *)&value, sizeof(value));
            break;
        case PAUSE_X:
            mksCfg.filament_change_x_pos = value;
            mksReprint.mks_pausePrint_x = mksCfg.filament_change_x_pos;
            epr_write_data(EPR_FILAMENT_CHANGE_X_POS,(uint8_t *)&value, sizeof(value));
            break;
        case PAUSE_Y:
            mksCfg.filament_change_y_pos = value;
            mksReprint.mks_pausePrint_y = mksCfg.filament_change_y_pos;
            epr_write_data(EPR_FILAMENT_CHANGE_Y_POS,(uint8_t *)&value, sizeof(value));
            break;
        case PAUSE_dZ:
            mksCfg.filament_change_z_add = value;
            mksReprint.mks_pausePrint_z = mksCfg.filament_change_z_add;
            epr_write_data(EPR_FILAMENT_CHANGE_Z_ADD,(uint8_t *)&value, sizeof(value));
            break;
    }
}


void HomeConfigUI::on_button(UI_BUTTON hBtn) {
    if (ui_is_double_button(hBtn, this->ui.x_dir)) {
        mksCfg.x_home_dir = (mksCfg.x_home_dir==1) ? -1 : 1;
        epr_write_data(EPR_X_HOME_DIR, (uint8_t *)&mksCfg.x_home_dir,1);
        this->updateCheckButton(this->ui.x_dir.button, mksCfg.x_home_dir==1, &lang_str.min_max);
    } else if (ui_is_double_button(hBtn, this->ui.y_dir)) {
        mksCfg.y_home_dir = (mksCfg.y_home_dir==1) ? -1 : 1;
        epr_write_data(EPR_Y_HOME_DIR, (uint8_t *)&mksCfg.y_home_dir,1);
        this->updateCheckButton(this->ui.y_dir.button, mksCfg.y_home_dir==1, &lang_str.min_max);
    }if (ui_is_double_button(hBtn, this->ui.z_dir)) {
        mksCfg.z_home_dir = (mksCfg.z_home_dir==1) ? -1 : 1;
        epr_write_data(EPR_Z_HOME_DIR, (uint8_t *)&mksCfg.z_home_dir,1);
        this->updateCheckButton(this->ui.z_dir.button, mksCfg.z_home_dir==1, &lang_str.min_max);
    } else if (hBtn==this->ui.xy_speed.button) {
        this->calculator(lang_str.config_ui.speed, " XY:", mksCfg.homing_feedrate_xy, SPEED_XY);
    } else if (hBtn==this->ui.z_speed.button) {
        this->calculator(lang_str.config_ui.speed, " Z:", mksCfg.homing_feedrate_z, SPEED_Z_FAST);
    } else if (hBtn==this->ui.px.button) {
        this->calculator(lang_str.config_ui.pause, " X:", mksCfg.filament_change_x_pos, PAUSE_X);
    } else if (hBtn==this->ui.py.button) {
        this->calculator(lang_str.config_ui.pause, " Y:", mksCfg.filament_change_y_pos, PAUSE_Y);
    } else if (hBtn==this->ui.pz.button) {
        this->calculator(lang_str.config_ui.pause, " +Z:", mksCfg.filament_change_z_add, PAUSE_dZ);
    } else {
        if (hBtn == this->ui.z_speed.dflt) {
            this->_setValue(SPEED_Z_FAST, 1200);
        } else if (hBtn == this->ui.xy_speed.dflt) {
            this->_setValue(SPEED_XY, 2400);
        } else if (hBtn == this->ui.px.dflt) {
            this->_setValue(PAUSE_X, 5);
        } else if (hBtn == this->ui.py.dflt) {
            this->_setValue(PAUSE_Y, 5);
        } else if (hBtn == this->ui.pz.dflt) {
            this->_setValue(PAUSE_dZ, 5);
        } else {
            ConfigurationWidget::on_button(hBtn);
            return;
        }
        this->updateValues();
    }
}

void HomeConfigUI::createControls() {
    ConfigurationWidget::createControls();
    memset(&this->ui, 0, sizeof(this->ui));
    this->dual_columns = 1;
    if (page==0) {
        this->ui.placement = this->createLabel(0, 0, lang_str.config_ui.direction);
        this->ui.speed = this->createLabel(1, 0, lang_str.config_ui.speed);
        this->createCheckPair(0, 1, &this->ui.x_dir, "X", mksCfg.x_home_dir == 1, &lang_str.min_max);
        this->createCheckPair(0, 2, &this->ui.y_dir, "Y", mksCfg.y_home_dir == 1, &lang_str.min_max);
        this->createCheckPair(0, 3, &this->ui.z_dir, "Z", mksCfg.z_home_dir == 1, &lang_str.min_max);

        this->createInputWithDefault(1, 1, &this->ui.xy_speed, "XY", 0);
        this->createInputWithDefault(1, 2, &this->ui.z_speed, "Z", 0);
    } else if (page==1) {
        this->ui.pause = this->createLabel(0, 0, lang_str.config_ui.pause);
        this->createInputWithDefault(0, 1, &this->ui.px, "X", 0, 0);
        this->createInputWithDefault(0, 2, &this->ui.py, "Y", 0, 0);
        this->createInputWithDefault(0, 3, &this->ui.pz, "+Z", 0, 0);
    }
    this->updateValues();
}
