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
#include "serial.h"
#include "fatfs.h"

static void find_float(char * msg, float * value) {
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
	} else
        *value = 0;
}

static void _explore_file() {
	#define READ_SIZE 512
	FIL file;
	if(f_open(&file, ui_print_process.file_name, FA_OPEN_EXISTING | FA_READ) == FR_OK) {
		ui_print_process.size = file.fsize;
		int offset;
		ui_file_check_preview(ui_print_process.file_name, &ui_print_process.meta);
		unsigned long seek = 0;
        switch (ui_print_process.meta.mode) {
            case PREVIEW_NONE:
                seek = 0;
                break;
            case PREVIEW_50:
                seek = PREVIEW_SIZE_50;
                break;
            case PREVIEW_100:
                seek = PREVIEW_SIZE_100;
                break;
        }
        seek+=ui_print_process.meta.offset;
		if (ui_file_with_preview(ui_print_process.file_name, &offset)) {
			f_lseek(&file, seek); //809 - длина строки в preview
		} else {
			f_lseek(&file, 0);
		}
		memset(bmp_public_buf, 0, READ_SIZE+1);
		unsigned int readed;
		if (f_read(&file, bmp_public_buf, READ_SIZE, &readed) == FR_OK) {
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

		}
		f_close(&file);
	}
}


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
	    int seek = 0;
	    if (ui_print_process.meta.mode==PREVIEW_50) {
            seek = PREVIEW_LITTLE_PIC_SIZE_50;
        } else {
            seek = PREVIEW_LITTLE_PIC_SIZE_100;
	    }
        seek = seek + ui_print_process.preview_offset + 809 * ui_print_process.preview_row + 8;
		f_lseek(&file, seek); //809 - длина строки в preview
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
			epr_write_data(EPR_PREVIEW_FROM_FLASH, (const unsigned char *)&done,1);
		}
	} else {
		ui_print_process.preview_state_flags = 0;
	}
}


void FileInfoBaseUI::refresh() {
	if (ui_print_process.size == 0) {
		_explore_file();
	} else {
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

	StdWidget::refresh();
}
