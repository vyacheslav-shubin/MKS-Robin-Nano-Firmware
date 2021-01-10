/*
 * PrintingToolsUI.h
 *
 *  Created on: May 13, 2020
 *      Author: shubin
 */

#ifndef USER_UI_WIDGETS_PRINTINGTOOLSUI_H_
#define USER_UI_WIDGETS_PRINTINGTOOLSUI_H_

#include "StdWidget.h"

typedef struct {
	UI_BUTTON	preheat;
	UI_BUTTON 	fan;
    UI_BUTTON 	speed;
    UI_BUTTON 	std_preheat;
	UI_BUTTON 	babystep;
	UI_BUTTON 	move;

	UI_BUTTON 	filament;

	UI_BUTTON 	ret;
}PRINTING_TOOLS_UI_CONTROLS;

class PrintingToolsUI : public StdWidget {
private:
	PRINTING_TOOLS_UI_CONTROLS ui;
	unsigned char current_preset = 0;
protected:
	virtual void createControls();
	virtual void on_button(UI_BUTTON hBtn);
public:
	virtual const char * getTitle() {return lang_str.ui_title_printing_tools;};
	PrintingToolsUI() : StdWidget(OPERATE_UI) {};
};

extern PrintingToolsUI printing_tools_ui;

#endif /* USER_UI_WIDGETS_PRINTINGTOOLSUI_H_ */
