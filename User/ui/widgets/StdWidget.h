/*
 * StdWidget.h
 *
 *  Created on: May 8, 2020
 *      Author: shubin
 */

#ifndef USER_UI_WIDGETS_STDWIDGET_H_
#define USER_UI_WIDGETS_STDWIDGET_H_

#include "Widget.h"

typedef struct {
	BUTTON_Handle button;
	TEXT_Handle	label;
} STATE_BUTTON;

class StdWidget : public Widget{
protected:
	void createStateButton(int x, int y, STATE_BUTTON * btn, const char * picture, const char * title);
	void updateStateButton(STATE_BUTTON * btn, const char * img, const char * title);
	BUTTON_Handle createButton(int x, int y, const char * picture, const char * title);
	BUTTON_Handle createButtonAt(int phx, int phy, const char * picture, const char * title);
	BUTTON_Handle createButtonRet();
	void action_back();
	void action_leveling();
	void draw_xyz();
public:
	StdWidget(DISP_STATE id) : Widget(id) {};
};

#endif /* USER_UI_WIDGETS_STDWIDGET_H_ */
