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
#include "Marlin.h"
#include "PrintingUI.h"
#include "ff.h"
#include "fatfs.h"
#include "ili9320.h"
#include "pic_manager.h"
#include "spi_flash.h"
#include "mks_reprint.h"
#include "mks_touch_screen.h"
#include "dialog/ProgressDialogUI.h"

Application ui_app;
ProgressUI progress_ui;

static FATFS fat;
volatile u8 ui_timing_flags;

#define STANDBY_TIME 	(gCfgItems.standby_time)

typedef enum {
    DIALOG_ID_NOFILE = 0
} DIALOG_ID;

static void _calc_rate(void);

void Application::defaultUI() {
	GUI_SetBkColor(gCfgItems.background_color);
	GUI_SetColor(gCfgItems.title_color);
    if (is_chinese()) {
    	GUI_SetFont(&GUI_FontHZ16);
    	BUTTON_SetDefaultFont(&GUI_FontHZ16);
    	TEXT_SetDefaultFont(&GUI_FontHZ16);
    	GUI_UC_SetEncodeNone();
    } else {
    	GUI_SetFont(&FONT_TITLE);
    	BUTTON_SetDefaultFont(&FONT_TITLE);
    	TEXT_SetDefaultFont(&FONT_TITLE);
    	GUI_UC_SetEncodeUTF8();
	}
}


const char * Application::getTitle() {
	if (this->current_ui!=0)
		return this->current_ui->getTitle();
	return 0;
}

void Application::drawTitle() {
	const char * title = this->getTitle();
	if (title!=0)
		GUI_DispStringAt(title,  TITLE_XPOS, TITLE_YPOS);
}

void Application::start() {
	GUI_Init();
	this->drawLogo();
	Lcd_Light_ON;
    gui_view_init();
}

void Application::setup() {
	this->screenOffCountDown = STANDBY_TIME;
	if (this->screenOffCountDown < 60)
		this->screenOffCountDown = 60;

	TEXT_SetDefaultTextColor(gCfgItems.title_color);
    GUI_SetBkColor(gCfgItems.background_color);
    GUI_SetColor(gCfgItems.title_color);

    BUTTON_SetDefaultBkColor(gCfgItems.btn_color, BUTTON_CI_UNPRESSED);
    BUTTON_SetDefaultBkColor(gCfgItems.btn_color, BUTTON_CI_PRESSED);
    BUTTON_SetDefaultTextColor(gCfgItems.btn_textcolor, BUTTON_CI_UNPRESSED);
    BUTTON_SetDefaultTextColor(gCfgItems.btn_textcolor, BUTTON_CI_PRESSED);
    GUI_SetFont(&FONT_TITLE);
    BUTTON_SetDefaultFont(&FONT_TITLE);
    TEXT_SetDefaultFont(&FONT_TITLE);
    GUI_UC_SetEncodeUTF8();
    make_lang_str();
}

void Application::closeCurrentWidget() {
	if (this->current_ui)
		this->current_ui->hide();
	else
        GUI_Clear();
}

void Application::refresh() {

}

void Application::refresh_05() {
    if (beeper.count>0)
        BEEPER_OP = 1;
	if (this->waitPenUp > 1) {
		if (--this->waitPenUp==1) {
			this->waitPenUp = 0;
			this->screenOffCountDown = STANDBY_TIME;
		}
	}
}

void Application::refresh_1s() {
    if (beeper.count > 0) {
        BEEPER_OP = 0;
        beeper.count--;
    };
    _calc_rate();
    if ((gCfgItems.standby_mode) && (this->screenOffCountDown>0)) {
        if (--this->screenOffCountDown==0)
            lcd_light_off();
    }
}

static void _calc_rate(void) {
    if (card.filesize == 0) {
        ui_print_process.rate = 0;
    } else {
        float rate;
        if ((ui_print_process.preview_state_flags & (1<<PREVIEW_EXISTS_BIT)) != 0) {
            int offset = PREVIEW_SIZE + ui_print_process.preview_offset;
            if ((card.sdpos <= offset) || (card.filesize <= offset))
                rate = 0;
            else
                rate = ((float)(card.sdpos - offset)) / (card.filesize - offset);
        } else
            rate = ((float)card.sdpos) / card.filesize;
        ui_print_process.rate = (u8)(rate * 100);
        if ((ui_print_process.rate==100) && (card.filesize>card.sdpos))
            ui_print_process.rate = 99;
    }
}

char Application::touch(u8 action) {
	if (gCfgItems.standby_mode) {
		if (this->screenOffCountDown==0) {
			if ((action==PEN_DOWN) && (this->waitPenUp==0)) {
                lcd_light_on();
				this->waitPenUp = 1;
			} else if ((action==PEN_UP) && (this->waitPenUp==1)) {
				this->waitPenUp = 3;
			}
			return 0;
		} else if (action==PEN_DOWN)
			this->screenOffCountDown = STANDBY_TIME;
	}
	return 1;
}

void Application::loop() {

}

void Application::idle() {
    if (wifi_link_state != WIFI_TRANS_FILE) {
        this->refresh();
        if (this->current_ui)
            this->current_ui->refresh();
    }

    if (is_ui_timing(F_UI_TIMING_HALF_SEC)) {
        ui_timing_clear(F_UI_TIMING_HALF_SEC);
        this->refresh_05();
    }
    if (is_ui_timing(F_UI_TIMING_SEC)) {
        ui_timing_clear(F_UI_TIMING_SEC);
        this->refresh_1s();
    }

    GUI_TOUCH_Exec();
    GUI_Exec();
}

void Application::systick() {

	if(!(TimeIncrease * TICK_CYCLE % 500))	// 0.5 sec
		ui_timing_set(F_UI_TIMING_HALF_SEC);

	if(!(TimeIncrease * TICK_CYCLE % 1000)) { //1 sec
	    ui_timing_set(F_UI_TIMING_SEC);
		if(print_time.start == 1) {
			print_time.seconds++;
			if(print_time.seconds >= 60) {
				print_time.seconds = 0;
				print_time.minutes++;
				if(print_time.minutes >= 60) {
					print_time.minutes = 0;
					print_time.hours++;
				}
			}
		}
	}
}


void Application::dropPreview() {
	ui_print_process.preview_state_flags = 0;
	char has_preview = 0;
	epr_write_data(EPR_PREVIEW_FROM_FLASH, &has_preview,1);
}

void Application::terminatePrintFile() {
	stop_print_time();
	card.stopSDPrint();
	wait_for_heatup = false;
	mksReprint.mks_printer_state = MKS_STOP;
	gCfgItems.breakpoint_reprint_flg = 0;
	gCfgItems.breakpoint_z_pos = 0;
	gCfgItems.breakpoint_flg=0;
	reset_file_info();
	ui_app.showMainWidget();
}

char f_exists(const char * name) {
    FIL file;
    if (f_open(&file, name, FA_OPEN_EXISTING | FA_READ) == FR_OK) {
        f_close(&file);
        return 1;
    }
    return 0;
}


static void _do_continue_print() {
    card.openFile(mksReprint.filename, true);
    ui_print_process.preview_state_flags = 0;
    strcpy(ui_print_process.file_name, mksReprint.filename);
    if((mksReprint.sdpos > MIN_FILE_PRINTED)||(mksReprint.sdpos_from_epr>MIN_FILE_PRINTED)) {
        epr_write_data(EPR_SAV_FILENAME, (uint8_t *)&mksReprint.filename[0],sizeof(mksReprint.filename));
        card.sdprinting = 0;

        if(mksReprint.resume == MKS_RESUME_PWDWN)
            mks_getPositionXYZE();

        if(gCfgItems.pwroff_save_mode != 1)
            card.setIndex(mksReprint.sdpos);
        else
            card.setIndex(mksReprint.sdpos_from_epr);
        current_position[X_AXIS] = mksReprint.current_position[0];
        current_position[Y_AXIS] = mksReprint.current_position[1];
        current_position[Z_AXIS] = mksReprint.current_position[2];
        printing_ui.show();
    } else {
        mksReprint.resume = MKS_RESUME_IDLE;
        mksReprint.mks_printer_state = MKS_IDLE;
        if(gCfgItems.pwroff_save_mode != 1)
            epr_write_data(EPR_SAV_FLAG, (uint8_t *)&mksReprint.mks_printer_state,sizeof(mksReprint.mks_printer_state));  //
        ui_app.showMainWidget();
    }
}


static unsigned char _reread_state_from_file = 0;

void Application::on_action_dialog(u8 action, u8 dialog_id) {
    if (action==UI_BUTTON_OK) {
        if(f_exists(mksReprint.filename)) {
            confirm_dialog_ui.hide();
            if (_reread_state_from_file)
                mks_read_state_from_file();
            _do_continue_print();
        } else
            this->beep(1);
    } else if (action==UI_BUTTON_CANCEL) {
        confirm_dialog_ui.hide();
        this->showMainWidget();
    }
}


void Application::continuePrintFile(unsigned char reread_state_from_file) {
    _reread_state_from_file = reread_state_from_file;
    SERIAL_PROTOCOLLN("CONTINUE PRINT");
    if(!f_exists(mksReprint.filename)) {
        if (this->current_ui)
            this->current_ui->hide();
        this->beep(1);
        sprintf(ui_buf1_100, lang_str.dialog.confirm_file_not_found,mksReprint.filename);
        confirm_dialog_ui.show(ui_buf1_100, this, DIALOG_ID_NOFILE);
    } else {
        _do_continue_print();
    }
}


void Application::startPrintFile(unsigned char savedPreview) {
	this->closeCurrentWidget();

	reset_print_time();
	start_print_time();
	ui_print_process.rate = 0;
	if(gCfgItems.breakpoint_reprint_flg == 1)
		gCfgItems.breakpoint_z_pos = current_position[Z_AXIS];

	ui_print_process.suicide_enabled = gCfgItems.print_finish_close_machine_flg;
	if (savedPreview==0)
		this->dropPreview();

	if(card.openFile(ui_print_process.file_name, true)) {
		filament_counter = 0;
		feedrate_percentage = 100;
		saved_feedrate_percentage = feedrate_percentage;
		planner.flow_percentage[0] = 100;
		planner.e_factor[0]= planner.flow_percentage[0]*0.01;
		if(mksCfg.extruders==2) {
			planner.flow_percentage[1] = 100;
			planner.e_factor[1]= planner.flow_percentage[1]*0.01;
		}
		card.startFileprint();
		ui_print_process.once = 0;
	}

	printing_ui.show();
}

void Application::showMainWidget() {
    if (this->current_ui!=&main_ui) {
        this->closeCurrentWidget();
        main_ui.show();
    }
}

void Application::pop() {
	if (disp_state_stack._disp_index>0)
		disp_state_stack._disp_index--;
	disp_state = disp_state_stack._disp_state[disp_state_stack._disp_index];
}

void Application::push(Widget * widget) {
	this->current_ui = widget;
	if (widget==&main_ui)
        resetStack(widget);
	else {
		ui_push_disp_stack(widget->id);
	}
}

void Application::resetStack(Widget * widget) {
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
	if (f_open(&file, logo_file, FA_OPEN_EXISTING | FA_READ) == FR_OK) {
		while (size>0) {
			UINT readed;
			if((f_read(&file, bmp_public_buf, sizeof(bmp_public_buf), &readed) == FR_OK) && (readed != 0)) {
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
    delay(2000);
}

void ProgressUI::on_action_dialog(u8 action, u8 dialog_id) {
    ui_app.showMainWidget();
}

void ProgressUI::progress(const char * message, char progress) {
    if (ui_app.current_ui != &progress_dialog_ui) {
        Widget * last = ui_app.current_ui;
        if (last) {
            last->hide();
            if (last->getType() != WIDGET_DIALOG)
                this->stored_last_ui = last;
        }
        progress_dialog_ui.show(message, progress);
    } else {
        if (message)
            progress_dialog_ui.setMessage(message);
        if (progress >= 0)
            progress_dialog_ui.setProgress(progress);
    }
}

void ProgressUI::fail(const char * message) {
    if (ui_app.current_ui != &confirm_dialog_ui) {
        Widget * last = ui_app.current_ui;
        if (last) {
            last->hide();
            if (last->getType() != WIDGET_DIALOG)
                this->stored_last_ui = last;
        }
        confirm_dialog_ui.showEx(message, this, 0, 0, CONFIRM_DIALOG_OK_BUTTON);
    } else {
        confirm_dialog_ui.setMessage(message);
    }
}

void ProgressUI::done() {
    progress_dialog_ui.hide();
    if (this->stored_last_ui) {
        ui_app.current_ui = this->stored_last_ui;
        ui_app.current_ui->show();
    } else
        ui_app.showMainWidget();
    this->stored_last_ui = 0;
}
