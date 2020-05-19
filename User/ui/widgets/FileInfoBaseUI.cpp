/*
 * PrintProcessUI.cpp
 *
 *  Created on: May 19, 2020
 *      Author: shubin
 */

#include "FileInfoBaseUI.h"

#include "spi_flash.h"
#include "ili9320.h"
#include "pic_manager.h"
#include "ui_tools.h"

static void _preview(int x,int y, int src) {
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


static void _preview_cache() {
	FIL file;
	UINT readed;
	volatile uint32_t i,j;
	if(f_open(&file, ui_print_process.file_name, FA_OPEN_EXISTING | FA_READ) == FR_OK) {
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


void FileInfoBaseUI::refresh() {
	StdWidget::refresh();
	if (!this->preview_done) {
		if (ui_print_process.preview_state_flags & (1<<PREVIEW_CHECKED_BIT)) {
			if (ui_print_process.preview_state_flags & (1<<PREVIEW_EXISTS_BIT)) {
				if (ui_print_process.preview_state_flags & (1<<PREVIEW_CACHED_BIT)) {
					_preview(2, 36, BAK_VIEW_ADDR);
					this->preview_done = 1;
				} else {
					_preview_cache();
				}
			} else {
				_preview(2, 36, DEFAULT_VIEW_ADDR);
				this->preview_done = 1;
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
