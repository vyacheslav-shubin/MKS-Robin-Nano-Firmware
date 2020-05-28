//
// Created by shubin on 27.05.2020.
//

#include "LevelingPointsUI.h"
#include "serial.h"

LevelingPointsUI leveling_points_ui;


typedef enum{
    POINT_X = 0, POINT_Y
} VALUES;

void LevelingPointsUI::updateValues() {
    for (int i=0;i<5;i++) {
        sprintf(ui_buf1_100, "%d", gCfgItems.leveling_points[i].x);
        this->setButtonText(this->ui.points[i].button1, ui_buf1_100);
        sprintf(ui_buf1_100, "%d", gCfgItems.leveling_points[i].y);
        this->setButtonText(this->ui.points[i].button2, ui_buf1_100);
    }
}

unsigned char LevelingPointsUI::checkButtonSet(UI_BUTTON hBtn, unsigned char index) {
    if (hBtn==this->ui.points[index].button1) {
        this->hide();
        sprintf(ui_buf1_100, lang_str.config_ui.point, index + 1);
        strcat(ui_buf1_100, ".X:");
        calculator_dialog_ui.show(ui_buf1_100, gCfgItems.leveling_points[index].x, POINT_X + index * 2, this, this);
    }if (hBtn==this->ui.points[index].button2) {
        this->hide();
        sprintf(ui_buf1_100, lang_str.config_ui.point, index + 1);
        strcat(ui_buf1_100, ".Y:");
        calculator_dialog_ui.show(ui_buf1_100, gCfgItems.leveling_points[index].y, POINT_Y + index * 2, this, this);
    } else
        return 0;
    return 1;
}

void LevelingPointsUI::on_button(UI_BUTTON hBtn) {
    for (unsigned char i=0;i<5;i++) {
        if (checkButtonSet(hBtn, i))
            return;
    }
    if (hBtn==this->ui.deflt) {
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
        this->updateValues();
    } else
        ConfigurationWidget::on_button(hBtn);
}

void LevelingPointsUI::createControls() {
    ConfigurationWidget::createControls();
    memset(&this->ui, 0, sizeof(this->ui));
    this->dual_columns = 1;

    for (unsigned char i=0;i<5;i++) {
        sprintf(ui_buf1_100, lang_str.config_ui.point, i + 1);
        unsigned char a = strlen(ui_buf1_100);
        ui_buf1_100[a] = ':';
        ui_buf1_100[a+1] = 0;
        this->createInputDial(i / 4, i % 4, &this->ui.points[i], ui_buf1_100, 0, 0);
    }

    sprintf(ui_buf1_100, "X (min-max): %d - %d", (u16)mksCfg.x_min_pos, (u16)mksCfg.x_max_pos);
    this->ui.x_info = this->createLabel(1, 1, ui_buf1_100);
    sprintf(ui_buf1_100, "Y (min-max): %d - %d", (u16)mksCfg.y_min_pos, (u16)mksCfg.y_max_pos);
    this->ui.y_info = this->createLabel(1, 2, ui_buf1_100);
    this->ui.deflt = this->createDefaultSetButtonAt(1, 3, 0, 0);
    this->updateValues();
}

static void _set_value(unsigned char value_id, u16 value) {
    ((u16 *)&gCfgItems.leveling_points)[value_id] = value;
    epr_write_data(EPR_LEVELING_POINTS + value_id * sizeof(u16), (unsigned char *)&value, sizeof(value));
}


void LevelingPointsUI::on_calculator(unsigned char action, double result, unsigned char dialog_id) {
    calculator_dialog_ui.hide();
    _set_value(dialog_id, (u16)result);
    this->show();
}
