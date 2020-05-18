/*
 * LanguageUI.h
 *
 *  Created on: May 18, 2020
 *      Author: shubin
 */

#ifndef USER_UI_WIDGETS_LANGUAGEUI_H_
#define USER_UI_WIDGETS_LANGUAGEUI_H_

#include "StdWidget.h"

typedef struct {
	UI_BUTTON button;
	unsigned char lang;
} UI_LANGUAGE_BUTTON;

typedef struct {
	UI_LANGUAGE_BUTTON langs[5];
	UI_BUTTON back;
} LANGUAGE_UI_CONTROLS;

class LanguageUI : public StdWidget{
private:
	LANGUAGE_UI_CONTROLS ui;
protected:
	virtual void createControls();
	virtual void on_button(UI_BUTTON hBtn);
public:
	LanguageUI(): StdWidget(LANGUAGE_UI){}
};

extern LanguageUI language_ui;

#endif /* USER_UI_WIDGETS_LANGUAGEUI_H_ */
