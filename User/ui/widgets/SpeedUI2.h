//
// Created by shubin on 30.12.2020.
//

#ifndef WORK_SPEEDUI2_H
#define WORK_SPEEDUI2_H

#include "StdWidget.h"

typedef struct {
    STATE_BUTTON state;
    UI_BUTTON plus;
    UI_BUTTON minus;
} SPEED_UI_SET;


typedef struct{
    SPEED_UI_SET mooving;
    SPEED_UI_SET ext1;
    SPEED_UI_SET ext2;
    SPEED_UI_SET fan;
    UI_SELECTOR step;
    UI_BUTTON ret;
} SPEED_UI_CONTROLS2;

class SpeedUI2: public StdWidgetWithCalc {
private:
    SPEED_UI_CONTROLS2 ui;
    void updateStepButton();
    void updateStateButtons();
protected:
    UI_BUTTON createButtonAt90x60(int phx, int phy, const char * picture);
    virtual void createControls();
    virtual void on_button(UI_BUTTON hBtn);
    virtual unsigned char on_repeatable_button(UI_BUTTON hBtn);
    virtual unsigned char is_repeated_button(UI_BUTTON hBtn);
    void refresh_05();
    void refresh_1s();
    virtual void setValue(unsigned char id, double value);
public:
    virtual const char * getTitle() {return lang_str.ui_title_speed;};
    SpeedUI2() : StdWidgetWithCalc(CHANGE_SPEED_UI) {};
};

extern SpeedUI2 speed_ui_2;

#endif //WORK_SPEEDUI2_H
