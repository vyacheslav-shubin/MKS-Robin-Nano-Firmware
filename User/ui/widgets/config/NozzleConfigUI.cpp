//
// Created by shubin on 02.06.2020.
//

#include "NozzleConfigUI.h"
#include "integration.h"
#include "temperature.h"
#include "mks_cfg.h"

NozzleConfigUI nozzle_config_ui;

typedef enum {
    NUMBER_OF_NOZZLES = 0, T_LO, T_HI, PID_P, PID_I, PID_D
} VALUES;

static const char * _get_e0_sensor_type() {
    return mksCfg.temp_sensor_0 == -3 ? "MAX" : "NTC";
}


void NozzleConfigUI::updatePidControls(unsigned char index) {
    if (this->ui.nozzle[index].pid.p.button!=0) {
        sprintf(ui_buf1_100,"%.3f",PID_PARAM(Kp, index));
        this->setButtonText(this->ui.nozzle[index].pid.p.button, ui_buf1_100);
    }
    if (this->ui.nozzle[index].pid.i.button!=0) {
        sprintf(ui_buf1_100,"%.3f",unscalePID_i(PID_PARAM(Ki, index)));
        this->setButtonText(this->ui.nozzle[index].pid.i.button, ui_buf1_100);
    }
    if (this->ui.nozzle[index].pid.d.button!=0) {
        sprintf(ui_buf1_100,"%.3f",unscalePID_d(PID_PARAM(Kd, index)));
        this->setButtonText(this->ui.nozzle[index].pid.d.button, ui_buf1_100);
    }
}

void NozzleConfigUI::updateControls() {
    if (this->ui.nozzles.button!=0) {
        sprintf(ui_buf1_100, "%d", mksCfg.extruders);
        this->setButtonText(this->ui.nozzles.button, ui_buf1_100);
    }
    if (this->ui.e0sensorType.button!=0) {
        this->setButtonText(this->ui.e0sensorType.button, _get_e0_sensor_type());
    }

    if (this->ui.nozzle[0].temp.lo.button!=0) {
        sprintf(ui_buf1_100, "%d", mksCfg.heater_0_mintemp);
        this->setButtonText(this->ui.nozzle[0].temp.lo.button, ui_buf1_100);
    }
    if (this->ui.nozzle[0].temp.hi.button!=0) {
        sprintf(ui_buf1_100, "%d", mksCfg.heater_0_maxtemp);
        this->setButtonText(this->ui.nozzle[0].temp.hi.button, ui_buf1_100);
    }
    if (this->ui.nozzle[1].temp.lo.button!=0) {
        sprintf(ui_buf1_100, "%d", mksCfg.heater_1_mintemp);
        this->setButtonText(this->ui.nozzle[1].temp.lo.button, ui_buf1_100);
    }
    if (this->ui.nozzle[1].temp.hi.button!=0) {
        sprintf(ui_buf1_100, "%d", mksCfg.heater_1_maxtemp);
        this->setButtonText(this->ui.nozzle[1].temp.hi.button, ui_buf1_100);
    }

    this->updatePidControls(0);
    this->updatePidControls(1);

}

void NozzleConfigUI::_setValue(unsigned char index, float value) {
    unsigned char nozzle = 0;
    if (index>10) {
        index -= 10;
        nozzle = 1;
    }
    switch (index) {
        case NUMBER_OF_NOZZLES: {
            unsigned char a = value;
            if (a < 1)
                a = 1;
            if (a > 2)
                a = 2;
            mksCfg.extruders = a;
            epr_write_data(EPR_EXTRUDERS, &a, 1);
            break;
        }
        case T_LO: {
            if (nozzle==0) {
                mksCfg.heater_0_mintemp = value;
                epr_write_data(EPR_HEATER_0_MINTEMP, (uint8_t *)&mksCfg.heater_0_mintemp, sizeof(mksCfg.heater_0_mintemp));
            } else {
                mksCfg.heater_1_mintemp = value;
                epr_write_data(EPR_HEATER_1_MINTEMP,(uint8_t *)&mksCfg.heater_1_mintemp, sizeof(mksCfg.heater_1_mintemp));
            }
            break;
        }
        case T_HI: {
            if (nozzle==0) {
                mksCfg.heater_0_maxtemp = value;
                epr_write_data(EPR_HEATER_0_MAXTEMP,(uint8_t *)&mksCfg.heater_0_maxtemp,sizeof(mksCfg.heater_0_maxtemp));
            } else {
                mksCfg.heater_1_maxtemp = value;
                epr_write_data(EPR_HEATER_1_MAXTEMP,(uint8_t *)&mksCfg.heater_1_maxtemp,sizeof(mksCfg.heater_1_maxtemp));
            }
            break;
        }
        case PID_P: {
            PID_PARAM(Kp, nozzle) = value;
            shUI::saveConfig();
            break;
        }
        case PID_I: {
            PID_PARAM(Ki, nozzle) = scalePID_i(value);
            shUI::saveConfig();
            break;
        }
        case PID_D: {
            PID_PARAM(Kd, nozzle) = scalePID_d(value);
            shUI::saveConfig();
            break;
        }
    }
}

unsigned char NozzleConfigUI::on_default_button(UI_BUTTON hBtn, unsigned char index) {
    if (hBtn ==  this->ui.nozzle[index].temp.hi.dflt) {
        this->_setValue( T_HI + index * 10, 300);
    } else if (hBtn ==  this->ui.nozzle[index].temp.lo.dflt) {
        this->_setValue(T_LO + index * 10, 10);
    } else if (hBtn ==  this->ui.nozzle[index].pid.p.dflt) {
        this->_setValue(PID_P + index * 10, 22.2);
    } else if (hBtn ==  this->ui.nozzle[index].pid.i.dflt) {
        this->_setValue(PID_I + index * 10, 1.08);
    } else if (hBtn ==  this->ui.nozzle[index].pid.d.dflt) {
        this->_setValue(PID_D + index * 10, 114);
    } else
        return 0;
    return 1;
}


unsigned char NozzleConfigUI::on_input_button(UI_BUTTON hBtn, unsigned char index) {
    if (hBtn ==  this->ui.nozzle[index].temp.lo.button) {
        this->calculator(lang_str.config_ui.minumumT, index==0 ? mksCfg.heater_0_mintemp : mksCfg.heater_1_mintemp, T_LO + index * 10);
    } else if (hBtn ==  this->ui.nozzle[index].temp.hi.button) {
        this->calculator(lang_str.config_ui.maximumT, index==0 ? mksCfg.heater_0_maxtemp: mksCfg.heater_1_maxtemp, T_HI + index * 10);
    } else if (hBtn ==  this->ui.nozzle[index].pid.p.button) {
        this->calculator(lang_str.config_ui.pid_termostat,"/P", PID_PARAM(Kp, index), PID_P + index * 10);
    } else if (hBtn ==  this->ui.nozzle[index].pid.i.button) {
        this->calculator(lang_str.config_ui.pid_termostat,"/I", unscalePID_i(PID_PARAM(Ki, index)), PID_I + index * 10);
    } else if (hBtn ==  this->ui.nozzle[index].pid.d.button) {
        this->calculator(lang_str.config_ui.pid_termostat,"/D", unscalePID_d(PID_PARAM(Kd, index)), PID_D + index * 10);
    } else
        return 0;
    return 1;
}


void NozzleConfigUI::on_button(UI_BUTTON hBtn) {
    if (hBtn ==  this->ui.nozzles.button) {
        this->calculator(lang_str.config_ui.number_of_nozzles, mksCfg.extruders, NUMBER_OF_NOZZLES);
    } else if (on_input_button(hBtn, 0) || on_input_button(hBtn, 1)) {
        //nothing
    } else {
        if (hBtn == this->ui.e0sensorType.button) {
            mksCfg.temp_sensor_0 = mksCfg.temp_sensor_0 == -3 ? 1 : -3;
            epr_write_data(EPR_TEMP_SENSOR_0, (uint8_t *) &mksCfg.temp_sensor_0, 1);
        } else if (ui_is_double_button(hBtn, this->ui.pid_enabled)) {
            mksCfg.pidtemp = mksCfg.pidtemp ? 0 : 1;
            this->updateCheckButton(this->ui.pid_enabled.button, mksCfg.pidtemp);
            epr_write_data(EPR_PIDTEMP,(uint8_t *)&mksCfg.pidtemp,1);
            if (mksCfg.pidtemp)
                shUI::loadConfig();
        } else if (on_default_button(hBtn, 0) || on_default_button(hBtn, 1)) {
            //nothing
        } else {
            ConfigurationWidgetWithCalc::on_button(hBtn);
            return;
        }

        this->updateControls();
    }
}

void NozzleConfigUI::makeNozzleControls(unsigned char index) {
    strcpy(ui_buf1_100, lang_str.config_ui.nozzle);
    sprintf(&ui_buf1_100[strlen(ui_buf1_100)], " %d", index + 1);
    this->createLabel(0, 0, ui_buf1_100);
    this->createInputWithDefault(0, 1, &this->ui.nozzle[index].temp.lo,lang_str.config_ui.minumumT, 0);
    this->createInputWithDefault(0, 2, &this->ui.nozzle[index].temp.hi,lang_str.config_ui.maximumT, 0);
    this->createLabel(1, 0, lang_str.config_ui.pid_termostat);
    this->createInputWithDefault(1, 1, &this->ui.nozzle[index].pid.p, "P", 0);
    this->createInputWithDefault(1, 2, &this->ui.nozzle[index].pid.i, "I", 0);
    this->createInputWithDefault(1, 3, &this->ui.nozzle[index].pid.d, "D", 0);
}

void NozzleConfigUI::createControls() {
    ConfigurationWidgetWithCalc::createControls();
    memset(&this->ui, 0, sizeof(this->ui));
    this->dual_columns = 1;
    if (page==0) {
        this->createInputSingle(0, 0, &this->ui.nozzles, "Number of nozzles", 0);
        this->createInputSingle(0, 1, &this->ui.e0sensorType, "E0 type", _get_e0_sensor_type());
        this->createCheckPair(0, 2, &this->ui.pid_enabled, lang_str.config_ui.pid_termostat, mksCfg.pidtemp);
    } if (page==1) {
        this->makeNozzleControls(0);
    } if (page==2) {
        this->makeNozzleControls(1);
    }

    this->updateControls();
}
