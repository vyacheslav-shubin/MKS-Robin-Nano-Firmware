//
// Created by shubin on 25.05.2020.
//

#include "CalculatorDialogUI.h"
#include "ui_tools.h"
#include "math.h"


CalculatorDialogUI calculator_dialog_ui;

#define _col(x) ((x)*70+6)
#define _row(y) ((y)*50+36)


UI_BUTTON CalculatorDialogUI::createCalcButton(int x, int y,  const char * text) {
    return this->create70x50Button(_col(x), _row(y), img_calc_button, text);
}

void _draw_operand(CALC_OPERAND * operand, char * buf) {
    int l = strlen(buf);
    if (operand->flags & (1<<CAL_OPERAND_FLAG_NEG))
        buf[l++] = '-';

    strcat(&buf[l], operand->str);
}

double _get_op(CALC_OPERAND * op) {
    double f = std::atof(op->str);
    if (op->flags & 1<<CAL_OPERAND_FLAG_NEG)
        f = -f;
    return f;
}

unsigned char _has_point(char * val) {
    unsigned char i = 0;
    do {
        if (val[i]=='.')
            return i;
        if (val[i]==0)
            return 0;
        i++;
    } while (i);
    return 0;
}

unsigned char _double_to_str(double value, char *buf) {
    sprintf(buf, "%1.3f", value);
    unsigned char i = strlen(buf);
    if (_has_point(buf)) {
        while (i > 0) {
            i--;
            char  a = buf[i];
            if ((a=='0') || (a=='.'))
                buf[i] = 0;
            else
                break;
            if (a=='.')
                break;
        }
        ++i;
    }
    return i;
}

unsigned char _init_operand(CALC_OPERAND * operand, double value) {
    value = round(value*1000)/1000;
    memset(operand, 0, sizeof(CALC_OPERAND));
    if (value<0) {
        operand->flags |= (1 << CAL_OPERAND_FLAG_NEG);
        value = -value;
    } else
        operand->flags &= ~(1<<CAL_OPERAND_FLAG_NEG);


    unsigned char l = _double_to_str(value, ui_buf1_100);
    //При переполнении можно посмотреть еще дробную часть
    if (l <= CALC_OPERAND_BUFFER_SIZE) {
        strcpy(operand->str, ui_buf1_100);
        return 1;
    } else {
        return 0;
    }
}


unsigned char _init_calc(CALC_MATH * cm, double value) {
    memset(cm, 0, sizeof(CALC_MATH));
    cm->op2.str[0] = '0';
    _init_operand(&cm->op1, value);
}


void CalculatorDialogUI::createControls() {
    memset(&this->ui, 0, sizeof(this->ui));
    if (!_init_calc(&this->cm, this->init_value))
        this->cm.operation = OVFL;
    this->currentOperand = &this->cm.op1;
    #define _x (DIALOG_WIDTH/2)
    this->ui.value = TEXT_CreateEx(DIALOG_WIDTH/2, 6, DIALOG_WIDTH - 6 - _x, 28, this->hWnd, WM_CF_SHOW, TEXT_CF_RIGHT|TEXT_CF_VCENTER,  GUI_ID_TEXT0, 0);
    TEXT_SetBkColor(ui.value,  0xFFFFFF);
    TEXT_SetTextColor(ui.value, 0x00);

    this->ui.digs[1] = this->createCalcButton(0, 0, "1");
    this->ui.digs[2] = this->createCalcButton(1, 0, "2");
    this->ui.digs[3] = this->createCalcButton(2, 0, "3");
    this->ui.digs[4] = this->createCalcButton(0, 1, "4");
    this->ui.digs[5] = this->createCalcButton(1, 1, "5");
    this->ui.digs[6] = this->createCalcButton(2, 1, "6");
    this->ui.digs[7] = this->createCalcButton(0, 2, "7");
    this->ui.digs[8] = this->createCalcButton(1, 2, "8");
    this->ui.digs[9] = this->createCalcButton(2, 2, "9");
    this->ui.digs[0] = this->createCalcButton(0, 3, "0");
    this->ui.point = this->createCalcButton(1, 3, ".");
    this->ui.sign = this->createCalcButton(2, 3, "+/-");

    this->ui.div = this->createCalcButton(3, 0, "/");
    this->ui.mul = this->createCalcButton(3, 1, "*");
    this->ui.minus = this->createCalcButton(3, 2, "-");
    this->ui.plus = this->createCalcButton(3, 3, "+");

    this->ui.equ = this->createCalcButton(4, 0, "=");
    this->ui.del = this->createCalcButton(4, 1, "DEL");
    this->ui.cancel = this->createCalcButton(4, 2, "ESC");
    this->ui.ok = this->createCalcButton(4, 3, "OK");
    this->updateDisplay();
}


void CalculatorDialogUI::updateDisplay() {
    if (cm.operation==OVFL) {
        TEXT_SetText(ui.value, lang_str.overflow);
    } else {
        memset(ui_buf1_100, 0, sizeof(ui_buf1_100));
        _draw_operand(&this->cm.op1, ui_buf1_100);
        int l = strlen(ui_buf1_100);
        if (this->cm.operation != EQU) {
            switch (this->cm.operation) {
                case MUL:
                    ui_buf1_100[l++] = '*';
                    break;
                case DIV:
                    ui_buf1_100[l++] = '/';
                    break;
                case SUB:
                    ui_buf1_100[l++] = '-';
                    break;
                case ADD:
                    ui_buf1_100[l++] = '+';
                    break;
            };
            _draw_operand(&this->cm.op2, &ui_buf1_100[l]);
        }
        TEXT_SetText(ui.value, ui_buf1_100);
    }
}

unsigned char CalculatorDialogUI::calculate() {
    double op1 = _get_op(&this->cm.op1);
    double op2 = _get_op(&this->cm.op2);
    switch (this->cm.operation) {
        case ADD:
            op1+=op2;
            break;
        case SUB:
            op1-=op2;
            break;
        case MUL:
            op1*=op2;
            break;
        case DIV:
            if (op2!=0)
                op1/=op2;
            break;
    }


    memset(&this->cm.op2, 0, sizeof(this->cm.op2));
    this->cm.op2.str[0] = '0';

    if (!_init_operand(&this->cm.op1, op1)) {
        cm.operation = OVFL;
        return 0;
    }
    return 1;
}


void CalculatorDialogUI::on_button(UI_BUTTON hBtn) {
    if (this->cm.operation == OVFL) {
        _init_calc(&this->cm, this->init_value);
        this->currentOperand = &this->cm.op1;
    } else {
        if (hBtn == this->ui.ok) {
            if (this->calculate()) {
                if (this->callback)
                    this->callback->on_calculator(UI_BUTTON_OK,  _get_op(&this->cm.op1), this->id);
                else {
                    Widget::hide();
                    ui_app.back_ui();
                }
            }
        } else if (hBtn == this->ui.cancel) {
            if (this->callback)
                this->callback->on_calculator(UI_BUTTON_CANCEL, 0, this->id);
            else {
                Widget::hide();
                ui_app.back_ui();
            }
        } else if (hBtn == this->ui.mul) {
            if (this->calculate()) {
                this->cm.operation = MUL;
                this->currentOperand = &this->cm.op2;
            }
        } else if (hBtn == this->ui.div) {
            if (this->calculate()) {
                this->cm.operation = DIV;
                this->currentOperand = &this->cm.op2;
            }
        } else if (hBtn == this->ui.plus) {
            if (this->calculate()) {
                this->cm.operation = ADD;
                this->currentOperand = &this->cm.op2;
            }
        } else if (hBtn == this->ui.minus) {
            if (this->calculate()) {
                this->cm.operation = SUB;
                this->currentOperand = &this->cm.op2;
            }
        } else if (hBtn == this->ui.equ) {
            if (this->calculate()) {
                this->cm.operation = EQU;
                this->currentOperand = &this->cm.op1;
            }
        } else if (hBtn == this->ui.sign) {
            this->currentOperand->flags ^= 1 << CAL_OPERAND_FLAG_NEG;
        } else {
            unsigned char l = strlen(this->currentOperand->str);
            if (hBtn == this->ui.point) {
                if (!_has_point(this->currentOperand->str) && (l < CALC_OPERAND_BUFFER_SIZE - 1))
                    this->currentOperand->str[l] = '.';
            } else if (hBtn == this->ui.del) {
                if (l > 1) {
                    this->currentOperand->str[l - 1] = 0;
                } else {
                    if (this->currentOperand->str[0] != '0')
                        this->currentOperand->str[0] = '0';
                    else {
                        this->cm.operation = EQU;
                        this->currentOperand = &this->cm.op1;
                    }
                }
            } else {
                if (l < CALC_OPERAND_BUFFER_SIZE) {
                    for (char i = 0; i < 10; i++) {
                        if (hBtn == this->ui.digs[i]) {
                            if ((l == 1) && (this->currentOperand->str[0] == '0'))
                                l = 0;
                            this->currentOperand->str[l] = '0' + i;
                            break;
                        }
                    }
                }
            }
        }
    }
    this->updateDisplay();
}
