//
// Created by shubin on 08.07.2020.
//

#include "sh_tools.h"
#include "PowerControlUI.h"
#include "Application.h"
#include "integration.h"

PowerControlUI power_control_ui;

void PowerControlUI::on_button(UI_BUTTON hBtn) {
    if (hBtn==this->ui.autoPowerOff.button) {
        gCfgItems.power_control_flags^=POWER_CONTROL_SUNCIDE;
        this->updateCheckButton(this->ui.autoPowerOff.button, gCfgItems.power_control_flags & POWER_CONTROL_SUNCIDE);
        epr_write_data(EPR_POWER_CONTROL_FLAGS, (const unsigned char*)&gCfgItems.power_control_flags, sizeof(gCfgItems.power_control_flags));
    } else if (hBtn==this->ui.powerDet.button) {
        gCfgItems.feature_mask ^= MASK_DETECTOR_POWER;
        this->updateCheckButton(ui.powerDet.button, !(gCfgItems.feature_mask & MASK_DETECTOR_POWER));
        epr_write_data(EPR_MASK_DET_FUNCTION, (unsigned char *)&gCfgItems.feature_mask, sizeof(gCfgItems.feature_mask));
        powerDetector.init();
    } else if (hBtn==this->ui.tempCtrl.button) {
        gCfgItems.power_control_flags^=POWER_CONTROL_WAIT_HOTEND;
        this->updateCheckButton(this->ui.tempCtrl.button, gCfgItems.power_control_flags & POWER_CONTROL_WAIT_HOTEND);
        epr_write_data(EPR_POWER_CONTROL_FLAGS, (const unsigned char*)&gCfgItems.power_control_flags, sizeof(gCfgItems.power_control_flags));
    }else if (hBtn==this->ui.hwAsSw.button) {
        gCfgItems.power_control_flags^=POWER_CONTROL_HARDWARE_AS_SOFTWARE;
        this->updateCheckButton(this->ui.hwAsSw.button, gCfgItems.power_control_flags & POWER_CONTROL_HARDWARE_AS_SOFTWARE);
        epr_write_data(EPR_POWER_CONTROL_FLAGS, (const unsigned char*)&gCfgItems.power_control_flags, sizeof(gCfgItems.power_control_flags));
    }else if (hBtn==this->ui.presents.button) {
        gCfgItems.power_control_flags^=POWER_CONTROL_MODULE_PRESENTS;
        this->updateCheckButton(this->ui.presents.button, is_power_control_presents());
        epr_write_data(EPR_POWER_CONTROL_FLAGS, (const unsigned char*)&gCfgItems.power_control_flags, sizeof(gCfgItems.power_control_flags));
    } else if (hBtn==this->ui.hold.button) {
        gCfgItems.power_control_flags^=POWER_CONTROL_LOCK;
        this->updateCheckButton(this->ui.hold.button, gCfgItems.power_control_flags & POWER_CONTROL_LOCK, &lang_str.gnd_vcc);
        epr_write_data(EPR_POWER_CONTROL_FLAGS, (const unsigned char*)&gCfgItems.power_control_flags, sizeof(gCfgItems.power_control_flags));
        shUI::power_hold();
    } else if (hBtn==this->ui.reboot.button) {
        gCfgItems.power_control_flags^=POWER_CONTROL_REBOOT;
        this->updateCheckButton(this->ui.reboot.button, is_power_control_reboot(), &lang_str.yes_no);
    }
        ConfigurationWidget::on_button(hBtn);
}

void PowerControlUI::createControls() {
    ConfigurationWidget::createControls();
    memset(&this->ui, 0, sizeof(this->ui));
    this->dual_columns = 0;
    switch (this->page) {
        case 0: {
            this->createCheckPair(0, 0, &this->ui.presents, lang_str.config_ui.module_exists,
                                  is_power_control_presents());
            this->createCheckPair(0, 1, &this->ui.autoPowerOff, lang_str.config_ui.power_off_after_print,
                                  gCfgItems.power_control_flags & POWER_CONTROL_SUNCIDE);
            this->createCheckPair(0, 2, &this->ui.powerDet, lang_str.config_ui.power_detector,
                                  !(gCfgItems.feature_mask & MASK_DETECTOR_POWER));
            this->createCheckPair(0, 3, &this->ui.tempCtrl, lang_str.config_ui.power_off_temperature_wait,
                                  gCfgItems.power_control_flags & POWER_CONTROL_WAIT_HOTEND);
            break;
        }
        case 1: {
            this->createCheckPair(0, 0, &this->ui.hwAsSw, lang_str.config_ui.power_off_hardware_button_as_sofware,
                                  gCfgItems.power_control_flags & POWER_CONTROL_HARDWARE_AS_SOFTWARE);
            this->createCheckPair(0, 1, &this->ui.hold, lang_str.config_ui.power_hold,
                                  gCfgItems.power_control_flags & POWER_CONTROL_LOCK, &lang_str.gnd_vcc);
            this->createCheckPair(0, 2, &this->ui.reboot, lang_str.config_ui.power_reset, is_power_control_reboot(), &lang_str.yes_no);
            break;
        }
    }
}
