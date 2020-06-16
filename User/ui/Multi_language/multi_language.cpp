#include "draw_ui.h"
#include "Multi_language.h"
#include "lang_common.h"

LANG_DEF lang_defs[LANG_COUNT] = {{"EN", LANG_ENGLISH}, {"RU", LANG_RUSSIAN}};

#define PRINTING_OTHER_LANGUGE				"Printing"
#define PRINTING_OPERATION_OTHER_LANGUGE		"Operation"
#define PRINTING_PAUSE_OTHER_LANGUGE			"Pause"

//*********************************************//




common_menu_def common_menu;
move_menu_def move_menu;

void disp_language_init() {
	move_menu.x_add = AXIS_X_ADD_TEXT;
	move_menu.x_dec = AXIS_X_DEC_TEXT;
	move_menu.y_add = AXIS_Y_ADD_TEXT;
	move_menu.y_dec = AXIS_Y_DEC_TEXT;
	move_menu.z_add = AXIS_Z_ADD_TEXT;
	move_menu.z_dec = AXIS_Z_DEC_TEXT;

	move_menu.step_001mm = TEXT_001MM;
	move_menu.step_005mm = TEXT_005MM;
	move_menu.step_01mm = TEXT_01MM;
	move_menu.step_1mm = TEXT_1MM;
	move_menu.step_10mm = TEXT_10MM;

	//todo: Весь код по локализации сюда
	make_lang_str();

	switch(gCfgItems.language) {
		case LANG_RUSSIAN:
			common_menu.dialog_confirm_title = TITLE_DIALOG_CONFIRM_RU;
			common_menu.text_back=BACK_TEXT_RU;
			common_menu.close_machine_tips = DIALOG_CLOSE_MACHINE_RU;
			common_menu.unbind_printer_tips = DIALOG_UNBIND_PRINTER_RU;
			common_menu.print_special_title = PRINTING_OTHER_LANGUGE;
			common_menu.pause_special_title = PRINTING_PAUSE_OTHER_LANGUGE;
			common_menu.operate_special_title = PRINTING_OPERATION_OTHER_LANGUGE;
			move_menu.title = MOVE_TEXT_RU;
		break;

		case LANG_ENGLISH:
		default:
			common_menu.dialog_confirm_title = TITLE_DIALOG_CONFIRM_EN;
			common_menu.text_back=BACK_TEXT_EN;
			common_menu.close_machine_tips = DIALOG_CLOSE_MACHINE_EN;
			common_menu.unbind_printer_tips = DIALOG_UNBIND_PRINTER_EN;
			common_menu.print_special_title = PRINTING_OTHER_LANGUGE;
			common_menu.pause_special_title = PRINTING_PAUSE_OTHER_LANGUGE;
			common_menu.operate_special_title = PRINTING_OPERATION_OTHER_LANGUGE;			
			move_menu.title = TITLE_MOVE_EN;
			common_menu.dialog_confirm_title = TITLE_DIALOG_CONFIRM_EN;
			common_menu.text_back=BACK_TEXT_EN;
			common_menu.close_machine_tips = DIALOG_CLOSE_MACHINE_EN;
			common_menu.unbind_printer_tips = DIALOG_UNBIND_PRINTER_EN;
			common_menu.print_special_title = PRINTING_OTHER_LANGUGE;
			common_menu.pause_special_title = PRINTING_PAUSE_OTHER_LANGUGE;
			common_menu.operate_special_title = PRINTING_OPERATION_OTHER_LANGUGE;			
			move_menu.title = TITLE_MOVE_EN;
		break;
	}
}

