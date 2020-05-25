//
// Created by shubin on 23.05.2020.
//

#include "PrintingInfoDialogUI.h"
#include "ui_tools.h"
#include "Marlin.h"
#include "cardreader.h"
#include "integration.h"

PrintingInfoDialogUI printing_info_dialog_ui;

void PrintingInfoDialogUI::createControls() {
    memset(&this->ui, 0, sizeof(this->ui));
    this->createDialogDecoration(img_dialog_info, 0);
    this->ui.ok = this->create96x80Button(DIALOG_WIDTH - (96 + 4), DIALOG_HEIGHT - 84, img_ok);
#define _row_size 20
#define _x  40
#define _w  210

#define _wl (DIALOG_WIDTH - (_x*2))
#define _row(idx) _row_size * idx + 60
    this->ui.file_size = this->createText(_x, _row(0), _wl, _row_size, 0);
    this->ui.time = this->createText(_x, _row(1), _wl, _row_size, 0);
    this->ui.layers = this->createText(_x, _row(2), _wl, _row_size, 0);
    this->ui.filament = this->createText(_x, _row(3), _wl, _row_size, 0);
    this->ui.mmx = this->createText(_x, _row(5), _w, _row_size, 0);
    this->ui.mmy = this->createText(_x, _row(6), _w, _row_size, 0);
    this->ui.mmz = this->createText(_x, _row(7), _w, _row_size, 0);
    this->updateLabels();
}

void PrintingInfoDialogUI::on_button(UI_BUTTON hBtn) {
    if (hBtn==this->ui.ok)
        doAction(UI_BUTTON_OK);
}

void PrintingInfoDialogUI::updateLabels() {
    int percent;

    percent = card.sdpos * 100 / card.filesize;

    //file size
    sprintf(ui_buf1_100, lang_str.file_info_ui.size_progress, card.sdpos, card.filesize, percent);
    this->setText(this->ui.file_size, ui_buf1_100);

    //time progress
    PRINT_TIME pt;
    unsigned int t = ui_print_process.time;
    pt.seconds = t % 60;
    t /= 60;
    pt.minutes = t % 60;
    pt.hours = t / 60;
    print_time_to_str(&pt, &ui_buf1_100[80]);
    print_time_to_str(&print_time, &ui_buf1_100[60]);

    if (ui_print_process.time != 0) {
        t = print_time.seconds + print_time.minutes * 60 + print_time.hours * 3600;
        percent = (float) t * 100 / ui_print_process.time;
    } else
        percent = 0;

    sprintf(ui_buf1_100, lang_str.file_info_ui.time_progress, &ui_buf1_100[60], &ui_buf1_100[80], percent);
    this->setText(this->ui.time, ui_buf1_100);

    //filament progress
    percent = (ui_print_process.filament_used != 0) ? filament_counter / 10 / ui_print_process.filament_used
                                                    : 0; // m/mm
    sprintf(ui_buf1_100, lang_str.file_info_ui.filament_progress, filament_counter / 1000, ui_print_process.filament_used,
            percent);
    this->setText(this->ui.filament, ui_buf1_100);


    //x, y, z info
    sprintf(ui_buf1_100, lang_str.file_info_ui.min_max_x, ui_print_process.mmx.minv, ui_print_process.mmx.maxv);
    this->setText(this->ui.mmx, ui_buf1_100);
    sprintf(ui_buf1_100, lang_str.file_info_ui.min_max_y, ui_print_process.mmy.minv, ui_print_process.mmy.maxv);
    this->setText(this->ui.mmy, ui_buf1_100);
    sprintf(ui_buf1_100, lang_str.file_info_ui.min_max_z, ui_print_process.mmz.minv, ui_print_process.mmz.maxv);
    this->setText(this->ui.mmz, ui_buf1_100);


    int current_layer = 0;
    //layer info
    if (ui_print_process.layer_height != 0) {
        float max = ui_print_process.mmz.maxv;
        if (max == 0)
            max = ui_print_process.layer_height * (float)ui_print_process.layer_count;
        if (max != 0) {
            shUI::CURRENT_POSITION cp;
            shUI::getCurrentPosition(&cp);
            int zest = round((max - cp.z) / ui_print_process.layer_height);
            current_layer =  ui_print_process.layer_count - zest;
            if (current_layer<0)
                current_layer = 0;
            percent = current_layer * 100 / ui_print_process.layer_count;
        }
    } else
        percent = 0;

    sprintf(ui_buf1_100, lang_str.file_info_ui.layers_progress, ui_print_process.layer_height, current_layer, ui_print_process.layer_count, percent);
    this->setText(this->ui.layers, ui_buf1_100);


    this->drawXYZ();
}

void PrintingInfoDialogUI::refresh_1s() {
    this->updateLabels();
}
