//
// Created by shubin on 03.06.2020.
//

#ifndef WORK_LANGUAGECONFIGUI_H
#define WORK_LANGUAGECONFIGUI_H

#include "ConfigurationWidget.h"
#include "lang_common.h"

typedef struct {
    UI_RADIO  langs[LANG_COUNT];
} LANGUAGE_CONFIG_UI_CONTROLS;

class LanguageConfigUI : public ConfigurationWidget{
private:
    LANGUAGE_CONFIG_UI_CONTROLS ui;
protected:
    virtual void on_button(UI_BUTTON hBtn);
    virtual void createControls();
public:
    virtual const char * getTitle() {return lang_str.ui_title_language;};
    LanguageConfigUI() : ConfigurationWidget(LANGUAGE_UI, 1) {};

};

extern LanguageConfigUI language_config_ui;

#endif //WORK_LANGUAGECONFIGUI_H
