/*
 * FileInfoUI.cpp
 *
 *  Created on: May 19, 2020
 *      Author: shubin
 */

#include "FileInfoUI.h"
#include "FileBrowserUI.h"
#include "PrintingToolsUI.h"
#include "ff.h"
#include "serial.h"
#include "ConfirmDialogUI.h"
#include "Application.h"


FileInfoUI file_info_ui;


void FileInfoUI::refresh() {
	if ((this->ui.info_updated==0) && (ui_print_process.size != 0)) {
		this->update();
		this->ui.info_updated = 1;
	} else {
		FileInfoBaseUI::refresh();
	}
}

void FileInfoUI::update() {
	sprintf(ui_buf1_100, lang_str.file_info_ui.size, ui_print_process.size);
	this->setText(this->ui.file_size, ui_buf1_100);

	PRINT_TIME pt;
	unsigned int t = ui_print_process.time;
	pt.seconds = t % 60;
	t/=60;
	pt.minutes = t % 60;
	pt.hours = t/60;
	print_time_to_str(&pt, &ui_buf1_100[80]);
    sprintf(ui_buf1_100, lang_str.file_info_ui.time, (char *)&ui_buf1_100[80]);
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

}


void FileInfoUI::createControls() {
	FileInfoBaseUI::createControls();
	memset(&this->ui, 0, sizeof(this->ui));
	#define _col(ph_x) (INTERVAL_H + (100+INTERVAL_H)*ph_x)
	#define _y 204
	this->ui.run = this->create96x80Button(_col(3) + 70, _y, img_ok);
	this->ui.cancel = this->create96x80Button(_col(2) + 70, _y, img_cancel);
	this->ui.tools = this->create96x80Button(_col(1) + 70, _y, img_print_tools);
	this->ui.del = this->create96x80Button(_col(0), _y, img_file_delete);

#define _row_size 22
#define _row(idx) _row_size * idx
#define _text_x 220

	this->ui.file_size = this->createText(_text_x, _row(0), 240, _row_size, 0);
	this->ui.time = this->createText(_text_x, _row(1), 240, _row_size, 0);
	this->ui.layers = this->createText(_text_x, _row(2), 240, _row_size, 0);
	this->ui.filament = this->createText(_text_x, _row(3), 240, _row_size, 0);
	this->ui.mmx = this->createText(_text_x, _row(4), 240, _row_size, 0);
	this->ui.mmy = this->createText(_text_x, _row(5), 240, _row_size, 0);
	this->ui.mmz = this->createText(_text_x, _row(6), 240, _row_size, 0);

    this->ui.continuePrint.text= this->createText(_text_x + 100, _row(7) + 16, 120, _row_size, lang_str.continue_print);
    this->ui.continuePrint.button = this->createCheckButton(_text_x, _row(7) + 10, this->ui.contine_print);
    UI_CHECK continuePrint;

	this->update();
}


const char * FileInfoUI::getTitle() {
	return ui_print_process.file_name;
}

void FileInfoUI::on_action_dialog(u8 action, u8 dialog_id) {
	confirm_dialog_ui.hide();
	if (action==UI_BUTTON_OK) {
		f_unlink(ui_print_process.file_name);
		ui_app.back_ui();
	} else {
		this->show();
	}
}

void FileInfoUI::on_button(UI_BUTTON hBtn) {
	if (hBtn==this->ui.run) {
	    if (this->ui.contine_print) {
            gCfgItems.breakpoint_reprint_flg = 1;
            gCfgItems.breakpoint_flg = 1;
	    } else {
            gCfgItems.breakpoint_reprint_flg = 0;
            gCfgItems.breakpoint_flg = 0;
	    }

        ui_app.startPrintFile(1);
	} if (hBtn==this->ui.del) {
		this->hide();
		sprintf(
					ui_buf1_100,
					lang_str.dialog.confirm_delete_file,
					get_long_file_name(ui_print_process.file_name)
		);
		confirm_dialog_ui.show(ui_buf1_100, this, 0, this);
	} if (hBtn==this->ui.cancel) {
		this->hide();
		ui_app.back_ui();
	}  if (hBtn==this->ui.tools) {
		this->hide();
		printing_tools_ui.show(this);
	} else if (hBtn==this->ui.continuePrint.button) {
        this->ui.contine_print = !this->ui.contine_print;
        this->updateCheckButton(ui.continuePrint.button, this->ui.contine_print);
	}
}
