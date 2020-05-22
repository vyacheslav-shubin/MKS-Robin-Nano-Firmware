/*
 * MoreUI.h
 *
 *  Created on: May 15, 2020
 *      Author: shubin
 */

#ifndef USER_UI_WIDGETS_MOREUI_H_
#define USER_UI_WIDGETS_MOREUI_H_

#include "StdWidget.h"

typedef struct {
	UI_BUTTON tools[6];
	UI_BUTTON ret;
} MORE_UI_CONTROLS;

class MoreUI  : public StdWidget {
private:
	MORE_UI_CONTROLS ui;
protected:
	virtual void createControls();
	virtual void on_button(UI_BUTTON hBtn);
public:
	virtual const char * getTitle() {return lang_str.ui_title_more;};
	MoreUI() : StdWidget(MORE_UI) {};
};

extern MoreUI more_ui;


#endif /* USER_UI_WIDGETS_MOREUI_H_ */
