#ifndef _LANG_COMMON
#define _LANG_COMMON


#define LANG_RU		1
#define LANG_EN		1

typedef struct {
	const char * power_off_after_print;
	const char * simple_main_ui;
	const char * axis_inversion;
	const char * motor_steps_settings;
	const char * home_speed_settings;
	const char * acceleration_settings;
	const char * maximum_speed_settings;
	const char * jerk_settings;

	const char * preheat;
	const char * extrusion;
	const char * move;
	const char * home;
	const char * leveling;
	const char * filament;
	const char * back;
	const char * more;
	const char * print;
	const char * settings;
	const char * tools;
} CONFIG_LANG;

#if defined(__cplusplus)
extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif

extern CONFIG_LANG lang_str;
extern void make_lang_str(void);
extern void lang_std(void);
extern void lang_ru(void);

#if defined(__cplusplus)
}
#endif


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

#define TEXT_VALUE                    "%d/%d"
#define TEXT_VALUE_T                  ": %d℃"
#define TEXT_VALUE_mm                 ": %dmm"
#define TEXT_VALUE_cm                 ": %dcm"
#define TEXT_VALUE_m                  ": %dm"

#define TEMP_UNIT_SYBOL               "%d℃"
#define FLOAT_TEMP_UNIT_SYBOL               "%.1f℃"

#define TEXT_1C						"1℃"
#define TEXT_5C						"5℃"
#define TEXT_10C					"10℃"

#define AXIS_X_ADD_TEXT			    "X+"
#define AXIS_X_DEC_TEXT			    "X-"
#define AXIS_Y_ADD_TEXT			    "Y+"
#define AXIS_Y_DEC_TEXT			    "Y-"
#define AXIS_Z_ADD_TEXT			    "Z+"
#define AXIS_Z_DEC_TEXT			    "Z-"
#define TEXT_001MM					"0.01mm"
#define TEXT_005MM					"0.05mm"
#define TEXT_01MM					"0.1mm"
#define TEXT_1MM					"1mm"
#define TEXT_10MM					"10mm"

#define EXTRUDE_1MM_TEXT			"1mm"
#define EXTRUDE_5MM_TEXT			"5mm"
#define EXTRUDE_10MM_TEXT		    "10mm"

#define LANGUAGE_S_CN				"简体"
#define LANGUAGE_T_CN				"繁体"
#define LANGUAGE_EN					"English"
#define LANGUAGE_JP					"日本語"
#define LANGUAGE_GE					"Deutsch"
#define LANGUAGE_FR					"français"
#define LANGUAGE_IT                 "Italia"
#define LANGUAGE_PR                 "português"
#define LANGUAGE_KR                 "Korean"
#define LANGUAGE_BR                 "Brazil"
#define LANGUAGE_RU                 "русский"
#define LANGUAGE_SP                 "español"

#define HOME_X_TEXT					"X"
#define HOME_Y_TEXT					"Y"
#define HOME_Z_TEXT					"Z"
#define HOME_ALL_TEXT				"All"




#endif
