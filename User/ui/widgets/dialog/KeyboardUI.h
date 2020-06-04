//
// Created by shubin on 03.06.2020.
//

#ifndef WORK_KEYBOARDUI_H
#define WORK_KEYBOARDUI_H

#include "ActionDialog.h"

typedef enum {
    KB_LC = 0,
    KB_UC,
    KB_DIG,
    KB_SYMB,
    KB_END,
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
    char maxSize;
    char cursor;
    KEYBOARD_UI_CONTROLS ui;
    unsigned char viewState = KB_LC;
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

    void show(char * title, char * buf, char maxSize, ActionDialogCallback * callback, u8 id, Widget * caller = 0) {
        this->title = title;
        this->buf = buf;
        this->maxSize = maxSize;
        this->cursor = buf ? strlen(buf) : 0;
        ActionDialog::show(callback, id, caller);
    }
    KeyboardUI() : ActionDialog() {};
};


extern KeyboardUI keyboard_ui;


#endif //WORK_KEYBOARDUI_H