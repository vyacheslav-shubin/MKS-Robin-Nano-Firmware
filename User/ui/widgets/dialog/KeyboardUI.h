//
// Created by shubin on 03.06.2020.
//

#ifndef WORK_KEYBOARDUI_H
#define WORK_KEYBOARDUI_H

#include "ActionDialog.h"

typedef enum {
    KB_FIRST = 1,
    KB_LC = 1,
    KB_UC = 2,
    KB_DIG = 4,
    KB_SYMB = 8,
    KB_GCODE = 16,
    KB_END = 32,
    KB_STD = KB_LC + KB_UC + KB_DIG + KB_SYMB,
    KB_GCODE_SET = KB_GCODE + KB_UC,
    KB_ALL = KB_END - 1,
};

typedef struct {
    UI_BUTTON button[28];
    UI_TEXT input;
    UI_BUTTON ok;
    UI_BUTTON mode;
    UI_BUTTON cancel;
} KEYBOARD_UI_CONTROLS;

class KeyboardUI : public ActionDialog {
private:
    char * title;
    char * buf;
    unsigned char maxSize;
    unsigned char cursor;
    KEYBOARD_UI_CONTROLS ui;
    unsigned char charSet = KB_LC;
    unsigned char enabledCharsets = KB_STD;
    void updateCharsFrom(const char start);
    void updateCharsSeq(const char * seq);
    void updateChars();
    void addChars(char index);
protected:
    virtual void createFrame();
    virtual void createControls();
    virtual void on_button(UI_BUTTON hBtn);
    virtual void refresh_05();
    virtual void refresh_1s();
public:
    virtual void on_message(WM_MESSAGE * pMsg);

    void show(char * title, char * buf, unsigned char maxSize, unsigned char enabledCharsets, unsigned char charset,  ActionDialogCallback * callback, u8 id, Widget * caller = 0) {
            this->title = title;
            this->buf = buf;
            this->maxSize = maxSize;
            this->cursor = buf ? strlen(buf) : 0;
            this->enabledCharsets = (enabledCharsets & KB_ALL)?enabledCharsets:KB_STD;
            this->charSet = charset;
            ActionDialog::show(callback, id, caller);
    }

    void show(char * title, char * buf, unsigned char maxSize, ActionDialogCallback * callback, u8 id, Widget * caller = 0) {
        this->show(title, buf, maxSize, KB_STD, KB_LC,  callback, id, caller);
    }
    KeyboardUI() : ActionDialog() {};
};


extern KeyboardUI keyboard_ui;


#endif //WORK_KEYBOARDUI_H
