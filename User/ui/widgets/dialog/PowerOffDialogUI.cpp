//
// Created by shubin on 08.07.2020.
//

#include "widgets/integration.h"
#include "PowerOffDialogUI.h"
#include "ui_tools.h"
#include "Application.h"
#include "../../Src/sh_tools.h"

PowerOffDialogUI power_off_dialog_ui;

#define _down_row(idx) (190 - idx*40)
#define check_temerature() (gCfgItems.power_control_flags & POWER_CONTROL_WAIT_HOTEND)

bool PowerOffDialogUI::isTemperatureValid() {
    if (!check_temerature())
        return 1;
    shUI::SPRAYER_TEMP sp;
    shUI::getSprayerTemperature(0, &sp);
    if (sp.current>POWER_CONTROL_TEMPERATURE_MAX)
        return 0;
    if (is_dual_extruders()) {
        shUI::getSprayerTemperature(1, &sp);
        return (sp.current > 50) ? 0 : 1;
    } else
        return 1;
}


void PowerOffDialogUI::createControls() {
    memset(&this->ui, 0, sizeof(this->ui));
    this->createDialogDecoration(img_dialog_confirm, 0);
    this->timeout = this->duration;
    this->ui.ok = this->create96x80Button(DIALOG_WIDTH - (96 + 4), DIALOG_HEIGHT - 84, img_ok);
    this->ui.cancel = this->create96x80Button(DIALOG_WIDTH - (96 + 4) * 2, DIALOG_HEIGHT - 84, img_cancel);
    this->ui.progress = ui_create_std_progbar(100, 20, DIALOG_WIDTH - 100 - 20, 20, this->hWnd);
    PROGBAR_SetValue(this->ui.progress, 0);
    unsigned char test_height;
    if (check_temerature()) {
        if (is_dual_extruders()) {
            ui_std_ext1_state_button(10, _down_row(1), &this->ui.ext1);
            ui_std_ext2_state_button(10, _down_row(0), &this->ui.ext2);
            test_height = DIALOG_HEIGHT - 60 - 10 - 40 * 3;
            shUI::setSprayerTemperature(1, 0);
        } else {
            ui_std_ext1_state_button(10, _down_row(0), &this->ui.ext1);
            test_height = DIALOG_HEIGHT - 60 - 40 * 2 - 10;
        }
        shUI::setBedTemperature(0);
        shUI::setSprayerTemperature(0, 0);
    } else
        test_height = DIALOG_HEIGHT - 60 - 40 * 2 - 10;
    this->ui.text = this->createTextF(10, 60, DIALOG_WIDTH - 20, test_height, TEXT_CF_HCENTER | TEXT_CF_VCENTER, lang_str.dialog.power_off);
};

void PowerOffDialogUI::on_button(UI_BUTTON hBtn) {
    this->hide();
    if (hBtn == this->ui.ok) {
        shUI::powerOff();
    } else if (hBtn == this->ui.cancel) {
        ui_app.showMainWidget();
    }
}

void PowerOffDialogUI::refresh_1s() {
    ui_update_bed_state_button(&this->ui.bed);
    ui_update_ext_state_button(&this->ui.ext1, 0);
    if (is_dual_extruders())
        ui_update_ext_state_button(&this->ui.ext2, 1);

    if (this->timeout) {
        if (this->timeout > 20)
            --this->timeout;
        else {
            if (isTemperatureValid()) {
                if (--this->timeout==0) {
                    this->hide();
                    shUI::powerOffForce();
                } else
                    this->setText(this->ui.text, lang_str.dialog.power_off);
            } else {
                strcpy(ui_buf1_100, lang_str.dialog.power_off);
                strcat(ui_buf1_100, "\n");
                strcat(ui_buf1_100, lang_str.dialog.power_off_waiting_hotend);
                this->setText(this->ui.text, ui_buf1_100);
            }
        }
    }
    PROGBAR_SetValue(this->ui.progress, (this->duration - this->timeout) * 100 / this->duration);
}
