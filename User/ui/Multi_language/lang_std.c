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
}
