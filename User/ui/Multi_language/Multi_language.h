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

typedef struct machine_common_disp {
	const char *default_value;
	
	const char *next;
	const char *previous;
	
	const char *MachineConfigTitle;
	const char *MachineType;
	const char *Stroke;
	const char *HomeDir;
	const char *EndStopType;
	const char *FilamentConf;
	const char *LevelingConf;

	const char *MachineTypeConfTitle;
	const char *xyz;
	const char *delta;
	const char *corexy;
	
	const char *StrokeConfTitle;
	const char *xStroke;
	const char *yStroke;
	const char *zStroke;

	const char *xmin;
	const char *ymin;
	const char *zmin;

	const char *HomeDirConfTitle;
	const char *xHomeDir;
	const char *yHomeDir;
	const char *zHomeDir;
	const char *min;
	const char *max;

	const char *EndstopConfTitle;
	const char *xEndstop_min;
	const char *yEndstop_min;
	const char *zEndstop_min;
	const char *xEndstop_max;
	const char *yEndstop_max;
	const char *zEndstop_max;	
	const char *FilamentEndstop;
	const char *LevelingEndstop;
	const char *opened;
	const char *closed;

	const char *FilamentConfTitle;
	const char *InTemperature;
	const char *InLength;
	const char *InSpeed;
	const char *OutTemperature;
	const char *OutLength;
	const char *OutSpeed;	

	const char *LevelingParaConfTitle;
	const char *LevelingParaConf;
	const char *DeltaLevelConf;
	const char *XYZLevelconf;
	
	const char *LevelingSubConfTitle;
	const char *AutoLevelEnable;
	const char *BLtouchEnable;
	const char *ProbePort;
	const char *ProbeXoffset;
	const char *ProbeYoffset;
	const char *ProbeZoffset;
	const char *ProbeXYspeed;
	const char *ProbeZspeed;
	const char *enable;
	const char *disable;
	const char *z_min;
	const char *z_max;

	const char *LevelingSubDeltaConfTitle;
	const char *MachineRadius;
	const char *DiagonalRod;
	const char *PrintableRadius;
	const char *DeltaHeight;
	const char *SmoothRodOffset;
	const char *EffectorOffset;
	const char *CalibrationRadius;
	
	const char *LevelingSubXYZConfTitle;
	//const char *Level_positon1;
	//const char *Level_positon2;
	//const char *Level_positon3;
	//const char *Level_positon4;

	const char *TemperatureConfTitle;
	const char *NozzleConf;
	const char *HotBedConf;
	const char *PreheatTemperConf;

	const char *NozzleCnt;
	const char *NozzleConfTitle;
	const char *NozzleType;
	const char *NozzleAdjustType;
	const char *NozzleMinTemperature;
	const char *NozzleMaxTemperature;
	const char *Extrude_Min_Temper;

	const char *HotbedEnable;
	const char *HotbedConfTitle;
	const char *HotbedAjustType;
	const char *HotbedMinTemperature;
	const char *HotbedMaxTemperature;

	const char *MaxFeedRateConfTitle;
	const char *XMaxFeedRate;
	const char *YMaxFeedRate;
	const char *ZMaxFeedRate;
	const char *E0MaxFeedRate;
	const char *E1MaxFeedRate;

	const char *AccelerationConfTitle;
	const char *PrintAcceleration;
	const char *RetractAcceleration;
	const char *TravelAcceleration;
	const char *X_Acceleration;
	const char *Y_Acceleration;
	const char *Z_Acceleration;
	const char *E0_Acceleration;
	const char *E1_Acceleration;

	const char *JerkConfTitle;
	const char *X_Jerk;
	const char *Y_Jerk;
	const char *Z_Jerk;
	const char *E_Jerk;

	const char *StepsConfTitle;
	const char *X_Steps;
	const char *Y_Steps;
	const char *Z_Steps;
	const char *E0_Steps;
	const char *E1_Steps;

	const char *MotorDirConfTitle;

	//TODO: не нужно
	const char *X_MotorDir;
	const char *Y_MotorDir;
	const char *Z_MotorDir;
	const char *E0_MotorDir;
	const char *E1_MotorDir;
	const char *Invert_1;
	const char *Invert_0;

	const char *HomeFeedRateConfTitle;
	const char *XY_HomeFeedRate;
	const char *Y_HomeFeedRate;
	const char *Z_HomeFeedRate;

	const char *AdvancedConfTitle;
	const char *PwrOffDection;
	const char *PwrOffAfterPrint;
	const char *HaveUps;
	const char *Z2andZ2Endstop;
	const char *EnablePinsInvert;
	
	const char *Z2ConfTitle;
	const char *Z2Enable;
	const char *Z2EndstopEnable;
	const char *Z2Port;

	const char *EnablePinsInvertTitle;
	const char *XInvert;
	const char *YInvert;
	const char *ZInvert;
	const char *EInvert;

	const char *key_1;	
	const char *key_2;	
	const char *key_3;	
	const char *key_4;	
	const char *key_5;	
	const char *key_6;	
	const char *key_7;	
	const char *key_8;	
	const char *key_9;	
	const char *key_0;	
	const char *key_point;	
	const char *key_back;	
	const char *key_rest;	
	const char *key_confirm;	
	const char *negative;
	const char *low_level;
	const char *high_level;
	
}machine_common_def;
extern machine_common_def machine_menu;

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

typedef struct main_menu_disp
{
	const char *title;
	const char *preheat;
	const char *move;
	const char *home;
	const char *print;
	const char *extrude;
	const char *leveling;
	const char *autoleveling;
	const char *fan;
	const char *set;
	const char *tool;
	const char *more;
	const char *machine_para;
}main_menu_def;
extern main_menu_def main_menu;
typedef struct preheat_menu_disp
{
	const char *adjust_title;
	const char *title;
	const char *add;
	const char *dec;
	const char *ext1;
	const char *ext2;
	const char *hotbed;
	const char *off;
	const char *step_1c;
	const char *step_5c;
	const char *step_10c;
	const char *back;
	
	const char *value_state;	
	
	const char *dialog_tips;
	
}preheat_menu_def;
extern preheat_menu_def preheat_menu;
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
typedef struct home_menu_disp
{
	const char *title;
	const char *home_all;
	const char *home_x;
	const char *home_y;
	const char *home_z;
	const char *stopmove;
	const char *back;
}home_menu_def;
extern home_menu_def home_menu;
typedef struct file_menu_disp
{
	const char *title;
	const char *page_up;
	const char *page_down;
	const char *back;

	const char *file_loading;
	const char *no_file;
	const char *no_file_and_check;
	
}file_menu_def;
extern file_menu_def file_menu;
typedef struct extrude_menu_disp
{
	const char *title;
	const char *in;
	const char *out;
	const char *ext1;
	const char *ext2;
	const char *step_1mm;
	const char *step_5mm;
	const char *step_10mm;
	const char *low;
	const char *normal;
	const char *high;
	const char *back;

	const char *count_value_mm;
	const char *count_value_cm;
	const char *count_value_m;
	const char *temp_value;
	const char *temper_text;
}extrude_menu_def;
extern extrude_menu_def extrude_menu;
typedef struct leveling_menu_disp
{
	const char *title;
	const char *position1;
	const char *position2;
	const char *position3;
	const char *position4;
	const char *position5;
	
	char *back;
}leveling_menu_def;
extern leveling_menu_def leveling_menu;
typedef struct set_menu_disp
{
	const char *title;
	const char *filesys;
	const char *wifi;
	const char *about;
	const char *fan;
	const char *filament;
	const char *breakpoint;
	const char *motoroff;
	const char *shutdown;
	const char *language;
	const char *machine_para;
	const char *back;
}set_menu_def;
extern set_menu_def set_menu;
typedef struct filesys_menu_disp
{
const char *title;
const char *filesys;
const char *sd_sys;
const char *usb_sys;
const char *back;

}filesys_menu_def;
extern filesys_menu_def filesys_menu;
typedef struct more_menu_disp
{
	const char *title;
	const char *zoffset;
	const char *back;
}more_menu_def;
extern more_menu_def more_menu;
typedef struct wifi_menu_disp
{
	const char *title;
	const char *ip;
	const char *wifi;
	const char *key;
	const char *state_ap;
	const char *state_sta;
	const char *cloud;
	const char *connected;
	const char *disconnected;
	const char *exception;
	const char *back;
	const char *reconnect;
}wifi_menu_def;
extern wifi_menu_def wifi_menu;
typedef struct cloud_menu_disp
{
	const char *title;
	const char *unbind;
	const char *unbinding;
	const char *unbinded;
	const char *bind; 
	const char *binding;
	const char *binded;
	const char *disable;
	const char *disconnected;
	const char *back;
	const char *unbind_printer_tips;
}cloud_menu_def;
extern cloud_menu_def cloud_menu;

typedef struct about_menu_disp
{
	const char *title;
	const char *type_name;
	const char *firmware_v;
	const char *type;
	const char *version; 
	const char *wifi;
	const char *type_robin;
	const char *type_robin_mini;
	const char *back;
}about_menu_def;
extern about_menu_def about_menu;

typedef struct fan_menu_disp
{
	const char *title;
	const char *add;
	const char *dec; 
	const char *back;
	
	const char *state;
	const char *state_value;
}fan_menu_def;
extern fan_menu_def fan_menu;

typedef struct filament_menu_disp
{
	const char *title;
	const char *in;
	const char *out; 
	const char *ext1;
	const char *ext2;
	const char *back;
	const char *stat_temp;
	const char *ready_replace;
	const char *replacing;
	const char *loading;
	const char *unloading;
	const char *heating;
	const char *complete_and_back;
	const char *filament_dialog_load_heat;
	const char *filament_dialog_unload_heat;
	const char *filament_dialog_load_heat_confirm;
	const char *filament_dialog_unload_heat_confirm;
	const char *filament_dialog_loading;
	const char *filament_dialog_unloading;
	const char *filament_dialog_load_completed;
	const char *filament_dialog_unload_completed;
	const char *filament_dialog_ok;
	const char *filament_dialog_back;
}filament_menu_def;
extern filament_menu_def filament_menu;

typedef struct language_menu
{
	const char *title;
	const char *chinese_s;
	const char *chinese_t;
	const char *english;
	const char *russian;
	const char *japan;
	const char *italy;
	const char *german;
	const char *spanish;
	const char *korean;
	const char *french;
	const char *brazil;
	const char *portuguese;
	const char *next;
	const char *up;
	const char *back;
}language_menu_def;
extern language_menu_def language_menu;


typedef struct printing_menu_disp
{
	const char *title;
	const char *option;
	const char *temp1; 
	const char *temp2;
	const char *bed_temp;
	const char *fan_speed;	
	const char *pause;
	const char *resume;
	const char *stop;
}printing_menu_def;
extern printing_menu_def printing_menu;

typedef struct operation_menu_disp
{
	const char *title;
	const char *pause;
	const char *stop; 
	const char *temp;
	const char *fan;
	const char *filament;
	const char *extr;
	const char *speed;
	const char *move;
	const char *more;	
	const char *auto_off;
	const char *manual_off;
	const char *back;
	const char *babystep;
}operation_menu_def;
extern operation_menu_def operation_menu;
typedef struct pause_menu_disp
{
	const char *title;
	const char *resume;
	const char *stop; 
	const char *extrude;
	const char *move;
	const char *filament;	
	const char *more;
}pause_menu_def;
extern pause_menu_def pause_menu;

typedef struct speed_menu_disp
{
	const char *title;
	const char *add;
	const char *dec; 
	const char *extrude;
	const char *move;
	const char *back;
	const char *move_speed;
	const char *extrude_speed;
}speed_menu_def;
extern speed_menu_def speed_menu;

typedef struct printing_more_menu_disp
{
	const char *title;
	const char *fan;
	const char *auto_close; 
	const char *manual;
	const char *temp;	
	const char *speed;
	const char *back;
}printing_more_menu_def;
extern printing_more_menu_def printing_more_menu;

typedef struct dialog_menu_disp
{
	const char *confirm_title;
	
	const char *error1_repint_no_file;
	const char *error2_communication_fail;
	const char *error3_filename_too_long;
	const char *error4_no_file;
	const char *error5_check_filesys;

	const char *tip1_print_file;
	const char *tip2_stop_file;
}dialog_menu_def;
extern dialog_menu_def dialog_menu;

typedef struct print_file_dialog_disp
{
    const char *title;
	const char *confirm;
	const char *cancle;
	const char *print_file;
	const char *cancle_print;
	const char *retry;
	const char *stop;
	const char *no_file_print_tips;
	const char *print_from_breakpoint;
	const char *file_name_too_long_error;
	const char *close_machine_error;
	const char *filament_no_press;
	char *print_finish;
	char *print_time;
	char *reprint;
	char *wifi_enable_tips;
}print_file_dialog_menu_def;
extern print_file_dialog_menu_def print_file_dialog_menu;

typedef struct zoffset_menu_disp
{
	const char *title;
	const char *inc;
	const char *dec;
	const char *step001;
	const char *step01;
	const char *step1;
	const char *back;
}zoffset_menu_def;
extern zoffset_menu_def zoffset_menu;
	
typedef struct tool_menu_disp
{
	const char *title;
	const char *preheat;
	const char *extrude;
	const char *move;
	const char *home;
	const char *leveling;
	const char *autoleveling;
	const char *filament;
	const char *more;
	const char *back;
}tool_menu_def;
extern tool_menu_def tool_menu;

typedef struct MachinePara_menu_disp
{
	const char *title;
	const char *MachineSetting;
	const char *TemperatureSetting;
	const char *MotorSetting;
	const char *AdvanceSetting;
	//const char *back;
}MachinePara_menu_def;
extern MachinePara_menu_def MachinePara_menu;

typedef struct MachineSettings_menu_disp
{
	const char *title;
	const char *Machine;
	const char *Stroke;
	const char *HomeDir;
	const char *EndStopType;
	const char *filamet;
	const char *leveling;
	const char *back;
}MachineSettings_menu_def;
extern MachineSettings_menu_def MachineSettings_menu;

typedef struct TemperatureSettings_menu_disp
{
	const char *title;
	const char *nozzle;
	const char *hotbed;
	const char *preheat;
	const char *back;
}TemperatureSettings_menu_def;
extern TemperatureSettings_menu_def TemperatureSettings_menu;

#if defined(MKS_ROBIN_NANO)
#define ABOUT_TYPE_TEXT				"MKS Robin Nano"
#elif defined(MKS_ROBIN_MINI)
#define ABOUT_TYPE_TEXT				"MKS Robin Mini"
#endif
#define ABOUT_VERSION_TEXT			"1.0.3"
//#define ABOUT_WIFI_TEXT				"WiFi:"

#define WIFI_TEXT					"WIFI"
#define WIFI_IP_TEXT			"IP: "
#define WIFI_NAME_TEXT		"WiFi: "
#define WIFI_KEY_TEXT		"Key: "
#define WIFI_STATE_AP_TEXT			"State: AP"
#define WIFI_STATE_STA_TEXT			"State: STA"
#define WIFI_CONNECTED_TEXT			"Connected"
#define WIFI_DISCONNECTED_TEXT  "Disconnected"
#define WIFI_EXCEPTION_TEXT				"Exception"

#define FILAMENT_TIPS2_TEXT			"T:"

#define DIALOG_UPLOAD_ING_EN				"Uploading......"
#define DIALOG_UPLOAD_ERROR_EN				"Upload error"
#define DIALOG_UPLOAD_FINISH_EN				"Upload finished"
#define DIALOG_UPLOAD_SIZE_EN				"Size"
#define DIALOG_UPLOAD_TIME_EN				"Time"
#define DIALOG_UPLOAD_SPEED_EN				"Speed"
#define DIALOG_UPDATE_WIFI_FIRMWARE_EN		"Updating wifi model firmware"
#define DIALOG_UPDATE_WIFI_WEB_EN			"Updating wifi model web data"

#define ZOFFSET_STEP001      "0.01mm"
#define ZOFFSET_STEP01      "0.1mm"
#define ZOFFSET_STEP1      "1mm"



#if 0
//****************日文*************************//
#define TOOL_TEXT_JP		"ツール"
#define PREHEAT_TEXT_JP     "予熱"
#define	MOVE_TEXT_JP		"モバイル"
#define HOME_TEXT_JP        "ゼロ"
#define PRINT_TEXT_JP		"プリント"
#define EXTRUDE_TEXT_JP     "押出"
#define LEVELING_TEXT_JP    "レベリング"
#define AUTO_LEVELING_TEXT_JP   "オートレベリング"
#define SET_TEXT_JP			    "セット\nアップ"
#define MORE_TEXT_JP            "もっと"

#define ADD_TEXT_JP			    "さらに"
#define DEC_TEXT_JP			    "リダクション"
#define EXTRUDER_1_TEXT_JP	    "シャワーノズル1"
#define EXTRUDER_2_TEXT_JP		"シャワーノズル1"
#define HEATBED_TEXT_JP			"ホットベッド"
#define TEXT_1C_JP				"1℃"
#define TEXT_5C_JP				"5℃"
#define TEXT_10C_JP				"10℃"
#define CLOSE_TEXT_JP			"クローズ"

#define BACK_TEXT_JP					"リターン"

#define TOOL_PREHEAT_JP            "预热"
#define TOOL_EXTRUDE_JP             "挤出"
#define TOOL_MOVE_JP                "移动"
#define TOOL_HOME_JP               "回零"
#define TOOL_LEVELING_JP               "调平"
#define TOOL_AUTO_LEVELING_JP         "自动调平"
#define TOOL_FILAMENT_JP                "换料"
#define TOOL_MORE_JP                    "更多"

#define AXIS_X_ADD_TEXT_JP				"X+"
#define AXIS_X_DEC_TEXT_JP				"X-"
#define AXIS_Y_ADD_TEXT_JP				"Y+"
#define AXIS_Y_DEC_TEXT_JP				"Y-"
#define AXIS_Z_ADD_TEXT_JP				"Z+"
#define AXIS_Z_DEC_TEXT_JP				"Z-"
#define TEXT_01MM_JP					"0.1mm"
#define TEXT_1MM_JP						"1mm"
#define TEXT_10MM_JP					"10mm"

#define HOME_X_TEXT_JP					"X"
#define HOME_Y_TEXT_JP					"Y"
#define HOME_Z_TEXT_JP					"Z"
#define HOME_ALL_TEXT_JP				"ゼロ"
#define HOME_STOPMOVE_JP                "Quickstop"

#define PAGE_UP_TEXT_JP					"前のページ"
#define PAGE_DOWN_TEXT_JP				"下一ページ"

#define EXTRUDER_IN_TEXT_JP		    "フィード"
#define EXTRUDER_OUT_TEXT_JP		    "OUT"
#define EXTRUDE_1MM_TEXT_JP			    "1mm"
#define EXTRUDE_5MM_TEXT_JP			    "5mm"
#define EXTRUDE_10MM_TEXT_JP		    "10mm"
#define EXTRUDE_LOW_SPEED_TEXT_JP		"LOW"
#define EXTRUDE_MEDIUM_SPEED_TEXT_JP	"Medium"
#define EXTRUDE_HIGH_SPEED_TEXT_JP		"High"

#define LEVELING_POINT1_TEXT_JP				"Point1"
#define LEVELING_POINT2_TEXT_JP				"Point2"
#define LEVELING_POINT3_TEXT_JP			    "Point3"
#define LEVELING_POINT4_TEXT_JP				"Point4"
#define LEVELING_POINT5_TEXT_JP				"Point5"

#define FILESYS_TEXT_JP							    "FileSys"
#define WIFI_TEXT_JP							    "WIFI"
#define FAN_TEXT_JP								    "Fan"
#define ABOUT_TEXT_JP							    "About"
#define BREAK_POINT_TEXT_JP						    "Continue"
#define FILAMENT_TEXT_JP							"Filament"
#define LANGUAGE_TEXT_JP							"Language"
#define MOTO_OFF_TEXT_JP							"Moto-off"
#define SHUTDOWN_TEXT_JP							"Shutdown"

#define U_DISK_TEXT_JP								"USB"
#define SD_CARD_TEXT_JP								"SD"
#define WIFI_NAME_TEXT_JP							"WIFI: "
#define WIFI_KEY_TEXT_JP							"KEY: "
#define WIFI_IP_TEXT_JP								"IP: "
#define WIFI_AP_TEXT_JP								"STATE: AP"
#define WIFI_STA_TEXT_JP							"STATE: STA"
#define WIFI_CONNECTED_TEXT_JP				        "CONNECTED"
#define WIFI_DISCONNECTED_TEXT_JP			        "DISCONNECTED"
#define WIFI_EXCEPTION_TEXT_JP				        "EXCEPTION"
#define CLOUD_TEXT_JP								"Cloud"
#define CLOUD_BIND_JP								"Bind"
#define CLOUD_UNBIND_JP							    "Unbind"
#define CLOUD_UNBINDING_JP					        "Unbinding"
#define CLOUD_DISCONNECTED_JP				        "Disconnected"
#define CLOUD_UNBINDED_JP					        "Unbinded"
#define CLOUD_BINDED_JP						        "Binded"
#define CLOUD_DISABLE_JP						    "Disable"

#define FAN_ADD_TEXT_JP								"ADD"
#define FAN_DEC_TEXT_JP								"DEC"
#define FAN_OPEN_TEXT_JP							"100%"
#define FAN_HALF_TEXT_JP							"50%"
#define FAN_CLOSE_TEXT_JP							"Close"
#define FAN_TIPS1_TEXT_JP							"FAN\n%d"
#define FAN_TIPS2_TEXT_JP							"FAN\nClose"

#define FILAMENT_IN_TEXT_JP					"In"
#define FILAMENT_OUT_TEXT_JP				"Out"
#define FILAMENT_EXT0_TEXT_JP				"Extrusion1"
#define FILAMENT_EXT1_TEXT_JP				"Extrusion2"
#define FILAMENT_HEAT_TEXT_JP				"Preheat"					
#define FILAMENT_STOP_TEXT_JP				"Stop"
#define FILAMENT_CHANGE_TEXT_JP				"Filament replace"
#define FILAMENT_TIPS2_TEXT_JP				"T:%d℃/%d℃"
#define FILAMENT_TIPS3_TEXT_JP				"loading..."
#define FILAMENT_TIPS4_TEXT_JP				"unloading..."
#define FILAMENT_TIPS5_TEXT_JP				"Temp is too low to go,please heat"
#define FILAMENT_TIPS6_TEXT_JP				"Completed"

#define FILAMENT_CHANGE_TEXT_JP				"Please click <Load> \nor <unload>,After \npinter pause."
#define FILAMENT_DIALOG_LOAD_HEAT_TIPS_JP		"Heating up the nozzle,\nplease wait..."
#define FILAMENT_DIALOG_UNLOAD_HEAT_TIPS_JP		"Heating up the nozzle,\nplease wait..."
#define FILAMENT_DIALOG_LOAD_CONFIRM1_TIPS_JP           "Heat completed,please load filament \nto extruder,and click <confirm> \nfor start loading."
#define FILAMENT_DIALOG_LOAD_CONFIRM2_TIPS_JP           "Please load filament to extruder,\nand click <confirm> for start loading."
#define FILAMENT_DIALOG_UNLOAD_CONFIRM_TIPS_JP          "Heat completed,please \nclick <confirm> for start unloading.!"
#define FILAMENT_DIALOG_LOADING_TIPS_JP                 "Is loading ,please wait!"
#define FILAMENT_DIALOG_UNLOADING_TIPS_JP               "Is unloading,please wait!"
#define FILAMENT_DIALOG_LOAD_COMPLETE_TIPS_JP           "Load filament completed,\nclick <confirm> for return!"
#define FILAMENT_DIALOG_UNLOAD_COMPLETE_TIPS_JP         "Unload filament completed,\nclick <confirm> for return!"

#define PRE_HEAT_EXT_TEXT_JP				"E%d\n%d℃/%d℃"
#define PRE_HEAT_BED_TEXT_JP				"Bed%d\n%d℃/%d℃"

#define FILE_LOADING_JP					"にロード......"
#define NO_FILE_AND_CHECK_JP					"いいえファイル!\nファイルシステムをチェックしてください!"
#define NO_FILE_JP					"いいえファイル!"

#define EXTRUDER_TEMP_TEXT_JP				"T:%d℃"
#define EXTRUDER_E_LENGTH1_TEXT_JP          "E%d:%dmm"
#define EXTRUDER_E_LENGTH2_TEXT_JP          "E%d:%dcm"
#define EXTRUDER_E_LENGTH3_TEXT_JP          "E%d:%dm"

#define ABOUT_TYPE_TEXT_JP					"Type: "
#define ABOUT_VERSION_TEXT_JP				"Firmware: "
#define ABOUT_WIFI_TEXT_JP					"Wifi: "

#define PRINTING_OPERATION_JP				"Operation"
#define PRINTING_PAUSE_JP					"Pause"
#define PRINTING_TEMP_JP					"Temp."
#define PRINTING_CHANGESPEED_JP				"Speed"
#define PRINTING_RESUME_JP					"Resume"
#define PRINTING_STOP_JP					"Stop"
#define PRINTING_MORE_JP					"More"
#define PRINTING_EXTRUDER_JP				"Extruder"
#define PRINTING_MOVE_JP                    "Move"

#define EXTRUDER_SPEED_JP					"Extrude"
#define MOVE_SPEED_JP						"Move"
#define EXTRUDER_SPEED_STATE_JP				"Extrude Speed"
#define MOVE_SPEED_STATE_JP					"Move Speed"
#define STEP_1PERCENT_JP					"1%"
#define STEP_5PERCENT_JP					"5%"
#define STEP_10PERCENT_JP					"10%"

#define ZOFFSET_JP                          "Z Offset"
#define ZOFFSET_INC_JP                          "Add"
#define ZOFFSET_DEC_JP                          "Dec"

#define TITLE_READYPRINT_JP						"ReadyPrint"
#define TITLE_PREHEAT_JP						"Preheat"
#define TITLE_MOVE_JP					        "Move"
#define TITLE_HOME_JP							"Home"
#define TITLE_EXTRUDE_JP						"Extrude"
#define TITLE_LEVELING_JP						"Leveling"
#define TITLE_SET_JP							"Set"
#define TITLE_MORE_JP							"More"
#define TITLE_CHOOSEFILE_JP						"ChooseFile"
#define TITLE_PRINTING_JP						"Printing"
#define TITLE_OPERATION_JP						"Operation"
#define TITLE_ADJUST_JP							"Adjust"
#define	TITLE_WIRELESS_JP						"Wireless"
#define	TITLE_FILAMENT_JP						"Filament"
#define TITLE_ABOUT_JP							"About"
#define TITLE_FAN_JP							"Fan"
#define TITLE_LANGUAGE_JP						"Language"
#define TITLE_PAUSE_JP							"Pause"
#define TITLE_CHANGESPEED_JP				    "Speed"
#define TITLE_CLOUD_TEXT_JP				        "Cloud"
#define TITLE_DIALOG_CONFIRM_JP				"Confirm"
#define TITLE_FILESYS_JP								"FileSys"

#define AUTO_SHUTDOWN_JP					"Auto"
#define MANUAL_SHUTDOWN_JP					"Manual"

#define DIALOG_CONFIRM_JP					"Confirm"
#define DIALOG_CANCLE_JP					"Cancel"
#define DIALOG_OK_JP						"OK"
#define DIALOG_RESET_JP						"Reset"
#define DIALOG_RETRY_JP						"Retry"
#define DIALOG_DISABLE_JP					"Disable"
#define DIALOG_PRINT_MODEL_JP				"Print model?"
#define DIALOG_CANCEL_PRINT_JP				"Stop print?"
#define DIALOG_RETRY_JP						"Retry"
#define DIALOG_STOP_JP						"Stop"
#define DIALOG_REPRINT_FROM_BREAKPOINT_JP	                "Reprint from breakpoint?"
#define DIALOG_UNBIND_PRINTER_JP						    "Unbind the printer?"
#define DIALOG_ERROR_TIPS1_JP								"error:no file,please check it again."
#define DIALOG_ERROR_TIPS2_JP								"error:transaction failed.please check display baudrate \nwhether as the same as mainboard!"
#define DIALOG_ERROR_TIPS3_JP								"error:file name or path is too long!"
#define DIALOG_CLOSE_MACHINE_JP             "Closing machine......"
#define DIALOG_UNBIND_PRINTER_JP            "Unbind the printer?"
//***************德文****************************//
#define TOOL_TEXT_GN		        "Vorbereiten"
#define PREHEAT_TEXT_GN             "vorheizen"
#define	MOVE_TEXT_GN		        "Mobile"
#define HOME_TEXT_GN                "Null"
#define PRINT_TEXT_GN		        "drucken"
#define EXTRUDE_TEXT_GN             "Extrusion"
#define LEVELING_TEXT_GN            "Leveling"
#define AUTO_LEVELING_TEXT_GN       "Leveling"
#define SET_TEXT_GN			        "einrichten"
#define MORE_TEXT_GN                "mehr"

#define ADD_TEXT_GN			        "Zusatz"
#define DEC_TEXT_GN			        "Reduzierung"
#define EXTRUDER_1_TEXT_GN		    "Extrusion1"
#define EXTRUDER_2_TEXT_GN		    "Extrusion2"
#define HEATBED_TEXT_GN			    "Hot Bett"
#define TEXT_1C_GN					"1℃"
#define TEXT_5C_GN					"5℃"
#define TEXT_10C_GN					"10℃"
#define CLOSE_TEXT_GN				"schlie?en"

#define BACK_TEXT_GN					"Rückkehr"

#define TOOL_PREHEAT_GN            "预热"
#define TOOL_EXTRUDE_GN             "挤出"
#define TOOL_MOVE_GN                "移动"
#define TOOL_HOME_GN                "回零"
#define TOOL_LEVELING_GN               "调平"
#define TOOL_AUTO_LEVELING_GN         "自动调平"
#define TOOL_FILAMENT_GN                "换料"
#define TOOL_MORE_GN                    "更多"

#define AXIS_X_ADD_TEXT_GN				"X+"
#define AXIS_X_DEC_TEXT_GN				"X-"
#define AXIS_Y_ADD_TEXT_GN				"Y+"
#define AXIS_Y_DEC_TEXT_GN				"Y-"
#define AXIS_Z_ADD_TEXT_GN				"Z+"
#define AXIS_Z_DEC_TEXT_GN				"Z-"
#define TEXT_01MM_GN					"0.1mm"
#define TEXT_1MM_GN						"1mm"
#define TEXT_10MM_GN					"10mm"

#define HOME_X_TEXT_GN					"X"
#define HOME_Y_TEXT_GN					"Y"
#define HOME_Z_TEXT_GN					"Z"
#define HOME_ALL_TEXT_GN				"ALL"
#define HOME_STOPMOVE_GN                "Quickstop"

#define PAGE_UP_TEXT_GN					"früher"
#define PAGE_DOWN_TEXT_GN				"n?chste\nSeite"

#define EXTRUDER_ADD_TEXT_GN		    "Futter"
#define EXTRUDER_DEC_TEXT_GN		    "Stripper"
#define EXTRUDE_1MM_TEXT_GN			    "1mm"
#define EXTRUDE_5MM_TEXT_GN			    "5mm"
#define EXTRUDE_10MM_TEXT_GN		    "10mm"
#define EXTRUDE_LOW_SPEED_TEXT_GN		"LOW"
#define EXTRUDE_MEDIUM_SPEED_TEXT_GN	"Mittlere"
#define EXTRUDE_HIGH_SPEED_TEXT_GN		"High"

#define LEVELING_POINT1_TEXT_GN				"Position1"
#define LEVELING_POINT2_TEXT_GN				"Position2"
#define LEVELING_POINT3_TEXT_GN				"Position3"
#define LEVELING_POINT4_TEXT_GN				"Position4"
#define LEVELING_POINT5_TEXT_GN				"Position5"

#define FILESYS_TEXT_GN						"FileSys"
#define WIFI_TEXT_GN						"WIFI"
#define FAN_TEXT_GN							"Fan"
#define ABOUT_TEXT_GN						"About"
#define BREAK_POINT_TEXT_GN					"Continu"
#define FILAMENT_TEXT_GN					"Filament"
#define LANGUAGE_TEXT_GN					"Language"
#define MOTO_OFF_TEXT_GN					"Moto-off"
#define SHUTDOWN_TEXT_GN					"Shutdown"

#define U_DISK_TEXT_GN						"USB"
#define SD_CARD_TEXT_GN						"SD"
#define WIFI_NAME_TEXT_GN					"WIFI: "
#define WIFI_KEY_TEXT_GN					"KEY: "
#define WIFI_IP_TEXT_GN						"IP: "
#define WIFI_AP_TEXT_GN						"STATE: AP"
#define WIFI_STA_TEXT_GN					"STATE: STA"
#define WIFI_CONNECTED_TEXT_GN				"CONNECTED"
#define WIFI_DISCONNECTED_TEXT_GN			"DISCONNECTED"
#define WIFI_EXCEPTION_TEXT_GN				"EXCEPTION"
#define CLOUD_TEXT_GN						"Cloud"
#define CLOUD_BIND_GN						"Bind"
#define CLOUD_UNBIND_GN						"Unbind"
#define CLOUD_UNBINDING_GN					"Unbinding"
#define CLOUD_DISCONNECTED_GN				"Disconnected"
#define CLOUD_UNBINDED_GN					"Unbinded"
#define CLOUD_BINDED_GN						"Binded"
#define CLOUD_DISABLE_GN					"Disable"

#define FAN_ADD_TEXT_GN						"ADD"
#define FAN_DEC_TEXT_GN						"DEC"
#define FAN_OPEN_TEXT_GN					"100%"
#define FAN_HALF_TEXT_GN					"50%"
#define FAN_CLOSE_TEXT_GN					"Close"
#define FAN_TIPS1_TEXT_GN					"FAN\n%d"
#define FAN_TIPS2_TEXT_GN					"FAN\nClose"

#define FILAMENT_IN_TEXT_GN					"In"
#define FILAMENT_OUT_TEXT_GN				"Out"
#define FILAMENT_EXT0_TEXT_GN				"Extrusion1"
#define FILAMENT_EXT1_TEXT_GN				"Extrusion2"
#define FILAMENT_HEAT_TEXT_GN				"Preheat"					
#define FILAMENT_STOP_TEXT_GN				"Stop"
#define FILAMENT_CHANGE_TEXT_GN				"Filament replace"
#define FILAMENT_TIPS2_TEXT_GN				"T:%d℃/%d℃"
#define FILAMENT_TIPS3_TEXT_GN				"loading..."
#define FILAMENT_TIPS4_TEXT_GN				"unloading..."
#define FILAMENT_TIPS5_TEXT_GN				"Temp is too low to go,please heat"
#define FILAMENT_TIPS6_TEXT_GN				"Completed"
#define FILAMENT_CHANGE_TEXT_GN				"Please click <Load> \nor <unload>,After \npinter pause."
#define FILAMENT_DIALOG_LOAD_HEAT_TIPS_GN		"Heating up the nozzle,\nplease wait..."
#define FILAMENT_DIALOG_UNLOAD_HEAT_TIPS_GN		"Heating up the nozzle,\nplease wait..."
#define FILAMENT_DIALOG_LOAD_CONFIRM1_TIPS_GN           "Heat completed,please load filament \nto extruder,and click <confirm> \nfor start loading."
#define FILAMENT_DIALOG_LOAD_CONFIRM2_TIPS_GN           "Please load filament to extruder,\nand click <confirm> for start loading."
#define FILAMENT_DIALOG_UNLOAD_CONFIRM_TIPS_GN          "Heat completed,please \nclick <confirm> for start unloading.!"
#define FILAMENT_DIALOG_LOADING_TIPS_GN                 "Is loading ,please wait!"
#define FILAMENT_DIALOG_UNLOADING_TIPS_GN               "Is unloading,please wait!"
#define FILAMENT_DIALOG_LOAD_COMPLETE_TIPS_GN           "Load filament completed,\nclick <confirm> for return!"
#define FILAMENT_DIALOG_UNLOAD_COMPLETE_TIPS_GN         "Unload filament completed,\nclick <confirm> for return!"

#define PRE_HEAT_EXT_TEXT_GN				"E%d\n%d℃/%d℃"
#define PRE_HEAT_BED_TEXT_GN				"Bed%d\n%d℃/%d℃"

#define FILE_LOADING_GN					"Loading......"
#define NO_FILE_AND_CHECK_GN					"No files found!\n   Check the file system configuration!"
#define NO_FILE_GN					"No files found!"

#define EXTRUDER_TEMP_TEXT_GN				"T:%d℃"
#define EXTRUDER_E_LENGTH1_TEXT_GN          "E%d:%dmm"
#define EXTRUDER_E_LENGTH2_TEXT_GN          "E%d:%dcm"
#define EXTRUDER_E_LENGTH3_TEXT_GN          "E%d:%dm"

#define ABOUT_TYPE_TEXT_GN					"Type: "
#define ABOUT_VERSION_TEXT_GN				"Firmware: "
#define ABOUT_WIFI_TEXT_GN					"Wifi: "

#define PRINTING_OPERATION_GN				"Operation"
#define PRINTING_PAUSE_GN					"Pause"
#define PRINTING_TEMP_GN					"Temp."
#define PRINTING_CHANGESPEED_GN				"Speed"
#define PRINTING_RESUME_GN					"Resume"
#define PRINTING_STOP_GN					"Stop"
#define PRINTING_MORE_GN					"More"
#define PRINTING_EXTRUDER_GN				"Extruder"
#define PRINTING_MOVE_GN                    "Move"

#define EXTRUDER_SPEED_GN					"Extrude"
#define MOVE_SPEED_GN						"Move"
#define EXTRUDER_SPEED_STATE_GN				"Extrude Speed"
#define MOVE_SPEED_STATE_GN					"Move Speed"
#define STEP_1PERCENT_GN					"1%"
#define STEP_5PERCENT_GN					"5%"
#define STEP_10PERCENT_GN					"10%"

#define ZOFFSET_GN                          "Z Offset"
#define ZOFFSET_INC_GN                          "Zusatz"
#define ZOFFSET_DEC_GN                          "Reduzierung"

#define TITLE_READYPRINT_GN						"ReadyPrint"
#define TITLE_PREHEAT_GN						"Preheat"
#define TITLE_MOVE_GN					        "Move"
#define TITLE_HOME_GN							"Home"
#define TITLE_EXTRUDE_GN						"Extrude"
#define TITLE_LEVELING_GN						"Leveling"
#define TITLE_SET_GN							"Set"
#define TITLE_MORE_GN							"More"
#define TITLE_CHOOSEFILE_GN						"ChooseFile"
#define TITLE_PRINTING_GN						"Printing"
#define TITLE_OPERATION_GN						"Operation"
#define TITLE_ADJUST_GN							"Adjust"
#define	TITLE_WIRELESS_GN						"Wireless"
#define	TITLE_FILAMENT_GN						"Filament"
#define TITLE_ABOUT_GN							"About"
#define TITLE_FAN_GN							"Fan"
#define TITLE_LANGUAGE_GN						"Language"
#define TITLE_PAUSE_GN							"Pause"
#define TITLE_CHANGESPEED_GN				    "Speed"
#define TITLE_CLOUD_TEXT_GN				        "Cloud"
#define TITLE_DIALOG_CONFIRM_GN				"Confirm"
#define TITLE_FILESYS_GN								"FileSys"

#define AUTO_SHUTDOWN_GN					"Auto"
#define MANUAL_SHUTDOWN_GN					"Manual"

#define DIALOG_CONFIRM_GN					"Confirm"
#define DIALOG_CANCLE_GN					"Cancel"
#define DIALOG_OK_GN						"OK"
#define DIALOG_RESET_GN						"Reset"
#define DIALOG_RETRY_GN						"Retry"
#define DIALOG_DISABLE_GN					"Disable"
#define DIALOG_PRINT_MODEL_GN				"Print model?"
#define DIALOG_CANCEL_PRINT_GN				"Stop print?"
#define DIALOG_RETRY_GN									"Retry"
#define DIALOG_STOP_GN									"Stop"
#define DIALOG_REPRINT_FROM_BREAKPOINT_GN	            "Reprint from breakpoint?"
#define DIALOG_UNBIND_PRINTER_GN						"Unbind the printer?"
#define DIALOG_ERROR_TIPS1_GN								"error:no file,please check it again."
#define DIALOG_ERROR_TIPS2_GN								"error:transaction failed.please check display baudrate \nwhether as the same as mainboard!"
#define DIALOG_ERROR_TIPS3_GN								"error:file name or path is too long!"
#define DIALOG_CLOSE_MACHINE_GN             "Closing machine......"
#define DIALOG_UNBIND_PRINTER_GN            "Unbind the printer?"

#endif
/*****************************************/

#endif
