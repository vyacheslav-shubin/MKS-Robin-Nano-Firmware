#ifndef _LANG_COMMON
#define _LANG_COMMON

#define LANG_SIMPLE_CHINESE			1
#define LANG_COMPLEX_CHINESE		2
#define LANG_ENGLISH				3
#define LANG_JAPAN					4
#define LANG_GERMAN					5
#define LANG_FRENCH					6
#define LANG_RUSSIAN				7
#define LANG_KOREAN					8
#define LANG_TURKISH				9
#define LANG_SPANISH				10
#define LANG_GREEK					11
#define LANG_ITALY					12
#define LANG_PORTUGUESE				13

//enabled langs
#define LANG_RU		1
#define LANG_EN		1
#define LANG_ES		0
#define LANG_IT		0
#define LANG_FR		0
#define LANG_CN_S	0
#define LANG_CN_C	0

#define FONT_TITLE		GUI_FontHZ_fontHz14

#define LANG_COUNT (LANG_RU+LANG_EN+LANG_ES+LANG_IT+LANG_FR+LANG_CN_S+LANG_CN_C)

typedef struct {
    const char * name;
    unsigned char id;
} LANG_DEF;

extern LANG_DEF lang_defs[LANG_COUNT];

typedef struct {
    const char * power_off_after_print;
    const char * power_off_temperature_wait;
    const char * power_off_hardware_button_as_sofware;
    const char * power_hold;
    const char * simple_main_ui;
    const char * module_wifi;
    const char * module_exists;
	const char * display_backlight_off;
	const char * zerro_calc;
	const char * axes_inversion;
	const char * motor_steps_settings;
	const char * home_speed_settings;
	const char * z_safe_homing;
	const char * acceleration_settings;
	const char * maximum_speed_settings;
	const char * jerk_settings;

    const char * filament_change_temperature;
    const char * speed;
    const char * length;
    const char * ntp;

    const char * machine_size;
    const char * machine_type;
    const char * parking;
    const char * direction;
    const char * endtop_type;
    const char * filament_settings;
    const char * manual_leveling_settings;
    const char * auto_leveling_settings;
    const char * nozzle_settings;
    const char * hotbed_settings;
    const char * connectorZ;

    const char * restore_config;

    const char * point;
    const char * probe_offset;
    const char * probe_speed;

    const char * pause;
    const char * heater;
    const char * pid_termostat;
    const char * minumumT;
    const char * maximumT;
    const char * number_of_nozzles;
    const char * e0_sensor_type;
    const char * nozzle;
    const char * steps;
    const char * acceleration;
    const char * maximum_speed;
    const char * print;
    const char * travel;
    const char * retract;
    const char * time_shift;
    const char * power_control;
    const char * power_detector;
    const char * filament_detector;
} CONFIG_LANG_STR;

typedef struct {
	const char * size;
    const char * size_progress;
    const char * time;
    const char * time_progress;
    const char * layers;
    const char * layers_progress;
    const char * filament;
    const char * filament_progress;
	const char * min_max_x;
	const char * min_max_y;
	const char * min_max_z;
} FILE_INFO_LANG_STR;

typedef struct {
    const char * confirm_delete_file;
    const char * confirm_continue_print;
    const char * confirm_delete_mesh;
    const char * confirm_file_not_found;
	const char * confirm_terminate_print;
	const char * confirm_print_again;
	const char * confirm_print_with_suicide;
	const char * error_filament_end_detected;
    const char * refactored_by;
    const char * confirm_filament_load;
    const char * power_off;
    const char * power_off_waiting_hotend;
    const char * no_sd_card;
} DIALOG_LANG_STR;

typedef struct {
    const char * lookup;
    const char * connected;
    const char * disconnected;
    const char * state;
    const char * joining;
    const char * joined;
    const char * fail_join;

    const char * update_start;
    const char * update_reseting;
    const char * update_idle;
    const char * update_eraising;
    const char * update_uploading;
    const char * update_done;
    const char * uploading;
    const char * uploaded;
    const char * upload_fail;
    const char * upload_fail_deiail_time_expired;
    const char * protocol_error;
    const char * file_write_fail;
} WIFI_UI_LANG_STR;

typedef struct{
    const char* tr;
    const char* fl;
} BOOLEAN_LANG;

typedef struct {

	const char * preheat;
	const char * move;
	const char * home;
	const char * leveling;
	const char * filament;
	const char * back;
	const char * more;
	const char * continue_print;
	const char * print;
	const char * settings;
	const char * tools;
    const char * power_off;

    const char * g_code;

	const char * plus;
	const char * minus;

	const char * off;
	const char * bed;
	const char * extruder1;
	const char * extruder2;

	const char * stop;

	const char * load;
	const char * unload;

	const char * change;
	const char * fan;
	const char * speed;
	const char * babysteps;

	const char * machine;
	const char * temperature;
	const char * motors;
	const char * advanced;

    BOOLEAN_LANG yes_no;
    BOOLEAN_LANG on_off;
    BOOLEAN_LANG min_max;
    BOOLEAN_LANG gnd_vcc;

    const char *wifi;
    const char *cloud;
    const char *wifi_reconnect;
	const char *language;
	const char *about;
	const char *machine_settings;

    const char *overflow;

	const char * moving;
	const char * waiting;
	const char * preparing;
	const char * point_n_fmt;
	const char * start;


	const char * ui_title_main;
	const char * ui_title_tools;
	const char * ui_title_file_browser;
	const char * ui_title_babysteps;
	const char * ui_title_settings;
	const char * ui_title_language;
	const char * ui_title_preheat;
	const char * ui_title_fan;
	const char * ui_title_filament;
	const char * ui_title_move;
	const char * ui_title_home;
	const char * ui_title_leveling;
	const char * ui_title_config;
    const char * ui_title_more;
    const char * ui_title_mesh_leveling;
	const char * ui_title_printing_tools;
	const char * ui_title_printing;
    const char * ui_title_speed;
    const char * ui_title_config_machine_size;
    const char * ui_title_wifi_list;
    const char * ui_title_config_filament;
    const char * ui_title_config_home;
    const char * ui_title_leveling_type;
    const char * ui_title_leveling_points;
    const char * ui_title_probe_settins;
    const char * ui_title_hotbed_config;
    const char * ui_title_sensor_levels;
    const char * ui_title_motor_settings;
    const char * ui_title_machine_type_settings;
    const char * ui_title_extruders_config;
    const char * ui_title_config_advanced;
    const char * ui_title_power_contol;

	CONFIG_LANG_STR 	config_ui;
	FILE_INFO_LANG_STR	file_info_ui;
	DIALOG_LANG_STR		dialog;
    WIFI_UI_LANG_STR    wf;
} LANG_STR;



#if defined(__cplusplus)
extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif

extern LANG_STR lang_str;
extern void make_lang_str(void);
extern void lang_std(void);
extern void lang_ru(void);

#if defined(__cplusplus)
}
#endif




#define is_chinese() ((gCfgItems.language == LANG_SIMPLE_CHINESE)||(gCfgItems.language == LANG_COMPLEX_CHINESE))


#endif
