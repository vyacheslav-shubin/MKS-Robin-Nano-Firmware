#include "GUI.h"
#include "BUTTON.h"
#include "PROGBAR.h"
#include "string_deal.h"
#include "draw_ui.h"
#include "ui_tools.h"
#include "marlin.h"
#include "draw_ready_print.h"
//#include "gcode.h"
//#include "printer.h"
#include "draw_machine.h"
#include "draw_log_ui.h"
#include "draw_language.h"
#include "draw_about.h"
#include "draw_wifi.h"
#include "draw_print_file.h"
#include "draw_printing.h"
#include "draw_move_motor.h"
#include "draw_operate.h"
#include "draw_pause_ui.h"
#include "draw_extrusion.h"
#include "draw_pre_heat.h"
#include "draw_change_speed.h"
#include "draw_fan.h"
#include "draw_set.h"
#include "draw_zero.h"
#include "draw_Sprayer.h"
#include "draw_machine.h"
#include "draw_language.h"
#include "draw_log_ui.h"
#include "draw_wifi.h"
#include "draw_more.h"
#include "ff.h"
#include "pic_manager.h"
#include "ili9320.h"
#include "draw_printing_moremenu.h"
#include "draw_filamentchange.h"
/******add********/
#include "draw_main.h"
#include "tim.h"
//#include "mks_tft_com.h"
#include "draw_manual_leveling.h"
#include "Configuration.h"
//#include "mks_tft_reprint.h"
#include "Configuration_adv.h"
#include "types.h"
#include "mks_reprint.h"

#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_flash_ex.h"

#include "at24cxx.h"
#include "draw_bind.h"
#include "draw_disk.h"
#include "draw_zoffset.h"
#include "draw_tool.h"
#include "fatfs.h"

#include "Marlin.h"
#include "mks_test.h"
#include "wifi_list.h"
#include "draw_wifi_list.h"
#include "draw_keyboard.h"
#include "draw_Tips.h"
#include "wifi_module.h"
#include "draw_babyStep.h"
//Screen TFT_screen;

value_state value;
PRINT_TIME  print_time;

void start_print_time() {
	print_time.start = 1;
}

void stop_print_time() {
	print_time.start = 0;
}

void reset_print_time() {
	print_time.hours = 0;
	print_time.minutes = 0;
	print_time.seconds = 0;
	print_time.ms_10 = 0;
}

static float zpos_bak = -1;
extern volatile uint8_t temper_error_flg;

extern volatile int16_t logo_time;
//extern PR_STATUS printerStaus;
extern uint8_t temp_update_flag;


extern uint8_t print_start_flg;
/******end********/
extern GUI_FLASH const GUI_FONT GUI_FontHZ_fontHz14;

extern GUI_CONST_STORAGE GUI_BITMAP bmpreheat;
extern GUI_CONST_STORAGE GUI_BITMAP bmadd;
extern GUI_CONST_STORAGE GUI_BITMAP bmdec;
extern GUI_CONST_STORAGE GUI_BITMAP bmdir;
extern GUI_CONST_STORAGE GUI_BITMAP bmextru;
extern GUI_CONST_STORAGE GUI_BITMAP bmfile;
extern GUI_CONST_STORAGE GUI_BITMAP bmmov;
extern GUI_CONST_STORAGE GUI_BITMAP bmpageDown;
extern GUI_CONST_STORAGE GUI_BITMAP bmpageUp;
extern GUI_CONST_STORAGE GUI_BITMAP bmprinting;
extern GUI_CONST_STORAGE GUI_BITMAP bmreturn;
extern GUI_CONST_STORAGE GUI_BITMAP bmset;
extern GUI_CONST_STORAGE GUI_BITMAP bmzero;

uint8_t temperature_change_frequency = 1;
uint16_t temperature_change_frequency_cnt;

extern uint8_t wifi_refresh_flg;
extern void disp_wifi_state();

extern void filament_dialog_handle(void);

FIL TEST_FIL1;
uint8_t from_flash_pic;

DISP_STATE_STACK disp_state_stack;
DISP_STATE disp_state = MAIN_UI;
DISP_STATE last_disp_state;

char BMP_PIC_X = 0 ;
char BMP_PIC_Y = 0;

uint32_t To_pre_view;

static   GUI_HWIN hMainWnd;
static  BUTTON_STRUCT button4;//button1, button2, button3, button4;

extern GUI_FLASH const GUI_FONT GUI_FontHZ_fontHz18;
#if _LFN_UNICODE
extern TCHAR curFileName[100];
//TCHAR curFileName[150];
#else
extern char curFileName[100];
//char curFileName[150];
#endif

extern GUI_CONST_STORAGE GUI_BITMAP bmlogo;
extern GUI_CONST_STORAGE GUI_BITMAP bmpreheat;

extern CFG_ITMES gCfgItems;

extern int X_ADD,X_INTERVAL;
static volatile uint8_t fan_move_flag;
extern uint8_t fan_change_flag;

extern uint8_t DialogType;


//PR_STATUS printerStaus = pr_idle;

#define MAX_TITLE_LEN	28

static CB_EVENT_STACK gCbEventStack;
//Tan21060406
#define INC_RD_POINT(b)	((b.r_idx+1 == sizeof(b.event) / sizeof(b.event[0])) ? 0 : b.r_idx+1)
#define INC_WR_POINT(b)	((b.w_idx+1 == sizeof(b.event) / sizeof(b.event[0])) ? 0 : b.w_idx+1)

FIL *srcfp;

void init_cb_stack()
{
	memset(&gCbEventStack.event, 0, sizeof(gCbEventStack.event));
	gCbEventStack.r_idx = 0;
	gCbEventStack.w_idx = 0;
}

void push_cb_stack(int16_t event_id) {
	if(INC_WR_POINT(gCbEventStack) == gCbEventStack.r_idx) //overflow
		return;
	gCbEventStack.event[gCbEventStack.w_idx] = event_id;
	gCbEventStack.w_idx = INC_WR_POINT(gCbEventStack);
}

int16_t pop_cb_stack() {
	int16_t ret;
	if(gCbEventStack.r_idx != gCbEventStack.w_idx) {
		ret = gCbEventStack.event[gCbEventStack.r_idx];
		gCbEventStack.r_idx = INC_RD_POINT(gCbEventStack);
		return ret;
	}
	return -1;
}

void GUI_callback()
{
	int16_t action_id = pop_cb_stack();
	if(action_id != (int16_t)-1)
	{
		//uid.executeAction(action_id);
	}
}


void titleText_cat(char *str, int strSize, char *addPart)
{
	if(str == 0 || addPart == 0)
	{
		return;
	}

	if(strlen(str) + strlen(addPart) >= strSize)
	{
		return;
	}

	strcat(str, addPart);
}

static char TitleText[30];

inline char * get_display_title_ref(int index) {
	switch(disp_state_stack._disp_state[index]) {
		case PRINT_READY_UI:	return main_menu.title;
		case PRINT_FILE_UI:		return file_menu.title;
		case PRINTING_UI:
				if(disp_state_stack._disp_state[disp_state_stack._disp_index] == PRINTING_UI) {
					return common_menu.print_special_title;
				} else {
					return printing_menu.title;
				}
		case MOVE_MOTOR_UI:		return move_menu.title;
		case OPERATE_UI:
				if(disp_state_stack._disp_state[disp_state_stack._disp_index] == PRINTING_UI) {
					return common_menu.operate_special_title;
				} else {
					return operation_menu.title;
				}
		case PAUSE_UI:
				if(
						disp_state_stack._disp_state[disp_state_stack._disp_index] == PRINTING_UI
						|| disp_state_stack._disp_state[disp_state_stack._disp_index] == OPERATE_UI
						|| disp_state_stack._disp_state[disp_state_stack._disp_index] == PAUSE_UI) {
					return common_menu.pause_special_title;
				} else {
					return pause_menu.title;
				}
		case EXTRUSION_UI: 		return extrude_menu.title;
		case CHANGE_SPEED_UI: 	return speed_menu.title;
		case FAN_UI:			return fan_menu.title;
		case PRE_HEAT_UI:

				if(
						(disp_state_stack._disp_state[disp_state_stack._disp_index - 1] == OPERATE_UI)) {
					return preheat_menu.adjust_title;
				} else {
					return preheat_menu.title;
				}
		case SET_UI:			return set_menu.title;
		case ZERO_UI:			return home_menu.title;

		case SPRAYER_UI:		return 0;
		case MACHINE_UI:		return 0;


		case LANGUAGE_UI:		return language_menu.title;
		case ABOUT_UI:			return about_menu.title;
		case LOG_UI:			return 0;
		case DISK_UI:			return filesys_menu.title;
		case DIALOG_UI:			return common_menu.dialog_confirm_title;
		case WIFI_UI:			return TitleText, wifi_menu.title;
		case MORE_UI:
		case PRINT_MORE_UI:
								return more_menu.title;
		case FILAMENTCHANGE_UI: return filament_menu.title;
		case LEVELING_UI:
        case MESHLEVELING_UI:	return leveling_menu.title;
		case BIND_UI:			return cloud_menu.title;
		case ZOFFSET_UI:		return zoffset_menu.title;
		case TOOL_UI:			return tool_menu.title;
		case WIFI_LIST_UI:		return list_menu.title;
        case MACHINE_PARA_UI:	return MachinePara_menu.title;
        case BABY_STEP_UI:		return operation_menu.babystep;
	}
	return 0;
}

char *getDispText(int index) {
	memset(TitleText, 0, sizeof(TitleText));
	char * ref = get_display_title_ref(index);
	if (ref)
		strcpy(TitleText, ref);
	return TitleText;
}

static char titleText[100] = {0};

uint8_t	ui_timing_flags = 0;


char *creat_title_text() {
	int index = 0;
	char *tmpText = 0;
	TCHAR tmpCurFileStr[20];
	memset(tmpCurFileStr, 0, sizeof(tmpCurFileStr));

	#if _LFN_UNICODE
	cutFileName((TCHAR *)curFileName, 16, 16, (TCHAR *)tmpCurFileStr);	
	#else
	cutFileName(curFileName, 16, 16, tmpCurFileStr);
	#endif
	
	memset(titleText, 0, sizeof(titleText));
	while(index <= disp_state_stack._disp_index) {
		tmpText = getDispText(index);
		if((*tmpText == 0) || (tmpText == 0)) {
			index++;
			continue;
		}
		titleText_cat(titleText, sizeof(titleText), tmpText);
		if(index < disp_state_stack._disp_index)
			titleText_cat(titleText, sizeof(titleText), ">");
		index++;
	}
	
	if(disp_state_stack._disp_state[disp_state_stack._disp_index] == PRINTING_UI) {
		titleText_cat(titleText, sizeof(titleText), ":");
		titleText_cat(titleText, sizeof(titleText), (char *)tmpCurFileStr);	
	}

	if(strlen(titleText) > MAX_TITLE_LEN) {
		memset(titleText, 0, sizeof(titleText));
		tmpText = getDispText(0);
		if(*tmpText != 0) {
			titleText_cat(titleText, sizeof(titleText), tmpText);
			titleText_cat(titleText, sizeof(titleText), ">...>");
			tmpText = getDispText(disp_state_stack._disp_index);
			if(*tmpText != 0)
				titleText_cat(titleText, sizeof(titleText), tmpText);
		}
	}
	return titleText;
}

void disp_sel_lang()
{
	if(gCfgItems.language == LANG_ENGLISH)
	{
		BUTTON_SetBkColor(button4.btnHandle, BUTTON_CI_UNPRESSED, GUI_FOCUS_CLOLOR);	
		//BUTTON_SetBkColor(button1, BUTTON_CI_UNPRESSED, GUI_BLUE);	
		//BUTTON_SetBkColor(button3, BUTTON_CI_UNPRESSED, GUI_BLUE);	
	}
	/*else if(gCfgItems.language == 3)
	{
		BUTTON_SetBkColor(button3, BUTTON_CI_UNPRESSED, GUI_FOCUS_CLOLOR);	
		BUTTON_SetBkColor(button1, BUTTON_CI_UNPRESSED, GUI_BLUE);	
		BUTTON_SetBkColor(button2, BUTTON_CI_UNPRESSED, GUI_BLUE);	
	}*/
	else
	{
		//BUTTON_SetBkColor(button1, BUTTON_CI_UNPRESSED, GUI_FOCUS_CLOLOR);	
		BUTTON_SetBkColor(button4.btnHandle, BUTTON_CI_UNPRESSED, GUI_BUTTON_COLOR);	
		//BUTTON_SetBkColor(button3, BUTTON_CI_UNPRESSED, GUI_BLUE);	
	}
		
}



void clear_cur_ui() {
	last_disp_state = disp_state_stack._disp_state[disp_state_stack._disp_index];
	switch(disp_state_stack._disp_state[disp_state_stack._disp_index]) {
		case PRINT_READY_UI:	clear_ready_print(); 	break;
		case PRINT_FILE_UI:		clear_print_file(); 	break;
		case PRINTING_UI:		clear_printing(); 		break;
		case MOVE_MOTOR_UI:		clear_move_motor(); 	break;
		case OPERATE_UI:		Clear_operate();		break;
		case PAUSE_UI:			Clear_pause();			break;
		case EXTRUSION_UI:		Clear_extrusion();		break;
		case PRE_HEAT_UI:		clear_preHeat();		break;
		case CHANGE_SPEED_UI:	Clear_changeSpeed();	break;
		case FAN_UI:			Clear_fan();			break;
		case SET_UI:			Clear_Set();			break;
		case ZERO_UI:			clear_zero();			break;
		case SPRAYER_UI:								break;
		case MACHINE_UI:								break;
		case LANGUAGE_UI:		Clear_Language();		break;
		case ABOUT_UI:			Clear_About();			break;
		case LOG_UI:									break;
		case DISK_UI:									break;
		case WIFI_UI:			Clear_Wifi();			break;
		case MORE_UI:			Clear_more();			break;
		case FILETRANSFER_UI:							break;
		case DIALOG_UI:			Clear_dialog();			break;
		case FILETRANSFERSTATE_UI:						break;
		case PRINT_MORE_UI:		Clear_Printmore();		break;
		case LEVELING_UI:		clear_leveling();		break;
		case BIND_UI:			Clear_Bind();			break;
		case ZOFFSET_UI:								break;
		case TOOL_UI:			Clear_Tool();			break;
        case MESHLEVELING_UI:	Clear_MeshLeveling();	break;
        case HARDWARE_TEST_UI:	Clear_Hardwaretest();	break;
        case WIFI_LIST_UI:		Clear_Wifi_list();		break;
        case KEY_BOARD_UI:		Clear_Keyboard();		break;
        case TIPS_UI:			Clear_Tips();			break;
        case MACHINE_PARA_UI:	Clear_MachinePara();	break;
        case MACHINE_SETTINGS_UI:		Clear_MachineSettings();	break;
        case TEMPERATURE_SETTINGS_UI:	Clear_TemperatureSettings();break;
        case MOTOR_SETTINGS_UI:	Clear_MotorSettings();	break;
        case MACHINETYPE_UI:	Clear_MachineType();	break;
        case STROKE_UI:			Clear_Stroke();			break;
        case HOME_DIR_UI:		Clear_HomeDir();		break;
        case ENDSTOP_TYPE_UI:	Clear_EndstopType();	break;
        case FILAMENT_SETTINGS_UI:		Clear_FilamentSettings();	break;
        case LEVELING_SETTIGNS_UI:		Clear_LevelingSettings();	break;
        case LEVELING_PARA_UI:	Clear_LevelingPara();	break;
		case DELTA_LEVELING_PARA_UI:	Clear_DeltaLevelPara();		break;
		case XYZ_LEVELING_PARA_UI:		Clear_XYZLevelPara();		break;
		case MAXFEEDRATE_UI:	Clear_MaxFeedRate();	break;
		case STEPS_UI:			Clear_Steps();			break;
		case ACCELERATION_UI:	Clear_Acceleration();	break;
		case JERK_UI:			Clear_Jerk();			break;
		case MOTORDIR_UI:		Clear_MotorDir();		break;
		case HOMESPEED_UI:		Clear_HomeSpeed();		break;
		case NOZZLE_CONFIG_UI:	Clear_NozzleConfig();	break;
		case HOTBED_CONFIG_UI:	Clear_HotbedConfig();	break;
		case ADVANCED_UI:		Clear_Advanced();		break;
		case DOUBLE_Z_UI:		Clear_DoubleZ();		break;
		case ENABLE_INVERT_UI:	Clear_EnableInvert();	break;
		case NUMBER_KEY_UI:		Clear_NumberKey();		break;
		case BABY_STEP_UI:		clear_babyStep();		break;
		default:	break;
	}
	GUI_Clear();
}

void draw_return_ui() {
	if(disp_state_stack._disp_index > 0) {
		disp_state_stack._disp_index--;
		switch(disp_state_stack._disp_state[disp_state_stack._disp_index]) {
			case PRINT_READY_UI: 	draw_ready_print();		break;
			case PRINT_FILE_UI: 	draw_print_file();		break;
			case PRINTING_UI:
				if(from_flash_pic == 1)
					flash_preview_begin = 1;
				else
					default_preview_flg = 1; 
				draw_printing();
				break;
			case MOVE_MOTOR_UI: 	draw_move_motor();		break;
			case OPERATE_UI:		draw_operate();			break;
			case PAUSE_UI:			draw_pause();			break;
			case EXTRUSION_UI:		draw_extrusion();		break;
			case PRE_HEAT_UI:		draw_preHeat();			break;
			case CHANGE_SPEED_UI:	draw_changeSpeed();		break;
			case FAN_UI:			draw_fan();				break;
			case SET_UI:			draw_Set();				break;
			case ZERO_UI:
				if(!(TimeIncrease * TICK_CYCLE % 500))	// 0.5s
					draw_zero();
				break;
			case SPRAYER_UI: 	break;
			case MACHINE_UI:	break;
			case LANGUAGE_UI:		draw_Language();		break;
			case ABOUT_UI:			draw_About();			break;

#if tan_mask
			case LOG_UI:			draw_Connect();			break;
#endif
			case CALIBRATE_UI:	break;
			case DISK_UI:		break;
			case WIFI_UI:			draw_Wifi();			break;
			case MORE_UI:			draw_More();			break;
			case PRINT_MORE_UI:		draw_printmore();		break;
			case FILAMENTCHANGE_UI:	draw_FilamentChange();	break;
			case LEVELING_UI:		draw_leveling();		break;
			case BIND_UI:			draw_bind();			break;
#if tan_mask
			case ZOFFSET_UI:		draw_Zoffset();			break;
#endif
			case TOOL_UI:			draw_tool();			break;
            case MESHLEVELING_UI:	draw_meshleveling();	break;
            case HARDWARE_TEST_UI:	draw_Hardwaretest();	break;
            case WIFI_LIST_UI:		draw_Wifi_list();		break;
            case KEY_BOARD_UI:		draw_Keyboard();		break;
            case TIPS_UI:			draw_Tips();			break;
            case MACHINE_PARA_UI:	draw_MachinePara();		break;
            case MACHINE_SETTINGS_UI:		draw_MachineSettings();			break;
            case TEMPERATURE_SETTINGS_UI:	draw_TemperatureSettings();		break;
            case MOTOR_SETTINGS_UI:	draw_MotorSettings();	break;
            case MACHINETYPE_UI:	draw_MachineType();		break;
            case STROKE_UI:			draw_Stroke();			break;
            case HOME_DIR_UI:		draw_HomeDir();			break;
            case ENDSTOP_TYPE_UI:	draw_EndstopType();		break;
            case FILAMENT_SETTINGS_UI:		draw_FilamentSettings();		break;
            case LEVELING_SETTIGNS_UI:		draw_LevelingSettings();		break;
            case LEVELING_PARA_UI:			draw_LevelingPara();			break;
            case DELTA_LEVELING_PARA_UI:	draw_DeltaLevelPara();			break;
            case XYZ_LEVELING_PARA_UI:		draw_XYZLevelPara();			break;
            case MAXFEEDRATE_UI:	draw_MaxFeedRate();		break;
            case STEPS_UI:			draw_Steps();			break;
            case ACCELERATION_UI:	draw_Acceleration();	break;
            case JERK_UI:			draw_Jerk();			break;
            case MOTORDIR_UI:		draw_MotorDir();		break;
            case HOMESPEED_UI:		draw_HomeSpeed();		break;
            case NOZZLE_CONFIG_UI:	draw_NozzleConfig();	break;
            case HOTBED_CONFIG_UI:	draw_HotbedConfig();	break;
            case ADVANCED_UI:		draw_Advanced();		break;
            case DOUBLE_Z_UI:		draw_DoubleZ();			break;
            case ENABLE_INVERT_UI:	draw_EnableInvert();	break;
            case NUMBER_KEY_UI:		draw_NumberKey();		break;
            case DIALOG_UI:			draw_dialog(DialogType);	break;
            case BABY_STEP_UI:		draw_babyStep();		break;
			default:
				break;
		}
	}
}

extern u16 DeviceCode;

void gui_view_init()
{
	GUI_SetFont(&FONT_TITLE);
	BUTTON_SetDefaultFont(&FONT_TITLE);
	TEXT_SetDefaultFont(&FONT_TITLE);
    TEXT_SetDefaultTextColor(gCfgItems.title_color);
    //TEXT_SetDefaultBKColor(gCfgItems.background_color);
	GUI_SetBkColor(gCfgItems.background_color);
    GUI_SetColor(gCfgItems.title_color);
	BUTTON_SetDefaultTextAlign(GUI_TA_BOTTOM | GUI_TA_HCENTER);

	BUTTON_SetDefaultBkColor(gCfgItems.btn_color, BUTTON_CI_UNPRESSED);
	BUTTON_SetDefaultBkColor(gCfgItems.btn_color, BUTTON_CI_PRESSED);
	BUTTON_SetDefaultTextColor(gCfgItems.btn_textcolor, BUTTON_CI_UNPRESSED);
	BUTTON_SetDefaultTextColor(gCfgItems.btn_textcolor, BUTTON_CI_PRESSED);
	if(gCfgItems.button_3d_effect_flag != 1)
		WIDGET_SetDefaultEffect(&WIDGET_Effect_Simple);

	init_cb_stack();

    gCfgItems.touch_adj_xMin = 3518;
    gCfgItems.touch_adj_xMax = 389;
    gCfgItems.touch_adj_yMin = 284;
    gCfgItems.touch_adj_yMax = 3770;

    if(DeviceCode == 0x9341) {
		gCfgItems.touch_adj_xMin = 400;
		gCfgItems.touch_adj_xMax = 3836;
		gCfgItems.touch_adj_yMin = 3805;
		gCfgItems.touch_adj_yMax = 212;

	} else if(DeviceCode == 0x8989) {
		gCfgItems.touch_adj_xMin = 389;
		gCfgItems.touch_adj_xMax = 3518;
		gCfgItems.touch_adj_yMin = 284;
		gCfgItems.touch_adj_yMax = 3770;
	} else if(DeviceCode == 0x1505) {
		gCfgItems.touch_adj_xMin = 304;
		gCfgItems.touch_adj_xMax = 3875;
		gCfgItems.touch_adj_yMin = 313;
		gCfgItems.touch_adj_yMax = 3875;
	} else if(DeviceCode==0x8552) {
		gCfgItems.touch_adj_xMin = 352;
		gCfgItems.touch_adj_xMax = 3924;
		gCfgItems.touch_adj_yMin = 3840;
		gCfgItems.touch_adj_yMax = 201;       
    } else if(DeviceCode == 0X9488) {
		if(gCfgItems.overturn_180 != 0xEE) {
			gCfgItems.touch_adj_flag = 0;
			gCfgItems.touch_adj_xMin = 201;
			gCfgItems.touch_adj_xMax = 3919;
			gCfgItems.touch_adj_yMin = 3776;
			gCfgItems.touch_adj_yMax = 164;
		} else {
			gCfgItems.touch_adj_flag = 0;
			gCfgItems.touch_adj_xMin = 3919;
			gCfgItems.touch_adj_xMax = 201;
			gCfgItems.touch_adj_yMin = 164;
			gCfgItems.touch_adj_yMax = 3776;
		}
    }
}

extern uint16_t z_high_count;
extern uint8_t move_speed_flg;
extern volatile WIFI_STATE wifi_link_state;

void GUI_RefreshPage() {
  	__IO uint32_t i =0;
	switch(disp_state) {
		case MAIN_UI:	draw_ready_print(); break;
		case ZERO_UI:
			if(!(TimeIncrease * TICK_CYCLE % 500))	// 0.5s
		    	  refresh_zero();
		    break;
		case LEVELING_UI:
			if(!(TimeIncrease * TICK_CYCLE % 500))	// 0.5s
				refresh_leveling();
			break;
		case EXTRUSION_UI:
			if(temperature_change_frequency == 1) {
				temperature_change_frequency = 0;
				disp_sprayer_temp();
			}
			break;
		case PRE_HEAT_UI: refresh_preHeat(); break;
		case PRINT_READY_UI: break;
		case PRINT_FILE_UI: break;
		case PRINTING_UI: refresh_printing(); break;
		case OPERATE_UI:
			if(temperature_change_frequency == 1) {
				temperature_change_frequency = 0;
				disp_temp_operate();
			}
			setProBarRateOpera();
			break;

		case PAUSE_UI:
			if(temperature_change_frequency == 1){
				temperature_change_frequency = 0;
				disp_temp_pause();
			}
			break;
			case FAN_UI:
				if(temperature_change_frequency == 1)
				{
					temperature_change_frequency = 0;
					disp_fan_speed();
				}
				break;
					
		case MOVE_MOTOR_UI: refresh_move_motor(); break;
/*
		if(mksReprint.mks_printer_state == MKS_IDLE) {
				if((z_high_count==1)&&(temper_error_flg != 1)) {
					z_high_count = 0;
					memset((char *)gCfgItems.move_z_coordinate,' ',sizeof(gCfgItems.move_z_coordinate));
					GUI_DispStringAt((const char *)gCfgItems.move_z_coordinate,380, TITLE_YPOS);
					sprintf((char *)gCfgItems.move_z_coordinate,"Z: %.3f",current_position[Z_AXIS]);
					GUI_DispStringAt((const char *)gCfgItems.move_z_coordinate,380, TITLE_YPOS);
				}
			}
			break;
*/
		case WIFI_UI:
			if(wifi_refresh_flg == 1) {
				disp_wifi_state();
				wifi_refresh_flg = 0;
			}
			break;
        case BIND_UI:		refresh_bind_ui();	break;
		case FILAMENTCHANGE_UI:
			if(temperature_change_frequency) {
				temperature_change_frequency = 0;
				disp_filament_sprayer_temp();
			}
			break;
		case DIALOG_UI:
			refresh_dialog();
			break;		
		case MESHLEVELING_UI:
            disp_zpos();
            break;
		case HARDWARE_TEST_UI:
			break;      
		case WIFI_LIST_UI:
			if(wifi_refresh_flg == 1) {
				disp_wifi_list();
				wifi_refresh_flg = 0;
			}
			break;
		case KEY_BOARD_UI:
			update_password_disp();
		    update_join_state_disp();
		    break;
		case TIPS_UI:
			switch(tips_type) {
				case TIPS_TYPE_JOINING:
					if(wifi_link_state == WIFI_CONNECTED && strcmp((const char *)wifi_list.wifiConnectedName,(const char *)wifi_list.wifiName[wifi_list.nameIndex]) == 0) {
						tips_disp.timer = TIPS_TIMER_STOP;
						tips_disp.timer_count = 0;
						Clear_Tips();
						tips_type = TIPS_TYPE_WIFI_CONECTED;
						draw_Tips();
					}
					if (tips_disp.timer_count >= 30) {
						tips_disp.timer = TIPS_TIMER_STOP;
						tips_disp.timer_count = 0;
						Clear_Tips();
						tips_type = TIPS_TYPE_TAILED_JOIN;
						draw_Tips();
					}
					break;
				case TIPS_TYPE_TAILED_JOIN:
					if (tips_disp.timer_count >= 3) {
						tips_disp.timer = TIPS_TIMER_STOP;
						tips_disp.timer_count = 0;
						last_disp_state = TIPS_UI;
						Clear_Tips();
						draw_Wifi_list();
					}
					break;
				case TIPS_TYPE_WIFI_CONECTED:
					if(tips_disp.timer_count >= 3) {
						tips_disp.timer = TIPS_TIMER_STOP;
						tips_disp.timer_count = 0;
						last_disp_state = TIPS_UI;
						Clear_Tips();
						draw_Wifi();
					}
					break;
				default:
					break;
			}
            break;
		case BABY_STEP_UI: refresh_babyStep();	break;
	    default:
	    	break;
				
	}
}

const char* logo_file = "1:/bmp_logo.bin";


void draw_logo() {
	FIL file;
	int size = 320*480;
	LCD_setWindowArea(0, 0, 480, 320);
	LCD_WriteRAM_Prepare();

	int res = f_open(&file, logo_file, FA_OPEN_EXISTING | FA_READ);
	if(res == FR_OK) {
		while (size>0) {
			UINT readed;
			res = f_read(&file, bmp_public_buf, sizeof(bmp_public_buf), &readed);
			if((res == FR_OK) && (readed!=0)) {
				for(UINT i=0;i<readed;i+=2) {
					uint16_t *color=(uint16_t *)&bmp_public_buf[i];
					LCD_WriteRAM(*color);
					if (--size==0)
						break;
				}
			} else
				break;
		}
		f_close(&file);
	} else {
		int offset = 0;
		while (size != 0) {
			SPI_FLASH_BufferRead(bmp_public_buf,PIC_LOGO_ADDR+offset,sizeof(bmp_public_buf));
			for (int i=0;i<sizeof(bmp_public_buf);i+=2) {
				uint16_t *color=(uint16_t *)&bmp_public_buf[i];
				LCD_WriteRAM(*color);
				if (--size==0)
					break;
			}
			offset+=sizeof(bmp_public_buf);
		}
	}
}

void Draw_default_preview(int xpos_pixel,int ypos_pixel,uint8_t sel)
{
	int index; 
	int x_off = 0, y_off = 0;
	int _x, _y;
	uint16_t *p_index;
	int i;
	uint16_t temp_p;
	
	for(index = 0; index < 10; index ++) { //200*200
		if(sel == 1) {
			flash_view_Read(bmp_public_buf, 8000);//20k
		} else {
			default_view_Read(bmp_public_buf, 8000);//20k
		}
		i = 0;
		LCD_setWindowArea(xpos_pixel, y_off * 20+ypos_pixel, 200,20);     //200*200
		LCD_WriteRAM_Prepare(); 
		for(_y = y_off * 20; _y < (y_off + 1) * 20; _y++) {
			for (x_off = 0; x_off < 200; x_off++) {
				if(sel==1) {
					temp_p = (uint16_t)(bmp_public_buf[i]|bmp_public_buf[i+1]<<8);
					p_index = &temp_p;
				} else {
					p_index = (uint16_t *)(&bmp_public_buf[i]); 	
				}
				LCD_WriteRAM(*p_index);
				i += 2;
			}
			if(i >= 8000)
				break;
		}
		y_off++;		
	}
}

char ascii2dec(char ascii) {
	int result = 0;
	if(ascii == 0)
		return 0;
	if(ascii >= '0' && ascii <= '9')
		result = ascii - '0';
	else if(ascii >= 'a' && ascii <= 'f')
		result = ascii - 'a' + 0x0a;
	else if(ascii >= 'A' && ascii <= 'F')
		result = ascii - 'A' + 0x0a;
	else
		return 0;
	return result;
}

uint8_t preview_pic=0;


FIL curFile_1;

uint32_t row;
uint8_t gcode_preview_over;
uint8_t flash_preview_begin;
uint8_t default_preview_flg;
//uint8_t from_flash_pic;

extern "C" void SPI_FLASH_BufferWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite);
extern "C" void SPI_FLASH_SectorErase(u32 SectorAddr);

void gcode_preview(FIL *file,int xpos_pixel,int ypos_pixel) {
	uint8_t ress;
	UINT read,write;
	volatile uint32_t i,j;
	volatile uint16_t *p_index;
	int res;
	res = f_open(file, curFileName, FA_OPEN_EXISTING | FA_READ);
	if(res == FR_OK) {
		f_lseek(file, (PREVIEW_LITTLE_PIC_SIZE+To_pre_view)+809*row+8); //809 - длина строки в preview
		LCD_setWindowArea(xpos_pixel, ypos_pixel + row, 200,1);
		LCD_WriteRAM_Prepare();
		f_read(file, bmp_public_buf, 800, &read);
		i=0;j=0;
		while (i<800) {
			uint16_t *color = (uint16_t *)&(bmp_public_buf[j]);
			bmp_public_buf[j++] = ascii2dec(bmp_public_buf[i++])<<4 | ascii2dec(bmp_public_buf[i++]);
			bmp_public_buf[j++] = ascii2dec(bmp_public_buf[i++])<<4 | ascii2dec(bmp_public_buf[i++]);
			if(*color == 0x0000) *color=gCfgItems.preview_bk_color;
			LCD_WriteRAM(*color);
		}
		if(row<20)
			SPI_FLASH_SectorErase(BAK_VIEW_ADDR+row*4096);

		SPI_FLASH_BufferWrite(bmp_public_buf, BAK_VIEW_ADDR+row*400, 400);

		row++;
		if(row >= 200) {
			row = 0;
			gcode_preview_over = 0;
			f_close(file);
			ui_start_print_process();
		}
		f_close(file);
	}
}

void disp_pre_gcode(int xpos_pixel,int ypos_pixel)
{
	if(gcode_preview_over==1)
	{
		gcode_preview(&TEST_FIL1,xpos_pixel,ypos_pixel);
	}
	if(flash_preview_begin == 1)
	{
		flash_preview_begin = 0;
		Draw_default_preview(xpos_pixel,ypos_pixel,1);	
	}
	if(default_preview_flg == 1)
	{
		Draw_default_preview(xpos_pixel,ypos_pixel,0);
		default_preview_flg = 0;
	}

}

void preview_gcode_prehandle(char *path)
{
	UINT read;
	uint32_t pre_read_cnt = 0;
	char *p;
	
	gcode_preview_over = 0;
	default_preview_flg = 0;
	from_flash_pic = 0;
	if(f_open(&TEST_FIL1, path, FA_OPEN_EXISTING | FA_READ) == FR_OK) {
		f_read(&TEST_FIL1,&bmp_public_buf[0],1024,&read);
		p = strstr((const char *)&bmp_public_buf[0],(const char *)";simage:");
		
		if (p) {
			pre_read_cnt = (uint32_t)p-(uint32_t)((uint32_t *)(&bmp_public_buf[0]));
			To_pre_view = pre_read_cnt;
			gcode_preview_over = 1;
			from_flash_pic = 1;
		} else {
			default_preview_flg = 1;
		}
		epr_write_data(EPR_PREVIEW_FROM_FLASH, &from_flash_pic,1);
	}
}

void gcode_has_preview(char *path)
{
#if 1
	uint8_t re;
	UINT read;
	uint32_t pre_read_cnt = 0;
	uint32_t *p1,*p2;
	
	re = f_open(&TEST_FIL1, path, FA_OPEN_EXISTING | FA_READ);//	
	#if 1
	if(re==FR_OK)
	{
		f_read(&TEST_FIL1,&bmp_public_buf[0],1024,&read);

		p2 = (uint32_t *)strstr((const char *)&bmp_public_buf[0],(const char *)";simage:");
		if(p2)
		{
			pre_read_cnt = (uint32_t)p2-(uint32_t)((uint32_t *)(&bmp_public_buf[0]));
			To_pre_view = pre_read_cnt;

			from_flash_pic = 1;
			epr_write_data(EPR_PREVIEW_FROM_FLASH, &from_flash_pic,1);			
		}
		else
		{
			from_flash_pic = 0; 
			epr_write_data(EPR_PREVIEW_FROM_FLASH, &from_flash_pic,1);		
		}
	}
	#endif
    f_close(&TEST_FIL1);
#endif
}
