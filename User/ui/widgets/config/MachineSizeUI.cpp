//
// Created by shubin on 26.05.2020.
//

#include "MachineSizeUI.h"
#include "Configuration.h"
#include "../dialog/CalculatorDialogUI.h"

MachineSizeUI machine_size_ui;

typedef struct {
    const char * x_min;
    const char * x_max;
    const char * y_min;
    const char * y_max;
    const char * z_min;
    const char * z_max;
} _LABELS;

const _LABELS labels = {
        "X min:",
        "X max:",
        "Y min:",
        "Y max:",
        "Z min:",
        "Z max:",
};

typedef enum{
    X_MAX, Y_MAX, Z_MAX, X_MIN, Y_MIN, Z_MIN
} VALUES;

inline static void _fix_size_values() {
    base_max_pos_P[0] = soft_endstop_max[0] = mksCfg.x_max_pos;
    base_max_pos_P[1] = soft_endstop_max[1] = mksCfg.y_max_pos;
    base_max_pos_P[2] = soft_endstop_max[2] = mksCfg.z_max_pos;

    base_min_pos_P[0] = soft_endstop_min[0] = mksCfg.x_min_pos;
    base_min_pos_P[1] = soft_endstop_min[1] = mksCfg.y_min_pos;
    base_min_pos_P[2] = soft_endstop_min[2] = mksCfg.z_min_pos;

    max_length_P[0] = mksCfg.x_max_pos - mksCfg.x_min_pos;
    max_length_P[1] = mksCfg.y_max_pos - mksCfg.y_min_pos;
    max_length_P[2] = mksCfg.z_max_pos - mksCfg.z_min_pos;

    if(MACHINETPYE != DELTA) {
        base_home_pos_P[0] = (mksCfg.x_home_dir < 0 ? mksCfg.x_min_pos : mksCfg.x_max_pos);
        base_home_pos_P[1] = (mksCfg.y_home_dir < 0 ? mksCfg.y_min_pos : mksCfg.y_max_pos);
        base_home_pos_P[2] = (mksCfg.z_home_dir < 0 ? mksCfg.z_min_pos : mksCfg.z_max_pos);
    }
}

void MachineSizeUI::_setValue(unsigned char value_id, float value) {
    switch (value_id) {
        case X_MAX:
            mksCfg.x_max_pos = value;
            epr_write_data(EPR_X_MAX_POS, (const unsigned char *)&value, sizeof(value));
            break;
        case Y_MAX:
            mksCfg.y_max_pos = value;
            epr_write_data(EPR_Y_MAX_POS, (const unsigned char *)&value, sizeof(value));
            break;
        case Z_MAX:
            mksCfg.z_max_pos = value;
            epr_write_data(EPR_Z_MAX_POS, (const unsigned char *)&value, sizeof(value));
            break;
        case X_MIN:
            mksCfg.x_min_pos = value;
            epr_write_data(EPR_X_MIN_POS, (const unsigned char *)&value, sizeof(value));
            break;
        case Y_MIN:
            mksCfg.y_min_pos = value;
            epr_write_data(EPR_Y_MIN_POS, (const unsigned char *)&value, sizeof(value));
            break;
        case Z_MIN:
            mksCfg.z_min_pos = value;
            epr_write_data(EPR_Z_MIN_POS, (const unsigned char *)&value, sizeof(value));
            break;
    }
    _fix_size_values();
}


void MachineSizeUI::on_button(UI_BUTTON hBtn) {
    if (hBtn==this->ui.xmax.button) {
        this->calculator(labels.x_max, mksCfg.x_max_pos, X_MAX);
    } else if (hBtn==this->ui.ymax.button) {
        this->calculator(labels.y_max, mksCfg.y_max_pos, Y_MAX);
    } else if (hBtn==this->ui.zmax.button) {
        this->calculator(labels.z_max, mksCfg.z_max_pos, Z_MAX);
    } else if (hBtn==this->ui.xmin.button) {
        this->calculator(labels.x_min, mksCfg.x_min_pos, X_MIN);
    } else if (hBtn==this->ui.ymin.button) {
        this->calculator(labels.y_min, mksCfg.y_min_pos, Y_MIN);
    } else if (hBtn==this->ui.zmin.button) {
        this->calculator(labels.z_min, mksCfg.z_min_pos, Z_MIN);
    } else {
        if (hBtn==this->ui.xmax.dflt) {
            this->setValue(X_MAX, 200);
        } else if (hBtn==this->ui.ymax.dflt) {
            this->setValue(Y_MAX, 200);
        } else if (hBtn==this->ui.zmax.dflt) {
            this->setValue(Z_MAX, 200);
        } else if (hBtn==this->ui.xmin.dflt) {
            this->setValue(X_MIN, 0);
        } else if (hBtn==this->ui.ymin.dflt) {
            this->setValue(Y_MIN, 0);
        } else if (hBtn==this->ui.zmin.dflt) {
            this->setValue(Z_MIN, 0);
        } else {
            ConfigurationWidget::on_button(hBtn);
            return;
        }
        this->updateValues();
    }
}


void MachineSizeUI::updateValues() {
    sprintf(ui_buf1_100,"%.1f",mksCfg.x_min_pos);
    this->setButtonText(this->ui.xmin.button, ui_buf1_100);
    sprintf(ui_buf1_100,"%.1f",mksCfg.y_min_pos);
    this->setButtonText(this->ui.ymin.button, ui_buf1_100);
    sprintf(ui_buf1_100,"%.1f",mksCfg.z_min_pos);
    this->setButtonText(this->ui.zmin.button, ui_buf1_100);

    sprintf(ui_buf1_100,"%.1f",mksCfg.x_max_pos);
    this->setButtonText(this->ui.xmax.button, ui_buf1_100);
    sprintf(ui_buf1_100,"%.1f",mksCfg.y_max_pos);
    this->setButtonText(this->ui.ymax.button, ui_buf1_100);
    sprintf(ui_buf1_100,"%.1f",mksCfg.z_max_pos);
    this->setButtonText(this->ui.zmax.button, ui_buf1_100);
}


void MachineSizeUI::createControls() {
    ConfigurationWidget::createControls();
    this->dual_columns = 1;
    this->createInputWithDefault(0, 0, &this->ui.xmin, labels.x_min, 0);
    this->createInputWithDefault(1, 0, &this->ui.xmax, labels.x_max, 0);
    this->createInputWithDefault(0, 1, &this->ui.ymin, labels.y_min,  0);
    this->createInputWithDefault(1, 1, &this->ui.ymax, labels.y_max,  0);
    this->createInputWithDefault(0, 2, &this->ui.zmin, labels.z_min,  0);
    this->createInputWithDefault(1, 2, &this->ui.zmax, labels.z_max,  0);
    this->updateValues();
}
