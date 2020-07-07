//
// Created by shubin on 06.07.2020.
//

#include "PIDSettingsUI.h"
#include "integration.h"
#include "ili9320.h"

PIDSettingsUI pid_settings_ui;

#define COL		(OTHER_BTN_XPIEL*3+INTERVAL_V*4)
#define ROW(py)	((OTHER_BTN_YPIEL+INTERVAL_H)*(py))

#define _col(ph_x) (96*ph_x)
#define _y 204

#define GRAPH_Y_MIN         0
#define GRAPH_Y_MAX         200
#define GRAPH_X_MIN         10
#define GRAPH_X_MAX         470


void PIDSettingsUI::on_message(WM_MESSAGE * pMsg) {
    switch (pMsg->MsgId) {
        case WM_PAINT: {
            GUI_SetColor(0x000000);
            GUI_FillRect(GRAPH_X_MIN, GRAPH_Y_MIN, GRAPH_X_MIN, GRAPH_Y_MAX);
            GUI_SetColor(0x00ff00);
            GUI_DrawLine(GRAPH_X_MIN, GRAPH_Y_MIN, GRAPH_X_MIN, GRAPH_Y_MAX);
            GUI_DrawLine(GRAPH_X_MAX, GRAPH_Y_MIN, GRAPH_X_MAX, GRAPH_Y_MAX);
            GUI_DrawLine(GRAPH_X_MIN, GRAPH_Y_MIN, GRAPH_X_MAX, GRAPH_Y_MIN);
            GUI_DrawLine(GRAPH_X_MIN, GRAPH_Y_MAX, GRAPH_X_MAX, GRAPH_Y_MAX);
        }
    }
    Widget::on_message(pMsg);
}

void PIDSettingsUI::createControls() {
    this->ui.back = this->create96x80Button(_col(4), _y, img_page_back);
    this->point=GRAPH_X_MIN+1;
}

#include "serial.h"

void PIDSettingsUI::refresh_05() {
    shUI::BED_TEMP bt;
    shUI::getBedTemperature(&bt);
    shUI::SPRAYER_TEMP st0;
    shUI::getSprayerTemperature(0, &st0);
    shUI::SPRAYER_TEMP st1;
    shUI::getSprayerTemperature(1, &st1);
    sprintf(ui_buf1_80,"B:%d/%d S1:%.1f/%.1f S2:%.1f/%.1f", bt.current, bt.target, st0.current,st0.target, st1.current, st1.target);
    GUI_SetColor(gCfgItems.background_color);
    GUI_FillRect(0, 10, LCD_WIDTH - 10, titleHeight);
    GUI_SetColor(gCfgItems.title_color);
    GUI_DispStringAt(ui_buf1_80, 10, TITLE_YPOS);
    GUI_SetColor(0xffffff);
    //GUI_DrawPoint(this->point, bt.current);
    //GUI_DrawLine(this->point, bt.current, this->point, bt.current);
    //GUI_Exec();
    SERIAL_ECHOLNPAIR("X:", this->point);
    SERIAL_ECHOLNPAIR("Y:", bt.current);

    #define DRAW_HEIGHT  (GRAPH_Y_MAX - GRAPH_Y_MIN - 2)

    LCD_setWindowArea(this->point, titleHeight + 1, 2, DRAW_HEIGHT);
    LCD_WriteRAM_Prepare();
    bt.current = (bt.current>20)?bt.current-20:0;
    st0.current = (st0.current>20)?st0.current-20:0;
    st1.current = (st1.current>20)?st1.current-20:0;

    for (unsigned short i=0;i<DRAW_HEIGHT;i++) {
        u16 color = 0;
        unsigned short v = DRAW_HEIGHT - i;
        if ((v==bt.current) || (v==(unsigned char)st0.current) || (v==(unsigned char)st1.current))
            color = 0xFFFF;

        LCD_WriteRAM(color);
        LCD_WriteRAM(0xF0F0);
    }

    this->point++;
    if (this->point>=GRAPH_X_MAX-1)
        this->point=GRAPH_X_MIN+1;
}

void PIDSettingsUI::on_button(UI_BUTTON hBtn) {
    if (this->ui.back==hBtn) {
        this->action_back();
    }
}
