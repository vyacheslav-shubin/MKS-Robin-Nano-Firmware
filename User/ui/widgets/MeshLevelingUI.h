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
    STATE_BUTTON z;
    STATE_BUTTON point;
} MESH_LEVELING_UI_CONTROLS;

class MeshLevelingUI : public StdWidget{
private:
    char step = 0;
    char mesh_step = 0;
    MESH_LEVELING_UI_CONTROLS ui;
    void updateButtons();
    UI_STEP_INFO * getStepInfo();
    void do_move(char direction);
    void do_mesh_step();
protected:
    virtual void createControls();
    virtual void on_button(UI_BUTTON hBtn);
    virtual void refresh_1s();
public:
    virtual const char * getTitle() {return lang_str.ui_title_mesh_leveling;};
    MeshLevelingUI() : StdWidget(MESHLEVELING_UI){};
};


extern MeshLevelingUI mesh_leveling_ui;

#endif //WORK_MESHLEVELINGUI_H
