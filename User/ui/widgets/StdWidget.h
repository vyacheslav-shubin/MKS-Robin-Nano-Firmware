/*
 * StdWidget.h
 *
 *  Created on: May 8, 2020
 *      Author: shubin
 */

#ifndef USER_UI_WIDGETS_STDWIDGET_H_
#define USER_UI_WIDGETS_STDWIDGET_H_

#include "Widget.h"
#include "ui_tools.h"

#define ui_std_plus_button(x, y) this->createButtonAt((x), (y), img_plus, lang_str.plus)
#define ui_std_minus_button(x, y) this->createButtonAt((x), (y), img_minus, lang_str.minus)
#define ui_std_bed_state_button(x, y, dest) this->createStateButton((x), (y), dest, img_state_bed, 0);
#define ui_std_speed_state_button(x, y, dest) this->createStateButton((x), (y), dest, img_state_speed, 0);
#define ui_std_ext1_state_button(x, y, dest) this->createStateButton((x), (y), dest, img_state_extruder1, 0)
#define ui_std_ext2_state_button(x, y, dest) this->createStateButton((x), (y), dest, img_state_extruder2, 0)


class StdWidget : public Widget{
protected:
	UI_BUTTON createButton(int x, int y, const char * picture, const char * title);
	UI_BUTTON createButtonAt(int phx, int phy, const char * picture, const char * title);
	UI_BUTTON createButtonRet();
	void action_back();
    void  actionFilamentChangeParking();
    void action_leveling();
	void updateFanState(STATE_BUTTON * stateButton);
public:
	StdWidget(DISP_STATE id) : Widget(id) {};
};

#endif /* USER_UI_WIDGETS_STDWIDGET_H_ */
