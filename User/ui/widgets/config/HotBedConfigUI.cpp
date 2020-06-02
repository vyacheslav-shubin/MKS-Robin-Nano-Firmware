//
// Created by shubin on 01.06.2020.
//

#include "HotBedConfigUI.h"
#include "mks_cfg.h"
#include "temperature.h"
#include "integration.h"

HotBedConfigUI hotbed_config_ui;

typedef enum {
    T_LO = 0, T_HI, PID_P, PID_I, PID_D
} VALUES;

void HotBedConfigUI::updateControls() {
    sprintf(ui_buf1_100,"%.3f",thermalManager.bedKp);
    this->setButtonText(this->ui.pid.p.button, ui_buf1_100);
    sprintf(ui_buf1_100,"%.3f",unscalePID_i(thermalManager.bedKi));
    this->setButtonText(this->ui.pid.i.button, ui_buf1_100);
    sprintf(ui_buf1_100,"%.3f",unscalePID_d(thermalManager.bedKd));
    this->setButtonText(this->ui.pid.d.button, ui_buf1_100);
    sprintf(ui_buf1_100,"%d",mksCfg.bed_mintemp);
    this->setButtonText(this->ui.temp.lo.button, ui_buf1_100);
    sprintf(ui_buf1_100,"%d",mksCfg.bed_maxtemp);
    this->setButtonText(this->ui.temp.hi.button, ui_buf1_100);
}

void HotBedConfigUI::_setValue(unsigned char index, float value) {
    switch(index) {
        case T_LO: {
            mksCfg.bed_maxtemp = value;
            epr_write_data(EPR_BED_MAXTEMP,(uint8_t *)&mksCfg.bed_maxtemp, sizeof(mksCfg.bed_maxtemp));
            thermalManager.init();
            break;
        }
        case T_HI: {
            mksCfg.bed_mintemp=value;
            epr_write_data(EPR_BED_MINTEMP,(uint8_t *)&mksCfg.bed_mintemp, sizeof(mksCfg.bed_mintemp));
            thermalManager.init();
            break;
        }
        case PID_P: {
            thermalManager.bedKp=value;
            shUI::saveConfig();
            break;
        }
        case PID_I: {
            thermalManager.bedKi=scalePID_i(value);
            shUI::saveConfig();
            break;
        }
        case PID_D: {
            thermalManager.bedKd=scalePID_d(value);
            shUI::saveConfig();
            break;
        }
    }
}


void HotBedConfigUI::on_button(UI_BUTTON hBtn) {
    if (ui_is_double_button(hBtn, this->ui.enabled)) {
        mksCfg.has_temp_bed = mksCfg.has_temp_bed ? 0 : 1;
        epr_write_data(EPR_HAS_TEMP_BED,(uint8_t *)&mksCfg.has_temp_bed,1);
        this->updateCheckButton(this->ui.enabled.button, mksCfg.has_temp_bed);
        if (mksCfg.has_temp_bed) {
            shUI::loadConfig();
            this->updateControls();
        }
    } else if (ui_is_double_button(hBtn, this->ui.pid.enabled)) {
        mksCfg.pidtempbed = mksCfg.pidtempbed ? 0 : 1;
        epr_write_data(EPR_PIDTEMPBED,(uint8_t *)&mksCfg.pidtempbed,1);
        this->updateCheckButton(this->ui.pid.enabled.button, mksCfg.pidtempbed);
        if (mksCfg.pidtempbed) {
            shUI::loadConfig();
            this->updateControls();
        }
    } else if (hBtn == this->ui.temp.lo.button) {
        this->calculator(lang_str.config_ui.minumumT, mksCfg.bed_mintemp, T_LO);
    } else if (hBtn == this->ui.temp.hi.button) {
        this->calculator(lang_str.config_ui.maximumT, mksCfg.bed_maxtemp, T_HI);
    } else if (hBtn == this->ui.pid.p.button) {
        this->calculator(lang_str.config_ui.pid_termostat,"/P", thermalManager.bedKp, PID_P);
    } else if (hBtn == this->ui.pid.i.button) {
        this->calculator(lang_str.config_ui.pid_termostat,"/I", unscalePID_i(thermalManager.bedKi), PID_I);
    } else if (hBtn == this->ui.pid.d.button) {
        this->calculator(lang_str.config_ui.pid_termostat,"/D", unscalePID_d(thermalManager.bedKd), PID_D);
    } else {
        if (hBtn==this->ui.temp.lo.dflt) {
            this->_setValue(T_LO, 10);
        } else if (hBtn==this->ui.temp.hi.dflt) {
            this->_setValue(T_HI, 110);
        } else if (hBtn==this->ui.pid.p.dflt) {
            this->_setValue(PID_P, 10);
        } else if (hBtn==this->ui.pid.i.dflt) {
            this->_setValue(PID_I, 0.023);
        } else if (hBtn==this->ui.pid.d.dflt) {
            this->_setValue(PID_D, 305.400);
        } else {
            ConfigurationWidget::on_button(hBtn);
            return;
        }
        this->updateControls();
    }
}

void HotBedConfigUI::createControls() {
    ConfigurationWidget::createControls();
    this->dual_columns = 1;
    memset(&this->ui, 0, sizeof(this->ui));
    this->createCheckPair(0, 0, &this->ui.enabled, lang_str.config_ui.heater, mksCfg.has_temp_bed);
    this->createInputWithDefault(0, 1, &this->ui.temp.lo, lang_str.config_ui.minumumT, 0);
    this->createInputWithDefault(0, 2, &this->ui.temp.hi, lang_str.config_ui.maximumT, 0);
    this->createCheckPair(1, 0, &this->ui.pid.enabled, lang_str.config_ui.pid_termostat, mksCfg.pidtempbed);
    this->createInputWithDefault(1, 1, &this->ui.pid.p, "P", 0);
    this->createInputWithDefault(1, 2, &this->ui.pid.i, "I", 0);
    this->createInputWithDefault(1, 3, &this->ui.pid.d, "D", 0);
    this->updateControls();
}
