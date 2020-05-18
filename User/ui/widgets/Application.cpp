/*
 * Application.cpp
 *
 *  Created on: May 8, 2020
 *      Author: shubin
 */

#include "Application.h"
#include "draw_ui.h"
#include "ui_tools.h"
#include "MainUI.h"
#include "PrintingUI.h"
#include "ff.h"
#include "ili9320.h"
#include "pic_manager.h"
#include "spi_flash.h"

Application ui_app;


void Application::drawTitle() {
	GUI_DispStringAt(creat_title_text(),  TITLE_XPOS, TITLE_YPOS);
}

void Application::start() {
	GUI_Init();
	this->drawLogo();
}

void Application::closeCurrentWidget() {
	if (this->current_ui)
		this->current_ui->hide();
	else
		clear_cur_ui();
}


void Application::loop() {
	if(wifi_link_state != WIFI_TRANS_FILE) {
		if (this->current_ui)
			this->current_ui->refresh();
		else
			GUI_RefreshPage();
	}
	GUI_TOUCH_Exec();
	GUI_Exec();
}

void Application::startPrintFile() {
	this->closeCurrentWidget();
	ui_start_print_file();
	printing_ui.show();
}

void Application::showMainWidget() {
	main_ui.show();
}

void Application::push(Widget * widget) {
	if (widget==&main_ui)
		reset_stack(widget);
	else {
		ui_push_disp_stack(widget->id);
	}
}

void Application::reset_stack(Widget * widget) {
	ui_reset_disp_stack(widget->id);
}

void Application::back_ui() {
	draw_return_ui();
}

#define logo_file "1:/bmp_logo.bin"

void Application::drawLogo() {
	FIL file;
	int size = 320*480;
	LCD_setWindowArea(0, 0, 480, 320);
	LCD_WriteRAM_Prepare();

	int res = f_open(&file, logo_file, FA_OPEN_EXISTING | FA_READ);
	if(res == FR_OK) {
		while (size>0) {
			UINT readed;
			res = f_read(&file, bmp_public_buf, sizeof(bmp_public_buf), &readed);
			if((res == FR_OK) && (readed!=0)) {
				for(UINT i=0;i<readed;i+=2) {
					uint16_t *color=(uint16_t *)&bmp_public_buf[i];
					LCD_WriteRAM(*color);
					if (--size==0)
						break;
				}
			} else
				break;
		}
		f_close(&file);
	} else {
		int offset = 0;
		while (size != 0) {
			SPI_FLASH_BufferRead(bmp_public_buf,PIC_LOGO_ADDR+offset,sizeof(bmp_public_buf));
			for (int i=0;i<sizeof(bmp_public_buf);i+=2) {
				uint16_t *color=(uint16_t *)&bmp_public_buf[i];
				LCD_WriteRAM(*color);
				if (--size==0)
					break;
			}
			offset+=sizeof(bmp_public_buf);
		}
	}
}

