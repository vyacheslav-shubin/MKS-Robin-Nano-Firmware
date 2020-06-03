//
// Created by shubin on 03.06.2020.
//

#include "LanguageConfigUI.h"
#include "lang_common.h"
#include "integration.h"

LanguageConfigUI language_config_ui;

void LanguageConfigUI::on_button(UI_BUTTON hBtn) {
    for(unsigned char i=0; i<LANG_COUNT; i++) {
        if (hBtn==this->ui.langs[i].button) {
            shUI::setLanguage(lang_defs[i].id);
            this->recreate();
            return;
        }
    }
    ConfigurationWidget::on_button(hBtn);
}

void LanguageConfigUI::createControls() {
    ConfigurationWidget::createControls();
    this->dual_columns = 1;
    for(unsigned char i=0; i<LANG_COUNT; i++)
        this->createRadio(i / 5, i % 5, &this->ui.langs[i], lang_defs[i].name, lang_defs[i].id==gCfgItems.language);
}
