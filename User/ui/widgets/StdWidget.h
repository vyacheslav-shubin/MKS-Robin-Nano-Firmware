/*
 * StdWidget.h
 *
 *  Created on: May 8, 2020
 *      Author: shubin
 */

#ifndef USER_UI_WIDGETS_STDWIDGET_H_
#define USER_UI_WIDGETS_STDWIDGET_H_

#include "Widget.h"

class StdWidget : public Widget{
protected:
	BUTTON_Handle createButton(int x, int y, const char * picture, const char * title);
	BUTTON_Handle createButtonAt(int phx, int phy, const char * picture, const char * title);
	BUTTON_Handle createButtonRet();
	void action_back();
	void action_leveling();
public:
	StdWidget(DISP_STATE id) : Widget(id) {};
};

#endif /* USER_UI_WIDGETS_STDWIDGET_H_ */
