/*
 * HomeUI.h
 *
 *  Created on: May 13, 2020
 *      Author: shubin
 */

#ifndef USER_UI_WIDGETS_HOMEUI_H_
#define USER_UI_WIDGETS_HOMEUI_H_

#include "StdWidget.h"

typedef struct{
	UI_BUTTON x;
	UI_BUTTON y;
    UI_BUTTON z;
    UI_BUTTON t;
    UI_BUTTON b;
	UI_BUTTON all;
	UI_BUTTON stop;
    UI_TEXT change;
	UI_BUTTON back;
} HOME_UI_CONTROLS;

class HomeUI: public StdWidget {
private:
	HOME_UI_CONTROLS ui;
protected:
	virtual void createControls();
	virtual void on_button(UI_BUTTON hBtn);
	virtual void refresh_1s();
public:
	virtual const char * getTitle() {return lang_str.ui_title_home;};
	HomeUI() : StdWidget(ZERO_UI) {};
};

extern HomeUI home_ui;

#endif /* USER_UI_WIDGETS_HOMEUI_H_ */
