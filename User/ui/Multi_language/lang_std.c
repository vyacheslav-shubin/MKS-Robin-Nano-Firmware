#include "lang_common.h"
#include "mks_cfg.h"

LANG_DEF lang_defs[LANG_COUNT] = {{"EN", LANG_ENGLISH}, {"RU", LANG_RUSSIAN}};
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
	lang_str.config_ui.power_off_after_print = "Auto shutdown after print";
    lang_str.config_ui.power_off_temperature_wait = "Wait for cooling before power off";
    lang_str.config_ui.power_off_hardware_button_as_sofware = "Hardware power button as software";
    lang_str.config_ui.power_hold = "Power hold";
	lang_str.config_ui.simple_main_ui = "Simple main UI";
    lang_str.config_ui.module_wifi = "WiFi module";
    lang_str.config_ui.module_exists = "Power module presents";
	lang_str.config_ui.display_backlight_off = "Standby display";
	lang_str.config_ui.axes_inversion = "Axes inversion";
	lang_str.config_ui.motor_steps_settings = "Steps settings";
	lang_str.config_ui.home_speed_settings = "Home speed setting";
    lang_str.config_ui.z_safe_homing = "Z safe homing";
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
    lang_str.config_ui.probe_offset = "Probe offset";
    lang_str.config_ui.probe_offset = "Probe speed";
    lang_str.config_ui.connectorZ = "Connector Z";
    lang_str.config_ui.pause = "Pause";

    lang_str.config_ui.restore_config = "Restore configuration file";

    lang_str.config_ui.heater = "Heater";
    lang_str.config_ui.pid_termostat = "PID termostat";
    lang_str.config_ui.minumumT = "Min T°C";
    lang_str.config_ui.maximumT = "Max T°C";;
    lang_str.config_ui.number_of_nozzles = "Number of nozzles";
    lang_str.config_ui.e0_sensor_type = "E0 sensor type";
    lang_str.config_ui.nozzle = "Nozzle";
    lang_str.config_ui.steps = "Steps";
    lang_str.config_ui.acceleration = "Acceleration";
    lang_str.config_ui.maximum_speed = "Maximum speed";
    lang_str.config_ui.print = "Print";
    lang_str.config_ui.travel = "Travel";
    lang_str.config_ui.retract = "Retract";
    lang_str.config_ui.time_shift = "Time shift (mins)";
    lang_str.config_ui.power_detector = "Power control loopback";
    lang_str.config_ui.filament_detector = "Filament detector";
    lang_str.config_ui.power_control = "Power control";
    lang_str.config_ui.ntp = "NTP supported";

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
    lang_str.power_off = "Power off";

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
    lang_str.cloud = "Cloud";
    lang_str.wifi_reconnect = "Select network";
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
    lang_str.ui_title_config_filament = "Filament settings";
    lang_str.ui_title_leveling_points = "Manual leveling points";
    lang_str.ui_title_leveling_type = "Leveling mode";
    lang_str.ui_title_mesh_leveling = "Mesh leveling execute";
    lang_str.ui_title_probe_settins = "Probe settings";
    lang_str.ui_title_config_home = "Home parking config";
    lang_str.ui_title_sensor_levels = "Sensors activalting level";
    lang_str.ui_title_motor_settings = "Motors settings";
    lang_str.ui_title_machine_type_settings = "Machine type settings";
    lang_str.ui_title_hotbed_config = "Hotbed configuration";
    lang_str.ui_title_extruders_config = "Nozzles config";
    lang_str.ui_title_wifi_list = "WiFi networks";
    lang_str.ui_title_config_advanced = "Advanced settings";
    lang_str.ui_title_power_contol = "Power control";

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
    lang_str.dialog.confirm_continue_print = "Printing start with Z:%.3f.\nContinue?";
    lang_str.dialog.confirm_delete_mesh = "Drop current mesh?";
    lang_str.dialog.confirm_file_not_found = "File not found: \n%s";
	lang_str.dialog.confirm_terminate_print = "Terminate printing?";
	lang_str.dialog.confirm_print_again = "Print again?";
	lang_str.dialog.confirm_print_with_suicide = "Auto power off.\nBreak and print again?";
	lang_str.dialog.error_filament_end_detected = "Filament end detected";
    lang_str.dialog.confirm_filament_load = "Load filament?";
    lang_str.dialog.power_off = "Power off";
    lang_str.dialog.power_off_waiting_hotend = "Waiting for cooling hotend";
    lang_str.dialog.no_sd_card = "No SD card.";

    lang_str.wf.state = "State";
    lang_str.wf.connected = "Connected";
    lang_str.wf.disconnected = "Disconnected";
    lang_str.wf.fail_join = "Failed join";
    lang_str.wf.joining = "Joining";
    lang_str.wf.joined = "Joined";
    lang_str.wf.lookup = "Lookup networks";
    lang_str.wf.uploading = "Uploading";
    lang_str.wf.uploaded = "Uploaded";
    lang_str.wf.update_start = "Wifi module firmware update";
    lang_str.wf.update_reseting = "Reseting module";
    lang_str.wf.update_idle = "Waiting for responce";
    lang_str.wf.update_eraising = "Memory eraiging";
    lang_str.wf.update_uploading = "Uploading firmware";
    lang_str.wf.update_done = "Done";
    lang_str.wf.upload_fail = "Upload fail";
    lang_str.wf.upload_fail_deiail_time_expired = "Timeout expired";
    lang_str.wf.protocol_error = "Protocol error";
    lang_str.wf.file_write_fail = "File write error";
}
