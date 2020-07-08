//
// Created by shubin on 08.07.2020.
//

#include "sh_tools.h"
#include "PowerControlUI.h"
#include "Application.h"

PowerControlUI power_control_ui;

void PowerControlUI::on_button(UI_BUTTON hBtn) {
    if (hBtn==this->ui.autoPowerOff.button) {
        gCfgItems.power_control_flags^=POWER_CONTROL_SUNCIDE;
        this->updateCheckButton(this->ui.autoPowerOff.button, gCfgItems.power_control_flags & POWER_CONTROL_SUNCIDE);
        epr_write_data(EPR_AUTO_CLOSE_MACHINE, (const unsigned char*)&gCfgItems.power_control_flags, sizeof(gCfgItems.power_control_flags));
    } else if (hBtn==this->ui.powerDet.button) {
        gCfgItems.mask_det_Function ^= MASK_DETECTOR_POWER;
        this->updateCheckButton(ui.powerDet.button, !(gCfgItems.mask_det_Function & MASK_DETECTOR_POWER));
        epr_write_data(EPR_MASK_DET_FUNCTION,(unsigned char *)&gCfgItems.mask_det_Function, sizeof(gCfgItems.mask_det_Function));
        powerDetector.init();
    } else if (hBtn==this->ui.tempCtrl.button) {
        gCfgItems.power_control_flags^=POWER_CONTROL_WAIT_HOTEND;
        this->updateCheckButton(this->ui.tempCtrl.button, gCfgItems.power_control_flags & POWER_CONTROL_WAIT_HOTEND);
        epr_write_data(EPR_AUTO_CLOSE_MACHINE, (const unsigned char*)&gCfgItems.power_control_flags, sizeof(gCfgItems.power_control_flags));
    }else if (hBtn==this->ui.hwAsSw.button) {
        gCfgItems.power_control_flags^=POWER_CONTROL_HARDWARE_AS_SOFTWARE;
        this->updateCheckButton(this->ui.hwAsSw.button, gCfgItems.power_control_flags & POWER_CONTROL_HARDWARE_AS_SOFTWARE);
        epr_write_data(EPR_AUTO_CLOSE_MACHINE, (const unsigned char*)&gCfgItems.power_control_flags, sizeof(gCfgItems.power_control_flags));
    } else
        ConfigurationWidget::on_button(hBtn);
}

void PowerControlUI::createControls() {
    ConfigurationWidget::createControls();
    memset(&this->ui, 0, sizeof(this->ui));
    this->dual_columns = 0;
    this->createCheckPair(0, 0, &this->ui.autoPowerOff, lang_str.config_ui.power_off_after_print, gCfgItems.power_control_flags & POWER_CONTROL_SUNCIDE);
    this->createCheckPair(0, 1, &this->ui.powerDet, lang_str.config_ui.power_detector, !(gCfgItems.mask_det_Function & MASK_DETECTOR_POWER));
    this->createCheckPair(0, 2, &this->ui.tempCtrl, lang_str.config_ui.power_off_temperature_wait, gCfgItems.power_control_flags & POWER_CONTROL_WAIT_HOTEND);
    this->createCheckPair(0, 3, &this->ui.hwAsSw, lang_str.config_ui.power_off_hardware_button_as_sofware, gCfgItems.power_control_flags & POWER_CONTROL_HARDWARE_AS_SOFTWARE);
}
