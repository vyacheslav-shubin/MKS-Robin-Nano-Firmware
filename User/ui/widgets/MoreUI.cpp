/*
 * MoreUI.cpp
 *
 *  Created on: May 15, 2020
 *      Author: shubin
 */

#include "MoreUI.h"
#include "pic_manager.h"
#include "mks_cfg.h"
#include "spi_flash.h"
#include "serial.h"
#include "Application.h"


static int _addresses[6] = {BUTTON_CMD1_ADDR, BUTTON_CMD2_ADDR, BUTTON_CMD3_ADDR, BUTTON_CMD4_ADDR, BUTTON_CMD5_ADDR, BUTTON_CMD6_ADDR};
static char has_image = 0;

MoreUI more_ui;

void static do_action(int index) {
	memset(cmd_code, 0, sizeof(cmd_code));
	SPI_FLASH_BufferRead(cmd_code,_addresses[index],CMD_CODE_LEN);
	MYSERIAL.inject(cmd_code);
}

static void check_images() {

	uint8_t cnt;
	SPI_FLASH_BufferRead(&cnt,PIC_COUNTER_ADDR,1);
	if(cnt == 0xff)
		cnt = 0;
	int a = PIC_NAME_ADDR;
	for(unsigned char i = 0;i < cnt;i++) {
		uint8_t j = 0;
		do {
			SPI_FLASH_BufferRead(&ui_buf1_100[j],a++,1);
			if (i==sizeof(ui_buf1_100))
			    break;
		} while (ui_buf1_100[j++] != 0);
		//for (char k=0;k<6;k++)
		//int sz;
		//SPI_FLASH_BufferRead((char*)&sz,PIC_SIZE_ADDR+i*4,4);
		if (strstr(ui_buf1_100, "bmp_custom")==&ui_buf1_100[0]) {
		    char c = ui_buf1_100[10];
		    if ((c>='1') && (c<='6')) {
		        c = c - '1';
                has_image|=1<<c;
		    }
		}
		//SERIAL_ECHOLNPAIR(" SIZE:", sz);
	}
	has_image|=1<<7;
}


void MoreUI::createControls() {
	memset(&this->ui, 0, sizeof(this->ui));
	if (has_image==0)
		check_images();
	this->ui.ret = this->createButtonRet();
	unsigned char cnt = gCfgItems.MoreItem_pic_cnt;
	if (cnt>6) cnt = 6;
	for (char i = 0; i<cnt; i++) {
		char * fn;
		if ((has_image & (1<<i)) != 0) {
			sprintf(ui_buf1_80, "bmp_custom%d.bin", i + 1);
			fn = ui_buf1_80;
		} else {
			fn = img_more;
		}
		this->ui.tools[i] = this->createButtonAt(i % 4, i/4, fn, 0);
	}
}

void MoreUI::on_button(UI_BUTTON hBtn) {
	if (hBtn==this->ui.ret) {
		this->hide();
		ui_app.back_ui();
	} else {
		for (char i=0;i<6;i++) {
			if (this->ui.tools[i]==hBtn) {
				do_action(i);
			}
		}
	}
}
