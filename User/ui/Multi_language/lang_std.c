#include "lang_common.h"
#include "mks_cfg.h"

CONFIG_LANG lang_str;

void make_lang_str(void) {
	lang_std();
	switch(gCfgItems.language) {
		#if LANG_EN
		case LANG_ENGLISH:
			//default
			break;
		#endif
		#if LANG_RU
		case LANG_RUSSIAN:
			lang_ru();
			break;
		#endif
	}
}

void lang_std(void) {
	lang_str.power_off_after_print = "Auto Shutdown after print";
	lang_str.simple_main_ui = "Simple main UI";
	lang_str.axis_inversion = "Axis inversion";
	lang_str.motor_steps_settings = "Steps settings";
	lang_str.home_speed_settings = "Home speed setting";
	lang_str.acceleration_settings = "Acceleration settings";
	lang_str.maximum_speed_settings = "Maximum speed settings";
	lang_str.jerk_settings = "Jerk settings";

	lang_str.preheat = "Preheat";
	lang_str.move = "Move";
	lang_str.home = "Home";
	lang_str.leveling = "Leveling";
	lang_str.filament = "Filament";
	lang_str.back = "Back";
	lang_str.more = "More";
	lang_str.print = "Print";
	lang_str.settings = "Settings";
	lang_str.tools = "Tools";

	lang_str.plus = "More";
	lang_str.minus = "Less";

	lang_str.off = "Off";
	lang_str.bed = "Bed";
	lang_str.extruder1 = "Extruder 1";
	lang_str.extruder2 = "Extruder 2";

	lang_str.stop = "Stop";
	lang_str.load = "Load";
	lang_str.unload = "Unload";
	lang_str.change = "Change";
}
