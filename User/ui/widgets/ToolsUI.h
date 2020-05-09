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
	BUTTON_Handle	preheat;
	BUTTON_Handle 	extrude;
	BUTTON_Handle 	move;
	BUTTON_Handle 	home;
	BUTTON_Handle 	leveling;
	BUTTON_Handle 	filament;
	BUTTON_Handle 	more;
	BUTTON_Handle 	ret;
}TOOLS_UI_CONTROLS;

class ToolsUI : public StdWidget{
private:
	TOOLS_UI_CONTROLS buttons;
protected:
	virtual void createControls();
	virtual void on_button(WM_HWIN hBtn);
public:
	ToolsUI() : StdWidget(TOOL_UI){};
};

extern ToolsUI tools_ui;

#endif /* USER_UI_WIDGETS_TOOLSUI_H_ */
