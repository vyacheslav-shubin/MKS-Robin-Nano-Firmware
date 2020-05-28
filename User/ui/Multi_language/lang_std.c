#include "lang_common.h"
#include "mks_cfg.h"

LANG_STR lang_str;

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
	lang_str.config_ui.power_off_after_print = "Auto Shutdown after print";
	lang_str.config_ui.simple_main_ui = "Simple main UI";
	lang_str.config_ui.display_backlight_off = "Standby display";
	lang_str.config_ui.axes_inversion = "Axes inversion";
	lang_str.config_ui.motor_steps_settings = "Steps settings";
	lang_str.config_ui.home_speed_settings = "Home speed setting";
	lang_str.config_ui.acceleration_settings = "Acceleration settings";
	lang_str.config_ui.maximum_speed_settings = "Maximum speed settings";
	lang_str.config_ui.jerk_settings = "Jerk settings";

    lang_str.config_ui.machine_size = "Machine size";
    lang_str.config_ui.machine_type = "Machine type";
    lang_str.config_ui.parking = "Parking";
    lang_str.config_ui.direction = "Direction";
    lang_str.config_ui.endtop_type = "Endstop";
    lang_str.config_ui.filament_settings = "Filament";
    lang_str.config_ui.manual_leveling_settings = "Leveling";
    lang_str.config_ui.auto_leveling_settings = "Auto leveling";
    lang_str.config_ui.nozzle_settings = "Nozzle";
    lang_str.config_ui.hotbed_settings = "Hotbed";

    lang_str.config_ui.filament_change_temperature = "T°C";
    lang_str.config_ui.speed = "Speed:";
    lang_str.config_ui.length= "Lenght:";;
    lang_str.config_ui.point = "Point %d";


    lang_str.dialog.refactored_by = "Software refactored by:\nVyacheslav Shubin\nshubin-vv@krista.ru";

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

	lang_str.fan = "Fan";
	lang_str.speed = "Speed";
	lang_str.babysteps = "Babysteps";
	lang_str.continue_print = "Continue";

	lang_str.yes_no.tr = "Yes";
	lang_str.yes_no.fl = "No";
    lang_str.on_off.tr = "On";
    lang_str.on_off.fl = "Off";
    lang_str.min_max.tr = "Max";
    lang_str.min_max.fl = "Min";
    lang_str.gnd_vcc.fl = "GND";
    lang_str.gnd_vcc.tr = "VCC";
	lang_str.wifi = "WiFi";
	lang_str.machine_settings = "Configuration";
	lang_str.language = "Language";
	lang_str.about = "Version";

	lang_str.machine = "Machine";
	lang_str.temperature = "Temperture";
	lang_str.motors = "Motors";
	lang_str.advanced = "Advanced";
    lang_str.overflow = "OVERFLOW";

	lang_str.ui_title_main = "Ready";
	lang_str.ui_title_tools = "Select tool";
	lang_str.ui_title_file_browser = "File browser";
	lang_str.ui_title_babysteps = "Babysteps settings";
	lang_str.ui_title_settings = "Select settings";
	lang_str.ui_title_language = "Select language";
	lang_str.ui_title_preheat = "Preheating tools";
	lang_str.ui_title_fan = "Fun tools";
	lang_str.ui_title_filament = "Filament tools";
	lang_str.ui_title_move = "Motors control";
	lang_str.ui_title_home = "Head parking";
	lang_str.ui_title_leveling = "Leveling tools";
	lang_str.ui_title_config = "Configuration";
	lang_str.ui_title_more = "More...";
	lang_str.ui_title_printing_tools = "Printing tools";
	lang_str.ui_title_printing = "Printing";
	lang_str.ui_title_speed = "Speed";
    lang_str.ui_title_config_machine_size = "Machine size";
    lang_str.ui_title_config_filament_change = "Filament change settings";
    lang_str.ui_title_leveling_points = "Manual leveling points";
    lang_str.ui_title_config_home = "Home parking config";
    lang_str.ui_title_sensor_levels = "Sensors activalting level";

	lang_str.file_info_ui.filament = "Filament (m): %1.3f";
    lang_str.file_info_ui.filament_progress = "Filament (m): %1.3f / %1.3f %d%%";
    lang_str.file_info_ui.size = "File readed: %d / %d %d%%";
    lang_str.file_info_ui.size_progress = "File size: %d / %d %d%%";
    lang_str.file_info_ui.time = "Printing time: %s";
    lang_str.file_info_ui.time_progress = "Printing time: %s / %s %d%%";
    lang_str.file_info_ui.layers = "Layers: %1.2f * %d";
    lang_str.file_info_ui.layers_progress = "Layers: %1.2f * %d / %d %d%%";
	lang_str.file_info_ui.min_max_x = "X: %1.2f - %1.2f";
	lang_str.file_info_ui.min_max_y = "Y: %1.2f - %1.2f";
	lang_str.file_info_ui.min_max_z = "Z: %1.2f - %1.2f";

	lang_str.dialog.confirm_delete_file = "Delete file:\n%s";
	lang_str.dialog.confirm_terminate_print = "Terminate printing?";
	lang_str.dialog.confirm_print_again = "Print again?";
	lang_str.dialog.confirm_print_with_suicide = "Auto power off.\nBreak and print again?";
	lang_str.dialog.error_filament_end_detected = "Filament end detected";
    lang_str.dialog.confirm_filament_load = "Load filament?";
}
