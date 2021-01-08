//
// Created by shubin on 30.12.2020.
//

#include "SpeedUI2.h"
#include "integration.h"

SpeedUI2 speed_ui_2;

#define STEPS_COUNT		3

static const UI_STEP_INFO speed_steps[STEPS_COUNT] = {
        {1, "1%", img_percent_step1},
        {5, "5%", img_percent_step5},
        {10, "10%", img_percent_step10},
};

void SpeedUI2::updateStepButton() {
    UI_STEP_INFO  * si = &speed_steps[this->ui.step.index];
    this->updateButton(this->ui.step.button, si->picture, si->title);
}

UI_BUTTON SpeedUI2::createButtonAt90x60(int phx, int phy, const char * picture) {
    return StdWidget::create90x60Button(ui_std_col90x60(phx) + 10 * (phx + 1),ui_std_row90x60(phy),picture);
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

    ui_std_ext1_state_button(10, ui_std_row90x60(row) +  10, &this->ui.ext1.state);
    this->ui.ext1.minus = this->createButtonAt90x60(1, row, img90x60_minus);
    this->ui.ext1.plus = this->createButtonAt90x60(2, row, img90x60_plus);
    row++;

    if (is_dual_extruders()) {
        ui_std_ext2_state_button(10, ui_std_row90x60(row) + 10, &this->ui.ext2.state);
        this->ui.ext2.minus = this->createButtonAt90x60(1, row, img90x60_minus);
        this->ui.ext2.plus = this->createButtonAt90x60(2, row, img90x60_plus);
        row++;
    }

    ui_std_fun_state_button(10, ui_std_row90x60(row) +  + 10, &this->ui.fun.state);
    this->ui.fun.minus = this->createButtonAt90x60(1, row, img90x60_minus);
    this->ui.fun.plus = this->createButtonAt90x60(2, row, img90x60_plus);
}

void SpeedUI2::on_button(UI_BUTTON hBtn) {
    if (hBtn==this->ui.ret) {
        this->action_back();
    }else if (hBtn==this->ui.step.button) {
        if (++this->ui.step.index > STEPS_COUNT - 1)
            this->ui.step.index = 0;
        this->updateStepButton();
    } else {
        UI_STEP_INFO * si = &speed_steps[ui.step.index];
        if (hBtn==this->ui.mooving.plus) {
            shUI::addFeedratePercentage(si->size);
        } else if (hBtn==this->ui.mooving.minus) {
            shUI::addFeedratePercentage(-si->size);
        } else if (hBtn==this->ui.ext1.plus) {
            shUI::addFlowPercentage(0, si->size);
        } else if (hBtn==this->ui.ext1.minus) {
            shUI::addFlowPercentage(0, -si->size);
        } else if (hBtn==this->ui.ext2.plus) {
            shUI::addFlowPercentage(1, si->size);
        } else if (hBtn==this->ui.ext2.minus) {
            shUI::addFlowPercentage(1, si->size);
        } else if (hBtn==this->ui.fun.plus) {
            shUI::fun_add_percentage(si->size);
        } else if (hBtn==this->ui.fun.minus) {
            shUI::fun_add_percentage(-si->size);
        }
    }
}

void SpeedUI2::refresh_05() {
    this->updateFanState(&this->ui.fun.state);
}

void SpeedUI2::refresh_1s() {
    sprintf(ui_buf1_80, "%d%%", shUI::getFeedratePercentage());
    this->updateStateButton(&this->ui.mooving.state, 0, ui_buf1_80);

    sprintf(ui_buf1_80, "%d%%", shUI::getFlowPercentage(0));
    this->updateStateButton(&this->ui.ext1.state, 0, ui_buf1_80);

    if (this->ui.ext2.state.button) {
        sprintf(ui_buf1_80, "%d%%", shUI::getFlowPercentage(1));
        this->updateStateButton(&this->ui.ext2.state, 0, ui_buf1_80);
    }
}
