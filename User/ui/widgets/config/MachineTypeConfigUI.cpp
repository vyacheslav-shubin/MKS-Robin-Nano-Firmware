//
// Created by shubin on 29.05.2020.
//

#include "ui_tools.h"
#include "MachineTypeConfigUI.h"
#include "mks_cfg.h"
#include "cardreader.h"


void MachineTypeConfigUI::on_button(UI_BUTTON hBtn) {
    if (ui_is_double_button(hBtn, this->ui.types[0])) {
        mksCfg.machinetype = 0;
    } else if (ui_is_double_button(hBtn, this->ui.types[1])) {
        mksCfg.machinetype = 1;
    } else if (ui_is_double_button(hBtn, this->ui.types[2])) {
        mksCfg.machinetype = 2;
    } else {
        ConfigurationWidget::on_button(hBtn);
        return;
    }
    epr_write_data(EPR_MACHINETPYE, (unsigned char*) &mksCfg.machinetype, sizeof(mksCfg.machinetype));
    mksCfg.machinetype = (1<<mksCfg.machinetype);
    this->updateControls();
    card.mksLoad();
}

void MachineTypeConfigUI::updateControls() {
    this->updateRadio(this->ui.types[0].button, (mksCfg.machinetype!=2) && (mksCfg.machinetype!=4));
    this->updateRadio(this->ui.types[1].button, mksCfg.machinetype==2);
    this->updateRadio(this->ui.types[2].button, mksCfg.machinetype==4);
}


void MachineTypeConfigUI::createControls() {
    ConfigurationWidget::createControls();
    this->dual_columns = 1;
    memset(&this->ui, 0, sizeof(this->ui));
    this->createRadio(0, 0, &this->ui.types[0], "XYZ", (mksCfg.machinetype!=2) && (mksCfg.machinetype!=4));
    this->createRadio(0, 1, &this->ui.types[1], "Delta", mksCfg.machinetype==2);
    this->createRadio(0, 2, &this->ui.types[2], "CoreXY", mksCfg.machinetype==4);
}

MachineTypeConfigUI machine_type_config_ui;
