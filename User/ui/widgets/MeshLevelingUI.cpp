//
// Created by shubin on 16.06.2020.
//

#include "MeshLevelingUI.h"
#include "ui_tools.h"
#include "integration.h"
#include "Marlin.h"
MeshLevelingUI mesh_leveling_ui;

static UI_STEP_INFO mesh_z_step_info[3] = {
        {0.01, "0.01 mm", img_move_step_1},
        {0.05, "0.05 mm", img_move_step_2},
        {0.1, "0.10 mm", img_move_step_3}
};

UI_STEP_INFO * MeshLevelingUI::getStepInfo() {
    return &mesh_z_step_info[step];
}

void MeshLevelingUI::updateButtons() {
    UI_STEP_INFO * step_info = this->getStepInfo();
    this->updateButton(this->ui.step, step_info->picture, step_info->title);
}

void MeshLevelingUI::refresh_1s() {
    this->drawXYZ();
    sprintf(ui_buf1_100,"%.3f",current_position[Z_AXIS]);
    this->updateStateButton(&ui.z, 0, ui_buf1_100);
}

void MeshLevelingUI::createControls() {
    memset(&this->ui, 0, sizeof(this->ui));
    this->mesh_step = 0;
    this->ui.ret = this->createButtonRet();
    this->ui.zInc = this->createButtonAt(2, 0, img_z_plus, "Z+");
    this->ui.zDec = this->createButtonAt(2, 1, img_z_minus, "Z-");
    this->ui.step = this->createButtonAt(3, 0, 0, 0);
    #define _col(ph_x) (96*ph_x) + 20
    #define _y 190
    this->ui.action = this->create96x80Button(_col(0), _y, img_print_resume);

    this->createStateButton(20, 10, &this->ui.z, img_state_z, 0);
    this->createStateButton(20, 50, &this->ui.point, img_state_mesh_point, "Start");
    this->updateButtons();
}

void MeshLevelingUI::do_move(char direction) {
    UI_STEP_INFO * step_info = this->getStepInfo();
    shUI::pushGcode("G91");
    sprintf(ui_buf1_100, "G1 Z%3.2f F%d", direction * step_info->size, gCfgItems.moveSpeed);
    shUI::pushGcode(ui_buf1_100);
    shUI::pushGcode("G90");
}

void MeshLevelingUI::do_mesh_step() {
    if (this->mesh_step==0)
        shUI::pushGcode("G29 S1");
    else
        shUI::pushGcode("G29 S2");
    this->mesh_step++;
    if(this->mesh_step > GRID_MAX_POINTS) {
        this->mesh_step = 0;
        this->updateStateButton(&ui.point, 0, "Done");
    } else {
        sprintf(ui_buf1_100, "Point %d", this->mesh_step);
        this->updateStateButton(&ui.point, 0, ui_buf1_100);
    }
}

void MeshLevelingUI::on_button(UI_BUTTON hBtn) {
    if (hBtn == this->ui.step) {
        if (++this->step >= 3)
            this->step = 0;
        this->updateButtons();
    } else if (hBtn == this->ui.zInc) {
        this->do_move(1);
    } else if (hBtn == this->ui.zDec) {
        this->do_move(-1);
    } else if (hBtn == this->ui.ret) {
        this->action_back();
    } else if (hBtn == this->ui.action) {
        this->do_mesh_step();
    }
}