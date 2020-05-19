/*
 * FileInfoUI.cpp
 *
 *  Created on: May 19, 2020
 *      Author: shubin
 */

#include "FileInfoUI.h"
#include "PrintingToolsUI.h"
#include "ff.h"
#include "serial.h"

FileInfoUI file_info_ui;

void find_float(char * msg, float * value) {
	memset(ui_buf1_100, 0, 11);
	char * p = (char *)strstr(bmp_public_buf, msg);
	if (p!=0) {
		p+=strlen(msg);
		while (*p==' ')
			p++;
		int i=0;
		while ((((p[i]>='0') && (p[i]<='9')) || (p[i]=='-') || (p[i]=='.')) && (i<10)) {
			ui_buf1_100[i] = p[i];
			i++;
		}
		if (i!=0)
			*value = atof(ui_buf1_100);
	}
}

static void _explore_file() {
#define PREPERD_SIZE 512
	FIL file;
	if(f_open(&file, ui_print_process.file_name, FA_OPEN_EXISTING | FA_READ) == FR_OK) {
		ui_print_process.size = file.fsize;
		int offset;
		if (ui_file_with_preview(ui_print_process.file_name, &offset)) {
			f_lseek(&file, (PREVIEW_LITTLE_PIC_SIZE+offset) + 809 * 200 + 8); //809 - длина строки в preview
		} else {
			f_lseek(&file, 0);
		}
		memset(bmp_public_buf, 0, PREPERD_SIZE+1);
		unsigned int readed;
		if (f_read(&file, bmp_public_buf, PREPERD_SIZE, &readed) == FR_OK) {
			find_float("MINX:", &ui_print_process.mmx.minv);
			find_float("MAXX:", &ui_print_process.mmx.maxv);
			find_float("MINY:", &ui_print_process.mmy.minv);
			find_float("MAXY:", &ui_print_process.mmy.maxv);
			find_float("MINZ:", &ui_print_process.mmz.minv);
			find_float("MAXZ:", &ui_print_process.mmz.maxv);
			find_float("Layer height:", &ui_print_process.layer_height);
			find_float("Filament used:", &ui_print_process.filament_used);
			float t;
			find_float("TIME:", &t);
			ui_print_process.time = (int)t;
			find_float("LAYER_COUNT:", &t);
			ui_print_process.layer_count = (int)t;

			SERIAL_ECHOLNPAIR("SIZE:", ui_print_process.size);
			SERIAL_ECHOLNPAIR("TIME:", ui_print_process.time);
			SERIAL_ECHOLNPAIR("LAYER COUNT:", ui_print_process.layer_count);
			SERIAL_ECHOLNPAIR("LAYER HEIGHT:", ui_print_process.layer_height);
			SERIAL_ECHOLNPAIR("FILAMENT USED:", ui_print_process.filament_used);
			SERIAL_ECHOPAIR("MIN_X:", ui_print_process.mmx.minv);
			SERIAL_ECHOLNPAIR(" MAX_X:", ui_print_process.mmx.maxv);
			SERIAL_ECHOPAIR("MIN_Y:", ui_print_process.mmy.minv);
			SERIAL_ECHOLNPAIR(" MAX_Y:", ui_print_process.mmy.maxv);
			SERIAL_ECHOPAIR("MIN_Z:", ui_print_process.mmz.minv);
			SERIAL_ECHOLNPAIR(" MAX_Z:", ui_print_process.mmz.maxv);
		}
		f_close(&file);
	}
}

void FileInfoUI::refresh() {
	if (ui_print_process.size == 0) {
		_explore_file();
		this->update();
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

}


void FileInfoUI::createControls() {
	FileInfoBaseUI::createControls();
	memset(&this->ui, 0, sizeof(this->ui));
	#define _col(ph_x) (INTERVAL_H + (100+INTERVAL_H)*ph_x)
	#define _y 204
	this->ui.run = this->create100x80Button(_col(3) + 70, _y, img_print_resume);
	this->ui.cancel = this->create100x80Button(_col(2) + 70,_y, img_print_back);
	this->ui.tools = this->create100x80Button(_col(1) + 70,_y, img_print_tools);
	this->ui.del = this->create100x80Button(_col(0),_y, img_file_delete);

#define _row_size 25
#define _row(idx) _row_size * idx
	this->ui.file_size = this->createText(240, _row(0), 240, _row_size, 0);
	this->ui.time = this->createText(240, _row(1), 240, _row_size, 0);
	this->ui.layers = this->createText(240, _row(2), 240, _row_size, 0);
	this->ui.filament = this->createText(240, _row(3), 240, _row_size, 0);
	this->ui.mmx = this->createText(240, _row(4), 240, _row_size, 0);
	this->ui.mmy = this->createText(240, _row(5), 240, _row_size, 0);
	this->ui.mmz = this->createText(240, _row(6), 240, _row_size, 0);
	this->update();
}

char * FileInfoUI::getTitle() {
	return ui_print_process.file_name;
}

void FileInfoUI::on_button(UI_BUTTON hBtn) {
	if (hBtn==this->ui.run) {
		ui_app.startPrintFile(1);
	} if (hBtn==this->ui.del) {
		f_unlink(ui_print_process.file_name);
		this->hide();
		ui_app.back_ui();
	} if (hBtn==this->ui.cancel) {
		this->hide();
		ui_app.back_ui();
	}  if (hBtn==this->ui.tools) {
		this->hide();
		printing_tools_ui.show(this);
	}
}
