/*
 * FilamentUI.h
 *
 *  Created on: May 13, 2020
 *      Author: shubin
 */

#ifndef USER_UI_WIDGETS_FILAMENTUI_H_
#define USER_UI_WIDGETS_FILAMENTUI_H_

#include "StdWidget.h"
#include "dialog/ConfirmDialogUI.h"

typedef struct{
	UI_BUTTON load;
	UI_BUTTON unload;
	UI_BUTTON selector;
	UI_BUTTON size;
	UI_BUTTON speed;
    UI_BUTTON preheat;
    UI_BUTTON home_t;
	STATE_BUTTON ext1;
	STATE_BUTTON ext2;
	UI_BUTTON back;
} FILAMENT_UI_CONTROLS;

class FilamentUI: public StdWidget, public ActionDialogCallback {
private:
	FILAMENT_UI_CONTROLS ui;
	char current_extruder = 0;
	char current_step = 2;
	char current_speed = 2;
    void fix_temperature(unsigned char extruder);
	void doFilament(char direction, unsigned char confirm = 1);
	void updateExtruderSelector();
	void updateStepSelector();
	void updateSpeedSelector();
	short getWantedTemperature(char direction);
protected:
	virtual void createControls();
	virtual void on_button(UI_BUTTON hBtn);
	virtual void refresh_1s();
	virtual void refresh_05();
public:
    virtual void on_action_dialog(u8 action, u8 dialog_id);
	virtual const char * getTitle() {return lang_str.ui_title_filament;};
	FilamentUI() : StdWidget(FILAMENT_UI) {};
};

extern FilamentUI filament_ui;

#endif /* USER_UI_WIDGETS_FILAMENTUI_H_ */
