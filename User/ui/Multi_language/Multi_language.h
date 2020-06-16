#ifndef MULTI_LANGUAGE_H
#define MULTI_LANGUAGE_H

#include "main.h"
#include "lang_common.h"
#include "Language_en.h"
#include "Language_ru.h"

extern void disp_language_init();

#define FONT_BUTTON	GUI_FontHZ_fontHz14
#define FONT_STATE_INF	GUI_FontHZ_fontHz14
#define FONT_TITLE		GUI_FontHZ_fontHz14

typedef struct common_menu_disp
{
	const char *text_back;
	const char *dialog_confirm_title;
	const char *close_machine_tips;
	const char *unbind_printer_tips;
	const char *print_special_title;
	const char *pause_special_title;
	const char *operate_special_title;
	const char *next;
	const char *previous;
}common_menu_def;
extern common_menu_def common_menu;


typedef struct move_menu_disp
{
	const char *title;
	const char *x_add;
	const char *x_dec;
	const char *y_add;
	const char *y_dec;
	const char *z_add;
	const char *z_dec;
	const char *step_001mm;
	const char *step_005mm;
	const char *step_01mm;
	const char *step_1mm;
	const char *step_10mm;
	const char *back;
}move_menu_def;
extern move_menu_def move_menu;

#if defined(MKS_ROBIN_NANO)
#define ABOUT_TYPE_TEXT				"MKS Robin Nano"
#elif defined(MKS_ROBIN_MINI)
#define ABOUT_TYPE_TEXT				"MKS Robin Mini"
#endif

#define ZOFFSET_STEP001      "0.01mm"
#define ZOFFSET_STEP01      "0.1mm"
#define ZOFFSET_STEP1      "1mm"


#endif
