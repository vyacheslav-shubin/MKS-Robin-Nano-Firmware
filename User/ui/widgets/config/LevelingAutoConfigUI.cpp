//
// Created by shubin on 29.05.2020.
//

#include "LevelingAutoConfigUI.h"
#include "CalculatorDialogUI.h"
#include "mks_cfg.h"
#include "configuration_store.h"


LevelingAutoConfigUI leveling_auto_config_ui;

typedef enum {
    OFFSET_X=0,
    OFFSET_Y,
    OFFSET_Z,
    SPEED_XY,
    SPEED_Z_FAST,
    SPEED_Z_SLOW,
} VALUES;

static void _set_value(unsigned char index, double value) {
    switch(index) {
        case OFFSET_X: {
            mksCfg.x_probe_offset_from_extruder = value;
            epr_write_data(EPR_X_PROBE_OFFSET_FROM_EXTRUDER, (uint8_t *)&mksCfg.x_probe_offset_from_extruder, sizeof(float));
            break;
        }
        case OFFSET_Y: {
            mksCfg.y_probe_offset_from_extruder = value;
            epr_write_data(EPR_Y_PROBE_OFFSET_FROM_EXTRUDER, (uint8_t *)&mksCfg.y_probe_offset_from_extruder, sizeof(float));
            break;
        }
        case OFFSET_Z: {
            zprobe_zoffset = value;
            settings.save();
            break;
        }
        case SPEED_XY: {
            mksCfg.xy_probe_speed = value;
            epr_write_data(EPR_XY_PROBE_SPEED, (uint8_t *)&mksCfg.xy_probe_speed,sizeof(float));
            break;
        }
        case SPEED_Z_FAST: {
            mksCfg.z_probe_speed_fast = value;
            epr_write_data(EPR_Z_PROBE_SPEED_FAST, (uint8_t *)&mksCfg.z_probe_speed_fast,sizeof(float));
            break;
        }
        case SPEED_Z_SLOW: {
            mksCfg.z_probe_speed_slow = value;
            epr_write_data(EPR_Z_PROBE_SPEED_SLOW, (uint8_t *)&mksCfg.z_probe_speed_slow,sizeof(float));
            break;
        }
    }
}

void LevelingAutoConfigUI::on_button(UI_BUTTON hBtn) {
    if (hBtn==this->ui.probe.x_offset.button) {
        this->hide();
        strcpy(ui_buf1_100, lang_str.config_ui.probe_offset);
        strcat(ui_buf1_100, " X:");
        calculator_dialog_ui.show(ui_buf1_100, mksCfg.x_probe_offset_from_extruder, OFFSET_X, this, this);
    } else if (hBtn==this->ui.probe.y_offset.button) {
        this->hide();
        strcpy(ui_buf1_100, lang_str.config_ui.probe_offset);
        strcat(ui_buf1_100, " Y:");
        calculator_dialog_ui.show(ui_buf1_100, mksCfg.y_probe_offset_from_extruder, OFFSET_Y, this, this);
    } else if (hBtn==this->ui.probe.z_offset.button) {
        this->hide();
        strcpy(ui_buf1_100, lang_str.config_ui.probe_offset);
        strcat(ui_buf1_100, " Z:");
        calculator_dialog_ui.show(ui_buf1_100, zprobe_zoffset, OFFSET_Z, this, this);
    } else if (hBtn==this->ui.probe.xy_speed.button) {
        this->hide();
        strcpy(ui_buf1_100, lang_str.config_ui.probe_speed);
        strcat(ui_buf1_100, " XY:");
        calculator_dialog_ui.show(ui_buf1_100, mksCfg.xy_probe_speed, SPEED_XY, this, this);
    } else if (hBtn==this->ui.probe.z_speed_fast.button) {
        this->hide();
        strcpy(ui_buf1_100, lang_str.config_ui.probe_speed);
        strcat(ui_buf1_100, " Z/1:");
        calculator_dialog_ui.show(ui_buf1_100, mksCfg.z_probe_speed_fast, SPEED_Z_FAST, this, this);
    } else if (hBtn==this->ui.probe.z_speed_slow.button) {
        this->hide();
        strcpy(ui_buf1_100, lang_str.config_ui.probe_speed);
        strcat(ui_buf1_100, " Z/2:");
        calculator_dialog_ui.show(ui_buf1_100, mksCfg.z_probe_speed_slow, SPEED_Z_SLOW, this, this);
    } else if (hBtn==this->ui.autoleveling_enable.button) {
        mksCfg.bed_leveling_method = mksCfg.bed_leveling_method==8 ? 0 : 3;
        gCfgItems.leveling_mode = mksCfg.bed_leveling_method ? 1 : 0;
        epr_write_data(EPR_BED_LEVELING_METHOD,&mksCfg.bed_leveling_method,1);
        mksCfg.bed_leveling_method = 1<<mksCfg.bed_leveling_method;
        this->updateCheckButton(ui.autoleveling_enable.button, mksCfg.bed_leveling_method==8);
    } else if (hBtn==this->ui.probe.enable.button) {
        mksCfg.mkstouch = mksCfg.mkstouch ? 0 : 1;
        if (mksCfg.mkstouch)  {
            if (mksCfg.bed_leveling_method != NULL_BED_LEVELING) {
                mksCfg.z_min_probe_endstop_inverting = 0;
                if(mksCfg.z_min_probe_pin_mode == 1)	//ZMIN
                    mksCfg.z_min_endstop_inverting = 0;
                else if(mksCfg.z_min_probe_pin_mode == 2)	//ZMAX
                    mksCfg.z_max_endstop_inverting = 0;
            }
        }
        epr_write_data(EPR_MKSTOUCH,&mksCfg.mkstouch,1);
        this->updateCheckButton(ui.probe.enable.button, mksCfg.mkstouch);
    } else if (hBtn==this->ui.probe.connector.button) {
        mksCfg.z_min_probe_pin_mode = mksCfg.z_min_probe_pin_mode==1 ? 2 : 1;
        epr_write_data(EPR_Z_MIN_PROBE_PIN_MODE,&mksCfg.z_min_probe_pin_mode,1);
        this->updateCheckButton(this->ui.probe.connector.button, mksCfg.z_min_probe_pin_mode!=1, &lang_str.min_max);
    } else {
        if (hBtn==this->ui.probe.x_offset.dflt) {
            _set_value(OFFSET_X, 0);
        } else if (hBtn==this->ui.probe.y_offset.dflt) {
            _set_value(OFFSET_Y, 0);
        } else if (hBtn==this->ui.probe.z_offset.dflt) {
            _set_value(OFFSET_Z, 0);
        } else if (hBtn==this->ui.probe.xy_speed.dflt) {
            _set_value(SPEED_XY, 4000);
        } else if (hBtn==this->ui.probe.z_speed_fast.dflt) {
            _set_value(SPEED_Z_FAST, 600);
        } else if (hBtn==this->ui.probe.z_speed_slow.dflt) {
            _set_value(SPEED_Z_SLOW, 300);
        } else {
            ConfigurationWidget::on_button(hBtn);
            return;
        }
        this->updateControls();
    }
}

void LevelingAutoConfigUI::createControls() {
    ConfigurationWidget::createControls();
    this->dual_columns = 1;
    memset(&this->ui, 0, sizeof(this->ui));
    if (page==0) {
        this->createCheckPair(0, 0, &this->ui.autoleveling_enable, lang_str.config_ui.auto_leveling_settings,
                              (unsigned char) (mksCfg.bed_leveling_method == 8));
        this->createCheckPair(0, 2, &this->ui.probe.enable, "BLtouch", mksCfg.mkstouch);
        //todo: проверить значение
        this->createCheckPair(0, 3, &this->ui.probe.connector, lang_str.config_ui.connectorZ, mksCfg.z_min_probe_pin_mode != 1,
                              &lang_str.min_max);

        this->ui.probe.offset_label = this->createLabel(1, 0, lang_str.config_ui.probe_offset);
        this->createInputWithDefault(1, 1, &this->ui.probe.x_offset, "X", 0, 0);
        this->createInputWithDefault(1, 2, &this->ui.probe.y_offset, "Y", 0, 0);
        this->createInputWithDefault(1, 3, &this->ui.probe.z_offset, "Z", 0, 0);

    } else {
        this->ui.probe.offset_label = this->createLabel(0, 0, lang_str.config_ui.probe_speed);
        this->createInputWithDefault(0, 1, &this->ui.probe.xy_speed, "XY", 0, 0);
        this->createInputWithDefault(0, 2, &this->ui.probe.z_speed_fast, "Z/1", 0, 0);
        this->createInputWithDefault(0, 3, &this->ui.probe.z_speed_slow, "Z/2", 0, 0);
    }
    this->updateControls();
}

void LevelingAutoConfigUI::updateControls() {
    if (this->ui.probe.x_offset.button!=0) {
        sprintf(ui_buf1_100, "%.1f", mksCfg.x_probe_offset_from_extruder);
        this->setButtonText(this->ui.probe.x_offset.button, ui_buf1_100);
    }
    if (this->ui.probe.y_offset.button!=0) {
        sprintf(ui_buf1_100, "%.1f", mksCfg.y_probe_offset_from_extruder);
        this->setButtonText(this->ui.probe.y_offset.button, ui_buf1_100);
    }
    if (this->ui.probe.z_offset.button!=0) {
        sprintf(ui_buf1_100, "%.1f", zprobe_zoffset);
        this->setButtonText(this->ui.probe.z_offset.button, ui_buf1_100);
    }
    if (this->ui.probe.xy_speed.button!=0) {
        sprintf(ui_buf1_100, "%.1f", mksCfg.xy_probe_speed);
        this->setButtonText(this->ui.probe.xy_speed.button, ui_buf1_100);
    }
    if (this->ui.probe.z_speed_fast.button != 0) {
        sprintf(ui_buf1_100, "%.1f", mksCfg.z_probe_speed_fast);
        this->setButtonText(this->ui.probe.z_speed_fast.button, ui_buf1_100);
    }
    if (this->ui.probe.z_speed_slow.button != 0) {
        sprintf(ui_buf1_100, "%.1f", mksCfg.z_probe_speed_slow);
        this->setButtonText(this->ui.probe.z_speed_slow.button, ui_buf1_100);
    }
}

void LevelingAutoConfigUI::on_calculator(unsigned char action, double result, unsigned char dialog_id) {
    calculator_dialog_ui.hide();
    if (action==UI_BUTTON_OK)
        _set_value(dialog_id, result);
    this->show();
}

