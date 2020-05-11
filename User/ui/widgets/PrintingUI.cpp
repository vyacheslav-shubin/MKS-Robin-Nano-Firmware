/*
 * PrintingUI.cpp
 *
 *  Created on: May 10, 2020
 *      Author: shubin
 */

#include "PrintingUI.h"
#include "ui_tools.h"
#include "temperature.h"
#include "mks_reprint.h"
#include "sh_tools.h"
#include "draw_dialog.h"
#include "spi_flash.h"
#include "ili9320.h"
#include "pic_manager.h"

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

void PrintingUI::doFinishPrint() {
	stop_print_time();
	this->hide();
	draw_dialog(DIALOG_TYPE_FINISH_PRINT);
}


void PrintingUI::createControls() {
	memset(&ui, 0, sizeof(ui));
	ui.progress = ui_create_std_progbar(COL(0), 0, 270, PB_HEIGHT, this->hWnd);
	this->createStateButtonAt(0, 0, &ui.time, img_state_time, 0);
	this->createStateButtonAt(0, 2, &ui.bed, img_state_bed, 0);
	this->createStateButtonAt(1, 2, &ui.fan, 0, 0);
	this->createStateButtonAt(1, 0, &ui.z, img_state_z, 0);

	this->createStateButtonAt(0, 1, &ui.ext1, img_state_extruder1, 0);
	if (is_dual_extruders())
		this->createStateButtonAt(1, 1, &ui.ext2, img_state_extruder2, 0);

	#define _col(ph_x) (INTERVAL_H + (100+INTERVAL_H)*ph_x)
	#define _y 204
	ui.pause = ui_create_100_80_button(_col(0), _y, this->hWnd, 0);
	ui.stop = ui_create_100_80_button(_col(1),_y, this->hWnd, img_print_stop);
	ui.tools = ui_create_100_80_button(_col(2),_y, this->hWnd, img_print_tools);
	ui.power_control = ui_create_100_80_button(_col(3) + 70,_y, this->hWnd, 0);
	this->updatePowerControlButton();
	this->updatePauseButton();
	ui_update_fan_button(ui.fan.button, ui.fan.label);
};

void PrintingUI::updateStateButtons() {
	sprintf(ui_buf1_20, "%d/%d°", (int)thermalManager.current_temperature[0], (int)thermalManager.target_temperature[0]);
	this->updateStateButton(&ui.ext1, 0, ui_buf1_20);
	if(is_dual_extruders()){
		sprintf(ui_buf1_20, "%d/%d°", (int)thermalManager.current_temperature[1], (int)thermalManager.target_temperature[1]);
		this->updateStateButton(&ui.ext2, 0, ui_buf1_20);
	}
	sprintf(ui_buf1_20, "%d/%d°", (int)thermalManager.current_temperature_bed,  (int)thermalManager.target_temperature_bed);
	this->updateStateButton(&ui.bed, 0, ui_buf1_20);
	print_time_to_str(&print_time, ui_buf1_20);
	this->updateStateButton(&ui.time, 0, ui_buf1_20);

	if (abs(zprobe_zoffset)<0.001) {
		sprintf(ui_buf1_20,"%.3f",current_position[Z_AXIS]);
	} else {
		sprintf(ui_buf1_20,"%.2f/%.2f",current_position[Z_AXIS], zprobe_zoffset);
	}
	this->updateStateButton(&ui.z, 0, ui_buf1_20);
}

void PrintingUI::updatePowerControlButton() {
    BUTTON_SetBmpFileName(ui.power_control, ui_print_process.suicide.enabled?img_print_auto_power_off:img_print_manual_power_off, 0);
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

//DEFAULT_VIEW_ADDR
//BAK_VIEW_ADDR
void _preview(int x,int y, int src) {
	for (int sector=0;sector<10;sector++) {
		SPI_FLASH_BufferRead(bmp_public_buf, src + sector * 8000, 8000);
		int k=0;
		LCD_setWindowArea(x, y+sector*20, 200, 20);     //200*200
		LCD_WriteRAM_Prepare();
		while(k<8000) {
			LCD_WriteRAM(*((uint16_t *)(&bmp_public_buf[k])));
			k+=2;
		}
	}
}


void _preview_cache() {
	FIL file;
	UINT readed;
	volatile uint32_t i,j;
	int res;
	res = f_open(&file, ui_print_process.file_name, FA_OPEN_EXISTING | FA_READ);
	if(res == FR_OK) {
		f_lseek(&file, (PREVIEW_LITTLE_PIC_SIZE+ui_print_process.preview_offset) + 809 * ui_print_process.preview_row + 8); //809 - длина строки в preview
		f_read(&file, bmp_public_buf, 800, &readed);
		i=0;j=0;
		while (i<800) {
			uint16_t *color = (uint16_t *)&(bmp_public_buf[j]);
			bmp_public_buf[j++] = ascii2dec(bmp_public_buf[i++])<<4 | ascii2dec(bmp_public_buf[i++]);
			bmp_public_buf[j++] = ascii2dec(bmp_public_buf[i++])<<4 | ascii2dec(bmp_public_buf[i++]);
			if(*color == 0x0000) *color=gCfgItems.preview_bk_color;
		}
		if(ui_print_process.preview_row<20)
			SPI_FLASH_SectorErase(BAK_VIEW_ADDR+ui_print_process.preview_row*4096);

		SPI_FLASH_BufferWrite(bmp_public_buf, BAK_VIEW_ADDR+ui_print_process.preview_row*400, 400);

		ui_print_process.preview_row++;
		f_close(&file);
		if(ui_print_process.preview_row >= 200) {
			ui_print_process.preview_row = 0;
			ui_print_process.preview_state_flags |= 1<<PREVIEW_CACHED_BIT;
			char done=1;
			epr_write_data(EPR_PREVIEW_FROM_FLASH, &done,1);
		}
	} else {
		ui_print_process.preview_state_flags = 0;
	}
}




void PrintingUI::refresh() {
	StdWidget::refresh();
	if (!this->ui.preview_done) {
		if (ui_print_process.preview_state_flags & (1<<PREVIEW_CHECKED_BIT)) {
			if (ui_print_process.preview_state_flags & (1<<PREVIEW_EXISTS_BIT)) {
				if (ui_print_process.preview_state_flags & (1<<PREVIEW_CACHED_BIT)) {
					_preview(2, 36, BAK_VIEW_ADDR);
					this->ui.preview_done = 1;
				} else {
					_preview_cache();
				}
			} else {
				_preview(2, 36, DEFAULT_VIEW_ADDR);
				this->ui.preview_done = 1;
			}
		} else {
			if (ui_print_process.file_name[0]!=0) {
				ui_print_process.preview_state_flags = 1<<PREVIEW_CHECKED_BIT;
				unsigned char has_preview;
				epr_read_data(EPR_PREVIEW_FROM_FLASH, &has_preview, 1);
				if (has_preview) {
					ui_print_process.preview_state_flags = PREVIEW_CACHED;
				} else {
					if (ui_file_with_preview(ui_print_process.file_name, &ui_print_process.preview_offset)) {
						ui_print_process.preview_state_flags |= 1<<PREVIEW_EXISTS_BIT;
						ui_print_process.preview_row = 0;
						_preview(2, 36, DEFAULT_VIEW_ADDR);
					}
				}
			}
		}
	}
}


void PrintingUI::refresh_05() {
	ui_update_fan_button(ui.fan.button, ui.fan.label);
}

void PrintingUI::refresh_1s() {
	_calc_rate();
	if((mksReprint.mks_printer_state == MKS_IDLE) && (ui_print_process.rate == 100)) {
		this->doFinishPrint();
	} else {
		this->draw_xyz();
		this->updateStateButtons();
		this->updateProgress();
	}
}
#include "draw_fan.h"
#include "draw_pre_heat.h"
#include "draw_operate.h"

void PrintingUI::on_button(WM_HWIN hBtn) {
	if(hBtn == ui.tools) {
		this->hide();
		draw_operate();
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
			draw_dialog(DIALOG_TYPE_STOP);
		}
	} else if (hBtn == ui.fan.button) {
		this->hide();
		draw_fan();
	} else if ((hBtn == ui.ext1.button) || (hBtn == ui.ext2.button) || (hBtn == ui.bed.button)){
		this->hide();
		draw_preHeat();
	} else if (hBtn == ui.power_control) {
		ui_print_process.suicide.enabled = ~ui_print_process.suicide.enabled;
		this->updatePowerControlButton();
	}

};
