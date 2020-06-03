//
// Created by shubin on 29.05.2020.
//

#include "MachineTypeConfigUI.h"
#include "Configuration.h"
#include "integration.h"

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

void MachineTypeConfigUI::_setValue(unsigned char value_id, float value) {
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


void MachineTypeConfigUI::on_button(UI_BUTTON hBtn) {
    switch (page) {
        case 0: {
            if (ui_is_double_button(hBtn, this->ui.mech.types[0])) {
                mksCfg.machinetype = 0;
            } else if (ui_is_double_button(hBtn, this->ui.mech.types[1])) {
                mksCfg.machinetype = 1;
            } else if (ui_is_double_button(hBtn, this->ui.mech.types[2])) {
                mksCfg.machinetype = 2;
            } else {
                ConfigurationWidget::on_button(hBtn);
                return;
            }
            epr_write_data(EPR_MACHINETPYE, (unsigned char *) &mksCfg.machinetype, sizeof(mksCfg.machinetype));
            mksCfg.machinetype = (1 << mksCfg.machinetype);
            shUI::loadConfig();
            this->updateControls();
            break;
        }
        case 1: {
            if (hBtn==this->ui.size.xmax.button) {
                this->calculator(labels.x_max, mksCfg.x_max_pos, X_MAX);
            } else if (hBtn==this->ui.size.ymax.button) {
                this->calculator(labels.y_max, mksCfg.y_max_pos, Y_MAX);
            } else if (hBtn==this->ui.size.zmax.button) {
                this->calculator(labels.z_max, mksCfg.z_max_pos, Z_MAX);
            } else if (hBtn==this->ui.size.xmin.button) {
                this->calculator(labels.x_min, mksCfg.x_min_pos, X_MIN);
            } else if (hBtn==this->ui.size.ymin.button) {
                this->calculator(labels.y_min, mksCfg.y_min_pos, Y_MIN);
            } else if (hBtn==this->ui.size.zmin.button) {
                this->calculator(labels.z_min, mksCfg.z_min_pos, Z_MIN);
            } else {
                if (hBtn==this->ui.size.xmax.dflt) {
                    this->setValue(X_MAX, 200);
                } else if (hBtn==this->ui.size.ymax.dflt) {
                    this->setValue(Y_MAX, 200);
                } else if (hBtn==this->ui.size.zmax.dflt) {
                    this->setValue(Z_MAX, 200);
                } else if (hBtn==this->ui.size.xmin.dflt) {
                    this->setValue(X_MIN, 0);
                } else if (hBtn==this->ui.size.ymin.dflt) {
                    this->setValue(Y_MIN, 0);
                } else if (hBtn==this->ui.size.zmin.dflt) {
                    this->setValue(Z_MIN, 0);
                } else {
                    ConfigurationWidget::on_button(hBtn);
                    break;
                }
                this->updateControls();
            }
            break;
        }
    }
}

void MachineTypeConfigUI::updateControls() {
    switch (page) {
        case 0: {
            this->updateRadio(this->ui.mech.types[0].button, (mksCfg.machinetype != 2) && (mksCfg.machinetype != 4));
            this->updateRadio(this->ui.mech.types[1].button, mksCfg.machinetype == 2);
            this->updateRadio(this->ui.mech.types[2].button, mksCfg.machinetype == 4);
            break;
        }
        case 1: {
            sprintf(ui_buf1_100,"%.1f",mksCfg.x_min_pos);
            this->setButtonText(this->ui.size.xmin.button, ui_buf1_100);
            sprintf(ui_buf1_100,"%.1f",mksCfg.y_min_pos);
            this->setButtonText(this->ui.size.ymin.button, ui_buf1_100);
            sprintf(ui_buf1_100,"%.1f",mksCfg.z_min_pos);
            this->setButtonText(this->ui.size.zmin.button, ui_buf1_100);

            sprintf(ui_buf1_100,"%.1f",mksCfg.x_max_pos);
            this->setButtonText(this->ui.size.xmax.button, ui_buf1_100);
            sprintf(ui_buf1_100,"%.1f",mksCfg.y_max_pos);
            this->setButtonText(this->ui.size.ymax.button, ui_buf1_100);
            sprintf(ui_buf1_100,"%.1f",mksCfg.z_max_pos);
            this->setButtonText(this->ui.size.zmax.button, ui_buf1_100);
        }
    }
}


void MachineTypeConfigUI::createControls() {
    ConfigurationWidget::createControls();
    this->dual_columns = 1;
    memset(&this->ui, 0, sizeof(this->ui));
    switch (page) {
        case 0: {
            this->createRadio(0, 0, &this->ui.mech.types[0], "XYZ", (mksCfg.machinetype!=2) && (mksCfg.machinetype!=4));
            this->createRadio(0, 1, &this->ui.mech.types[1], "Delta", mksCfg.machinetype==2);
            this->createRadio(0, 2, &this->ui.mech.types[2], "CoreXY", mksCfg.machinetype==4);
            break;
        }
        case 1: {
            this->createInputWithDefault(0, 0, &this->ui.size.xmin, labels.x_min, 0);
            this->createInputWithDefault(1, 0, &this->ui.size.xmax, labels.x_max, 0);
            this->createInputWithDefault(0, 1, &this->ui.size.ymin, labels.y_min,  0);
            this->createInputWithDefault(1, 1, &this->ui.size.ymax, labels.y_max,  0);
            this->createInputWithDefault(0, 2, &this->ui.size.zmin, labels.z_min,  0);
            this->createInputWithDefault(1, 2, &this->ui.size.zmax, labels.z_max,  0);
            this->updateControls();
            break;
        }
    }
}

MachineTypeConfigUI machine_type_config_ui;
