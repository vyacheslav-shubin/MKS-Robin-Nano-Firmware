//
// Created by shubin on 16.06.2020.
//

#include "LevelingConfigUI.h"
#include "Configuration.h"
#include "integration.h"

LevelingConfigUI leveling_config_ui;

typedef enum {
    OFFSET_X=0,
    OFFSET_Y,
    OFFSET_Z,
    SPEED_XY,
    SPEED_Z_FAST,
    SPEED_Z_SLOW,
	M_PROBING,
    POINT_X,
    POINT_Y,
} VALUES;

typedef enum {
} POINT_VALUES;

void LevelingConfigUI::_setValue(unsigned char id, float value) {
    switch(id) {
        case OFFSET_X: {
            mksCfg.x_probe_offset_from_extruder = value;
            epr_write_data(EPR_X_PROBE_OFFSET_FROM_EXTRUDER, (uint8_t *)&value, sizeof(float));
            break;
        }
        case OFFSET_Y: {
            mksCfg.y_probe_offset_from_extruder = value;
            epr_write_data(EPR_Y_PROBE_OFFSET_FROM_EXTRUDER, (uint8_t *)&value, sizeof(float));
            break;
        }
        case OFFSET_Z: {
            zprobe_zoffset = value;
            shUI::saveConfig();
            break;
        }
        case SPEED_XY: {
            mksCfg.xy_probe_speed = value;
            epr_write_data(EPR_XY_PROBE_SPEED, (uint8_t *)&value, sizeof(float));
            break;
        }
        case SPEED_Z_FAST: {
            mksCfg.z_probe_speed_fast = value;
            epr_write_data(EPR_Z_PROBE_SPEED_FAST, (uint8_t *)&value, sizeof(float));
            break;
        }
        case SPEED_Z_SLOW: {
            mksCfg.z_probe_speed_slow = value;
            epr_write_data(EPR_Z_PROBE_SPEED_SLOW, (uint8_t *)&value, sizeof(float));
            break;
        }
        case M_PROBING: {
            mksCfg.multiple_probing = value;
            epr_write_data(EPR_MULTILPE_PROBING, (uint8_t *)&value, 1);
            break;
        }
        default: {
            if (id>=POINT_X) {
                id-=POINT_X;
                ((u16 *) &gCfgItems.leveling_points)[id] = value;
                epr_write_data(EPR_LEVELING_POINTS + id * sizeof(u16), (unsigned char *) &value, sizeof(value));
            }
            break;
        }
    }
}

static void _set_leveling_type(char method) {
    mksCfg.bed_leveling_method = method;
    gCfgItems.leveling_mode = method ? 1 : 0;
    epr_write_data(EPR_LEVELING_MODE, (uint8_t *) &gCfgItems.leveling_mode, 1);
    epr_write_data(EPR_BED_LEVELING_METHOD, &mksCfg.bed_leveling_method, 1);
    mksCfg.bed_leveling_method = 1 << mksCfg.bed_leveling_method;
}

void LevelingConfigUI::updateControls() {
    switch(this->page) {
        case 0: {
            this->updateRadio(this->ui.type.none.button, (mksCfg.bed_leveling_method==1) || (mksCfg.bed_leveling_method==0));
            this->updateRadio(this->ui.type.point3.button, mksCfg.bed_leveling_method==AUTO_BED_LEVELING_3POINT);
            this->updateRadio(this->ui.type.linear.button,  mksCfg.bed_leveling_method==AUTO_BED_LEVELING_LINEAR);
            this->updateRadio(this->ui.type.bilinear.button, mksCfg.bed_leveling_method==AUTO_BED_LEVELING_BILINEAR);
            this->updateRadio(this->ui.type.ubl.button, mksCfg.bed_leveling_method==AUTO_BED_LEVELING_UBL);
            this->updateRadio(this->ui.type.mesh.button,  mksCfg.bed_leveling_method==MESH_BED_LEVELING);
            sprintf(ui_buf1_100, "%d", mksCfg.multiple_probing);
            this->setButtonText(this->ui.probe.multiple_probing.button, ui_buf1_100);
            break;
        }
        case 1: {
            sprintf(ui_buf1_100, "%.2f", mksCfg.x_probe_offset_from_extruder);
            this->setButtonText(this->ui.probe.x_offset.button, ui_buf1_100);
            sprintf(ui_buf1_100, "%.2f", mksCfg.y_probe_offset_from_extruder);
            this->setButtonText(this->ui.probe.y_offset.button, ui_buf1_100);
            sprintf(ui_buf1_100, "%.2f", zprobe_zoffset);
            this->setButtonText(this->ui.probe.z_offset.button, ui_buf1_100);
            sprintf(ui_buf1_100, "%.0f", mksCfg.xy_probe_speed);
            this->setButtonText(this->ui.probe.xy_speed.button, ui_buf1_100);
            sprintf(ui_buf1_100, "%.0f", mksCfg.z_probe_speed_fast);
            this->setButtonText(this->ui.probe.z_speed_fast.button, ui_buf1_100);
            sprintf(ui_buf1_100, "%.0f", mksCfg.z_probe_speed_slow);
            this->setButtonText(this->ui.probe.z_speed_slow.button, ui_buf1_100);
            break;
        }
        case 2: {
            for (int i=0;i<5;i++) {
                sprintf(ui_buf1_100, "%d", gCfgItems.leveling_points[i].x);
                this->setButtonText(this->ui.points.points[i].button1, ui_buf1_100);
                sprintf(ui_buf1_100, "%d", gCfgItems.leveling_points[i].y);
                this->setButtonText(this->ui.points.points[i].button2, ui_buf1_100);
            }
            break;
        }
    }
}

unsigned char LevelingConfigUI::checkPointsButtonSet(UI_BUTTON hBtn, unsigned char index) {
    if (hBtn==this->ui.points.points[index].button1) {
        sprintf(ui_buf1_100, lang_str.config_ui.point, index + 1);
        this->calculator(ui_buf1_100, ".X:", gCfgItems.leveling_points[index].x, POINT_X + index * 2);
    }if (hBtn==this->ui.points.points[index].button2) {
        sprintf(ui_buf1_100, lang_str.config_ui.point, index + 1);
        this->calculator(ui_buf1_100, ".Y:", gCfgItems.leveling_points[index].y, POINT_Y + index * 2);
    } else
        return 0;
    return 1;
}

void LevelingConfigUI::on_button(UI_BUTTON hBtn) {
    switch(this->page) {
        case 0: {
            if (ui_is_double_button(hBtn, this->ui.type.none)) {
                _set_leveling_type(0);
            } else if (ui_is_double_button(hBtn, this->ui.type.mesh)) {
                _set_leveling_type(5);
            } else if (ui_is_double_button(hBtn, this->ui.type.bilinear)) {
                _set_leveling_type(3);
            } else if (ui_is_double_button(hBtn, this->ui.type.linear)) {
                _set_leveling_type(2);
            } else if (ui_is_double_button(hBtn, this->ui.type.point3)) {
                _set_leveling_type(1);
            } else if (ui_is_double_button(hBtn, this->ui.type.ubl)) {
                _set_leveling_type(4);
            } else if (ui_is_double_button(hBtn, this->ui.type.mesh)) {
                _set_leveling_type(5);
            } else if (hBtn == this->ui.probe.multiple_probing.dflt) {
            	this->setValue(M_PROBING, 2);
            } else if (hBtn == this->ui.probe.multiple_probing.button) {
            	this->calculator(lang_str.config_ui.probe_count, mksCfg.multiple_probing, M_PROBING);
                return;
            } else if (hBtn==this->ui.probe.enable.button) {
                mksCfg.mkstouch = mksCfg.mkstouch ? 0 : 1;
                if (mksCfg.mkstouch) {
                    if (mksCfg.bed_leveling_method != NULL_BED_LEVELING) {
                        mksCfg.z_min_probe_endstop_inverting = 0;
                        if (mksCfg.z_min_probe_pin_mode == 1)    //ZMIN
                            mksCfg.z_min_endstop_inverting = 0;
                        else if (mksCfg.z_min_probe_pin_mode == 2)    //ZMAX
                            mksCfg.z_max_endstop_inverting = 0;
                    }
                }
                epr_write_data(EPR_MKSTOUCH, &mksCfg.mkstouch,1);
                this->updateCheckButton(ui.probe.enable.button, mksCfg.mkstouch);
                break;
            } else if (hBtn==this->ui.probe.connector.button) {
                mksCfg.z_min_probe_pin_mode = mksCfg.z_min_probe_pin_mode==1 ? 2 : 1;
                epr_write_data(EPR_Z_MIN_PROBE_PIN_MODE,&mksCfg.z_min_probe_pin_mode,1);
                this->updateCheckButton(this->ui.probe.connector.button, mksCfg.z_min_probe_pin_mode!=1, &lang_str.min_max);
                break;
            }
            this->updateControls();
            break;
        }
        case 1: {
            if (hBtn == this->ui.probe.x_offset.button) {
                this->calculator(lang_str.config_ui.probe_offset, " dX:", mksCfg.x_probe_offset_from_extruder,
                                 OFFSET_X);
            } else if (hBtn == this->ui.probe.y_offset.button) {
                this->calculator(lang_str.config_ui.probe_offset, " dY:", mksCfg.y_probe_offset_from_extruder,
                                 OFFSET_Y);
            } else if (hBtn == this->ui.probe.z_offset.button) {
                this->calculator(lang_str.config_ui.probe_offset, " dZ:", zprobe_zoffset, OFFSET_Z);
            } else if (hBtn == this->ui.probe.xy_speed.button) {
                this->calculator(lang_str.config_ui.probe_speed, " XY:", mksCfg.xy_probe_speed, SPEED_XY);
            } else if (hBtn == this->ui.probe.z_speed_fast.button) {
                this->calculator(lang_str.config_ui.probe_speed, " Z/1:", mksCfg.z_probe_speed_fast, SPEED_Z_FAST);
            } else if (hBtn == this->ui.probe.z_speed_slow.button) {
                this->calculator(lang_str.config_ui.probe_speed, " Z/2:", mksCfg.z_probe_speed_slow, SPEED_Z_SLOW);
            } else {
                if (hBtn == this->ui.probe.x_offset.dflt) {
                    this->_setValue(OFFSET_X, 0);
                } else if (hBtn == this->ui.probe.y_offset.dflt) {
                    this->_setValue(OFFSET_Y, 0);
                } else if (hBtn == this->ui.probe.z_offset.dflt) {
                    this->_setValue(OFFSET_Z, 0);
                } else if (hBtn == this->ui.probe.xy_speed.dflt) {
                    this->_setValue(SPEED_XY, 4000);
                } else if (hBtn == this->ui.probe.z_speed_fast.dflt) {
                    this->_setValue(SPEED_Z_FAST, 600);
                } else if (hBtn == this->ui.probe.z_speed_slow.dflt) {
                    this->_setValue(SPEED_Z_SLOW, 300);
                } else
                    break;
                this->updateControls();
            }
            break;
        }
        case 2: {
            for (unsigned char i=0;i<5;i++) {
                if (checkPointsButtonSet(hBtn, i))
                    return;
            }
            if (hBtn==this->ui.points.deflt) {
                gCfgItems.leveling_points[0].x = mksCfg.x_min_pos + 20;
                gCfgItems.leveling_points[0].y = mksCfg.y_min_pos + 20;

                gCfgItems.leveling_points[1].x = mksCfg.x_max_pos - 20;
                gCfgItems.leveling_points[1].y = mksCfg.y_min_pos + 20;

                gCfgItems.leveling_points[2].x = mksCfg.x_min_pos + 20;
                gCfgItems.leveling_points[2].y = mksCfg.y_max_pos - 20;

                gCfgItems.leveling_points[3].x = mksCfg.x_max_pos - 20;
                gCfgItems.leveling_points[3].y = mksCfg.y_max_pos - 20;

                gCfgItems.leveling_points[3].x = mksCfg.x_max_pos - 20;
                gCfgItems.leveling_points[3].y = mksCfg.y_max_pos - 20;

                gCfgItems.leveling_points[4].x = (mksCfg.x_max_pos + mksCfg.x_min_pos) / 2;
                gCfgItems.leveling_points[4].y = (mksCfg.y_max_pos + mksCfg.y_min_pos) / 2;
                epr_write_data(EPR_LEVELING_POINTS, (unsigned char *)&gCfgItems.leveling_points, sizeof(gCfgItems.leveling_points));
                this->updateControls();
            }
            break;
        }
    }
    ConfigurationWidgetWithCalc::on_button(hBtn);
}


void LevelingConfigUI::createControls() {
    this->dual_columns = 1;
    memset(&this->ui, 0, sizeof(this->ui));
    ConfigurationWidgetWithCalc::createControls();
    switch(this->page) {
        case 0: {
            this->createRadio(0, 0, &this->ui.type.none, "Manual", 0);
            this->createRadio(0, 1, &this->ui.type.point3, "3 point", 0);
            this->createRadio(0, 2, &this->ui.type.linear, "Linear", 0);
            this->createRadio(0, 3, &this->ui.type.bilinear, "Bilinear", 0);
            this->createRadio(0, 4, &this->ui.type.ubl, "UBL", 0);
            this->createRadio(1, 0, &this->ui.type.mesh, "Mesh", 0);
            this->createInputWithDefault(1, 1, &this->ui.probe.multiple_probing, lang_str.config_ui.probe_count, 0);
            this->createCheckPair(1, 2, &this->ui.probe.enable, "BLtouch", mksCfg.mkstouch);
            //todo: проверить значение
            this->createCheckPair(1, 3, &this->ui.probe.connector, lang_str.config_ui.connectorZ, mksCfg.z_min_probe_pin_mode != 1,
                                  &lang_str.min_max);

            break;
        }
        case 1: {
            this->ui.probe.offset_label = this->createLabel(0, 0, lang_str.config_ui.probe_offset);
            this->createInputWithDefault(0, 1, &this->ui.probe.x_offset, "dX", 0, 0);
            this->createInputWithDefault(0, 2, &this->ui.probe.y_offset, "dY", 0, 0);
            this->createInputWithDefault(0, 3, &this->ui.probe.z_offset, "dZ", 0, 0);
            this->ui.probe.offset_label = this->createLabel(1 , 0, lang_str.config_ui.probe_speed);
            this->createInputWithDefault(1, 1, &this->ui.probe.xy_speed, "XY", 0, 0);
            this->createInputWithDefault(1, 2, &this->ui.probe.z_speed_fast, "Z/1", 0, 0);
            this->createInputWithDefault(1, 3, &this->ui.probe.z_speed_slow, "Z/2", 0, 0);
            break;
        }
        case 2: {
            for (unsigned char i=0;i<5;i++) {
                sprintf(ui_buf1_100, lang_str.config_ui.point, i + 1);
                unsigned char a = strlen(ui_buf1_100);
                ui_buf1_100[a] = ':';
                ui_buf1_100[a+1] = 0;
                this->createInputDial(i / 4, i % 4, &this->ui.points.points[i], ui_buf1_100, 0, 0);
            }

            sprintf(ui_buf1_100, "X (min-max): %d - %d", (u16)mksCfg.x_min_pos, (u16)mksCfg.x_max_pos);
            this->ui.points.x_info = this->createLabel(1, 1, ui_buf1_100);
            sprintf(ui_buf1_100, "Y (min-max): %d - %d", (u16)mksCfg.y_min_pos, (u16)mksCfg.y_max_pos);
            this->ui.points.y_info = this->createLabel(1, 2, ui_buf1_100);
            this->ui.points.deflt = this->createDefaultSetButtonAt(1, 3, 0, 0);
            break;
        }
    }
    this->updateControls();
}
