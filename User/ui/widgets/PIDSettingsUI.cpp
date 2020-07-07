//
// Created by shubin on 06.07.2020.
//

#include "PIDSettingsUI.h"

//PIDSettingsUI pid_settings_ui;

#define COL		(OTHER_BTN_XPIEL*3+INTERVAL_V*4)
#define ROW(py)	((OTHER_BTN_YPIEL+INTERVAL_H)*(py))

#define _col(ph_x) (96*ph_x)
#define _y 204

void PIDSettingsUI::on_message(WM_MESSAGE * pMsg) {
    switch (pMsg->MsgId) {
        case WM_PAINT: {
            GUI_SetColor(0xff5449);
            GUI_FillRect(10, 50, 230, 50);
            GUI_FillRect(10, 100, 230, 100);
            GUI_FillRect(10, 150, 230, 150);
            GUI_FillRect(10, 200, 230, 200);

            GUI_FillRect(250, 50, 470, 50);
            GUI_FillRect(250, 100, 470, 100);
            GUI_FillRect(250, 150, 470, 150);
            GUI_FillRect(250, 200, 470, 200);
        }
    }
    Widget::on_message(pMsg);
}

void PIDSettingsUI::createControls() {
    this->ui.back = this->create96x80Button(_col(4), _y, img_page_back);
}

void PIDSettingsUI::refresh_05() {

}

void PIDSettingsUI::on_button(UI_BUTTON hBtn) {
    if (this->ui.back==hBtn) {
        this->action_back();
    }
}
