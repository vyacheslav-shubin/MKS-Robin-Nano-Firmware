//
// Created by shubin on 16.06.2020.
//

#ifndef WORK_MESHLEVELINGUI_H
#define WORK_MESHLEVELINGUI_H

#include "StdWidget.h"


typedef struct {
    UI_BUTTON ret;
    UI_BUTTON zInc;
    UI_BUTTON zDec;
    UI_BUTTON step;
    UI_BUTTON action;
    UI_BUTTON disable;
    STATE_BUTTON z;
    STATE_BUTTON point;
} MESH_LEVELING_UI_CONTROLS;

class MeshLevelingUI : public StdWidget, public ActionDialogCallback{
private:
    int dx = 0;
    int dy = 0;
    char step_selector = 0;
    int mesh_step;
    unsigned char wait_change = 0;
    MESH_LEVELING_UI_CONTROLS ui;
    void updateButtons();
    UI_STEP_INFO * getStepInfo();
    void do_move(char direction);
    void do_mesh_step();
    void fill_cell(signed char step, int colour);
    void clear_cell(signed char step);
    void fill_cell(signed char step);
protected:
    virtual void createControls();
    virtual void on_button(UI_BUTTON hBtn);
    virtual void refresh_1s();
public:
    virtual void on_action_dialog(u8 action, u8 dialog_id);
    void on_message(WM_MESSAGE * pMsg);
    virtual const char * getTitle() {return lang_str.ui_title_mesh_leveling;};
    MeshLevelingUI() : StdWidget(MESHLEVELING_UI){};
};


extern MeshLevelingUI mesh_leveling_ui;

#endif //WORK_MESHLEVELINGUI_H
