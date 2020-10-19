//
// Created by shubin on 06.07.2020.
//

#include "PIDSettingsUI.h"
#include "integration.h"
#include "Application.h"
#include "ili9320.h"

PIDSettingsUI pid_settings_ui;

#define COL		(OTHER_BTN_XPIEL*3+INTERVAL_V*4)
#define ROW(py)	((OTHER_BTN_YPIEL+INTERVAL_H)*(py))

#define _col(ph_x) (96*ph_x)
#define _y 212

#define GRAPH_Y_MIN         0
#define GRAPH_Y_MAX         210
#define GRAPH_X_MIN         4
#define GRAPH_X_MAX         476


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
            break;
        }
    }
    Widget::on_message(pMsg);
}

void PIDSettingsUI::drawHistory() {
    unsigned short limit;
    if (tempStat.data.loop)
        limit = TEMP_STAT_COUNT;
    else
        limit = tempStat.data.cursor;
    unsigned short cursor = tempStat.data.cursor;
    while (limit!=0) {
        if (cursor>=TEMP_STAT_COUNT)
            cursor = 0;
        this->draw(tempStat.data.bed[cursor], tempStat.data.ext0[cursor], tempStat.data.ext1[cursor]);
        cursor++;
        limit--;
    }
    this->history_done = 1;
}


void PIDSettingsUI::createControls() {
    memset(&this->ui, 0, sizeof(this->ui));
    this->ui.back = this->create96x80Button(_col(4), _y, img_page_back);
    //this->ui.run = this->create96x80Button(_col(0), _y, img_print_resume);
    this->point=GRAPH_X_MIN+1;
    this->history_done = 0;
}

#include "serial.h"

void PIDSettingsUI::draw(unsigned short bed, unsigned short st0, unsigned short st1) {
#define DRAW_HEIGHT  (GRAPH_Y_MAX - GRAPH_Y_MIN - 2)

    LCD_setWindowArea(this->point, titleHeight + 1, 2, DRAW_HEIGHT);
    LCD_WriteRAM_Prepare();
    bed = (bed>20)?bed-20:0;
    st0 = (st0>20)?st0-20:0;
    st1 = (st1>20)?st1-20:0;

    for (unsigned short i=0;i<DRAW_HEIGHT;i++) {
        u16 color = 0;
        unsigned short v = DRAW_HEIGHT - i;
        if ((v==bed) || (v==st0) || (v==st1))
            color = 0xFFFF;

        LCD_WriteRAM(color);
        LCD_WriteRAM(0xF0F0);
    }

    this->point++;
    if (this->point>=GRAPH_X_MAX-1)
        this->point=GRAPH_X_MIN+1;
}


void PIDSettingsUI::refresh_05() {
    if (!this->history_done)
        this->drawHistory();

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
    this->draw(bt.current, st0.current, st1.current);
    //GUI_DrawPoint(this->point, bt.current);
    //GUI_DrawLine(this->point, bt.current, this->point, bt.current);
    //GUI_Exec();

}

void PIDSettingsUI::on_button(UI_BUTTON hBtn) {
    if (this->ui.back==hBtn) {
        this->action_back();
    } else if (this->ui.run==hBtn) {
        shUI::pushGcode("M303 S200");
    }
}
