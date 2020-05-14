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
	UI_BUTTON 	extrusion;
	UI_BUTTON 	babystep;
	UI_BUTTON 	more;
	UI_BUTTON 	more;
	UI_BUTTON 	ret;
}TOOLS_UI_CONTROLS;

class PrintingToolsUI : public StdWidget {
public:
	PrintingToolsUI();
	virtual ~PrintingToolsUI();
};

#endif /* USER_UI_WIDGETS_PRINTINGTOOLSUI_H_ */
