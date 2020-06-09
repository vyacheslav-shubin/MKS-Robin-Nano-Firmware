//
// Created by shubin on 03.06.2020.
//

#include "KeyboardUI.h"
#include "ui_tools.h"
#include "Application.h"


KeyboardUI keyboard_ui;
const char * digital_key_value="1234567890-|:;()$&@\"";
const char * symbol_key_value = ",?!\'[]#{}%^*+=_\\/~<>";

#define VALUE_DISP_HEIGHT 60
#define KEY_WIDTH  66
#define KEY_HEIGHT  42

#define KEY_LINE_WIDTH  2
#define PADDING  (LCD_WIDTH - (KEY_WIDTH+KEY_LINE_WIDTH) * 7 - KEY_LINE_WIDTH)/2

extern GUI_CONST_STORAGE GUI_FONT GUI_FontHelvetica26;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontHelvetica36;

void KeyboardUI::on_message(WM_MESSAGE * pMsg) {
    switch (pMsg->MsgId) {
        case WM_PAINT: {
            GUI_SetColor(0xf85448);

            GUI_FillRect(PADDING, 0, LCD_WIDTH - PADDING - 1,
                         0 + KEY_LINE_WIDTH - 1);
            for (int i=0;i<5;i++) {
                int y = VALUE_DISP_HEIGHT + (KEY_HEIGHT + KEY_LINE_WIDTH) * i;
                GUI_FillRect(PADDING, y, LCD_WIDTH - PADDING - 1,
                             y + KEY_LINE_WIDTH - 1);
            }

            for (int i=0;i<8;i++) {
                int x = PADDING + (KEY_WIDTH + KEY_LINE_WIDTH)*i;
                int y1 = (i==0||i==7)?0:VALUE_DISP_HEIGHT;
                GUI_FillRect(x, y1, x+KEY_LINE_WIDTH - 1, VALUE_DISP_HEIGHT + KEY_LINE_WIDTH + (KEY_HEIGHT+KEY_LINE_WIDTH) * 4 - 1);
            }
        }
    }
    ActionDialog::on_message(pMsg);
}

void KeyboardUI::createFrame() {
    GUI_Clear();
    ui_app.drawTitle();
    this->hWnd = WM_CreateWindow(0, 0, LCD_WIDTH, LCD_HEIGHT, WM_CF_SHOW, widget_callback, 0);
}

#define BUTTON_PRESS_BK_COLOR  0xF8FAF8
#define BUTTON_PRESS_TEXT_COLOR  0x000000


UI_BUTTON create_keyboard_button(unsigned char px, unsigned char py, UI_WND wnd) {
    UI_BUTTON button = BUTTON_CreateEx(
            PADDING + KEY_LINE_WIDTH + (KEY_WIDTH + KEY_LINE_WIDTH) * px,
            VALUE_DISP_HEIGHT+KEY_LINE_WIDTH+(KEY_HEIGHT+KEY_LINE_WIDTH) * py,
            KEY_WIDTH, KEY_HEIGHT,
            wnd, BUTTON_CF_SHOW, 0, 0);

    BUTTON_SetBkColor(button, BUTTON_CI_PRESSED, BUTTON_PRESS_BK_COLOR);
    BUTTON_SetBkColor(button, BUTTON_CI_UNPRESSED, gCfgItems.btn_color);
    BUTTON_SetTextColor(button, BUTTON_CI_PRESSED, BUTTON_PRESS_TEXT_COLOR);
    BUTTON_SetTextColor(button, BUTTON_CI_UNPRESSED, gCfgItems.btn_textcolor);
    BUTTON_SetTextAlign(button,GUI_TA_HCENTER|GUI_TA_VCENTER);
    BUTTON_SetFont(button,&GUI_FontHelvetica26);
    BUTTON_SetBmpFileName(button,0,1);
    return button;
}

void KeyboardUI::createControls() {
    memset(&this->ui, 0, sizeof(this->ui));
    for (unsigned char i=0;i<28;i++)
        this->ui.button[i] = create_keyboard_button(i%7, i/7, this->hWnd);
    this->ui.ok = this->create96x80Button(LCD_WIDTH-96, LCD_HEIGHT-80, img_ok);
    this->ui.cancel = this->create96x80Button(LCD_WIDTH-96-100, LCD_HEIGHT-80, img_cancel);
    this->ui.mode = this->create96x80Button(0, LCD_HEIGHT-80, img_Aa);
    this->ui.input = this->createText(10, 10, LCD_WIDTH - 20, VALUE_DISP_HEIGHT - 20, "Hello world. This is password");
    TEXT_SetFont(this->ui.input, &GUI_FontHelvetica26);
    this->updateChars();
}

void KeyboardUI::updateCharsFrom(const char start) {
    for (unsigned char i = 0; i < 26; i++) {
        ui_buf1_100[0] = start + i;
        ui_buf1_100[1] = 0;
        BUTTON_SetText(this->ui.button[i], ui_buf1_100);
    }
}

void KeyboardUI::updateCharsSeq(const char * seq) {
    unsigned char stop = 0;
    char c;
    char b[2];
    for (unsigned char i = 0; i < 26; i++) {
        if (!stop) {
            c = seq[i];
            if (c==0) {
                c = ' ';
                stop = 1;
            }
        }
        b[0] = c;
        b[1] = 0;
        BUTTON_SetText(this->ui.button[i], b);
    }
}


void KeyboardUI::updateChars() {
    switch(this->viewState) {
        case KB_LC:
            this->updateCharsFrom('a');
            break;
        case KB_UC:
            this->updateCharsFrom('A');
            break;
        case KB_DIG:
            this->updateCharsSeq(digital_key_value);
            break;
        case KB_SYMB:
            this->updateCharsSeq(symbol_key_value);
            break;
    }
    BUTTON_SetText(this->ui.button[26], "SP");
    BUTTON_SetText(this->ui.button[27], "del");
}


void KeyboardUI::refresh_05() {
    strcpy(ui_buf1_100, buf);
    strcat(ui_buf1_100, "_");
    this->setText(this->ui.input, ui_buf1_100);
};

void KeyboardUI::refresh_1s() {
    strcpy(ui_buf1_100, buf);
    this->setText(this->ui.input, ui_buf1_100);
};

void KeyboardUI::addChars(char index) {
    if (this->cursor < this->maxSize) {
        if (index==26)
            buf[this->cursor++] = ' ';
        else {
            char a = 0;
            if (this->viewState==KB_LC)
                a = 'a';
            else if (this->viewState==KB_UC)
                a = 'A';

            if (a!=0)
                buf[this->cursor++] = a+index;
            else {
                const char * pa = 0;
                if (this->viewState==KB_DIG)
                    pa = digital_key_value;
                else if (this->viewState==KB_SYMB)
                    pa = symbol_key_value;
                if (pa && (index<strlen(pa)))
                    buf[this->cursor++] = pa[index];
            }
        }
    }
}

void KeyboardUI::on_button(UI_BUTTON hBtn) {
    if (hBtn== this->ui.mode) {
        if (++this->viewState==KB_END)
            this->viewState = 0;
        this->updateChars();
    } else if (hBtn== this->ui.ok) {
        this->doAction(UI_BUTTON_OK);
    } else if (hBtn== this->ui.cancel) {
        this->doAction(UI_BUTTON_CANCEL);
    } else for (char i=0;i<28;i++) {
        if (hBtn==this->ui.button[i]) {
            if (i==27) {
                if (this->cursor!=0) {
                    this->cursor--;
                    buf[this->cursor] = 0;
                }
            } else
                this->addChars(i);
        }
    }
}
