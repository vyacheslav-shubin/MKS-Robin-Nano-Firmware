/*
 * FileBrowserUI.cpp
 *
 *  Created on: May 15, 2020
 *      Author: shubin
 */

#include "FileBrowserUI.h"
#include "ConfirmDialogUI.h"
#include "FileInfoUI.h"
#include "ui_tools.h"
#include "fatfs.h"
#include "serial.h"
#include "ff.h"
#include "sh_tools.h"
#include "integration.h"
#include "Application.h"


FileBrowserUI file_browser_ui;

#define COL		(OTHER_BTN_XPIEL*3+INTERVAL_V*4)
#define ROW(py)	((OTHER_BTN_YPIEL+INTERVAL_H)*(py))

#define MAX_FILE_NAME_SIZE	100

static TCHAR lfn[_MAX_LFN + 1];

void concat_file_name(char * dest,  char * directory, char * src) {
    strcpy(dest, directory);
    strcat(dest, "/");
    strncat(dest, src, 12);
}

char * get_long_file_name(char * fileName) {
	int i = strlen(fileName);
	while ((fileName[i] != '/') && (i>=0))
		i--;
	char * fn = &fileName[i+1];
	memset(ui_buf1_100, 0, sizeof(ui_buf1_100));
	strncpy(ui_buf1_100, fileName, i);
	FILINFO fi;
	fi.lfname = lfn;
	fi.lfsize = _MAX_LFN + 1;
	DIR dir;
	memset(&dir, 0, sizeof(dir));
	if (f_opendir(&dir, ui_buf1_100) == FR_OK) {
		while (f_readdir(&dir, &fi) == FR_OK) {
			SERIAL_ECHOLN(fi.fname);
			if (fi.fname[0] == 0)
				break;
			if (strncmp(fi.fname, fn, 12)==0) {
				if ((lfn!=0) && (lfn[0]!=0))
					fn = lfn;
				break;
			}
		}
		f_closedir(&dir);
	}
	return fn;
}


char Browser::is_wanted(FILINFO * fi) {
	if ((fi->fname[0] == '.') || (fi->fattrib & (AM_SYS|AM_HID)))
		return 0;
	if (
			(strstr(fi->fname, "mks_pic")!=0)
			|| (strstr(fi->fname, "bak_pic")!=0)
			|| (strstr(fi->fname, "mks_font")!=0)
			|| (strstr(fi->fname, "bak_font")!=0)
	)
		return 0;
	return (strstr(fi->fname, ".GCO") || (fi->fattrib & AM_DIR));
}
void Browser::lookup() {
	FILINFO fi;
	fi.lfname = lfn;
	fi.lfsize = _MAX_LFN + 1;

	DIR dir;
	memset(&dir, 0, sizeof(dir));
	if (f_opendir(&dir, current_dir) == FR_OK) {
		while (f_readdir(&dir, &fi) == FR_OK) {
			if (fi.fname[0] == 0)
				break;
			if (is_wanted(&fi)) {
				if (!onFile(&dir, &fi))
					break;
			}
		}
		f_closedir(&dir);
	} else
		this->close();
}

void Browser::close() {
	strcpy(this->current_dir, "1:");
	this->dir_level = 0;
}

void Browser::pushDirectory(char * name) {
	strcat(current_dir, "/");
	strcat(current_dir, name);
	this->dir_level++;
}

void Browser::popDirectory() {
	if (this->dir_level!=0) {
		unsigned char i=0;
		unsigned char p=0;
		while (current_dir[i] != 0) {
			if (current_dir[i] == '/')
				p = i;
			i++;
		}
		if (p != 0) {
            current_dir[p] = 0;
			this->dir_level--;
		}
	}
}


typedef struct {
	char more;
	unsigned char skip = 0;
} DIRECTORY_MARKER;

/*
Letter	CP1251	UTF16	UTF16->CP1251	                    UTF8	UTF8->CP1251
Ё	    A8	    04 01	byte1==0x04 && byte2==0x01 ? 0xA8	D0 81	byte1==0xD0 && byte2==0x81 ? 0xA8
ё	    B8	    04 51	byte1==0x04 && byte2==0x51 ? 0xB8	D1 91	byte1==0xD1 && byte2==0x91 ? 0xB8
А	    C0	    04 10	byte1 == 0x04 ? byte2 + 0xB0	    D0 90	byte1 == 0xD0 ? byte2 + 0x30
Я	    DF	    04 2F	                                    D0 AF
а	    E0	    04 30	                                    D0 B0
п	    EF	    04 3F	                                    D0 BF
р	    F0	    04 40	                                    D1 80	byte1 == 0xD1 ? byte2 + 0x70
я	    FF	    04 4F	                                    D1 8F
*/



class UIFileBrouser: public Browser {
private:
	unsigned char index;
	unsigned char current = 0;
	DIRECTORY_MARKER dirm[10];
protected:
	virtual char onFile(DIR * dir, FILINFO * fi) {
		if (this->current < this->dirm[this->dir_level].skip) {
			this->current++;
			return 1;
		}

		if (this->index>5) {
			this->dirm[this->dir_level].more = 1;
			return 0;
		}
		const char * pic;
		UI_FILE_BUTTON * ufb = &file_browser_ui.ui.files[this->index];
		ufb->isDirectory = fi->fattrib & AM_DIR;
		strncpy(ufb->fileName, fi->fname, 12);
#if 0
        SERIAL_ECHOLNPAIR("S_ ", fi->fname);
        SERIAL_ECHO("H_ ");
        for (int jj=0;jj<12;jj++)
            SERIAL_ECHOPAIR(",", (int)((unsigned char)fi->fname[jj] & 0x00FF));
        SERIAL_EOL();
        SERIAL_ECHOLNPAIR("L_ ", fi->lfname);
#endif
		if (ufb->isDirectory) {
			pic = img_dir;
		} else {
		    concat_file_name(ui_buf1_100, current_dir, ufb->fileName);
			//sprintf(ui_buf1_100, "%s/%s", curent_dir, ufb->fileName);
            ui_file_check_preview(ui_buf1_100, &ufb->previewMeta);
            switch (ufb->previewMeta.mode) {
                case PREVIEW_50:
                    pic = img_file_wpv;
                    break;
                case PREVIEW_100:
                    pic = 0;
                    break;
                case PREVIEW_NONE:
                default:
                    pic = img_file;
                    break;
            }
		}

        if ((fi->lfname!=0) && (fi->lfname[0]!=0)) {
            strcpy(ui_buf1_100, fi->lfname);
        } else {
            strcpy(ui_buf1_100, fi->fname);
        }

		if (!ufb->isDirectory) {
            unsigned char i = strlen(ui_buf1_100) - 1;
            while ((ui_buf1_100[i] != '.') && (i > 0)) i--;
            ui_buf1_100[i] = 0;
        }
		ufb->button = file_browser_ui.createButtonAt(this->index % 3, this->index/3, pic, ui_buf1_100);
		this->index++;
		return 1;
	}
public:
	char isHaveMore() {
		return dirm[this->dir_level].more;
	}

	char isStart() {
		return dirm[this->dir_level].skip == 0;
	}

	virtual void lookup() {
		this->index = 0;
		this->current = 0;
		this->dirm[this->dir_level].more = 0;
		Browser::lookup();
	}
	void nextPage() {
		if (this->dirm[this->dir_level].more)
			this->dirm[this->dir_level].skip += 6;
	}
	void prevPage() {
		if (this->dirm[this->dir_level].skip != 0)
			this->dirm[this->dir_level].skip -= 6;
	}
	virtual void close() {
		memset(this->dirm, 0, sizeof(this->dirm));
		Browser::close();
	}

	virtual void pushDirectory(char * name) {
		Browser::pushDirectory(name);
		this->dirm[this->dir_level].skip = 0;
		this->dirm[this->dir_level].more = 0;
	}
};

UIFileBrouser browser;

void FileBrowserUI::createControls() {
	memset(&this->ui, 0, sizeof(this->ui));
	this->ui.prev = this->create96x80Button(COL, ROW(0), img_page_up);
	this->ui.next = this->create96x80Button(COL, ROW(1), img_page_down);
	this->ui.back = this->create96x80Button(COL, ROW(2), img_page_back);
	GUI_Exec();
	f_mount(&fs, (TCHAR const*)SD_Path, 0);
	browser.lookup();
	GUI_Exec();
	this->drawPreview();
}

void FileBrowserUI::doBack(){
	if (browser.dir_level>0) {
		browser.popDirectory();
		this->recreate();
	} else {
		browser.close();
		this->action_back();
	}
}
void FileBrowserUI::doNext() {
	if (browser.isHaveMore()) {
		browser.nextPage();
		this->recreate();
	}
}
void FileBrowserUI::doPrev(){
	if (!browser.isStart()) {
		browser.prevPage();
		this->recreate();
	}
}

void FileBrowserUI::activatePrint(u8 index) {
    concat_file_name(ui_buf1_100, browser.current_dir, this->ui.files[index].fileName);
    ui_app.selectPrintFile(ui_buf1_100);
	file_info_ui.show(this);
}

void FileBrowserUI::on_action_dialog(u8 action, u8 dialog_id) {
	if (action == UI_BUTTON_OK) {
		if (is_filament_fail()) {
			shUI::pushGcode("M300 P300");
		} else {
			confirm_dialog_ui.hide();
			this->activatePrint(this->ui.selectedButton);
		}
	} else if (action == UI_BUTTON_CANCEL){
		confirm_dialog_ui.hide();
		this->recreate();
	}
}


void FileBrowserUI::doButton(char index) {
	if (this->ui.files[index].isDirectory) {
		browser.pushDirectory(this->ui.files[index].fileName);
		this->recreate();
	} else {
		this->hide();
		if (is_filament_fail()) {
			this->ui.selectedButton = index;
			confirm_dialog_ui.show(lang_str.dialog.error_filament_end_detected, this, 0, this);
		} else {
			this->activatePrint(index);
		}
	}
}

void FileBrowserUI::on_button(UI_BUTTON hBtn) {
	if (hBtn==this->ui.back) {
		doBack();
	} else if (hBtn==this->ui.next) {
		doNext();
	} else if (hBtn==this->ui.prev) {
		doPrev();
	} else {
		for (char i=0;i<6;i++) {
			if (this->ui.files[i].button==hBtn) {
				this->doButton(i);
				break;
			}
		}
	}
}

void FileBrowserUI::drawPreview() {
	for(unsigned char i=0; i<6; i++) {
		if (this->ui.files[i].previewMeta.mode != PREVIEW_NONE) {
            concat_file_name(ui_buf1_100, browser.current_dir, ui.files[i].fileName);
            int x = 0;
            int y = 0;
            switch (this->ui.files[i].previewMeta.mode) {
                case PREVIEW_50:
                    x = ui_std_col(i%3) + 33;
                    y = ui_std_row(i/3) + 28 + titleHeight;
                    break;
                case PREVIEW_100:
                    x = ui_std_col(i%3) + 6;
                    y = ui_std_row(i/3) + 1 + titleHeight;
                    break;
            }
			ui_gcode_small_preview(ui_buf1_100, &(this->ui.files[i].previewMeta), x, y);
		}
	}
}
