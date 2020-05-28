//
// Created by shubin on 28.05.2020.
//

#include "SensorLevelConfigUI.h"


SensorLevelConfigUI sensor_level_config_ui;

void SensorLevelConfigUI::on_button(UI_BUTTON hBtn) {
    if (hBtn==this->ui.x.mn.button) {
        mksCfg.x_min_endstop_inverting = mksCfg.x_min_endstop_inverting ? 0 : 1;
        epr_write_data(EPR_X_MIN_ENDSTOP_INVERTING, &mksCfg.x_min_endstop_inverting,1);
        this->updateCheckButton(this->ui.x.mn.button, mksCfg.x_min_endstop_inverting==0, &lang_str.gnd_vcc);
    } else if (hBtn==this->ui.x.mx.button) {
        mksCfg.x_max_endstop_inverting = mksCfg.x_max_endstop_inverting ? 0 : 1;
        epr_write_data(EPR_X_MAX_ENDSTOP_INVERTING, &mksCfg.x_max_endstop_inverting,1);
        this->updateCheckButton(this->ui.x.mx.button, mksCfg.x_max_endstop_inverting==0, &lang_str.gnd_vcc);
    } else if (hBtn==this->ui.y.mn.button) {
        mksCfg.y_min_endstop_inverting = mksCfg.y_min_endstop_inverting ? 0 : 1;
        epr_write_data(EPR_Y_MIN_ENDSTOP_INVERTING, &mksCfg.y_min_endstop_inverting,1);
        this->updateCheckButton(this->ui.y.mn.button, mksCfg.y_min_endstop_inverting==0, &lang_str.gnd_vcc);
    } else if (hBtn==this->ui.y.mx.button) {
        mksCfg.y_max_endstop_inverting = mksCfg.y_max_endstop_inverting ? 0 : 1;
        epr_write_data(EPR_Y_MAX_ENDSTOP_INVERTING, &mksCfg.y_max_endstop_inverting,1);
        this->updateCheckButton(this->ui.y.mx.button, mksCfg.y_max_endstop_inverting==0, &lang_str.gnd_vcc);
    } else if (hBtn==this->ui.z.mn.button) {
        mksCfg.z_min_endstop_inverting = mksCfg.z_min_endstop_inverting ? 0 : 1;
        epr_write_data(EPR_Z_MIN_ENDSTOP_INVERTING, &mksCfg.z_min_endstop_inverting,1);
        this->updateCheckButton(this->ui.z.mn.button, mksCfg.z_min_endstop_inverting==0, &lang_str.gnd_vcc);
    } else if (hBtn==this->ui.z.mx.button) {
        mksCfg.z_max_endstop_inverting = mksCfg.z_max_endstop_inverting ? 0 : 1;
        epr_write_data(EPR_Z_MAX_ENDSTOP_INVERTING, &mksCfg.z_max_endstop_inverting,1);
        this->updateCheckButton(this->ui.z.mx.button, mksCfg.z_max_endstop_inverting==0, &lang_str.gnd_vcc);
    } else if (hBtn==this->ui.e1.button) {
        gCfgItems.filament_det0_level_flg = gCfgItems.filament_det0_level_flg ? 0 : 1;
        epr_write_data(EPR_FILAMENT_DET0_LEVEL,(uint8_t *)&gCfgItems.filament_det0_level_flg,1);
        this->updateCheckButton(this->ui.e1.button, gCfgItems.filament_det0_level_flg, &lang_str.gnd_vcc);
    } else if (hBtn==this->ui.e2.button) {
        gCfgItems.filament_det1_level_flg = gCfgItems.filament_det1_level_flg ? 0 : 1;
        epr_write_data(EPR_FILAMENT_DET1_LEVEL,(uint8_t *)&gCfgItems.filament_det1_level_flg,1);
        this->updateCheckButton(this->ui.e2.button, gCfgItems.filament_det1_level_flg, &lang_str.gnd_vcc);
    } else if (hBtn==this->ui.lvl.button) {
        mksCfg.z_min_probe_endstop_inverting = mksCfg.z_min_probe_endstop_inverting ? 0 : 1;
        epr_write_data(EPR_Z_MIN_PROBE_ENDSTOP_INVERTING,&mksCfg.z_min_probe_endstop_inverting,1);
        this->updateCheckButton(this->ui.lvl.button, mksCfg.z_min_probe_endstop_inverting==0, &lang_str.gnd_vcc);
    }
    ConfigurationWidget::on_button(hBtn);
}

void SensorLevelConfigUI::createControls() {
    ConfigurationWidget::createControls();
    this->dual_columns = 1;
    memset(&this->ui, 0, sizeof(this->ui));
    this->createCheckPair(0, 0, &this->ui.x.mn, "X min", mksCfg.x_min_endstop_inverting==0, &lang_str.gnd_vcc);
    this->createCheckPair(1, 0, &this->ui.x.mx, "X max", mksCfg.x_max_endstop_inverting==0, &lang_str.gnd_vcc);
    this->createCheckPair(0, 1, &this->ui.y.mn, "Y min", mksCfg.y_min_endstop_inverting==0, &lang_str.gnd_vcc);
    this->createCheckPair(1, 1, &this->ui.y.mx, "Y max", mksCfg.y_max_endstop_inverting==0, &lang_str.gnd_vcc);
    this->createCheckPair(0, 2, &this->ui.z.mn, "Z min", mksCfg.z_min_endstop_inverting==0, &lang_str.gnd_vcc);
    this->createCheckPair(1, 2, &this->ui.z.mx, "Z max", mksCfg.z_max_endstop_inverting==0, &lang_str.gnd_vcc);
    this->createCheckPair(0, 3, &this->ui.e1, "E1", gCfgItems.filament_det0_level_flg, &lang_str.gnd_vcc);
    this->createCheckPair(1, 3, &this->ui.e2, "E2", gCfgItems.filament_det1_level_flg, &lang_str.gnd_vcc);
    this->createCheckPair(0, 4, &this->ui.lvl, lang_str.leveling, mksCfg.z_min_probe_endstop_inverting==0, &lang_str.gnd_vcc);
}
