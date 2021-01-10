//
// Created by shubin on 30.12.2020.
//

#include "SpeedUI2.h"
#include "integration.h"

SpeedUI2 speed_ui_2;

#define STEPS_COUNT        3

enum{
    CALC_FAN,
    CALC_EXT1,
    CALC_EXT2,
    CALC_MOOVING,
} CALC_STATE;

static const UI_STEP_INFO speed_steps[STEPS_COUNT] = {
        {1,  "1%",  img_percent_step1},
        {5,  "5%",  img_percent_step5},
        {10, "10%", img_percent_step10},
};

void SpeedUI2::updateStepButton() {
    const UI_STEP_INFO * si = &speed_steps[this->ui.step.index];
    this->updateButton(this->ui.step.button, si->picture, si->title);
}

UI_BUTTON SpeedUI2::createButtonAt90x60(int phx, int phy, const char *picture) {
    return StdWidget::create90x60Button(ui_std_col90x60(phx) + 10 * (phx + 1), ui_std_row90x60(phy), picture);
}

void SpeedUI2::createControls() {
    memset(&this->ui, 0, sizeof(this->ui));
    this->ui.ret = this->createButtonRet();
    this->ui.step.index = 2;
    this->ui.step.button = this->createButtonAt(3, 0, 0, 0);
    this->updateStepButton();
    unsigned char row = 0;
    ui_std_speed_state_button(10, ui_std_row90x60(row) + 10, &this->ui.mooving.state);
    this->ui.mooving.minus = this->createButtonAt90x60(1, row, img90x60_minus);
    this->ui.mooving.plus = this->createButtonAt90x60(2, row, img90x60_plus);
    row++;

    ui_std_ext1_state_button(10, ui_std_row90x60(row) + 10, &this->ui.ext1.state);
    this->ui.ext1.minus = this->createButtonAt90x60(1, row, img90x60_minus);
    this->ui.ext1.plus = this->createButtonAt90x60(2, row, img90x60_plus);
    row++;

    if (is_dual_extruders()) {
        ui_std_ext2_state_button(10, ui_std_row90x60(row) + 10, &this->ui.ext2.state);
        this->ui.ext2.minus = this->createButtonAt90x60(1, row, img90x60_minus);
        this->ui.ext2.plus = this->createButtonAt90x60(2, row, img90x60_plus);
        row++;
    }

    ui_std_fun_state_button(10, ui_std_row90x60(row) + +10, &this->ui.fan.state);
    this->ui.fan.minus = this->createButtonAt90x60(1, row, img90x60_minus);
    this->ui.fan.plus = this->createButtonAt90x60(2, row, img90x60_plus);
}

//virtual unsigned char on_repeatable_button(UI_BUTTON hBtn) {return 0;};
unsigned char SpeedUI2::is_repeated_button(UI_BUTTON hBtn) {
    return (hBtn == ui.mooving.plus)
    || (hBtn == ui.mooving.minus)
    || (hBtn == ui.ext1.plus)
    || (hBtn == ui.ext1.minus)
    || (hBtn == ui.ext2.plus)
    || (hBtn == ui.ext2.minus)
    || (hBtn == ui.fan.plus)
    || (hBtn == ui.fan.minus);

}

unsigned char SpeedUI2::on_repeatable_button(UI_BUTTON hBtn) {
    const UI_STEP_INFO * si = &speed_steps[ui.step.index];
    if (hBtn == this->ui.mooving.plus) {
        shUI::addFeedratePercentage(si->size);
    } else if (hBtn == this->ui.mooving.minus) {
        shUI::addFeedratePercentage(-si->size);
    } else if (hBtn == this->ui.ext1.plus) {
        shUI::addFlowPercentage(0, si->size);
    } else if (hBtn == this->ui.ext1.minus) {
        shUI::addFlowPercentage(0, -si->size);
    } else if (hBtn == this->ui.ext2.plus) {
        shUI::addFlowPercentage(1, si->size);
    } else if (hBtn == this->ui.ext2.minus) {
        shUI::addFlowPercentage(1, -si->size);
    } else if (hBtn == this->ui.fan.plus) {
        shUI::fun_add_percentage(si->size);
    } else if (hBtn == this->ui.fan.minus) {
        shUI::fun_add_percentage(-si->size);
    } else
        return 0;
    this->updateStateButtons();
    return 1;
}

void SpeedUI2::on_button(UI_BUTTON hBtn) {
    if (hBtn == this->ui.ret) {
        this->action_back();
    } else if (hBtn == this->ui.step.button) {
        if (++this->ui.step.index > STEPS_COUNT - 1)
            this->ui.step.index = 0;
        this->updateStepButton();
    } else if (hBtn == this->ui.fan.state.button) {
        this->calculator(lang_str.ui_title_fan, (double) shUI::fan_get_percent(), CALC_FAN);
    } else if (hBtn == this->ui.ext1.state.button) {
        sprintf(ui_buf1_100, "%s: 1", lang_str.config_ui.nozzle);
        this->calculator(ui_buf1_100, (double) shUI::getFlowPercentage(0), CALC_EXT1);
    } else if (hBtn == this->ui.ext2.state.button) {
        sprintf(ui_buf1_100, "%s: 2", lang_str.config_ui.nozzle);
        this->calculator(ui_buf1_100, (double) shUI::getFlowPercentage(1), CALC_EXT2);
    } else if (hBtn == this->ui.mooving.state.button) {
        sprintf(ui_buf1_100, "%s: 2", lang_str.config_ui.speed);
        this->calculator(ui_buf1_100, (double) shUI::getFeedratePercentage(), CALC_MOOVING);
    } else {
        this->on_repeatable_button(hBtn);
    }
}

void SpeedUI2::refresh_05() {
    this->updateFanState(&this->ui.fan.state);
}

void SpeedUI2::updateStateButtons() {
    sprintf(ui_buf1_80, "%d%%", shUI::getFeedratePercentage());
    this->updateStateButton(&this->ui.mooving.state, 0, ui_buf1_80);

    sprintf(ui_buf1_80, "%d%%", shUI::getFlowPercentage(0));
    this->updateStateButton(&this->ui.ext1.state, 0, ui_buf1_80);

    if (this->ui.ext2.state.button) {
        sprintf(ui_buf1_80, "%d%%", shUI::getFlowPercentage(1));
        this->updateStateButton(&this->ui.ext2.state, 0, ui_buf1_80);
    }
}


void SpeedUI2::refresh_1s() {
    this->updateStateButtons();
}

void SpeedUI2::setValue(unsigned char id, double value) {
    switch (id) {
        case CALC_FAN:
            shUI::fan_set_percent(value);
            break;
        case CALC_EXT1:
            shUI::setFlowPercentage(0, value);
            break;
        case CALC_EXT2:
            shUI::setFlowPercentage(1, value);
            break;
        case CALC_MOOVING:
            shUI::setFeedratePercentage(value);
            break;
    }
}
