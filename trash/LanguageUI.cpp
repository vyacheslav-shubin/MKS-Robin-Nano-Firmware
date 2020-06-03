/*
 * LanguageUI.cpp
 *
 *  Created on: May 18, 2020
 *      Author: shubin
 */

#include "LanguageUI.h"
#include "lang_common.h"
#include "integration.h"


LanguageUI language_ui;

void LanguageUI::createControls() {
    memset(&this->ui, 0, sizeof(this->ui));

	this->ui.back = this->createButtonRet();
	int i=0;

#if LANG_EN
	this->ui.langs[i].button = this->createButtonAt(i % 4, i/4, img_lang_eng, 0);
	this->ui.langs[i].lang = LANG_ENGLISH;
	i++;
#endif

#if LANG_RU
	this->ui.langs[i].button = this->createButtonAt(i % 4, i/4, img_lang_rus, 0);
	this->ui.langs[i].lang = LANG_RUSSIAN;
	i++;
#endif

#if LANG_FR
	this->ui.langs[i].button = this->createButtonAt(i % 4, i/4, img_lang_fra, 0);
	this->ui.langs[i].lang = LANG_FRENCH;
	i++;
#endif

#if LANG_ES
	this->ui.langs[i].button = this->createButtonAt(i % 4, i/4, img_lang_spa, 0);
	this->ui.langs[i].lang = LANG_SPANISH;
	i++;
#endif

#if LANG_IT
	this->ui.langs[i].button = this->createButtonAt(i % 4, i/4, img_lang_ita, 0);
	this->ui.langs[i].lang = LANG_ITALY;
	i++;
#endif

#if LANG_CN_C
	this->ui.langs[i].button = this->createButtonAt(i % 4, i/4, img_lang_cn_comp, 0);
	this->ui.langs[i].lang = LANG_CN_C;
	i++;
#endif

#if LANG_CN_S
	this->ui.langs[i].button = this->createButtonAt(i % 4, i/4, img_lang_cn_simp, 0);
	this->ui.langs[i].lang = LANG_CN_S;
	i++;
#endif

}

void LanguageUI::on_button(UI_BUTTON hBtn) {
	if (hBtn==this->ui.back) {
		this->hide();
		ui_app.back_ui();
	} else for (unsigned char i=0; i<sizeof(this->ui.langs)/sizeof(this->ui.langs[0]); i++)
		if (hBtn==this->ui.langs[i].button) {
			shUI::setLanguage(ui.langs[i].lang);
			this->hide();
			ui_app.back_ui();
			break;
		}
}
