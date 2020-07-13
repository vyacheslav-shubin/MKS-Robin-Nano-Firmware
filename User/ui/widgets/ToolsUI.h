/*
 * ToolsUI.h
 *
 *  Created on: May 9, 2020
 *      Author: shubin
 */

#ifndef USER_UI_WIDGETS_TOOLSUI_H_
#define USER_UI_WIDGETS_TOOLSUI_H_

#include "StdWidget.h"

typedef struct {
	UI_BUTTON	preheat;
	UI_BUTTON 	move;
	UI_BUTTON 	home;
	UI_BUTTON 	leveling;
	UI_BUTTON 	filament;
	UI_BUTTON 	fan;
	UI_BUTTON 	more;
	UI_BUTTON 	files;
	UI_BUTTON 	ret;
}TOOLS_UI_CONTROLS;

class ToolsUI : public StdWidget{
private:
	TOOLS_UI_CONTROLS ui;
protected:
	virtual void createControls();
	virtual void on_button(UI_BUTTON hBtn);
public:
	virtual const char * getTitle() {return lang_str.ui_title_tools;};
	ToolsUI() : StdWidget(TOOL_UI){};
};

extern ToolsUI tools_ui;

#endif /* USER_UI_WIDGETS_TOOLSUI_H_ */
