/*
 * AdvancedConfigUI.cpp
 *
 *  Created on: May 21, 2020
 *      Author: shubin
 */

#include "AdvancedConfigUI.h"
#include "ConfirmDialogUI.h"

#include "ili9320.h"
#include "Application.h"

AdvancedConfigUI advanced_config_ui;

#define CFG_FILE        "1:/robin_nano35_cfg.txt"
#define CFG_BACUP_FILE  "1:/robin_nano35_cfg.CUR"

#define DIALOG_ID_CONFIG_RENAME     0
#define DIALOG_ID_CONFIG_NOT_FOUND  1

void AdvancedConfigUI::on_action_dialog(u8 action, u8 dialog_id) {
    confirm_dialog_ui.hide();
    switch (dialog_id) {
        case DIALOG_ID_CONFIG_NOT_FOUND:
            break;
        case DIALOG_ID_CONFIG_RENAME:
            f_unlink(CFG_FILE);
            f_rename(CFG_BACUP_FILE, CFG_FILE);
            break;
    }
    this->show();
}

void AdvancedConfigUI::on_button(UI_BUTTON hBtn) {
    if(hBtn == this->ui.simpleMainUI.button) {
		ui_invert_u8_flag(gCfgItems.display_style);
		this->updateCheckButton(ui.simpleMainUI.button, (char)gCfgItems.display_style);
		epr_write_data(EPR_SCREEN_DISPLAY_STYLE, (const unsigned char*)&gCfgItems.display_style, sizeof(gCfgItems.display_style));
	} else if(hBtn == this->ui.diplayBackLight.button) {
		ui_invert_u8_flag(gCfgItems.standby_mode);
		this->updateCheckButton(ui.diplayBackLight.button, (char)gCfgItems.standby_mode);
		epr_write_data(EPR_STANDBY_MODE, (const unsigned char*)&gCfgItems.standby_mode, sizeof(gCfgItems.standby_mode));
    } else if (hBtn == this->ui.restore.button) {
        this->hide();
        FIL f;
        if (f_open(&f, CFG_BACUP_FILE, FA_OPEN_EXISTING | FA_READ) == FR_OK) {
            f_close(&f);
            strcpy(ui_buf1_100, lang_str.config_ui.restore_config);
            strcat(ui_buf1_100, "?");
            confirm_dialog_ui.show(ui_buf1_100, this, DIALOG_ID_CONFIG_RENAME, this);
        } else {
            sprintf(ui_buf1_100, lang_str.dialog.confirm_file_not_found, CFG_BACUP_FILE);
            confirm_dialog_ui.showEx(ui_buf1_100, this, DIALOG_ID_CONFIG_NOT_FOUND, 0, CONFIRM_DIALOG_OK_BUTTON);
        }
	} else
        ConfigurationWidget::on_button(hBtn);
}

void AdvancedConfigUI::createControls() {
	ConfigurationWidget::createControls();
	memset(&this->ui, 0, sizeof(this->ui));
    this->dual_columns = 0;
	switch (this->page) {
	    case 0: {
            this->createCheckPair(0, 0, &this->ui.simpleMainUI, lang_str.config_ui.simple_main_ui,
                                  gCfgItems.display_style == 1);
            this->createCheckPair(0, 1, &this->ui.diplayBackLight, lang_str.config_ui.display_backlight_off,
                                  gCfgItems.standby_mode == 1);
            this->createConfigButton(0, 2, &this->ui.restore, lang_str.config_ui.restore_config);
            break;
        }
    }

}

