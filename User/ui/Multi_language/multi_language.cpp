#include "draw_ui.h"
#include "Multi_language.h"
#include "lang_common.h"
#include "wifi_list.h"

LANG_DEF lang_defs[LANG_COUNT] = {{"EN", LANG_ENGLISH}, {"RU", LANG_RUSSIAN}};

//********************************************//
//������ʵ�ַ��������⣬�������������ֱ�����ANSI��׼��ʽ��ʾ
//"���ڴ�ӡ""����""��ͣ"��������ı�����ֻ��"��Ӣ�л�"��
#define PRINTING_GBK							"���ڴ�ӡ"
#define PRINTING_OPERATION_GBK				"����"
#define PRINTING_PAUSE_GBK						"��ͣ"

#define PRINTING_OTHER_LANGUGE				"Printing"
#define PRINTING_OPERATION_OTHER_LANGUGE		"Operation"
#define PRINTING_PAUSE_OTHER_LANGUGE			"Pause"

#define PRINTING_SP				"Imprimiendo"
#define PRINTING_AJUSTES_SP		"Ajustes"
#define PRINTING_PAUSAR_SP			"Pausar"
//*********************************************//




common_menu_def common_menu;
main_menu_def main_menu;
preheat_menu_def preheat_menu;
move_menu_def move_menu;
home_menu_def home_menu;
file_menu_def file_menu;
extrude_menu_def extrude_menu;
leveling_menu_def leveling_menu;
set_menu_def set_menu;
more_menu_def more_menu;
wifi_menu_def wifi_menu;
cloud_menu_def cloud_menu;
about_menu_def about_menu;
fan_menu_def fan_menu;
filament_menu_def filament_menu;
printing_menu_def printing_menu;
operation_menu_def operation_menu;
pause_menu_def pause_menu;
speed_menu_def speed_menu;
printing_more_menu_def printing_more_menu;
dialog_menu_def dialog_menu;
language_menu_def language_menu;
print_file_dialog_menu_def print_file_dialog_menu;
filesys_menu_def filesys_menu;
zoffset_menu_def zoffset_menu;
tool_menu_def tool_menu;
MachinePara_menu_def MachinePara_menu;
MachineSettings_menu_def MachineSettings_menu;
TemperatureSettings_menu_def TemperatureSettings_menu;

machine_common_def machine_menu;

void machine_setting_disp()
{
    //��������
    MachinePara_menu.title = MACHINE_PARA_TITLE_EN;
    MachinePara_menu.MachineSetting = MACHINE_TYPE_CNOFIG_EN;
    MachinePara_menu.TemperatureSetting=TEMPERATURE_CONFIG_EN;
    MachinePara_menu.MotorSetting=MOTOR_CONFIG_EN;
    MachinePara_menu.AdvanceSetting=ADVANCE_CONFIG_EN;

    machine_menu.default_value = DEFAULT_EN;
    machine_menu.next=NEXT_EN;
    machine_menu.previous=PREVIOUS_EN;

    machine_menu.MachineConfigTitle = MACHINE_CONFIG_TITLE_EN;
    machine_menu.MachineType=MACHINE_TYPE_EN;
    machine_menu.Stroke=MACHINE_STROKE_EN;
    machine_menu.HomeDir=MACHINE_HOMEDIR_EN;
    machine_menu.EndStopType=MACHINE_ENDSTOP_TYPE_EN;
    machine_menu.FilamentConf=MACHINE_FILAMENT_CONFIG_EN;
    machine_menu.LevelingConf=MACHINE_LEVELING_CONFIG_EN;

    machine_menu.MachineTypeConfTitle = MACHINE_TYPE_CONFIG_TITLE_EN;
    machine_menu.xyz=MACHINE_TYPE_XYZ_EN;
    machine_menu.delta=MACHINE_TYPE_DELTA_EN;
    machine_menu.corexy=MACHINE_TYPE_COREXY_EN;

    machine_menu.StrokeConfTitle=MACHINE_STROKE_CONF_TITLE_EN;
    machine_menu.xStroke=X_MAX_LENGTH_EN;
    machine_menu.yStroke=Y_MAX_LENGTH_EN;
    machine_menu.zStroke=Z_MAX_LENGTH_EN;
	
    machine_menu.xmin=X_MIN_LENGTH_EN;
    machine_menu.ymin=Y_MIN_LENGTH_EN;
    machine_menu.zmin=Z_MIN_LENGTH_EN;

    machine_menu.HomeDirConfTitle=HOME_DIR_CONF_TITLE_EN;
    machine_menu.xHomeDir=HOME_DIR_X_EN;
    machine_menu.yHomeDir=HOME_DIR_Y_EN;
    machine_menu.zHomeDir=HOME_DIR_Z_EN;
    machine_menu.min=HOME_MIN_EN;
    machine_menu.max=HOME_MAX_EN;

    machine_menu.EndstopConfTitle=ENDSTOP_CONF_TITLE_EN;
    machine_menu.xEndstop_min=MIN_ENDSTOP_X_EN;
    machine_menu.yEndstop_min=MIN_ENDSTOP_Y_EN;
    machine_menu.zEndstop_min=MIN_ENDSTOP_Z_EN;
    machine_menu.xEndstop_max=MAX_ENDSTOP_X_EN;
    machine_menu.yEndstop_max=MAX_ENDSTOP_Y_EN;
    machine_menu.zEndstop_max=MAX_ENDSTOP_Z_EN;            
    machine_menu.FilamentEndstop=ENDSTOP_FIL_EN;
    machine_menu.LevelingEndstop=ENDSTOP_LEVEL_EN;
    machine_menu.opened=ENDSTOP_OPENED_EN;
    machine_menu.closed=ENDSTOP_CLOSED_EN;

    machine_menu.FilamentConfTitle=FILAMENT_CONF_TITLE_EN;
    machine_menu.InTemperature=FILAMENT_IN_TEMPERATURE_EN;
    machine_menu.InLength=FILAMENT_IN_LENGTH_EN;
    machine_menu.InSpeed=FILAMENT_IN_SPEED_EN;
    machine_menu.OutTemperature=FILAMENT_OUT_TEMPERATURE_EN;
    machine_menu.OutLength=FILAMENT_OUT_LENGTH_EN;
    machine_menu.OutSpeed=FILAMENT_OUT_SPEED_EN;

    machine_menu.LevelingParaConfTitle=LEVELING_CONF_TITLE_EN;
    machine_menu.LevelingParaConf=LEVELING_PARA_CONF_EN;
    machine_menu.DeltaLevelConf=LEVELING_DELTA_EN;
    machine_menu.XYZLevelconf=LEVELING_XYZ_EN;

    machine_menu.LevelingSubConfTitle=LEVELING_PARA_CONF_TITLE_EN;
    machine_menu.AutoLevelEnable=AUTO_LEVELING_ENABLE_EN;
    machine_menu.BLtouchEnable=BLTOUCH_LEVELING_ENABLE_EN;
    machine_menu.ProbePort=PROBE_PORT_EN;
    machine_menu.ProbeXoffset=PROBE_X_OFFSET_EN;
    machine_menu.ProbeYoffset=PROBE_Y_OFFSET_EN;
    machine_menu.ProbeZoffset=PROBE_Z_OFFSET_EN;
    machine_menu.ProbeXYspeed=PROBE_XY_SPEED_EN;
    machine_menu.ProbeZspeed=PROBE_Z_SPEED_EN;
    machine_menu.enable = ENABLE_EN;
    machine_menu.disable = DISABLE_EN;
    machine_menu.z_min = Z_MIN_EN;
    machine_menu.z_max = Z_MAX_EN;

    machine_menu.LevelingSubDeltaConfTitle=DELTA_LEVEL_CONF_TITLE_EN;
    machine_menu.MachineRadius=DELTA_MACHINE_RADIUS_EN;
    machine_menu.DiagonalRod=DELTA_DIAGONAL_ROD_EN;
    machine_menu.PrintableRadius=DELTA_PRINT_RADIUS_EN;
    machine_menu.DeltaHeight=DELTA_HEIGHT_EN;
    machine_menu.SmoothRodOffset=SMOOTH_ROD_OFFSET_EN;
    machine_menu.EffectorOffset=EFFECTOR_OFFSET_EN;
    machine_menu.CalibrationRadius=CALIBRATION_RADIUS_EN;

    machine_menu.LevelingSubXYZConfTitle=XYZ_LEVEL_CONF_TITLE_EN;
    //machine_menu.Level_positon=PROBE_REACH_MAX_LEFT_EN;
    //machine_menu.ProbeMaxRigh=PROBE_REACH_MAX_RIGHT_EN;
    //machine_menu.ProbeMaxfront=PROBE_REACH_MAX_FRONT_EN;
   // machine_menu.ProbeMaxback=PROBE_REACH_MAX_BACK_EN;

    machine_menu.TemperatureConfTitle=TEMPERATURE_CONF_TITLE_EN;
    machine_menu.NozzleConf=NOZZLE_CONF_EN;
    machine_menu.HotBedConf=HOTBED_CONF_EN;
    machine_menu.PreheatTemperConf=PREHEAT_TEMPER_EN;

    machine_menu.NozzleConfTitle=NOZZLE_CONF_TITLE_EN;
    machine_menu.NozzleCnt=NOZZLECNT_EN;
    machine_menu.NozzleType=NOZZLE_TYPE_EN;
    machine_menu.NozzleAdjustType=NOZZLE_ADJUST_TYPE_EN;
    machine_menu.NozzleMinTemperature=NOZZLE_MIN_TEMPERATURE_EN;
    machine_menu.NozzleMaxTemperature=NOZZLE_MAX_TEMPERATURE_EN;
    machine_menu.Extrude_Min_Temper=EXTRUD_MIN_TEMPER_EN;

    machine_menu.HotbedEnable=HOTBED_ENABLE_EN;
    machine_menu.HotbedConfTitle=HOTBED_CONF_TITLE_EN;
    machine_menu.HotbedAjustType=HOTBED_ADJUST_EN;
    machine_menu.HotbedMinTemperature=HOTBED_MIN_TEMPERATURE_EN;
    machine_menu.HotbedMaxTemperature=HOTBED_MAX_TEMPERATURE_EN;

    machine_menu.MaxFeedRateConfTitle=MAXFEEDRATE_CONF_TITLE_EN;
    machine_menu.XMaxFeedRate=X_MAXFEEDRATE_EN;
    machine_menu.YMaxFeedRate=Y_MAXFEEDRATE_EN;
    machine_menu.ZMaxFeedRate=Z_MAXFEEDRATE_EN;
    machine_menu.E0MaxFeedRate=E0_MAXFEEDRATE_EN;
    machine_menu.E1MaxFeedRate=E1_MAXFEEDRATE_EN;

    machine_menu.AccelerationConfTitle=ACCELERATION_CONF_TITLE_EN;
    machine_menu.PrintAcceleration=PRINT_ACCELERATION_EN;
    machine_menu.RetractAcceleration=RETRACT_ACCELERATION_EN;
    machine_menu.TravelAcceleration=TRAVEL_ACCELERATION_EN;
    machine_menu.X_Acceleration=X_ACCELERATION_EN;
    machine_menu.Y_Acceleration=Y_ACCELERATION_EN;
    machine_menu.Z_Acceleration=Z_ACCELERATION_EN;
    machine_menu.E0_Acceleration=E0_ACCELERATION_EN;
    machine_menu.E1_Acceleration=E1_ACCELERATION_EN;

    machine_menu.JerkConfTitle=JERK_CONF_TITLE_EN;
    machine_menu.X_Jerk=X_JERK_EN;
    machine_menu.Y_Jerk=Y_JERK_EN;
    machine_menu.Z_Jerk=Z_JERK_EN;
    machine_menu.E_Jerk=E_JERK_EN;

    machine_menu.StepsConfTitle=STEPS_CONF_TITLE_EN;
    machine_menu.X_Steps=X_STEPS_EN;
    machine_menu.Y_Steps=Y_STEPS_EN;
    machine_menu.Z_Steps=Z_STEPS_EN;
    machine_menu.E0_Steps=E0_STEPS_EN;
    machine_menu.E1_Steps=E1_STEPS_EN;

    machine_menu.X_MotorDir=X_MOTORDIR_EN;
    machine_menu.Y_MotorDir=Y_MOTORDIR_EN;
    machine_menu.Z_MotorDir=Z_MOTORDIR_EN;
    machine_menu.E0_MotorDir=E0_MOTORDIR_EN;
    machine_menu.E1_MotorDir=E1_MOTORDIR_EN;
    machine_menu.Invert_0=INVERT_P_EN;
    machine_menu.Invert_1=INVERT_N_EN;

    machine_menu.HomeFeedRateConfTitle=HOMEFEEDRATE_CONF_TITLE_EN;
    machine_menu.XY_HomeFeedRate=X_HOMESPEED_EN;
    //machine_menu.Y_HomeFeedRate=Y_HOMESPEED_EN;
    machine_menu.Z_HomeFeedRate=Z_HOMESPEED_EN;

    machine_menu.AdvancedConfTitle=ADVANCED_CONF_TITLE_EN;
    machine_menu.PwrOffDection=PWROFF_DECTION_EN;
    machine_menu.HaveUps=HAVE_UPS_EN;
    machine_menu.Z2andZ2Endstop=Z2_AND_Z2ENDSTOP_CONF_EN;
    machine_menu.EnablePinsInvert=ENABLE_PINS_CONF_EN;

    machine_menu.Z2ConfTitle=Z2_AND_Z2ENDSTOP_CONF_TITLE_EN;
    machine_menu.Z2Enable=Z2_ENABLE_EN;
    machine_menu.Z2EndstopEnable=Z2_ENDSTOP_EN;
    machine_menu.Z2Port=Z2_PORT_EN;

    machine_menu.EnablePinsInvertTitle=ENABLE_PINS_CONF_TITLE_EN;
    machine_menu.XInvert=X_ENABLE_PINS_INVERT_EN;
    machine_menu.YInvert=Y_ENABLE_PINS_INVERT_EN;
    machine_menu.ZInvert=Z_ENABLE_PINS_INVERT_EN;
    machine_menu.EInvert=E_ENABLE_PINS_INVERT_EN;

    machine_menu.key_back = KEY_BACK_EN;
    machine_menu.key_rest = KEY_REST_EN;
    machine_menu.key_confirm = KEY_CONFIRM_EN;
    //

    set_menu.machine_para = MACHINE_PARA_EN;

	machine_menu.high_level = MOTOR_EN_HIGH_LEVEL_EN;
	machine_menu.low_level = MOTOR_EN_LOW_LEVEL_EN;

}


void disp_language_init() {
	preheat_menu.value_state= TEXT_VALUE;
	preheat_menu.step_1c= TEXT_1C;
	preheat_menu.step_5c= TEXT_5C;
	preheat_menu.step_10c= TEXT_10C;

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

	home_menu.home_x= HOME_X_TEXT;
	home_menu.home_y= HOME_Y_TEXT;
	home_menu.home_z= HOME_Z_TEXT;
	home_menu.home_all= HOME_ALL_TEXT;

	extrude_menu.temp_value = TEXT_VALUE_T;
	extrude_menu.count_value_mm= TEXT_VALUE_mm;
	extrude_menu.count_value_cm= TEXT_VALUE_cm;
	extrude_menu.count_value_m= TEXT_VALUE_m;
	extrude_menu.step_1mm = EXTRUDE_1MM_TEXT;
	extrude_menu.step_5mm = EXTRUDE_5MM_TEXT;

	language_menu.chinese_s = LANGUAGE_S_CN;
	language_menu.chinese_t = LANGUAGE_T_CN;
	language_menu.english = LANGUAGE_EN;
	language_menu.russian = LANGUAGE_RU;
	language_menu.spanish = LANGUAGE_SP;
	language_menu.german = LANGUAGE_GE;
	language_menu.japan = LANGUAGE_JP;
	language_menu.korean = LANGUAGE_KR;
	language_menu.portuguese = LANGUAGE_PR;
	language_menu.italy = LANGUAGE_IT;
	language_menu.brazil = LANGUAGE_BR;
	language_menu.french = LANGUAGE_FR;
    
	about_menu.type_name= ABOUT_TYPE_TEXT;
	about_menu.firmware_v= ABOUT_VERSION_TEXT;
	//about_menu.wifi = ABOUT_WIFI_TEXT;	

	wifi_menu.ip = WIFI_IP_TEXT;
	wifi_menu.wifi = WIFI_NAME_TEXT;
	wifi_menu.key = WIFI_KEY_TEXT;
	wifi_menu.state_ap = WIFI_STATE_AP_TEXT;
	wifi_menu.state_sta = WIFI_STATE_STA_TEXT;
	wifi_menu.connected = WIFI_CONNECTED_TEXT;
	wifi_menu.disconnected = WIFI_DISCONNECTED_TEXT;
	wifi_menu.exception = WIFI_EXCEPTION_TEXT;

	printing_menu.temp1 = TEXT_VALUE;
	printing_menu.temp2 = TEXT_VALUE;
	printing_menu.bed_temp = TEXT_VALUE;

	filament_menu.stat_temp = TEXT_VALUE;

	zoffset_menu.step001 = ZOFFSET_STEP001;
	zoffset_menu.step01 = ZOFFSET_STEP01;
	zoffset_menu.step1 = ZOFFSET_STEP1;

    machine_menu.key_0 = KEYBOARD_KEY0_EN;
    machine_menu.key_1 = KEYBOARD_KEY1_EN;
    machine_menu.key_2 = KEYBOARD_KEY2_EN;
    machine_menu.key_3 = KEYBOARD_KEY3_EN;
    machine_menu.key_4 = KEYBOARD_KEY4_EN;
    machine_menu.key_5 = KEYBOARD_KEY5_EN;
    machine_menu.key_6 = KEYBOARD_KEY6_EN;
    machine_menu.key_7 = KEYBOARD_KEY7_EN;
    machine_menu.key_8 = KEYBOARD_KEY8_EN;
    machine_menu.key_9 = KEYBOARD_KEY9_EN;
    machine_menu.key_point= KEYBOARD_KEY_POINT_EN;
	machine_menu.negative=KEYBOARD_KEY_NEGATIVE_EN;

	//wifi-list
	list_menu.title=TEXT_WIFI_MENU_TITLE_EN;
	list_menu.file_pages = FILE_PAGES_EN;
	//keyboard
	keyboard_menu.apply=MANUAL_IP_APPLY_EN;
	keyboard_menu.password=TEXT_WIFI_PASSWORD_EN;
	keyboard_menu.space = TEXT_WIFI_SAPCE_EN;
      keyboard_menu.letter = TEXT_WIFI_LETTER_EN;
      keyboard_menu.digital = TEXT_WIFI_DIGITAL_EN;
      keyboard_menu.symbol = TEXT_WIFI_SYMBOL_EN;
      //tips
      tips_menu.pointBold = TEXT_WIFI_POINT_BOLD_EN;
	   tips_menu.joining = TEXT_WIFI_JOINING_EN;
	   tips_menu.failedJoin = TEXT_WIFI_FAILED_JOIN_EN;
	   tips_menu.wifiConected = TEXT_WIFI_WIFI_CONECTED_EN;
      

    machine_setting_disp();

	operation_menu.babystep = TEXT_BABY_STEP_EN;
    

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
			main_menu.title=TITLE_READYPRINT_RU;
			main_menu.preheat=PREHEAT_TEXT_RU;
			main_menu.move=MOVE_TEXT_RU;
			main_menu.home=HOME_TEXT_RU;
			main_menu.print=PRINT_TEXT_RU;
			main_menu.extrude=EXTRUDE_TEXT_RU;
			main_menu.leveling=LEVELING_TEXT_RU;
			main_menu.autoleveling=AUTO_LEVELING_TEXT_RU;
			main_menu.fan = FAN_TEXT_RU;
			main_menu.set=SET_TEXT_RU;
			main_menu.more=MORE_TEXT_RU;
			main_menu.tool = TOOL_TEXT_RU;
			//TOOL
			tool_menu.title = TOOL_TEXT_RU;
			tool_menu.preheat = TOOL_PREHEAT_RU;
			tool_menu.extrude = TOOL_EXTRUDE_RU;
			tool_menu.move = TOOL_MOVE_RU;
			tool_menu.home= TOOL_HOME_RU;
			tool_menu.leveling = TOOL_LEVELING_RU;
			tool_menu.autoleveling = TOOL_AUTO_LEVELING_RU;
			tool_menu.filament = TOOL_FILAMENT_RU;
			tool_menu.more = TOOL_MORE_RU;

			preheat_menu.adjust_title = TITLE_ADJUST_RU;
			preheat_menu.title=TITLE_PREHEAT_RU;
			preheat_menu.add=ADD_TEXT_RU;
			preheat_menu.dec=DEC_TEXT_RU;
			preheat_menu.ext1=EXTRUDER_1_TEXT_RU;
			preheat_menu.ext2=EXTRUDER_2_TEXT_RU;
			preheat_menu.hotbed=HEATBED_TEXT_RU;
			preheat_menu.off=CLOSE_TEXT_RU;

			move_menu.title = MOVE_TEXT_RU;
			home_menu.title=TITLE_HOME_RU;
			home_menu.stopmove = HOME_STOPMOVE_RU;

			file_menu.title=TITLE_CHOOSEFILE_RU;
			file_menu.page_up=PAGE_UP_TEXT_RU;
			file_menu.page_down=PAGE_DOWN_TEXT_RU;
			file_menu.file_loading = FILE_LOADING_RU;
			file_menu.no_file = NO_FILE_RU;
			file_menu.no_file_and_check = NO_FILE_RU;//NO_FILE_AND_CHECK_RU;

			extrude_menu.title=TITLE_EXTRUDE_RU;
			extrude_menu.in=EXTRUDER_IN_TEXT_RU;
			extrude_menu.out=EXTRUDER_OUT_TEXT_RU;
			extrude_menu.ext1=EXTRUDER_1_TEXT_RU;
			extrude_menu.ext2=EXTRUDER_2_TEXT_RU;
			extrude_menu.low=EXTRUDE_LOW_SPEED_TEXT_RU;
			extrude_menu.normal=EXTRUDE_MEDIUM_SPEED_TEXT_RU;
			extrude_menu.high=EXTRUDE_HIGH_SPEED_TEXT_RU;
			extrude_menu.temper_text=EXTRUDER_TEMP_TEXT_RU;

			leveling_menu.title=TITLE_LEVELING_RU;
			leveling_menu.position1=LEVELING_POINT1_TEXT_RU;
			leveling_menu.position2=LEVELING_POINT2_TEXT_RU;
			leveling_menu.position3=LEVELING_POINT3_TEXT_RU;
			leveling_menu.position4=LEVELING_POINT4_TEXT_RU;
			leveling_menu.position5=LEVELING_POINT5_TEXT_RU;

			set_menu.title=TITLE_SET_RU;
			set_menu.filesys=FILESYS_TEXT_RU;
			set_menu.wifi=WIFI_TEXT_RU;
			set_menu.about=ABOUT_TEXT_RU;
			set_menu.fan=FAN_TEXT_RU;
			set_menu.filament=FILAMENT_TEXT_RU;
			set_menu.breakpoint=BREAK_POINT_TEXT_RU;
			set_menu.motoroff=MOTOR_OFF_TEXT_RU;
			set_menu.language=LANGUAGE_TEXT_RU;

			more_menu.title = TITLE_MORE_RU;
			more_menu.zoffset = ZOFFSET_RU;

			filesys_menu.title = TITLE_FILESYS_RU;
			filesys_menu.sd_sys = SD_CARD_TEXT_RU;
			filesys_menu.usb_sys = U_DISK_TEXT_RU;

			wifi_menu.title=WIFI_TEXT;
			wifi_menu.cloud= CLOUD_TEXT_RU;
			wifi_menu.reconnect = WIFI_RECONNECT_TEXT_RU;

			cloud_menu.title = TITLE_CLOUD_TEXT_RU;
			cloud_menu.bind = CLOUD_BINDED_RU;
			cloud_menu.binded = CLOUD_BINDED_RU;
			cloud_menu.unbind = CLOUD_UNBIND_RU;
			cloud_menu.unbinding = CLOUD_UNBINDED_RU;
			cloud_menu.disconnected = CLOUD_DISCONNECTED_RU;
			cloud_menu.disable = CLOUD_DISABLE_RU;

			about_menu.title = ABOUT_TEXT_RU;
			about_menu.type = ABOUT_TYPE_TEXT_RU;
			about_menu.version = ABOUT_VERSION_TEXT_RU;
			about_menu.wifi = ABOUT_WIFI_TEXT_RU;

			fan_menu.title = FAN_TEXT_RU;
			fan_menu.add = FAN_ADD_TEXT_RU;
			fan_menu.dec = FAN_DEC_TEXT_RU;
			fan_menu.state = FAN_TIPS1_TEXT_RU;

			filament_menu.title = TITLE_FILAMENT_RU;
			filament_menu.in = FILAMENT_IN_TEXT_RU;
			filament_menu.out = FILAMENT_OUT_TEXT_RU;
			filament_menu.ext1 = FILAMENT_EXT0_TEXT_RU;
			filament_menu.ext2 = FILAMENT_EXT1_TEXT_RU;
			filament_menu.ready_replace = FILAMENT_CHANGE_TEXT_RU;
			filament_menu.filament_dialog_load_heat = FILAMENT_DIALOG_LOAD_HEAT_TIPS_RU;
			filament_menu.filament_dialog_load_heat_confirm = FILAMENT_DIALOG_LOAD_CONFIRM1_TIPS_RU;
			filament_menu.filament_dialog_loading = FILAMENT_DIALOG_LOADING_TIPS_RU;
			filament_menu.filament_dialog_load_completed = FILAMENT_DIALOG_LOAD_COMPLETE_TIPS_RU;
			filament_menu.filament_dialog_unload_heat = FILAMENT_DIALOG_UNLOAD_HEAT_TIPS_RU;
			filament_menu.filament_dialog_unload_heat_confirm = FILAMENT_DIALOG_UNLOAD_CONFIRM_TIPS_RU;
			filament_menu.filament_dialog_unloading = FILAMENT_DIALOG_UNLOADING_TIPS_RU;
			filament_menu.filament_dialog_unload_completed = FILAMENT_DIALOG_UNLOAD_COMPLETE_TIPS_RU;

			language_menu.title = LANGUAGE_TEXT_RU;
			language_menu.next = PAGE_DOWN_TEXT_RU;
			language_menu.up = PAGE_UP_TEXT_RU;

			printing_menu.title = TITLE_PRINTING_RU;
			printing_menu.option = PRINTING_OPERATION_RU;
			printing_menu.stop = PRINTING_STOP_RU;
			printing_menu.pause = PRINTING_PAUSE_RU;
			printing_menu.resume = PRINTING_RESUME_RU;

			operation_menu.title = TITLE_OPERATION_RU;
			operation_menu.pause = PRINTING_PAUSE_RU;
			operation_menu.stop = PRINTING_STOP_RU;
			operation_menu.temp = PRINTING_TEMP_RU;
			operation_menu.fan = FAN_TEXT_RU;
			operation_menu.extr = PRINTING_EXTRUDER_RU;
			operation_menu.speed = PRINTING_CHANGESPEED_RU;
			operation_menu.filament= FILAMENT_TEXT_RU;
			operation_menu.more = PRINTING_MORE_RU;
			operation_menu.move = PRINTING_MOVE_RU;
			operation_menu.auto_off = AUTO_SHUTDOWN_RU;
			operation_menu.manual_off = MANUAL_SHUTDOWN_RU;

			pause_menu.title= TITLE_PAUSE_RU;
			pause_menu.resume = PRINTING_RESUME_RU;
			pause_menu.stop = PRINTING_STOP_RU;
			pause_menu.extrude = PRINTING_EXTRUDER_RU;
			pause_menu.move = PRINTING_MOVE_RU;
			pause_menu.filament= FILAMENT_TEXT_RU;
			pause_menu.more = PRINTING_MORE_RU;

			speed_menu.title = PRINTING_CHANGESPEED_RU;
			speed_menu.add = ADD_TEXT_RU;
			speed_menu.dec = DEC_TEXT_RU;
			speed_menu.move = MOVE_SPEED_RU;
			speed_menu.extrude = EXTRUDER_SPEED_RU;
			speed_menu.extrude_speed = EXTRUDER_SPEED_STATE_RU;
			speed_menu.move_speed = MOVE_SPEED_STATE_RU;

			printing_more_menu.title = TITLE_MORE_RU;
			printing_more_menu.fan = FAN_TEXT_RU;
			printing_more_menu.auto_close = AUTO_SHUTDOWN_RU;
			printing_more_menu.manual = MANUAL_SHUTDOWN_RU;
			printing_more_menu.speed = PRINTING_CHANGESPEED_RU;
			printing_more_menu.temp = PRINTING_TEMP_RU;

			print_file_dialog_menu.confirm = DIALOG_CONFIRM_RU;
			print_file_dialog_menu.cancle = DIALOG_CANCLE_RU;
			print_file_dialog_menu.print_file = DIALOG_PRINT_MODEL_RU;
			print_file_dialog_menu.cancle_print = DIALOG_CANCEL_PRINT_RU;
			print_file_dialog_menu.retry = DIALOG_RETRY_RU;
			print_file_dialog_menu.stop = DIALOG_STOP_RU;
			print_file_dialog_menu.no_file_print_tips = DIALOG_ERROR_TIPS1_RU;
			print_file_dialog_menu.print_from_breakpoint = DIALOG_REPRINT_FROM_BREAKPOINT_RU;
			print_file_dialog_menu.close_machine_error = DIALOG_ERROR_TIPS2_RU;
			print_file_dialog_menu.filament_no_press=DIALOG_FILAMENT_NO_PRESS_RU;
			print_file_dialog_menu.print_finish=DIALOG_PRINT_FINISH_RU;
			print_file_dialog_menu.print_time=DIALOG_PRINT_TIME_RU;
			print_file_dialog_menu.reprint=DIALOG_REPRINT_RU;
			print_file_dialog_menu.wifi_enable_tips=DIALOG_WIFI_ENABLE_TIPS_RU;

			zoffset_menu.title = TITLE_ZOFFSET_RU;
			zoffset_menu.inc = ZOFFSET_INC_RU;
			zoffset_menu.dec = ZOFFSET_DEC_RU;
		break;

		case LANG_ENGLISH:
		default:
			MachinePara_menu.title = MACHINE_PARA_TITLE_EN;
			MachinePara_menu.MachineSetting = MACHINE_TYPE_CNOFIG_EN;
			MachinePara_menu.TemperatureSetting=TEMPERATURE_CONFIG_EN;
			MachinePara_menu.MotorSetting=MOTOR_CONFIG_EN;
			MachinePara_menu.AdvanceSetting=ADVANCE_CONFIG_EN;
            
			machine_menu.default_value = DEFAULT_EN;
			machine_menu.next=NEXT_EN;
			machine_menu.previous=PREVIOUS_EN;
            
			machine_menu.MachineConfigTitle = MACHINE_CONFIG_TITLE_EN;
			machine_menu.MachineType=MACHINE_TYPE_EN;
			machine_menu.Stroke=MACHINE_STROKE_EN;
			machine_menu.HomeDir=MACHINE_HOMEDIR_EN;
			machine_menu.EndStopType=MACHINE_ENDSTOP_TYPE_EN;
			machine_menu.FilamentConf=MACHINE_FILAMENT_CONFIG_EN;
			machine_menu.LevelingConf=MACHINE_LEVELING_CONFIG_EN;

			machine_menu.MachineTypeConfTitle = MACHINE_TYPE_CONFIG_TITLE_EN;
			machine_menu.xyz=MACHINE_TYPE_XYZ_EN;
			machine_menu.delta=MACHINE_TYPE_DELTA_EN;
			machine_menu.corexy=MACHINE_TYPE_COREXY_EN;

			machine_menu.StrokeConfTitle=MACHINE_STROKE_CONF_TITLE_EN;
			machine_menu.xStroke=X_MAX_LENGTH_EN;
			machine_menu.yStroke=Y_MAX_LENGTH_EN;
			machine_menu.zStroke=Z_MAX_LENGTH_EN;

			machine_menu.xmin=X_MIN_LENGTH_EN;
			machine_menu.ymin=Y_MIN_LENGTH_EN;
			machine_menu.zmin=Z_MIN_LENGTH_EN;
            
			machine_menu.HomeDirConfTitle=HOME_DIR_CONF_TITLE_EN;
			machine_menu.xHomeDir=HOME_DIR_X_EN;
			machine_menu.yHomeDir=HOME_DIR_Y_EN;
			machine_menu.zHomeDir=HOME_DIR_Z_EN;
			machine_menu.min=HOME_MIN_EN;
			machine_menu.max=HOME_MAX_EN;

			machine_menu.EndstopConfTitle=ENDSTOP_CONF_TITLE_EN;
			machine_menu.xEndstop_min=MIN_ENDSTOP_X_EN;
			machine_menu.yEndstop_min=MIN_ENDSTOP_Y_EN;
			machine_menu.zEndstop_min=MIN_ENDSTOP_Z_EN;
			machine_menu.xEndstop_max=MAX_ENDSTOP_X_EN;
			machine_menu.yEndstop_max=MAX_ENDSTOP_Y_EN;
			machine_menu.zEndstop_max=MAX_ENDSTOP_Z_EN;
			machine_menu.FilamentEndstop=ENDSTOP_FIL_EN;
			machine_menu.LevelingEndstop=ENDSTOP_LEVEL_EN;
			machine_menu.opened=ENDSTOP_OPENED_EN;
			machine_menu.closed=ENDSTOP_CLOSED_EN;

			machine_menu.FilamentConfTitle=FILAMENT_CONF_TITLE_EN;
			machine_menu.InTemperature=FILAMENT_IN_TEMPERATURE_EN;
			machine_menu.InLength=FILAMENT_IN_LENGTH_EN;
			machine_menu.InSpeed=FILAMENT_IN_SPEED_EN;
			machine_menu.OutTemperature=FILAMENT_OUT_TEMPERATURE_EN;
			machine_menu.OutLength=FILAMENT_OUT_LENGTH_EN;
			machine_menu.OutSpeed=FILAMENT_OUT_SPEED_EN;

			machine_menu.LevelingParaConfTitle=LEVELING_CONF_TITLE_EN;
			machine_menu.LevelingParaConf=LEVELING_PARA_CONF_EN;
			machine_menu.DeltaLevelConf=LEVELING_DELTA_EN;
			machine_menu.XYZLevelconf=LEVELING_XYZ_EN;

			machine_menu.LevelingSubConfTitle=LEVELING_PARA_CONF_TITLE_EN;
			machine_menu.AutoLevelEnable=AUTO_LEVELING_ENABLE_EN;
			machine_menu.BLtouchEnable=BLTOUCH_LEVELING_ENABLE_EN;
			machine_menu.ProbePort=PROBE_PORT_EN;
			machine_menu.ProbeXoffset=PROBE_X_OFFSET_EN;
			machine_menu.ProbeYoffset=PROBE_Y_OFFSET_EN;
			machine_menu.ProbeZoffset=PROBE_Z_OFFSET_EN;
			machine_menu.ProbeXYspeed=PROBE_XY_SPEED_EN;
			machine_menu.ProbeZspeed=PROBE_Z_SPEED_EN;
			machine_menu.enable = ENABLE_EN;
			machine_menu.disable = DISABLE_EN;
			machine_menu.z_min = Z_MIN_EN;
			machine_menu.z_max = Z_MAX_EN;

			machine_menu.LevelingSubDeltaConfTitle=DELTA_LEVEL_CONF_TITLE_EN;
			machine_menu.MachineRadius=DELTA_MACHINE_RADIUS_EN;
			machine_menu.DiagonalRod=DELTA_DIAGONAL_ROD_EN;
			machine_menu.PrintableRadius=DELTA_PRINT_RADIUS_EN;
			machine_menu.DeltaHeight=DELTA_HEIGHT_EN;
			machine_menu.SmoothRodOffset=SMOOTH_ROD_OFFSET_EN;
			machine_menu.EffectorOffset=EFFECTOR_OFFSET_EN;
			machine_menu.CalibrationRadius=CALIBRATION_RADIUS_EN;

			machine_menu.LevelingSubXYZConfTitle=XYZ_LEVEL_CONF_TITLE_EN;
			//machine_menu.ProbeMaxLeft=PROBE_REACH_MAX_LEFT_EN;
			//machine_menu.ProbeMaxRigh=PROBE_REACH_MAX_RIGHT_EN;
			//machine_menu.ProbeMaxfront=PROBE_REACH_MAX_FRONT_EN;
			//machine_menu.ProbeMaxback=PROBE_REACH_MAX_BACK_EN;

			machine_menu.TemperatureConfTitle=TEMPERATURE_CONF_TITLE_EN;
			machine_menu.NozzleConf=NOZZLE_CONF_EN;
			machine_menu.HotBedConf=HOTBED_CONF_EN;
			machine_menu.PreheatTemperConf=PREHEAT_TEMPER_EN;

			machine_menu.NozzleConfTitle=NOZZLE_CONF_TITLE_EN;
			machine_menu.NozzleCnt=NOZZLECNT_EN;
			machine_menu.NozzleType=NOZZLE_TYPE_EN;
			machine_menu.NozzleAdjustType=NOZZLE_ADJUST_TYPE_EN;
			machine_menu.NozzleMinTemperature=NOZZLE_MIN_TEMPERATURE_EN;
			machine_menu.NozzleMaxTemperature=NOZZLE_MAX_TEMPERATURE_EN;
			machine_menu.Extrude_Min_Temper=EXTRUD_MIN_TEMPER_EN;

			machine_menu.HotbedEnable=HOTBED_ENABLE_EN;
			machine_menu.HotbedConfTitle=HOTBED_CONF_TITLE_EN;
			machine_menu.HotbedAjustType=HOTBED_ADJUST_EN;
			machine_menu.HotbedMinTemperature=HOTBED_MIN_TEMPERATURE_EN;
			machine_menu.HotbedMaxTemperature=HOTBED_MAX_TEMPERATURE_EN;

			machine_menu.MaxFeedRateConfTitle=MAXFEEDRATE_CONF_TITLE_EN;
			machine_menu.XMaxFeedRate=X_MAXFEEDRATE_EN;
			machine_menu.YMaxFeedRate=Y_MAXFEEDRATE_EN;
			machine_menu.ZMaxFeedRate=Z_MAXFEEDRATE_EN;
			machine_menu.E0MaxFeedRate=E0_MAXFEEDRATE_EN;
			machine_menu.E1MaxFeedRate=E1_MAXFEEDRATE_EN;

			machine_menu.AccelerationConfTitle=ACCELERATION_CONF_TITLE_EN;
			machine_menu.PrintAcceleration=PRINT_ACCELERATION_EN;
			machine_menu.RetractAcceleration=RETRACT_ACCELERATION_EN;
			machine_menu.TravelAcceleration=TRAVEL_ACCELERATION_EN;
			machine_menu.X_Acceleration=X_ACCELERATION_EN;
			machine_menu.Y_Acceleration=Y_ACCELERATION_EN;
			machine_menu.Z_Acceleration=Z_ACCELERATION_EN;
			machine_menu.E0_Acceleration=E0_ACCELERATION_EN;
			machine_menu.E1_Acceleration=E1_ACCELERATION_EN;

			machine_menu.JerkConfTitle=JERK_CONF_TITLE_EN;
			machine_menu.X_Jerk=X_JERK_EN;
			machine_menu.Y_Jerk=Y_JERK_EN;
			machine_menu.Z_Jerk=Z_JERK_EN;
			machine_menu.E_Jerk=E_JERK_EN;

			machine_menu.StepsConfTitle=STEPS_CONF_TITLE_EN;
			machine_menu.X_Steps=X_STEPS_EN;
			machine_menu.Y_Steps=Y_STEPS_EN;
			machine_menu.Z_Steps=Z_STEPS_EN;
			machine_menu.E0_Steps=E0_STEPS_EN;
			machine_menu.E1_Steps=E1_STEPS_EN;

			machine_menu.MotorDirConfTitle=MOTORDIR_CONF_TITLE_EN;
			machine_menu.X_MotorDir=X_MOTORDIR_EN;
			machine_menu.Y_MotorDir=Y_MOTORDIR_EN;
			machine_menu.Z_MotorDir=Z_MOTORDIR_EN;
			machine_menu.E0_MotorDir=E0_MOTORDIR_EN;
			machine_menu.E1_MotorDir=E1_MOTORDIR_EN;
			machine_menu.Invert_0=INVERT_P_EN;
			machine_menu.Invert_1=INVERT_N_EN;

			machine_menu.HomeFeedRateConfTitle=HOMEFEEDRATE_CONF_TITLE_EN;
			machine_menu.XY_HomeFeedRate=X_HOMESPEED_EN;
			//machine_menu.Y_HomeFeedRate=Y_HOMESPEED_EN;
			machine_menu.Z_HomeFeedRate=Z_HOMESPEED_EN;

			machine_menu.AdvancedConfTitle=ADVANCED_CONF_TITLE_EN;
			machine_menu.PwrOffDection=PWROFF_DECTION_EN;
			machine_menu.HaveUps=HAVE_UPS_EN;
			machine_menu.Z2andZ2Endstop=Z2_AND_Z2ENDSTOP_CONF_EN;
			machine_menu.EnablePinsInvert=ENABLE_PINS_CONF_EN;

			machine_menu.Z2ConfTitle=Z2_AND_Z2ENDSTOP_CONF_TITLE_EN;
			machine_menu.Z2Enable=Z2_ENABLE_EN;
			machine_menu.Z2EndstopEnable=Z2_ENDSTOP_EN;
			machine_menu.Z2Port=Z2_PORT_EN;

			machine_menu.EnablePinsInvertTitle=ENABLE_PINS_CONF_TITLE_EN;
			machine_menu.XInvert=X_ENABLE_PINS_INVERT_EN;
			machine_menu.YInvert=Y_ENABLE_PINS_INVERT_EN;
			machine_menu.ZInvert=Z_ENABLE_PINS_INVERT_EN;
			machine_menu.EInvert=E_ENABLE_PINS_INVERT_EN;

			machine_menu.key_back = KEY_BACK_EN;
			machine_menu.key_rest = KEY_REST_EN;
			machine_menu.key_confirm = KEY_CONFIRM_EN;
			machine_menu.high_level = MOTOR_EN_HIGH_LEVEL_EN;
			machine_menu.low_level = MOTOR_EN_LOW_LEVEL_EN;
			//


			common_menu.dialog_confirm_title = TITLE_DIALOG_CONFIRM_EN;
			common_menu.text_back=BACK_TEXT_EN;
			common_menu.close_machine_tips = DIALOG_CLOSE_MACHINE_EN;
			common_menu.unbind_printer_tips = DIALOG_UNBIND_PRINTER_EN;
			common_menu.print_special_title = PRINTING_OTHER_LANGUGE;
			common_menu.pause_special_title = PRINTING_PAUSE_OTHER_LANGUGE;
			common_menu.operate_special_title = PRINTING_OPERATION_OTHER_LANGUGE;			
			//��ҳ��
			main_menu.title=TITLE_READYPRINT_EN;
			main_menu.preheat=PREHEAT_TEXT_EN;
			main_menu.move=MOVE_TEXT_EN;
			main_menu.home=HOME_TEXT_EN;
			main_menu.print=PRINT_TEXT_EN;
			main_menu.extrude=EXTRUDE_TEXT_EN;
			main_menu.leveling=LEVELING_TEXT_EN;
			main_menu.autoleveling=AUTO_LEVELING_TEXT_EN;
			main_menu.fan = FAN_TEXT_EN;
			main_menu.set=SET_TEXT_EN;
			main_menu.more=MORE_TEXT_EN;
			main_menu.tool = TOOL_TEXT_EN;
			//TOOL
			tool_menu.title = TOOL_TEXT_EN;
			tool_menu.preheat = TOOL_PREHEAT_EN;
			tool_menu.extrude = TOOL_EXTRUDE_EN;
			tool_menu.move = TOOL_MOVE_EN;
			tool_menu.home= TOOL_HOME_EN;
			tool_menu.leveling = TOOL_LEVELING_EN;
			tool_menu.autoleveling = TOOL_AUTO_LEVELING_EN;
			tool_menu.filament = TOOL_FILAMENT_EN;
			tool_menu.more = TOOL_MORE_EN;			
			//Ԥ��
			preheat_menu.adjust_title = TITLE_ADJUST_EN;
			preheat_menu.title=TITLE_PREHEAT_EN;
			preheat_menu.add=ADD_TEXT_EN;
			preheat_menu.dec=DEC_TEXT_EN;
			preheat_menu.ext1=EXTRUDER_1_TEXT_EN;
			preheat_menu.ext2=EXTRUDER_2_TEXT_EN;
			preheat_menu.hotbed=HEATBED_TEXT_EN;
			preheat_menu.off=CLOSE_TEXT_EN;
			//�ƶ�
			move_menu.title = TITLE_MOVE_EN;
			//����
			home_menu.title=TITLE_HOME_EN;
			home_menu.stopmove = HOME_STOPMOVE_EN;
			//�ļ�Ŀ¼
			file_menu.title=TITLE_CHOOSEFILE_EN;
			file_menu.page_up=PAGE_UP_TEXT_EN;
			file_menu.page_down=PAGE_DOWN_TEXT_EN;
			file_menu.file_loading = FILE_LOADING_EN;
			file_menu.no_file = NO_FILE_EN;
			file_menu.no_file_and_check = NO_FILE_EN;//NO_FILE_AND_CHECK_EN;			
			//����
			extrude_menu.title=TITLE_EXTRUDE_EN;
			extrude_menu.in=EXTRUDER_IN_TEXT_EN;
			extrude_menu.out=EXTRUDER_OUT_TEXT_EN;
			extrude_menu.ext1=EXTRUDER_1_TEXT_EN;
			extrude_menu.ext2=EXTRUDER_2_TEXT_EN;
			extrude_menu.low=EXTRUDE_LOW_SPEED_TEXT_EN;
			extrude_menu.normal=EXTRUDE_MEDIUM_SPEED_TEXT_EN;
			extrude_menu.high=EXTRUDE_HIGH_SPEED_TEXT_EN;
			extrude_menu.temper_text=EXTRUDER_TEMP_TEXT_EN;
			//��ƽ
			leveling_menu.title=TITLE_LEVELING_EN;
			leveling_menu.position1=LEVELING_POINT1_TEXT_EN;
			leveling_menu.position2=LEVELING_POINT2_TEXT_EN;
			leveling_menu.position3=LEVELING_POINT3_TEXT_EN;
			leveling_menu.position4=LEVELING_POINT4_TEXT_EN;
			leveling_menu.position5=LEVELING_POINT5_TEXT_EN;
			//����
			set_menu.title=TITLE_SET_EN;
			set_menu.filesys=FILESYS_TEXT_EN;
			set_menu.wifi=WIFI_TEXT_EN;
			set_menu.about=ABOUT_TEXT_EN;
			set_menu.fan=FAN_TEXT_EN;
			set_menu.filament=FILAMENT_TEXT_EN;
			set_menu.breakpoint=BREAK_POINT_TEXT_EN;
			set_menu.motoroff=MOTOR_OFF_TEXT_EN;
			set_menu.language=LANGUAGE_TEXT_EN;
			set_menu.machine_para = MACHINE_PARA_EN;
			//����
			more_menu.title = TITLE_MORE_EN;
			more_menu.zoffset = ZOFFSET_EN;
			//�ļ�ϵͳ
			filesys_menu.title = TITLE_FILESYS_EN;
			filesys_menu.sd_sys = SD_CARD_TEXT_EN;
			filesys_menu.usb_sys = U_DISK_TEXT_EN;
			//WIFI
			wifi_menu.title=WIFI_TEXT;
			//wifi_menu.key = WIFI_KEY_TEXT_EN;
			//wifi_menu.ip = WIFI_IP_TEXT_EN;
			//wifi_menu.state= WIFI_STA_TEXT_EN;
			wifi_menu.cloud= CLOUD_TEXT_EN;
			wifi_menu.reconnect = WIFI_RECONNECT_TEXT_EN;

			cloud_menu.title = TITLE_CLOUD_TEXT_EN;
			cloud_menu.bind = CLOUD_BINDED_EN;
			cloud_menu.binded = CLOUD_BINDED_EN;
			cloud_menu.unbind = CLOUD_UNBIND_EN;
			cloud_menu.unbinding = CLOUD_UNBINDED_EN;
			cloud_menu.disconnected = CLOUD_DISCONNECTED_EN;
			cloud_menu.disable = CLOUD_DISABLE_EN;
			//����
			about_menu.title = TITLE_ABOUT_EN;
			about_menu.type = ABOUT_TYPE_TEXT_EN;
			about_menu.version = ABOUT_VERSION_TEXT_EN;
			about_menu.wifi = ABOUT_WIFI_TEXT_EN;			
			//����
			fan_menu.title = TITLE_FAN_EN;
			fan_menu.add = FAN_ADD_TEXT_EN;
			fan_menu.dec = FAN_DEC_TEXT_EN;
			fan_menu.state = FAN_TIPS1_TEXT_EN;
			//����
			filament_menu.title = TITLE_FILAMENT_EN;
			filament_menu.in = FILAMENT_IN_TEXT_EN;
			filament_menu.out = FILAMENT_OUT_TEXT_EN;
			filament_menu.ext1 = FILAMENT_EXT0_TEXT_EN;
			filament_menu.ext2 = FILAMENT_EXT1_TEXT_EN;
			filament_menu.ready_replace = FILAMENT_CHANGE_TEXT_EN;
			filament_menu.filament_dialog_load_heat = FILAMENT_DIALOG_LOAD_HEAT_TIPS_EN;
			filament_menu.filament_dialog_load_heat_confirm = FILAMENT_DIALOG_LOAD_CONFIRM1_TIPS_EN;
			filament_menu.filament_dialog_loading = FILAMENT_DIALOG_LOADING_TIPS_EN;
			filament_menu.filament_dialog_load_completed = FILAMENT_DIALOG_LOAD_COMPLETE_TIPS_EN;
			filament_menu.filament_dialog_unload_heat = FILAMENT_DIALOG_UNLOAD_HEAT_TIPS_EN;
			filament_menu.filament_dialog_unload_heat_confirm = FILAMENT_DIALOG_UNLOAD_CONFIRM_TIPS_EN;
			filament_menu.filament_dialog_unloading = FILAMENT_DIALOG_UNLOADING_TIPS_EN;
			filament_menu.filament_dialog_unload_completed = FILAMENT_DIALOG_UNLOAD_COMPLETE_TIPS_EN;

			//����
			language_menu.title = TITLE_LANGUAGE_EN;
			language_menu.next = PAGE_DOWN_TEXT_EN;
			language_menu.up = PAGE_UP_TEXT_EN;			
			//���ڴ�ӡ����
			printing_menu.title = TITLE_PRINTING_EN;
			printing_menu.option = PRINTING_OPERATION_EN;
			printing_menu.stop = PRINTING_STOP_EN;
			printing_menu.pause = PRINTING_PAUSE_EN;
			printing_menu.resume = PRINTING_RESUME_EN;

			//��������
			operation_menu.title = TITLE_OPERATION_EN;
			operation_menu.pause = PRINTING_PAUSE_EN;
			operation_menu.stop = PRINTING_STOP_EN;
			operation_menu.temp = PRINTING_TEMP_EN;
			operation_menu.fan = FAN_TEXT_EN;
			operation_menu.extr = PRINTING_EXTRUDER_EN;
			operation_menu.speed = PRINTING_CHANGESPEED_EN;			
			operation_menu.filament= FILAMENT_TEXT_EN;
			operation_menu.more = PRINTING_MORE_EN;
			operation_menu.move = PRINTING_MOVE_EN;
			operation_menu.auto_off = AUTO_SHUTDOWN_EN;
			operation_menu.manual_off = MANUAL_SHUTDOWN_EN;			
			//��ͣ����
			pause_menu.title= TITLE_PAUSE_EN;
			pause_menu.resume = PRINTING_RESUME_EN;
			pause_menu.stop = PRINTING_STOP_EN;
			pause_menu.extrude = PRINTING_EXTRUDER_EN;
			pause_menu.move = PRINTING_MOVE_EN;
			pause_menu.filament = FILAMENT_TEXT_EN;
			pause_menu.more = PRINTING_MORE_EN;

			//���ٽ���
			speed_menu.title = TITLE_CHANGESPEED_EN;
			speed_menu.add = ADD_TEXT_EN;
			speed_menu.dec = DEC_TEXT_EN;
			speed_menu.move = MOVE_SPEED_EN;
			speed_menu.extrude = EXTRUDER_SPEED_EN;
			speed_menu.extrude_speed = EXTRUDER_SPEED_STATE_EN;
			speed_menu.move_speed = MOVE_SPEED_STATE_EN;
			//��ӡ��---�������
			printing_more_menu.title = TITLE_MORE_EN;
			printing_more_menu.fan = FAN_TEXT_EN;
			printing_more_menu.auto_close = AUTO_SHUTDOWN_EN;
			printing_more_menu.manual = MANUAL_SHUTDOWN_EN;
			printing_more_menu.speed = PRINTING_CHANGESPEED_EN;
			printing_more_menu.temp = PRINTING_TEMP_EN;

			//print_file_dialog_menu.title = TITLE_DIALOG_CONFIRM_EN;
			print_file_dialog_menu.confirm = DIALOG_CONFIRM_EN;
			print_file_dialog_menu.cancle = DIALOG_CANCLE_EN;
			print_file_dialog_menu.print_file = DIALOG_PRINT_MODEL_EN;
			print_file_dialog_menu.cancle_print = DIALOG_CANCEL_PRINT_EN;
			print_file_dialog_menu.retry = DIALOG_RETRY_EN;
			print_file_dialog_menu.stop = DIALOG_STOP_EN;
			print_file_dialog_menu.no_file_print_tips = DIALOG_ERROR_TIPS1_EN;	
			print_file_dialog_menu.print_from_breakpoint = DIALOG_REPRINT_FROM_BREAKPOINT_EN;
			print_file_dialog_menu.close_machine_error = DIALOG_ERROR_TIPS2_EN;
			print_file_dialog_menu.filament_no_press=DIALOG_FILAMENT_NO_PRESS_EN;
			print_file_dialog_menu.print_finish=DIALOG_PRINT_FINISH_EN;
			print_file_dialog_menu.print_time=DIALOG_PRINT_TIME_EN;
			print_file_dialog_menu.reprint=DIALOG_REPRINT_EN;
			print_file_dialog_menu.wifi_enable_tips=DIALOG_WIFI_ENABLE_TIPS_EN;
			//ZOFFSET
			zoffset_menu.title = TITLE_ZOFFSET_EN;
			zoffset_menu.inc = ZOFFSET_INC_EN;
			zoffset_menu.dec = ZOFFSET_DEC_EN;				

			MachinePara_menu.title = MACHINE_PARA_TITLE_EN;
			MachinePara_menu.MachineSetting = MACHINE_TYPE_CNOFIG_EN;
			MachinePara_menu.TemperatureSetting=TEMPERATURE_CONFIG_EN;
			MachinePara_menu.MotorSetting=MOTOR_CONFIG_EN;
			MachinePara_menu.AdvanceSetting=ADVANCE_CONFIG_EN;

			machine_menu.default_value = DEFAULT_EN;
			machine_menu.next=NEXT_EN;
			machine_menu.previous=PREVIOUS_EN;
			
			machine_menu.MachineConfigTitle = MACHINE_CONFIG_TITLE_EN;
			machine_menu.MachineType=MACHINE_TYPE_EN;
			machine_menu.Stroke=MACHINE_STROKE_EN;
			machine_menu.HomeDir=MACHINE_HOMEDIR_EN;
			machine_menu.EndStopType=MACHINE_ENDSTOP_TYPE_EN;
			machine_menu.FilamentConf=MACHINE_FILAMENT_CONFIG_EN;
			machine_menu.LevelingConf=MACHINE_LEVELING_CONFIG_EN;
			
			machine_menu.MachineTypeConfTitle = MACHINE_TYPE_CONFIG_TITLE_EN;
			machine_menu.xyz=MACHINE_TYPE_XYZ_EN;
			machine_menu.delta=MACHINE_TYPE_DELTA_EN;
			machine_menu.corexy=MACHINE_TYPE_COREXY_EN;
			
			machine_menu.StrokeConfTitle=MACHINE_STROKE_CONF_TITLE_EN;
			machine_menu.xStroke=X_MAX_LENGTH_EN;
			machine_menu.yStroke=Y_MAX_LENGTH_EN;
			machine_menu.zStroke=Z_MAX_LENGTH_EN;
			
			machine_menu.xmin=X_MIN_LENGTH_EN;
			machine_menu.ymin=Y_MIN_LENGTH_EN;
			machine_menu.zmin=Z_MIN_LENGTH_EN;

			machine_menu.HomeDirConfTitle=HOME_DIR_CONF_TITLE_EN;
			machine_menu.xHomeDir=HOME_DIR_X_EN;
			machine_menu.yHomeDir=HOME_DIR_Y_EN;
			machine_menu.zHomeDir=HOME_DIR_Z_EN;
			machine_menu.min=HOME_MIN_EN;
			machine_menu.max=HOME_MAX_EN;

			machine_menu.EndstopConfTitle=ENDSTOP_CONF_TITLE_EN;
			machine_menu.xEndstop_min=MIN_ENDSTOP_X_EN;
			machine_menu.yEndstop_min=MIN_ENDSTOP_Y_EN;
			machine_menu.zEndstop_min=MIN_ENDSTOP_Z_EN;
			machine_menu.xEndstop_max=MAX_ENDSTOP_X_EN;
			machine_menu.yEndstop_max=MAX_ENDSTOP_Y_EN;
			machine_menu.zEndstop_max=MAX_ENDSTOP_Z_EN;
			machine_menu.FilamentEndstop=ENDSTOP_FIL_EN;
			machine_menu.LevelingEndstop=ENDSTOP_LEVEL_EN;
			machine_menu.opened=ENDSTOP_OPENED_EN;
			machine_menu.closed=ENDSTOP_CLOSED_EN;

			machine_menu.FilamentConfTitle=FILAMENT_CONF_TITLE_EN;
			machine_menu.InTemperature=FILAMENT_IN_TEMPERATURE_EN;
			machine_menu.InLength=FILAMENT_IN_LENGTH_EN;
			machine_menu.InSpeed=FILAMENT_IN_SPEED_EN;
			machine_menu.OutTemperature=FILAMENT_OUT_TEMPERATURE_EN;
			machine_menu.OutLength=FILAMENT_OUT_LENGTH_EN;
			machine_menu.OutSpeed=FILAMENT_OUT_SPEED_EN;

			machine_menu.LevelingParaConfTitle=LEVELING_CONF_TITLE_EN;
			machine_menu.LevelingParaConf=LEVELING_PARA_CONF_EN;
			machine_menu.DeltaLevelConf=LEVELING_DELTA_EN;
			machine_menu.XYZLevelconf=LEVELING_XYZ_EN;

			machine_menu.LevelingSubConfTitle=LEVELING_PARA_CONF_TITLE_EN;
			machine_menu.AutoLevelEnable=AUTO_LEVELING_ENABLE_EN;
			machine_menu.BLtouchEnable=BLTOUCH_LEVELING_ENABLE_EN;
			machine_menu.ProbePort=PROBE_PORT_EN;
			machine_menu.ProbeXoffset=PROBE_X_OFFSET_EN;
			machine_menu.ProbeYoffset=PROBE_Y_OFFSET_EN;
			machine_menu.ProbeZoffset=PROBE_Z_OFFSET_EN;
			machine_menu.ProbeXYspeed=PROBE_XY_SPEED_EN;
			machine_menu.ProbeZspeed=PROBE_Z_SPEED_EN;
			machine_menu.enable = ENABLE_EN;
			machine_menu.disable = DISABLE_EN;
			machine_menu.z_min = Z_MIN_EN;
			machine_menu.z_max = Z_MAX_EN;

			machine_menu.LevelingSubDeltaConfTitle=DELTA_LEVEL_CONF_TITLE_EN;
			machine_menu.MachineRadius=DELTA_MACHINE_RADIUS_EN;
			machine_menu.DiagonalRod=DELTA_DIAGONAL_ROD_EN;
			machine_menu.PrintableRadius=DELTA_PRINT_RADIUS_EN;
			machine_menu.DeltaHeight=DELTA_HEIGHT_EN;
			machine_menu.SmoothRodOffset=SMOOTH_ROD_OFFSET_EN;
			machine_menu.EffectorOffset=EFFECTOR_OFFSET_EN;
			machine_menu.CalibrationRadius=CALIBRATION_RADIUS_EN;

			machine_menu.LevelingSubXYZConfTitle=XYZ_LEVEL_CONF_TITLE_EN;
			//machine_menu.ProbeMaxLeft=PROBE_REACH_MAX_LEFT_EN;
			//machine_menu.ProbeMaxRigh=PROBE_REACH_MAX_RIGHT_EN;
			//machine_menu.ProbeMaxfront=PROBE_REACH_MAX_FRONT_EN;
			//machine_menu.ProbeMaxback=PROBE_REACH_MAX_BACK_EN;

			machine_menu.TemperatureConfTitle=TEMPERATURE_CONF_TITLE_EN;
			machine_menu.NozzleConf=NOZZLE_CONF_EN;
			machine_menu.HotBedConf=HOTBED_CONF_EN;
			machine_menu.PreheatTemperConf=PREHEAT_TEMPER_EN;

			machine_menu.NozzleConfTitle=NOZZLE_CONF_TITLE_EN;
			machine_menu.NozzleCnt=NOZZLECNT_EN;
			machine_menu.NozzleType=NOZZLE_TYPE_EN;
			machine_menu.NozzleAdjustType=NOZZLE_ADJUST_TYPE_EN;
			machine_menu.NozzleMinTemperature=NOZZLE_MIN_TEMPERATURE_EN;
			machine_menu.NozzleMaxTemperature=NOZZLE_MAX_TEMPERATURE_EN;
			machine_menu.Extrude_Min_Temper=EXTRUD_MIN_TEMPER_EN;

			machine_menu.HotbedEnable=HOTBED_ENABLE_EN;
			machine_menu.HotbedConfTitle=HOTBED_CONF_TITLE_EN;
			machine_menu.HotbedAjustType=HOTBED_ADJUST_EN;
			machine_menu.HotbedMinTemperature=HOTBED_MIN_TEMPERATURE_EN;
			machine_menu.HotbedMaxTemperature=HOTBED_MAX_TEMPERATURE_EN;

			machine_menu.MaxFeedRateConfTitle=MAXFEEDRATE_CONF_TITLE_EN;
			machine_menu.XMaxFeedRate=X_MAXFEEDRATE_EN;
			machine_menu.YMaxFeedRate=Y_MAXFEEDRATE_EN;
			machine_menu.ZMaxFeedRate=Z_MAXFEEDRATE_EN;
			machine_menu.E0MaxFeedRate=E0_MAXFEEDRATE_EN;
			machine_menu.E1MaxFeedRate=E1_MAXFEEDRATE_EN;

			machine_menu.AccelerationConfTitle=ACCELERATION_CONF_TITLE_EN;
			machine_menu.PrintAcceleration=PRINT_ACCELERATION_EN;
			machine_menu.RetractAcceleration=RETRACT_ACCELERATION_EN;
			machine_menu.TravelAcceleration=TRAVEL_ACCELERATION_EN;
			machine_menu.X_Acceleration=X_ACCELERATION_EN;
			machine_menu.Y_Acceleration=Y_ACCELERATION_EN;
			machine_menu.Z_Acceleration=Z_ACCELERATION_EN;
			machine_menu.E0_Acceleration=E0_ACCELERATION_EN;
			machine_menu.E1_Acceleration=E1_ACCELERATION_EN;

			machine_menu.JerkConfTitle=JERK_CONF_TITLE_EN;
			machine_menu.X_Jerk=X_JERK_EN;
			machine_menu.Y_Jerk=Y_JERK_EN;
			machine_menu.Z_Jerk=Z_JERK_EN;
			machine_menu.E_Jerk=E_JERK_EN;

			machine_menu.StepsConfTitle=STEPS_CONF_TITLE_EN;
			machine_menu.X_Steps=X_STEPS_EN;
			machine_menu.Y_Steps=Y_STEPS_EN;
			machine_menu.Z_Steps=Z_STEPS_EN;
			machine_menu.E0_Steps=E0_STEPS_EN;
			machine_menu.E1_Steps=E1_STEPS_EN;

			machine_menu.MotorDirConfTitle=MOTORDIR_CONF_TITLE_EN;
			machine_menu.X_MotorDir=X_MOTORDIR_EN;
			machine_menu.Y_MotorDir=Y_MOTORDIR_EN;
			machine_menu.Z_MotorDir=Z_MOTORDIR_EN;
			machine_menu.E0_MotorDir=E0_MOTORDIR_EN;
			machine_menu.E1_MotorDir=E1_MOTORDIR_EN;
			machine_menu.Invert_0=INVERT_P_EN;
			machine_menu.Invert_1=INVERT_N_EN;

			machine_menu.HomeFeedRateConfTitle=HOMEFEEDRATE_CONF_TITLE_EN;
			machine_menu.XY_HomeFeedRate=X_HOMESPEED_EN;
			//machine_menu.Y_HomeFeedRate=Y_HOMESPEED_EN;
			machine_menu.Z_HomeFeedRate=Z_HOMESPEED_EN;

			machine_menu.AdvancedConfTitle=ADVANCED_CONF_TITLE_EN;
			machine_menu.PwrOffDection=PWROFF_DECTION_EN;
			machine_menu.HaveUps=HAVE_UPS_EN;
			machine_menu.Z2andZ2Endstop=Z2_AND_Z2ENDSTOP_CONF_EN;
			machine_menu.EnablePinsInvert=ENABLE_PINS_CONF_EN;

			machine_menu.Z2ConfTitle=Z2_AND_Z2ENDSTOP_CONF_TITLE_EN;
			machine_menu.Z2Enable=Z2_ENABLE_EN;
			machine_menu.Z2EndstopEnable=Z2_ENDSTOP_EN;
			machine_menu.Z2Port=Z2_PORT_EN;

			machine_menu.EnablePinsInvertTitle=ENABLE_PINS_CONF_TITLE_EN;
			machine_menu.XInvert=X_ENABLE_PINS_INVERT_EN;
			machine_menu.YInvert=Y_ENABLE_PINS_INVERT_EN;
			machine_menu.ZInvert=Z_ENABLE_PINS_INVERT_EN;
			machine_menu.EInvert=E_ENABLE_PINS_INVERT_EN;

			machine_menu.key_back = KEY_BACK_EN;
			machine_menu.key_rest = KEY_REST_EN;
			machine_menu.key_confirm = KEY_CONFIRM_EN;
			machine_menu.high_level = MOTOR_EN_HIGH_LEVEL_EN;
			machine_menu.low_level = MOTOR_EN_LOW_LEVEL_EN;
			//


			common_menu.dialog_confirm_title = TITLE_DIALOG_CONFIRM_EN;
			common_menu.text_back=BACK_TEXT_EN;
			common_menu.close_machine_tips = DIALOG_CLOSE_MACHINE_EN;
			common_menu.unbind_printer_tips = DIALOG_UNBIND_PRINTER_EN;
			common_menu.print_special_title = PRINTING_OTHER_LANGUGE;
			common_menu.pause_special_title = PRINTING_PAUSE_OTHER_LANGUGE;
			common_menu.operate_special_title = PRINTING_OPERATION_OTHER_LANGUGE;			
			//��ҳ��
			main_menu.title=TITLE_READYPRINT_EN;
			main_menu.preheat=PREHEAT_TEXT_EN;
			main_menu.move=MOVE_TEXT_EN;
			main_menu.home=HOME_TEXT_EN;
			main_menu.print=PRINT_TEXT_EN;
			main_menu.extrude=EXTRUDE_TEXT_EN;
			main_menu.leveling=LEVELING_TEXT_EN;
			main_menu.autoleveling=AUTO_LEVELING_TEXT_EN;
			main_menu.fan = FAN_TEXT_EN;
			main_menu.set=SET_TEXT_EN;
			main_menu.more=MORE_TEXT_EN;
			main_menu.tool = TOOL_TEXT_EN;
			//TOOL
			tool_menu.title = TOOL_TEXT_EN;
			tool_menu.preheat = TOOL_PREHEAT_EN;
			tool_menu.extrude = TOOL_EXTRUDE_EN;
			tool_menu.move = TOOL_MOVE_EN;
			tool_menu.home= TOOL_HOME_EN;
			tool_menu.leveling = TOOL_LEVELING_EN;
			tool_menu.autoleveling = TOOL_AUTO_LEVELING_EN;
			tool_menu.filament = TOOL_FILAMENT_EN;
			tool_menu.more = TOOL_MORE_EN;			
			//Ԥ��
			preheat_menu.adjust_title = TITLE_ADJUST_EN;
			preheat_menu.title=TITLE_PREHEAT_EN;
			preheat_menu.add=ADD_TEXT_EN;
			preheat_menu.dec=DEC_TEXT_EN;
			preheat_menu.ext1=EXTRUDER_1_TEXT_EN;
			preheat_menu.ext2=EXTRUDER_2_TEXT_EN;
			preheat_menu.hotbed=HEATBED_TEXT_EN;
			preheat_menu.off=CLOSE_TEXT_EN;
			//�ƶ�
			move_menu.title = TITLE_MOVE_EN;
			//����
			home_menu.title=TITLE_HOME_EN;
			home_menu.stopmove = HOME_STOPMOVE_EN;
			//�ļ�Ŀ¼
			file_menu.title=TITLE_CHOOSEFILE_EN;
			file_menu.page_up=PAGE_UP_TEXT_EN;
			file_menu.page_down=PAGE_DOWN_TEXT_EN;
			file_menu.file_loading = FILE_LOADING_EN;
			file_menu.no_file = NO_FILE_EN;
			file_menu.no_file_and_check = NO_FILE_EN;//NO_FILE_AND_CHECK_EN;			
			//����
			extrude_menu.title=TITLE_EXTRUDE_EN;
			extrude_menu.in=EXTRUDER_IN_TEXT_EN;
			extrude_menu.out=EXTRUDER_OUT_TEXT_EN;
			extrude_menu.ext1=EXTRUDER_1_TEXT_EN;
			extrude_menu.ext2=EXTRUDER_2_TEXT_EN;
			extrude_menu.low=EXTRUDE_LOW_SPEED_TEXT_EN;
			extrude_menu.normal=EXTRUDE_MEDIUM_SPEED_TEXT_EN;
			extrude_menu.high=EXTRUDE_HIGH_SPEED_TEXT_EN;
			extrude_menu.temper_text=EXTRUDER_TEMP_TEXT_EN;
			//��ƽ
			leveling_menu.title=TITLE_LEVELING_EN;
			leveling_menu.position1=LEVELING_POINT1_TEXT_EN;
			leveling_menu.position2=LEVELING_POINT2_TEXT_EN;
			leveling_menu.position3=LEVELING_POINT3_TEXT_EN;
			leveling_menu.position4=LEVELING_POINT4_TEXT_EN;
			leveling_menu.position5=LEVELING_POINT5_TEXT_EN;
			//����
			set_menu.title=TITLE_SET_EN;
			set_menu.filesys=FILESYS_TEXT_EN;
			set_menu.wifi=WIFI_TEXT_EN;
			set_menu.about=ABOUT_TEXT_EN;
			set_menu.fan=FAN_TEXT_EN;
			set_menu.filament=FILAMENT_TEXT_EN;
			set_menu.breakpoint=BREAK_POINT_TEXT_EN;
			set_menu.motoroff=MOTOR_OFF_TEXT_EN;
			set_menu.language=LANGUAGE_TEXT_EN;
			set_menu.machine_para = MACHINE_PARA_EN;
			//����
			more_menu.title = TITLE_MORE_EN;
			more_menu.zoffset = ZOFFSET_EN;
			//�ļ�ϵͳ
			filesys_menu.title = TITLE_FILESYS_EN;
			filesys_menu.sd_sys = SD_CARD_TEXT_EN;
			filesys_menu.usb_sys = U_DISK_TEXT_EN;
			//WIFI
			wifi_menu.title=WIFI_TEXT;
			//wifi_menu.key = WIFI_KEY_TEXT_EN;
			//wifi_menu.ip = WIFI_IP_TEXT_EN;
			//wifi_menu.state= WIFI_STA_TEXT_EN;
			wifi_menu.cloud= CLOUD_TEXT_EN;
			wifi_menu.reconnect = WIFI_RECONNECT_TEXT_EN;

			cloud_menu.title = TITLE_CLOUD_TEXT_EN;
			cloud_menu.bind = CLOUD_BINDED_EN;
			cloud_menu.binded = CLOUD_BINDED_EN;
			cloud_menu.unbind = CLOUD_UNBIND_EN;
			cloud_menu.unbinding = CLOUD_UNBINDED_EN;
			cloud_menu.disconnected = CLOUD_DISCONNECTED_EN;
			cloud_menu.disable = CLOUD_DISABLE_EN;
			//����
			about_menu.title = TITLE_ABOUT_EN;
			about_menu.type = ABOUT_TYPE_TEXT_EN;
			about_menu.version = ABOUT_VERSION_TEXT_EN;
			about_menu.wifi = ABOUT_WIFI_TEXT_EN;			
			//����
			fan_menu.title = TITLE_FAN_EN;
			fan_menu.add = FAN_ADD_TEXT_EN;
			fan_menu.dec = FAN_DEC_TEXT_EN;
			fan_menu.state = FAN_TIPS1_TEXT_EN;
			//����
			filament_menu.title = TITLE_FILAMENT_EN;
			filament_menu.in = FILAMENT_IN_TEXT_EN;
			filament_menu.out = FILAMENT_OUT_TEXT_EN;
			filament_menu.ext1 = FILAMENT_EXT0_TEXT_EN;
			filament_menu.ext2 = FILAMENT_EXT1_TEXT_EN;
			filament_menu.ready_replace = FILAMENT_CHANGE_TEXT_EN;
			filament_menu.filament_dialog_load_heat = FILAMENT_DIALOG_LOAD_HEAT_TIPS_EN;
			filament_menu.filament_dialog_load_heat_confirm = FILAMENT_DIALOG_LOAD_CONFIRM1_TIPS_EN;
			filament_menu.filament_dialog_loading = FILAMENT_DIALOG_LOADING_TIPS_EN;
			filament_menu.filament_dialog_load_completed = FILAMENT_DIALOG_LOAD_COMPLETE_TIPS_EN;
			filament_menu.filament_dialog_unload_heat = FILAMENT_DIALOG_UNLOAD_HEAT_TIPS_EN;
			filament_menu.filament_dialog_unload_heat_confirm = FILAMENT_DIALOG_UNLOAD_CONFIRM_TIPS_EN;
			filament_menu.filament_dialog_unloading = FILAMENT_DIALOG_UNLOADING_TIPS_EN;
			filament_menu.filament_dialog_unload_completed = FILAMENT_DIALOG_UNLOAD_COMPLETE_TIPS_EN;

			//����
			language_menu.title = TITLE_LANGUAGE_EN;
			language_menu.next = PAGE_DOWN_TEXT_EN;
			language_menu.up = PAGE_UP_TEXT_EN;			
			//���ڴ�ӡ����
			printing_menu.title = TITLE_PRINTING_EN;
			printing_menu.option = PRINTING_OPERATION_EN;
			printing_menu.stop = PRINTING_STOP_EN;
			printing_menu.pause = PRINTING_PAUSE_EN;
			printing_menu.resume = PRINTING_RESUME_EN;

			//��������
			operation_menu.title = TITLE_OPERATION_EN;
			operation_menu.pause = PRINTING_PAUSE_EN;
			operation_menu.stop = PRINTING_STOP_EN;
			operation_menu.temp = PRINTING_TEMP_EN;
			operation_menu.fan = FAN_TEXT_EN;
			operation_menu.extr = PRINTING_EXTRUDER_EN;
			operation_menu.speed = PRINTING_CHANGESPEED_EN;			
			operation_menu.filament= FILAMENT_TEXT_EN;
			operation_menu.more = PRINTING_MORE_EN;
			operation_menu.move = PRINTING_MOVE_EN;
			operation_menu.auto_off = AUTO_SHUTDOWN_EN;
			operation_menu.manual_off = MANUAL_SHUTDOWN_EN;			
			//��ͣ����
			pause_menu.title= TITLE_PAUSE_EN;
			pause_menu.resume = PRINTING_RESUME_EN;
			pause_menu.stop = PRINTING_STOP_EN;
			pause_menu.extrude = PRINTING_EXTRUDER_EN;
			pause_menu.move = PRINTING_MOVE_EN;
			pause_menu.filament = FILAMENT_TEXT_EN;
			pause_menu.more = PRINTING_MORE_EN;

			//���ٽ���
			speed_menu.title = TITLE_CHANGESPEED_EN;
			speed_menu.add = ADD_TEXT_EN;
			speed_menu.dec = DEC_TEXT_EN;
			speed_menu.move = MOVE_SPEED_EN;
			speed_menu.extrude = EXTRUDER_SPEED_EN;
			speed_menu.extrude_speed = EXTRUDER_SPEED_STATE_EN;
			speed_menu.move_speed = MOVE_SPEED_STATE_EN;
			//��ӡ��---�������
			printing_more_menu.title = TITLE_MORE_EN;
			printing_more_menu.fan = FAN_TEXT_EN;
			printing_more_menu.auto_close = AUTO_SHUTDOWN_EN;
			printing_more_menu.manual = MANUAL_SHUTDOWN_EN;
			printing_more_menu.speed = PRINTING_CHANGESPEED_EN;
			printing_more_menu.temp = PRINTING_TEMP_EN;

			//print_file_dialog_menu.title = TITLE_DIALOG_CONFIRM_EN;
			print_file_dialog_menu.confirm = DIALOG_CONFIRM_EN;
			print_file_dialog_menu.cancle = DIALOG_CANCLE_EN;
			print_file_dialog_menu.print_file = DIALOG_PRINT_MODEL_EN;
			print_file_dialog_menu.cancle_print = DIALOG_CANCEL_PRINT_EN;
			print_file_dialog_menu.retry = DIALOG_RETRY_EN;
			print_file_dialog_menu.stop = DIALOG_STOP_EN;
			print_file_dialog_menu.no_file_print_tips = DIALOG_ERROR_TIPS1_EN;	
			print_file_dialog_menu.print_from_breakpoint = DIALOG_REPRINT_FROM_BREAKPOINT_EN;
			print_file_dialog_menu.close_machine_error = DIALOG_ERROR_TIPS2_EN;
			print_file_dialog_menu.filament_no_press=DIALOG_FILAMENT_NO_PRESS_EN;
			print_file_dialog_menu.print_finish=DIALOG_PRINT_FINISH_EN;
			print_file_dialog_menu.print_time=DIALOG_PRINT_TIME_EN;
			print_file_dialog_menu.reprint=DIALOG_REPRINT_EN;
			print_file_dialog_menu.wifi_enable_tips=DIALOG_WIFI_ENABLE_TIPS_EN;
			//ZOFFSET
			zoffset_menu.title = TITLE_ZOFFSET_EN;
			zoffset_menu.inc = ZOFFSET_INC_EN;
			zoffset_menu.dec = ZOFFSET_DEC_EN;
		break;
	}
}

