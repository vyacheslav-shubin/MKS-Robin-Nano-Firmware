//
// Created by shubin on 28.05.2020.
//

#include "HomeConfigUI.h"
#include "../dialog/CalculatorDialogUI.h"

HomeConfigUI home_config_ui;

typedef enum {
    SPEED_XY, SPEED_Z_FAST
} VALUES;

void HomeConfigUI::updateValues() {
    sprintf(ui_buf1_100,"%.1f",mksCfg.homing_feedrate_xy);
    this->setButtonText(this->ui.xy_speed.button, ui_buf1_100);
    sprintf(ui_buf1_100,"%.1f",mksCfg.homing_feedrate_z);
    this->setButtonText(this->ui.z_speed.button, ui_buf1_100);
}

static void _set_value(unsigned char id, float value) {
    switch (id) {
        case SPEED_XY:
            mksCfg.homing_feedrate_xy = value;
            epr_write_data(EPR_HOMING_FEEDRATE_XY,(uint8_t *)&value, sizeof(value));
            break;
        case SPEED_Z_FAST:
            mksCfg.homing_feedrate_z = value;
            epr_write_data(EPR_HOMING_FEEDRATE_Z,(uint8_t *)&value, sizeof(value));
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
        this->hide();
        strcpy(ui_buf1_100, lang_str.config_ui.speed);
        strcat(ui_buf1_100, " XY:");
        calculator_dialog_ui.show(ui_buf1_100, mksCfg.homing_feedrate_xy, SPEED_XY, this, this);
    } else if (hBtn==this->ui.z_speed.button) {
        this->hide();
        strcpy(ui_buf1_100, lang_str.config_ui.speed);
        strcat(ui_buf1_100, " Z:");
        calculator_dialog_ui.show("Z:", mksCfg.homing_feedrate_z, SPEED_Z_FAST, this, this);
    } else {
        if (hBtn == this->ui.z_speed.dflt) {
            _set_value(SPEED_Z_FAST, 1200);
        } else if (hBtn == this->ui.xy_speed.dflt) {
            _set_value(SPEED_XY, 2400);
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
    this->ui.placement = this->createLabel(0, 0, lang_str.config_ui.direction);
    this->ui.placement = this->createLabel(1, 0, lang_str.config_ui.speed);
    this->createCheckPair(0, 1, &this->ui.x_dir, "X", mksCfg.x_home_dir==1, &lang_str.min_max);
    this->createCheckPair(0, 2, &this->ui.y_dir, "Y", mksCfg.y_home_dir==1, &lang_str.min_max);
    this->createCheckPair(0, 3, &this->ui.z_dir, "Z", mksCfg.z_home_dir==1, &lang_str.min_max);

    this->createInputWithDefault(1, 1, &this->ui.xy_speed, "XY", 0);
    this->createInputWithDefault(1, 2, &this->ui.z_speed, "Z", 0);
    this->updateValues();
}

void HomeConfigUI::on_calculator(unsigned char action, double result, unsigned char dialog_id) {
    calculator_dialog_ui.hide();
    if (action == UI_BUTTON_OK)
        _set_value(dialog_id, result);
    this->show();
}
