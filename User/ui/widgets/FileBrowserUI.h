/*
 * FileBrowserUI.h
 *
 *  Created on: May 15, 2020
 *      Author: shubin
 */

#ifndef USER_UI_WIDGETS_FILEBROWSERUI_H_
#define USER_UI_WIDGETS_FILEBROWSERUI_H_

#include "StdWidget.h"
#include "ConfirmDialogUI.h"

#define MAX_FILE_NAME_SIZE	100

extern char * get_long_file_name(char * fileName);
extern void delete_file(char * fileName);

class Browser{
private:
	FATFS fs;
	char is_wanted(FILINFO * fi);
protected:
	virtual char onFile(FILINFO * fi) {return 0;};
public:
	char curent_dir[MAX_FILE_NAME_SIZE];
	int dir_level = 0;
	virtual void lookup();
	virtual void close();
	virtual void pushDirectory(char * name);
	virtual void popDirectory();
	Browser() {this->close();};
	virtual ~Browser() {}
};

class UIFileBrouser;

typedef struct{
	UI_BUTTON button;
	char fileName[13];
	char isDirectory;
	PREVIEW_META previewMeta;
} UI_FILE_BUTTON;

typedef struct{
	UI_BUTTON prev;
	UI_BUTTON next;
	UI_BUTTON back;
	UI_FILE_BUTTON files[6];
	u8 selectedButton;
} FILE_BROWSER_UI_CONTROLS;

class FileBrowserUI: public StdWidget, public ActionDialogCallback {
private:
	FILE_BROWSER_UI_CONTROLS ui;
	void doBack();
	void doNext();
	void doPrev();
	void doButton(char index);
	void clearButtons();
	void drawPreview();
	void activatePrint(u8 index);
protected:
	virtual void createControls();
	virtual void on_button(UI_BUTTON hBtn);
	virtual void on_action_dialog(u8 action, u8 dialog_id);
public:
	virtual const char * getTitle() {return lang_str.ui_title_file_browser;};
	FileBrowserUI(): StdWidget(FILE_BROWSER_UI) {};
	friend	UIFileBrouser;
};

extern FileBrowserUI file_browser_ui;

#endif /* USER_UI_WIDGETS_FILEBROWSERUI_H_ */
