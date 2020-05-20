/*
 * PrintingUI.cpp
 *
 *  Created on: May 10, 2020
 *      Author: shubin
 */

#include "PrintingUI.h"
#include "ui_tools.h"
#include "integration.h"
#include "temperature.h"
#include "mks_reprint.h"
#include "sh_tools.h"
#include "draw_dialog.h"
#include "FanUI.h"
#include "BabystepUI.h"
#include "PreheatUI.h"
#include "PrintingToolsUI.h"
#include "ConfirmDialogUI.h"

#define PB_HEIGHT	25
#define SB_OFFSET	(PB_HEIGHT + 10)
#define ROW_SIZE	40
#define COL(x) (200 + 5 + ((120+5) * x))
#define ROW(y) (SB_OFFSET + (ROW_SIZE*y))

PrintingUI printing_ui;

uint8_t pause_resum=0;
uint8_t print_start_flg = 0;
extern uint8_t button_disp_pause_state;

void PrintingUI::createStateButtonAt(char col, char row, STATE_BUTTON * btn, const char * img, const char * title) {
	this->createStateButton(COL(col), ROW(row), btn, img, title);
}

void _calc_rate(void) {
		int rate;
		volatile long long rate_tmp_r =(long long) card.sdpos * 100;
		rate = rate_tmp_r / card.filesize;

	/*	volatile long long rate_tmp_r;
		if(from_flash_pic != 1) {
			rate_tmp_r =(long long) card.sdpos * 100;
			rate = rate_tmp_r / card.filesize;
		} else {
			rate_tmp_r =(long long)card.sdpos;
			rate = (rate_tmp_r-(PREVIEW_SIZE+To_pre_view))* 100 / (card.filesize-(PREVIEW_SIZE+To_pre_view));
		}
		ui_print_process.rate = rate;
		if(rate <= 0)
			rate = 0;
			*/
		ui_print_process.rate = rate;
}

void PrintingUI::updateProgress() {
	PROGBAR_SetValue(ui.progress, ui_print_process.rate);
	if (ui_print_process.rate != 0) {
		sprintf(ui_buf1_20, "%d%% - ", ui_print_process.rate);

		int total = print_time.seconds + print_time.minutes * 60 + print_time.hours * 3600;
		total = (total * 100 / ui_print_process.rate) - total;
		PRINT_TIME pt;
		pt.seconds = total % 60;
		total = total / 60;
		pt.minutes = total % 60;
		pt.hours = total / 60;
		print_time_to_str(&pt, &ui_buf1_20[strlen(ui_buf1_20)]);
		PROGBAR_SetText(ui.progress, ui_buf1_20);
	}
}

static void on_print_finish_confirm(u8 button) {
	confirm_dialog_ui.hide();
	if (button==UI_BUTTON_OK) {
		ui_app.startPrintFile();
	} else if (button==UI_BUTTON_CANCEL) {
		ui_app.showMainWidget();
	} else if (button==UI_BUTTON_TIMEOUT) {
		GUI_Clear();
		enqueue_and_echo_commands_P(PSTR("M81"));
	}
}

void PrintingUI::doFinishPrint() {
	stop_print_time();
	this->hide();
	confirm_dialog_ui.show(
			ui_print_process.suicide_enabled ?
					lang_str.dialog.confirm_print_with_suicide:
					lang_str.dialog.confirm_print_again,
					on_print_finish_confirm,
					ui_print_process.suicide_enabled ? SUICIDE_WAIT : 0);
	//draw_dialog(DIALOG_TYPE_FINISH_PRINT);
}


void PrintingUI::createControls() {
	FileInfoBaseUI::createControls();
	memset(&ui, 0, sizeof(ui));
	ui.progress = ui_create_std_progbar(COL(0), 0, 270, PB_HEIGHT, this->hWnd);
	this->createStateButtonAt(0, 0, &ui.time, img_state_time, 0);
	this->createStateButtonAt(0, 2, &ui.bed, img_state_bed, 0);
	this->createStateButtonAt(1, 2, &ui.fan, FAN_STATES[0], 0);
	this->createStateButtonAt(1, 0, &ui.z, img_state_z, 0);
	this->createStateButtonAt(0, 3, &ui.speed, img_state_speed, 0);

	this->createStateButtonAt(0, 1, &ui.ext1, img_state_extruder1, 0);
	if (is_dual_extruders())
		this->createStateButtonAt(1, 1, &ui.ext2, img_state_extruder2, 0);

	#define _col(ph_x) (INTERVAL_H + (100+INTERVAL_H)*ph_x)
	#define _y 204
	ui.pause = this->create100x80Button(_col(0), _y, 0);
	ui.stop = this->create100x80Button(_col(1),_y, img_print_stop);
	ui.tools = this->create100x80Button(_col(2),_y, img_print_tools);
	ui.power_control = this->create100x80Button(_col(3) + 70,_y, 0);
	this->updatePowerControlButton();
	this->updatePauseButton();
	this->updateFanState(&this->ui.fan);
};

void PrintingUI::updateStateButtons() {
	shUI::SPRAYER_TEMP st;
	shUI::getSprayerTemperature(0, &st);
	sprintf(ui_buf1_20, "%d/%d°", (int)st.current, (int)st.target);
	this->updateStateButton(&ui.ext1, 0, ui_buf1_20);
	if(is_dual_extruders()){
		shUI::getSprayerTemperature(1, &st);
		sprintf(ui_buf1_20, "%d/%d°", (int)st.current, (int)st.target);
		this->updateStateButton(&ui.ext2, 0, ui_buf1_20);
	}
	shUI::BED_TEMP bt;
	shUI::getBedTemperature(&bt);
	sprintf(ui_buf1_20, "%d/%d°", bt.current,  bt.target);
	this->updateStateButton(&ui.bed, 0, ui_buf1_20);
	print_time_to_str(&print_time, ui_buf1_20);
	this->updateStateButton(&ui.time, 0, ui_buf1_20);

	if (abs(zprobe_zoffset)<0.001) {
		sprintf(ui_buf1_20,"%.3f",current_position[Z_AXIS]);
	} else {
		sprintf(ui_buf1_20,"%.2f/%.2f",current_position[Z_AXIS], zprobe_zoffset);
	}
	this->updateStateButton(&ui.z, 0, ui_buf1_20);

	short p = shUI::getFeedratePercentage();
	if (p==100)
		sprintf(ui_buf1_20,"%.0f",feedrate_mm_s);
	else
		sprintf(ui_buf1_20,"%.0f/%d%%",feedrate_mm_s, p);
	this->updateStateButton(&ui.speed, 0, ui_buf1_20);

}

void PrintingUI::updatePowerControlButton() {
    BUTTON_SetBmpFileName(ui.power_control, ui_print_process.suicide_enabled?img_print_auto_power_off:img_print_manual_power_off, 0);
    BUTTON_SetBitmapEx(ui.power_control, 0, &bmp_struct_100x80,0,0);
}

void PrintingUI::updatePauseButton() {
	const char * fn;
	//Это условие избыточное
	if(gCfgItems.standby_mode==1 && mksReprint.mks_printer_state == MKS_REPRINTED && button_disp_pause_state==1) {
		fn = img_print_pause;
	} else {
		if(
				(mksReprint.mks_printer_state == MKS_REPRINTING)
				|| (mksReprint.mks_printer_state == MKS_PAUSING)
				|| (mksReprint.mks_printer_state == MKS_PAUSED)) {
			fn= img_print_resume;
		} else {
			fn = img_print_pause;
		}
	}
	BUTTON_SetBmpFileName(this->ui.pause, fn, 1);
	BUTTON_SetBitmapEx(this->ui.pause, 0, &bmp_struct_100x80, 0, 0);
}

void PrintingUI::refresh_05() {
	this->updateFanState(&this->ui.fan);
}

void PrintingUI::refresh_1s() {
	_calc_rate();
	if((mksReprint.mks_printer_state == MKS_IDLE) && (ui_print_process.rate == 100)) {
		this->doFinishPrint();
	} else {
		this->drawXYZ();
		this->updateStateButtons();
		this->updateProgress();
	}
}


static void on_stop_print_confirm(unsigned char button) {
	confirm_dialog_ui.hide();
	if (button==UI_BUTTON_OK) {
		ui_app.terminatePrintFile();
	} else if (button==UI_BUTTON_CANCEL) {
		printing_ui.show();
	}
}

void PrintingUI::on_button(UI_BUTTON hBtn) {
	if(hBtn == ui.tools) {
		this->hide();
		printing_tools_ui.show(this);
	} else if(hBtn == ui.pause) {
		if(mksReprint.mks_printer_state == MKS_WORKING) {
			stop_print_time();
			if(mksCfg.extruders==2) {
				gCfgItems.curSprayerChoose_bak= active_extruder;
				gCfgItems.moveSpeed_bak = feedrate_mm_s;
			}
			card.pauseSDPrint();
			print_job_timer.pause();
			mksReprint.mks_printer_state = MKS_PAUSING;

		} else if(mksReprint.mks_printer_state == MKS_PAUSED) {
			if (is_filament_fail()) {
				this->hide();
				draw_dialog(DIALOG_TYPE_FILAMENT_NO_PRESS);
				return;
			} else {
				start_print_time();
				pause_resum = 1;
				mksReprint.mks_printer_state = MKS_RESUMING;//MKS_WORKING;
			}
		} else if(mksReprint.mks_printer_state == MKS_REPRINTING) {
			start_print_time();
			mksReprint.mks_printer_state = MKS_REPRINTED;
		}
		this->updatePauseButton();
	} else if(hBtn == ui.stop) {
		if(mksReprint.mks_printer_state != MKS_IDLE) {
			this->hide();
			confirm_dialog_ui.show(lang_str.dialog.confirm_terminate_print, on_stop_print_confirm, 0, this);
		}
	} else if (hBtn == ui.fan.button) {
		this->hide();
		fan_ui.show(this);
	} else if ((hBtn == ui.ext1.button) || (hBtn == ui.ext2.button) || (hBtn == ui.bed.button)){
		this->hide();
		preheat_ui.show(this);
	} else if (hBtn == ui.z.button) {
		if(mksReprint.mks_printer_state == MKS_WORKING) {
			this->hide();
			babystep_ui.show();
		}
	} else if (hBtn == ui.power_control) {
		ui_print_process.suicide_enabled = ui_print_process.suicide_enabled?0:1;
		this->updatePowerControlButton();
	}

};
