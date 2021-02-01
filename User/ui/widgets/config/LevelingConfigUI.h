//
// Created by shubin on 16.06.2020.
//

#ifndef WORK_LEVELINGCONFIGUI_H
#define WORK_LEVELINGCONFIGUI_H

#include "ConfigurationWidget.h"

typedef struct {
    UI_INPUT_DUAL points[5];
    UI_TEXT x_info;
    UI_TEXT y_info;
    UI_BUTTON deflt;
} LEVELING_POINTS_CONFIG_UI_CONTROLS1;

typedef struct {
    UI_RADIO none;
    UI_RADIO point3;
    UI_RADIO linear;
    UI_RADIO bilinear;
    UI_RADIO ubl;
    UI_RADIO mesh;
} LEVELING_TYPE_CONFIG_UI_CONTROLS;

typedef struct {
    UI_CHECK enable;
    UI_INPUT_WITH_DEFAULT multiple_probing;
    UI_CHECK connector;
    UI_TEXT label;
    UI_TEXT offset_label;
    UI_INPUT_WITH_DEFAULT x_offset;
    UI_INPUT_WITH_DEFAULT y_offset;
    UI_INPUT_WITH_DEFAULT z_offset;
    UI_TEXT speed_label;
    UI_INPUT_WITH_DEFAULT xy_speed;
    UI_INPUT_WITH_DEFAULT z_speed_fast;
    UI_INPUT_WITH_DEFAULT z_speed_slow;
} LEVELING_AUTO_PROBE_UI_CONTROLS1;

typedef struct {
    LEVELING_TYPE_CONFIG_UI_CONTROLS type;
    LEVELING_AUTO_PROBE_UI_CONTROLS1 probe;
    LEVELING_POINTS_CONFIG_UI_CONTROLS1 points;
} LEVELING_CONFIG_UI_CONTROLS;

class LevelingConfigUI : public ConfigurationWidgetWithCalc {
private:
    LEVELING_CONFIG_UI_CONTROLS ui;
    void updateControls();
    void _setValue(unsigned char index, float value);
    unsigned char checkPointsButtonSet(UI_BUTTON hBtn, unsigned char index);
protected:
    virtual void createControls();
    virtual void setValue(unsigned char id, double value) {
        this->_setValue(id, value);
    }
    void on_button(UI_BUTTON hBtn);
public:
    virtual const char * getTitle() {
        if (this->page==0)
            return lang_str.ui_title_leveling_type;
        if (this->page==1)
            return lang_str.ui_title_probe_settins;
        if (this->page==2)
            return lang_str.ui_title_leveling_points;
        return 0;
    };
    LevelingConfigUI() : ConfigurationWidgetWithCalc(LEVELING_PARA_UI, 3) {};
};


extern LevelingConfigUI leveling_config_ui;

#endif //WORK_LEVELINGCONFIGUI_H
