//
// Created by shubin on 27.05.2020.
//

#include "FilamentChangeConfigUI.h"
#include "mks_cfg.h"


typedef enum{
    FILAMENT_T = 0, FILAMENT_L, FILAMENT_S
} VALUES;

FilamentChangeConfigUI filament_change_config_ui;


void _set_text_values(FILAMET_CHANGE_UI_CONTROLS_SET * set, FILAMENT_CHANGE_CONFIG_SET * config) {
    sprintf(ui_buf1_100,"%d",config->temper);
    BUTTON_SetText(set->termerature.button, ui_buf1_100);
    sprintf(ui_buf1_100,"%d",config->length);
    BUTTON_SetText(set->length.button, ui_buf1_100);
    sprintf(ui_buf1_100,"%d",config->speed);
    BUTTON_SetText(set->speed.button, ui_buf1_100);
}

void FilamentChangeConfigUI::updateValues() {
    _set_text_values(&this->ui.load, &gCfgItems.filamentchange.load);
    _set_text_values(&this->ui.unload, &gCfgItems.filamentchange.unload);
}

unsigned char FilamentChangeConfigUI::checkButtonSet(UI_BUTTON hBtn, unsigned char index) {
    FILAMET_CHANGE_UI_CONTROLS_SET * set = index==0 ? &this->ui.load: &this->ui.unload;
    FILAMENT_CHANGE_CONFIG_SET * config = index==0 ? &gCfgItems.filamentchange.load: &gCfgItems.filamentchange.unload;
    if (hBtn==set->speed.button) {
        this->calculator(lang_str.config_ui.speed, config->speed, FILAMENT_S + 3 * index);
    } else if (hBtn==set->termerature.button) {
        this->calculator(lang_str.config_ui.filament_change_temperature, config->temper, FILAMENT_T + 3 * index);
    } else if (hBtn==set->length.button) {
        this->calculator(lang_str.config_ui.length, config->length, FILAMENT_L + 3 * index);
    } else {
        if (hBtn==set->length.dflt) {
            this->_setValue(FILAMENT_L + 3 * index, 750);
        } else if (hBtn==set->speed.dflt) {
            this->_setValue(FILAMENT_S + 3 * index, 2000);
        } else if (hBtn==set->termerature.dflt) {
            this->_setValue(FILAMENT_T + 3 * index, 200);
        } else
            return 0;
        _set_text_values(set, config);
    }
    return 1;
}

void FilamentChangeConfigUI::on_button(UI_BUTTON hBtn) {
    if (!(checkButtonSet(hBtn, 0) || checkButtonSet(hBtn, 1)))
        ConfigurationWidget::on_button(hBtn);
}

void FilamentChangeConfigUI::createSet(FILAMET_CHANGE_UI_CONTROLS_SET * set, unsigned char col) {
    this->createInputWithDefault(col, 1, &set->termerature, lang_str.config_ui.filament_change_temperature, 0, 0);
    this->createInputWithDefault(col, 2, &set->speed, lang_str.config_ui.speed, 0, 0);
    this->createInputWithDefault(col, 3, &set->length, lang_str.config_ui.length, 0, 0);
}

void FilamentChangeConfigUI::createControls() {
    ConfigurationWidget::createControls();
    this->dual_columns = 1;
    memset(&this->ui, 0, sizeof(this->ui));
    this->createLabel(0, 0, lang_str.load);
    this->createLabel(1, 0, lang_str.unload);
    this->createSet(&this->ui.load, 0);
    this->createSet(&this->ui.unload, 1);
    this->updateValues();
}

void FilamentChangeConfigUI::_setValue(unsigned char value_id, u32 value) {
    switch (value_id) {
        case FILAMENT_L:
            gCfgItems.filamentchange.load.length = value;
            epr_write_data(EPR_FILAMENT_LOAD_LENGTH, (const unsigned char *)&value, sizeof(value));
            break;
        case FILAMENT_S:
            gCfgItems.filamentchange.load.speed = value;
            epr_write_data(EPR_FILAMENT_LOAD_SPEED, (const unsigned char *)&value, sizeof(value));
            break;
        case FILAMENT_T:
            gCfgItems.filamentchange.load.temper = value;
            epr_write_data(EPR_FILAMENT_LOAD_LIMIT_TEMPER, (const unsigned char *)&value, sizeof(value));
            break;
        case FILAMENT_L + 3:
            gCfgItems.filamentchange.unload.length = value;
            epr_write_data(EPR_FILAMENT_UNLOAD_LENGTH, (const unsigned char *)&value, sizeof(value));
            break;
        case FILAMENT_S + 3:
            gCfgItems.filamentchange.unload.speed = value;
            epr_write_data(EPR_FILAMENT_UNLOAD_SPEED, (const unsigned char *)&value, sizeof(value));
            break;
        case FILAMENT_T + 3:
            gCfgItems.filamentchange.unload.temper = value;
            epr_write_data(EPR_FILAMENT_UNLOAD_LIMIT_TEMPER, (const unsigned char *)&value, sizeof(value));
            break;
    }
}

