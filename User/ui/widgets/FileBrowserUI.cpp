/*
 * FileBrowserUI.cpp
 *
 *  Created on: May 15, 2020
 *      Author: shubin
 */

#include "FileBrowserUI.h"
#include "ui_tools.h"
#include "fatfs.h"
#include "serial.h"
#include "ff.h"
#include "ui_tools.h"
#include "sh_tools.h"

FileBrowserUI file_browser_ui;

#define COL		(OTHER_BTN_XPIEL*3+INTERVAL_V*4)
#define ROW(py)	((OTHER_BTN_YPIEL+INTERVAL_H)*(py))

#define MAX_FILE_NAME_SIZE	100

class Browser{
private:
	FATFS fs;

	char is_wanted(FILINFO * fi) {
		if ((fi->lfname[0] == '.') || (fi->fname[0] == '.') || (fi->fattrib & (AM_SYS|AM_HID)))
			return 0;
		return (strstr(fi->fname, ".GCO") || (fi->fattrib & AM_DIR));
	}
protected:

	virtual char onFile(FILINFO * fi) {
		return 0;
	}
public:
	char curent_dir[MAX_FILE_NAME_SIZE];
	int dir_level = 0;

	virtual void lookup() {
		FILINFO fi;
		static TCHAR lfn[_MAX_LFN + 1];
		fi.lfname = lfn;
		fi.lfsize = _MAX_LFN + 1;

		if (f_mount(&fs, (TCHAR const*)SD_Path, 0) == FR_OK) {
			DIR dir;
			memset(&dir, 0, sizeof(dir));
			SERIAL_ECHO("Open dir:");
			SERIAL_ECHOLN(curent_dir);
			if (f_opendir(&dir, curent_dir) == FR_OK) {
				while (f_readdir(&dir, &fi) == FR_OK) {
					if (fi.fname[0] == 0)
						break;
					if (is_wanted(&fi)) {
						if (!onFile(&fi))
							break;
					}
				}
				f_closedir(&dir);
			} else
				this->close();
			f_mount(&fs, SD_Path, 1);
		} else
			this->close();
	}

	virtual void close() {
		strcpy(this->curent_dir, "1:");
		this->dir_level = 0;
	}

	virtual void pushDirectory(char * name) {
		strcat(curent_dir, "/");
		strcat(curent_dir, name);
		this->dir_level++;
	}

	virtual void popDirectory() {
		if (this->dir_level!=0) {
			unsigned char i=0;
			unsigned char p=0;
			while (curent_dir[i]!=0) {
				if (curent_dir[i]=='/')
					p = i;
				i++;
			}
			if (p != 0) {
				curent_dir[p] = 0;
				this->dir_level--;
			}
		}
	}

	Browser() {
		this->close();
	}

	virtual ~Browser() {}
};

typedef struct {
	char more;
	unsigned char skip = 0;
} DIRECTORY_MARKER;

class UIFileBrouser: public Browser {
private:
	unsigned char index;
	unsigned char current = 0;
	DIRECTORY_MARKER dirm[10];
protected:
	virtual char onFile(FILINFO * fi) {
		if (this->current<this->dirm[this->dir_level].skip) {
			this->current++;
			return 1;
		}

		if (this->index>5) {
			this->dirm[this->dir_level].more = 1;
			return 0;
		}
		char * fn =  ((fi->lfname!=0) && (fi->lfname[0]!=0))? fi->lfname:fi->fname;
		char * pic;
		UI_FILE_BUTTON * ufb = &file_browser_ui.ui.files[this->index];
		ufb->isDirectory = fi->fattrib & AM_DIR;
		strncpy(ufb->fileName, fi->fname, 12);
		if (ufb->isDirectory) {
			pic = img_dir;
		} else {
			sprintf(ui_buf1_100, "%s/%s", curent_dir, ufb->fileName);
			ufb->withPreview = ui_file_with_preview(ui_buf1_100, &ufb->previewOffset);
			pic = ufb->withPreview ? img_file_wpv : img_file;
		}
		SERIAL_ECHOLN(fn);
		ufb->button = file_browser_ui.createButtonAt(this->index % 3, this->index/3, pic, fn);

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
	this->ui.prev = this->create100x80Button(COL,ROW(0), img_page_up);
	this->ui.next = this->create100x80Button(COL,ROW(1), img_page_down);
	this->ui.back = this->create100x80Button(COL,ROW(2), img_page_back);
	GUI_Exec();
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

#include "draw_dialog.h"

void FileBrowserUI::doButton(char index) {
	if (this->ui.files[index].isDirectory) {
		browser.pushDirectory(this->ui.files[index].fileName);
		this->recreate();
	} else {
		if (is_filament_fail()) {
			this->hide();
			draw_dialog(DIALOG_TYPE_FILAMENT_NO_PRESS);
		} else {
			sprintf(ui_print_process.file_name, "%s/%s", browser.curent_dir, this->ui.files[index].fileName);
			ui_app.startPrintFile();
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
		if (this->ui.files[i].withPreview) {
			sprintf(ui_buf1_100, "%s/%s", browser.curent_dir, ui.files[i].fileName);
			int x = ui_std_col(i%3) + 33;
			int y = ui_std_row(i/3) + 28 + titleHeight;
			ui_gcode_small_preview(ui_buf1_100, ui.files[i].previewOffset, x, y);
		}
	}
}
