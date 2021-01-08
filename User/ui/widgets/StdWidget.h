/*
 * StdWidget.h
 *
 *  Created on: May 8, 2020
 *      Author: shubin
 */

#ifndef USER_UI_WIDGETS_STDWIDGET_H_
#define USER_UI_WIDGETS_STDWIDGET_H_

#include "widgets/dialog/CalculatorDialogUI.h"
#include "Widget.h"
#include "ui_tools.h"

#define ui_std_plus_button(x, y) this->createButtonAt((x), (y), img_plus, lang_str.plus)
#define ui_std_minus_button(x, y) this->createButtonAt((x), (y), img_minus, lang_str.minus)
#define ui_std_bed_state_button(x, y, dest) this->createStateButton((x), (y), dest, img_state_bed, 0);
#define ui_std_speed_state_button(x, y, dest) this->createStateButton((x), (y), dest, img_state_speed, 0);
#define ui_std_ext1_state_button(x, y, dest) this->createStateButton((x), (y), dest, img_state_extruder1, 0)
#define ui_std_ext2_state_button(x, y, dest) this->createStateButton((x), (y), dest, img_state_extruder2, 0)
#define ui_std_fun_state_button(x, y, dest) this->createStateButton((x), (y), dest, img_fan_state0, 0)

typedef enum {
    PREHEAT_CALC_ID_BED = 0,
    PREHEAT_CALC_ID_SPR1,
    PREHEAT_CALC_ID_SPR2,
    PREHEAT_CALC_ID_LAST
} PREHEAT_CALC_ID;

extern void preheat_set_calc_value(PREHEAT_CALC_ID id, double value);


class StdWidget : public Widget{
protected:
	UI_BUTTON createButton(int x, int y, const char * picture, const char * title);
	UI_BUTTON createButtonAt(int phx, int phy, const char * picture, const char * title);
    UI_BUTTON createButtonRet();
    UI_BUTTON createButtonAt90x60(int phx, int phy, const char * picture);
	void action_back();
    void actionFilamentChangeParking();
    void actionBedParking();
    void action_leveling();
	void updateFanState(STATE_BUTTON * stateButton);
public:
	StdWidget(DISP_STATE id) : Widget(id) {};
};

class StdWidgetWithCalc : public StdWidget, public CalculatorDialogCallback {
protected:
    virtual void setValue(unsigned char id, double value) = 0;
    void calculator(const char * label, const char * sub_label, double value, unsigned char id);
    void calculator(const char * label,  double value, unsigned char id) {
        this->calculator(label, 0, value, id);
    }
public:
    virtual void on_calculator(unsigned char action, double result, unsigned char dialog_id) {
        calculator_dialog_ui.hide();
        if (action==UI_BUTTON_OK)
            this->setValue(dialog_id, result);
        this->show();
    }
    StdWidgetWithCalc(DISP_STATE id) : StdWidget(id) {};
};

#endif /* USER_UI_WIDGETS_STDWIDGET_H_ */
