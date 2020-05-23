//
// Created by shubin on 23.05.2020.
//

#include "PrintingInfoDialogUI.h"
#include "ui_tools.h"

PrintingInfoDialogUI printing_info_dialog_ui;

void PrintingInfoDialogUI::createControls() {
    memset(&this->ui, 0, sizeof(this->ui));
    this->createDialogDecoration(img_dialog_info, 0);
    this->ui.ok = this->create96x80Button(DIALOG_WIDTH - (96 + 4), DIALOG_HEIGHT - 84, img_ok);
#define _row_size 25
#define _row(idx) _row_size * idx + 60
    this->ui.file_size = this->createText(10, _row(0), 240, _row_size, 0);
    this->ui.time = this->createText(10, _row(1), 240, _row_size, 0);
    this->ui.layers = this->createText(10, _row(2), 240, _row_size, 0);
    this->ui.filament = this->createText(10, _row(3), 240, _row_size, 0);
    this->ui.mmx = this->createText(10, _row(4), 240, _row_size, 0);
    this->ui.mmy = this->createText(10, _row(5), 240, _row_size, 0);
    this->ui.mmz = this->createText(10, _row(6), 240, _row_size, 0);
    this->updateLabels();
}

void PrintingInfoDialogUI::on_button(UI_BUTTON hBtn) {
    if (hBtn==this->ui.ok)
        doAction(UI_BUTTON_OK);
}

void PrintingInfoDialogUI::updateLabels() {
    sprintf(ui_buf1_100, lang_str.file_info_ui.size, ui_print_process.size);
    this->setText(this->ui.file_size, ui_buf1_100);

    PRINT_TIME pt;
    unsigned int t = ui_print_process.time;
    pt.seconds = t % 60;
    t/=60;
    pt.minutes = t % 60;
    pt.hours = t/=60;
    strcpy(ui_buf1_100, lang_str.file_info_ui.time);
    print_time_to_str(&pt, &ui_buf1_100[strlen(ui_buf1_100)]);
    this->setText(this->ui.time, ui_buf1_100);
    sprintf(ui_buf1_100, lang_str.file_info_ui.filament, ui_print_process.filament_used);
    this->setText(this->ui.filament, ui_buf1_100);
    sprintf(ui_buf1_100, lang_str.file_info_ui.min_max_x, ui_print_process.mmx.minv, ui_print_process.mmx.maxv);
    this->setText(this->ui.mmx, ui_buf1_100);
    sprintf(ui_buf1_100, lang_str.file_info_ui.min_max_y, ui_print_process.mmy.minv, ui_print_process.mmy.maxv);
    this->setText(this->ui.mmy, ui_buf1_100);
    sprintf(ui_buf1_100, lang_str.file_info_ui.min_max_z, ui_print_process.mmz.minv, ui_print_process.mmz.maxv);
    this->setText(this->ui.mmz, ui_buf1_100);
    sprintf(ui_buf1_100, lang_str.file_info_ui.layers, ui_print_process.layer_height, ui_print_process.layer_count);
    this->setText(this->ui.layers, ui_buf1_100);
    this->drawXYZ();
}

void PrintingInfoDialogUI::refresh_1s() {
    this->updateLabels();
}
