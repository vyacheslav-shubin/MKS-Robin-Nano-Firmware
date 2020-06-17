/*
 * FileInfoUI.h
 *
 *  Created on: May 19, 2020
 *      Author: shubin
 */

#ifndef USER_UI_WIDGETS_FILEINFOUI_H_
#define USER_UI_WIDGETS_FILEINFOUI_H_

#include "FileInfoBaseUI.h"
#include "ConfirmDialogUI.h"

typedef struct{
	UI_BUTTON run;
	UI_BUTTON cancel;
	UI_BUTTON del;
	UI_BUTTON tools;
	UI_TEXT file_size;
	UI_TEXT layers;
	UI_TEXT time;
	UI_TEXT filament;
	UI_TEXT mmx;
	UI_TEXT mmy;
    UI_TEXT mmz;
	char preview_done;
	unsigned char info_updated;
} FILE_INFO_UI_CONTROLS;

class FileInfoUI  : public FileInfoBaseUI, public ActionDialogCallback{
private:
	FILE_INFO_UI_CONTROLS ui;
protected:
	virtual void refresh();
	virtual void on_button(UI_BUTTON hBtn);
	virtual void createControls();
	virtual void on_action_dialog(u8 action, u8 dialog_id);
	void update();
public:
	virtual const char * getTitle();
	FileInfoUI() : FileInfoBaseUI(FILE_INFO_UI) {};
};

extern FileInfoUI file_info_ui;

#endif /* USER_UI_WIDGETS_FILEINFOUI_H_ */
