//
// Created by shubin on 25.05.2020.
//

#ifndef WORK_CALCULATORDIALOGUI_H
#define WORK_CALCULATORDIALOGUI_H

#include "DialogWidget.h"

#define CALC_OPERAND_BUFFER_SIZE    10

typedef struct {
    UI_TEXT value;
    UI_TEXT title;
    UI_BUTTON digs[10];
    UI_BUTTON point;
    UI_BUTTON sign;
    UI_BUTTON div;
    UI_BUTTON mul;
    UI_BUTTON minus;
    UI_BUTTON plus;
    UI_BUTTON equ;
    UI_BUTTON del;
    UI_BUTTON ok;
    UI_BUTTON cancel;
} CALCULATOR_DIALOG_UI_CONTROLS;

typedef struct {
    unsigned char flags;
    char str[CALC_OPERAND_BUFFER_SIZE + 1];
    float value;
} CALC_OPERAND;

#define CAL_OPERAND_FLAG_NEG            0

typedef enum {
    EQU = 0, DIV, MUL, ADD, SUB, OVFL
} CALC_OPERADTION;

typedef struct {
    CALC_OPERAND op1;
    CALC_OPERAND op2;
    CALC_OPERADTION operation;
} CALC_MATH;


class CalculatorDialogCallback {
protected:
public:
    virtual void on_calculator(unsigned char action, double result, unsigned char dialog_id) = 0;
    virtual ~CalculatorDialogCallback() {};
};


class CalculatorDialogUI : public DialogWidget {
private:
    CALCULATOR_DIALOG_UI_CONTROLS ui;
    CALC_MATH cm;
    CALC_OPERAND * currentOperand;
    unsigned char id = 0;
    double init_value = 0;
    const char * title;
    UI_BUTTON createCalcButton(int x, int y,  const char * text);
    CalculatorDialogCallback * callback = 0;
    void updateDisplay();
    unsigned char calculate();
protected:
    virtual void createControls();
    virtual void on_button(UI_BUTTON hBtn);
public:
    void show(const char * title, double value, unsigned char  dialog_id, CalculatorDialogCallback * callback, Widget * caller) {
        this->id = dialog_id;
        this->callback = callback;
        this->init_value = value;
        this->title = title;
        DialogWidget::show(caller);
    }
    CalculatorDialogUI() : DialogWidget(){};
};

extern CalculatorDialogUI calculator_dialog_ui;

#endif //WORK_CALCULATORDIALOGUI_H
