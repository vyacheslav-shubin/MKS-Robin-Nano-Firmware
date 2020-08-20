//
// Created by shubin on 16.06.2020.
//

#include "mesh_bed_leveling.h"
#include "MeshLevelingUI.h"
#include "ui_tools.h"
#include "integration.h"
#include "Marlin.h"
#include "dialog/ConfirmDialogUI.h"

MeshLevelingUI mesh_leveling_ui;

extern int mbl_probe_index;

static UI_STEP_INFO mesh_z_step_info[3] = {
        {0.01, "0.01 mm", img_move_step_1},
        {0.05, "0.05 mm", img_move_step_2},
        {0.1, "0.10 mm", img_move_step_3}
};

#define FILELD_SIZE         180
#define FILELD_OFFSET_X     20
#define FILELD_OFFSET_Y     0

void MeshLevelingUI::on_message(WM_MESSAGE * pMsg) {
    int calc_size = FILELD_SIZE + 1;
    switch (pMsg->MsgId) {
        case WM_PAINT: {
            GUI_SetColor(0x000000);
            GUI_FillRect(FILELD_OFFSET_X, FILELD_OFFSET_Y, FILELD_OFFSET_X + this->dx * GRID_MAX_POINTS_X + 1, FILELD_OFFSET_Y + this->dy * GRID_MAX_POINTS_Y + 1);
            GUI_SetColor(0x00ff00);
            int _max=FILELD_OFFSET_Y + GRID_MAX_POINTS_Y * this->dy + 1;
            for (int i=0;i<=GRID_MAX_POINTS_X;i++) {
                int x = FILELD_OFFSET_X + i * this->dx;
                GUI_DrawLine(x, FILELD_OFFSET_Y, x, _max);
            }
            _max=FILELD_OFFSET_X + GRID_MAX_POINTS_X * this->dx + 1;
            for (int i=0;i<=GRID_MAX_POINTS_Y;i++) {
                int y = FILELD_OFFSET_Y + i * this->dy;
                GUI_DrawLine(FILELD_OFFSET_X, y, _max, y);
            }
            if (mbl_probe_index>0)
                this->fill_cell(mbl_probe_index - 1);
            break;
        }
    }


    Widget::on_message(pMsg);
}


UI_STEP_INFO * MeshLevelingUI::getStepInfo() {
    return &mesh_z_step_info[this->step_selector];
}


void MeshLevelingUI::updateButtons() {
    if ((this->wait_change) && (this->mesh_step==mbl_probe_index)) {
        if (mbl_probe_index==-1)
            this->updateStateButton(&ui.point, 0, "Waiting");
        else
            this->updateStateButton(&ui.point, 0, "Moving");
    } else {
        this->wait_change = 0;
        if (this->mesh_step != mbl_probe_index) {
            this->mesh_step = mbl_probe_index;
            GUI_RECT rect;
            rect.x0 = FILELD_OFFSET_X;
            rect.y0 = FILELD_OFFSET_Y;
            rect.x1 = FILELD_OFFSET_X + this->dx * GRID_MAX_POINTS_X + 1;
            rect.y1 = FILELD_OFFSET_Y + this->dy * GRID_MAX_POINTS_Y + 1;
            WM_InvalidateRect(this->hWnd, &rect);
            if (mbl_probe_index == -1) {
                this->updateStateButton(&ui.point, 0, "Start");
            } else if (mbl_probe_index == 0) {
                this->updateStateButton(&ui.point, 0, "Preparing");
            } else {
                //fill_cell(this->mesh_step - 1);
                sprintf(ui_buf1_100, "Point %d", mbl_probe_index);
                this->updateStateButton(&ui.point, 0, ui_buf1_100);
            }
        }
    }
    sprintf(ui_buf1_100,"%.3f",current_position[Z_AXIS]);
    this->updateStateButton(&ui.z, 0, ui_buf1_100);
}

void MeshLevelingUI::refresh_1s() {
    this->drawXYZ();
    this->updateButtons();
}

void MeshLevelingUI::createControls() {
    this->dx = FILELD_SIZE / GRID_MAX_POINTS_X;
    this->dy = FILELD_SIZE / GRID_MAX_POINTS_Y;
    memset(&this->ui, 0, sizeof(this->ui));
    this->ui.ret = this->createButtonRet();
    this->ui.step = this->createButtonAt(3, 0, 0, 0);
    #define _col(ph_x) (96*ph_x) + 20
    #define _y 190

    this->createStateButton(_col(2), 0, &this->ui.z, img_state_z, 0);
    this->createStateButton(_col(2), 40, &this->ui.point, img_state_mesh_point, 0);

    this->ui.zInc = this->create96x80Button(_col(2), _y - 80, img_page_up);
    this->ui.zDec = this->create96x80Button(_col(2), _y, img_page_down);

    this->ui.disable = this->create96x80Button(_col(0), _y, img_mesh_delete);
    this->ui.action = this->create96x80Button(_col(1), _y, img_print_resume);

    UI_STEP_INFO * step_info = this->getStepInfo();
    this->updateButton(this->ui.step, step_info->picture, step_info->title);

    this->updateButtons();
}

void MeshLevelingUI::do_move(char direction) {
    UI_STEP_INFO * step_info = this->getStepInfo();
    shUI::pushGcode("G91");
    sprintf(ui_buf1_100, "G1 Z%3.2f F%d", direction * step_info->size, gCfgItems.moveSpeed);
    shUI::pushGcode(ui_buf1_100);
    shUI::pushGcode("G90");
}

void MeshLevelingUI::fill_cell(signed char step, int colour) {
    signed char px, py;
    mbl.zigzag(step, px, py);
    py = GRID_MAX_POINTS_Y - py - 1;
    GUI_SetColor(colour);
    GUI_FillRect(
            FILELD_OFFSET_X + 1 + 2 + this->dx * px,
            FILELD_OFFSET_Y + 1 + 2 + this->dy * py,
            FILELD_OFFSET_X + this->dx * (px + 1) - 3,
            FILELD_OFFSET_Y + this->dy * (py + 1) - 3);
}

void MeshLevelingUI::clear_cell(signed char step) {
    fill_cell(step, 0x000000);
}

void MeshLevelingUI::fill_cell(signed char step) {
    fill_cell(step, 0x00FF00);
}

void MeshLevelingUI::do_mesh_step() {
    this->wait_change = 1;
    this->mesh_step=mbl_probe_index;
    if (mbl_probe_index==-1) {
        if (!(axis_homed[X_AXIS] && axis_homed[Y_AXIS] && axis_homed[Z_AXIS]))
            shUI::pushGcode("G28");
        shUI::pushGcode("G29 S1");
    } else
        shUI::pushGcode("G29 S2");
    this->updateButtons();
}

void MeshLevelingUI::on_action_dialog(u8 action, u8 dialog_id) {
    confirm_dialog_ui.hide();
    if (action==UI_BUTTON_OK) {
        this->wait_change = 1;
        this->mesh_step=mbl_probe_index;
        shUI::pushGcode("G29 S5");
    }
    this->show();
}

void MeshLevelingUI::on_button(UI_BUTTON hBtn) {
    if (hBtn == this->ui.step) {
        if (++this->step_selector >= 3)
            this->step_selector = 0;
        UI_STEP_INFO * step_info = this->getStepInfo();
        this->updateButton(this->ui.step, step_info->picture, step_info->title);
    } else if (hBtn == this->ui.zInc) {
        this->do_move(1);
    } else if (hBtn == this->ui.zDec) {
        this->do_move(-1);
    } else if (hBtn == this->ui.ret) {
        this->mesh_step = -2;
        this->action_back();
    } else if (hBtn == this->ui.action) {
        this->do_mesh_step();
    } else if (hBtn == this->ui.disable) {
        this->hide();
        confirm_dialog_ui.show(lang_str.dialog.confirm_delete_mesh, this, 0);
    }
}