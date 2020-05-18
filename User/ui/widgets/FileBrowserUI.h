/*
 * FileBrowserUI.h
 *
 *  Created on: May 15, 2020
 *      Author: shubin
 */

#ifndef USER_UI_WIDGETS_FILEBROWSERUI_H_
#define USER_UI_WIDGETS_FILEBROWSERUI_H_

#include "StdWidget.h"

class UIFileBrouser;

typedef struct{
	UI_BUTTON button;
	char fileName[13];
	int previewOffset;
	char withPreview;
	char isDirectory;
} UI_FILE_BUTTON;

typedef struct{
	UI_BUTTON prev;
	UI_BUTTON next;
	UI_BUTTON back;
	UI_FILE_BUTTON files[6];
} FILE_BROWSER_UI_CONTROLS;

class FileBrowserUI: public StdWidget {
private:
	FILE_BROWSER_UI_CONTROLS ui;
	void doBack();
	void doNext();
	void doPrev();
	void doButton(char index);
	void clearButtons();
	void drawPreview();
protected:
	virtual void createControls();
	virtual void on_button(UI_BUTTON hBtn);
public:
	virtual char * getTitle() {return lang_str.ui_title_file_browser;};
	FileBrowserUI(): StdWidget(FILE_BROWSER_UI) {};
	friend	UIFileBrouser;
};

extern FileBrowserUI file_browser_ui;

#endif /* USER_UI_WIDGETS_FILEBROWSERUI_H_ */
