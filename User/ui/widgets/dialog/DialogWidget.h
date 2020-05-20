/*
 * DialogWidget.h
 *
 *  Created on: May 20, 2020
 *      Author: shubin
 */

#ifndef USER_UI_WIDGETS_DIALOG_DIALOGWIDGET_H_
#define USER_UI_WIDGETS_DIALOG_DIALOGWIDGET_H_

#include "Widget.h"

#define DIALOG_X		(60)
#define DIALOG_Y		(40)
#define DIALOG_WIDTH	(LCD_WIDTH - DIALOG_X * 2)
#define DIALOG_HEIGHT	(LCD_HEIGHT - DIALOG_Y * 2)

#define UI_BUTTON_OK		0
#define UI_BUTTON_CANCEL	1

typedef void (*ui_dialog_callback)(unsigned char);

class DialogWidget : public Widget {
protected:
	void createDialogDecoration(const char * img, const char * title);
	virtual void createControls() {};
	virtual void createWindow();
public:
	void hide();
	DialogWidget() : Widget(CUSTOM_DIALOG_UI) {};
};

extern DialogWidget dialog_widget;


#endif /* USER_UI_WIDGETS_DIALOG_DIALOGWIDGET_H_ */
