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


#endif
