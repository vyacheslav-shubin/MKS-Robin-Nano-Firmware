#include "GUI.h"
#include "BUTTON.h"
#include "UI.h"
#include "draw_ui.h"
#include "Marlin.h"
#include "ff.h"
#include "pic_manager.h"
#include "ili9320.h"
/******add********/
#include "tim.h"
#include "Configuration.h"
#include "Configuration_adv.h"
#include "types.h"
#include "mks_reprint.h"

#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_flash_ex.h"

#include "at24cxx.h"
#include "fatfs.h"

#include "Marlin.h"
#include "mks_test.h"
#include "wifi_list.h"
#include "wifi_module.h"
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
}

static float zpos_bak = -1;
extern volatile uint8_t temper_error_flg;

extern volatile int16_t logo_time;
//extern PR_STATUS printerStaus;
extern uint8_t temp_update_flag;


extern uint8_t print_start_flg;
/******end********/
extern GUI_FLASH const GUI_FONT GUI_FontHZ_fontHz14;
extern uint8_t wifi_refresh_flg;
extern void disp_wifi_state();

extern void filament_dialog_handle(void);

FIL TEST_FIL1;

DISP_STATE_STACK disp_state_stack;
DISP_STATE disp_state = MAIN_UI;

//TODO: эта переменная не нужна...
DISP_STATE last_disp_state;

char BMP_PIC_X = 0 ;
char BMP_PIC_Y = 0;

uint32_t To_pre_view;

static   GUI_HWIN hMainWnd;
static  BUTTON_STRUCT button4;//button1, button2, button3, button4;

extern GUI_FLASH const GUI_FONT GUI_FontHZ_fontHz18;

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


void clear_cur_ui() {
	last_disp_state = disp_state_stack._disp_state[disp_state_stack._disp_index];
	switch(disp_state_stack._disp_state[disp_state_stack._disp_index]) {
		case SPRAYER_UI:								break;
		case MACHINE_UI:								break;
		case LOG_UI:									break;
		case DISK_UI:									break;
		case FILETRANSFER_UI:							break;
		case DIALOG_UI:			Clear_dialog();			break;
		case FILETRANSFERSTATE_UI:						break;
		case ZOFFSET_UI:								break;
        case MESHLEVELING_UI:	Clear_MeshLeveling();	break;
        case HARDWARE_TEST_UI:	Clear_Hardwaretest();	break;
		case DELTA_LEVELING_PARA_UI:	Clear_DeltaLevelPara();		break;
		case DOUBLE_Z_UI:		Clear_DoubleZ();		break;
		case ENABLE_INVERT_UI:	Clear_EnableInvert();	break;
		case NUMBER_KEY_UI:		Clear_NumberKey();		break;
		default:	break;
	}
	GUI_Clear();
}

void draw_return_ui() {
	if(disp_state_stack._disp_index > 0) {
		disp_state_stack._disp_index--;
		switch(disp_state_stack._disp_state[disp_state_stack._disp_index]) {
			case PRINT_READY_UI: 	main_ui.show();			break;
			case PRINTING_UI: printing_ui.show(); 			break;
			case MOVE_MOTOR_UI: 	motor_move_ui.show();	break;
			case PRE_HEAT_UI:		preheat_ui.show();		break;
			case FAN_UI:			fan_ui.show(); 			break;
			case ZERO_UI:			home_ui.show();			break;
			case FILAMENT_UI:		filament_ui.show();		break;
			case LEVELING_UI:		manual_leveling_ui.show();		break;
			case TOOL_UI:			tools_ui.show();		        break;
            case BABY_STEP_UI:		babystep_ui.show();		        break;
			case OPERATE_UI:		printing_tools_ui.show();		break;
			case CHANGE_SPEED_UI:	speed_ui.show();		        break;
			case MORE_UI:			more_ui.show();			        break;
			case FILE_BROWSER_UI: 	file_browser_ui.show();         break;
            case MOTORDIR_UI:		motor_dir_config_ui.show();     break;
			case MOTOR_SETTINGS_UI:	motor_settings_ui.show();	                    break;
			case FILE_INFO_UI:		file_info_ui.show();	                    break;
            case ADVANCED_UI:		advanced_config_ui.show();		            break;
			case SET_UI:			settings_ui.show();		                    break;
            case MACHINE_SETTINGS_UI:   machine_configuration_ui.show();		break;
            case FILAMENT_SETTINGS_UI:		filament_change_config_ui.show();	break;
            case XYZ_LEVELING_PARA_UI:		manual_leveling_ui.show();			break;
            case HOME_SETTINGS_UI:  home_config_ui.show();                      break;
            case ENDSTOP_TYPE_UI:	sensor_level_config_ui.show();		        break;
            case MACHINETYPE_UI:	machine_type_config_ui.show();              break;
            case LEVELING_PARA_UI:	leveling_auto_config_ui.show();             break;
            case HOTBED_CONFIG_UI:	hotbed_config_ui.show();	                break;
            case NOZZLE_CONFIG_UI:	nozzle_config_ui.show();	                break;
            case WIFI_UI:			wifi_ui.show();			                    break;
            case WIFI_LIST_UI:		wifi_list_ui.show();		                break;

            case MESHLEVELING_UI:	draw_meshleveling();	break;
            case HARDWARE_TEST_UI:	draw_Hardwaretest();	break;
            case DELTA_LEVELING_PARA_UI:	draw_DeltaLevelPara();			break;
            case DOUBLE_Z_UI:		draw_DoubleZ();			break;
            case ENABLE_INVERT_UI:	draw_EnableInvert();	break;
            case NUMBER_KEY_UI:		draw_NumberKey();		break;
            case DIALOG_UI:			draw_dialog(DialogType);	break;
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
		case MESHLEVELING_UI:
            disp_zpos();
            break;
	    default:
	    	break;
				
	}
}

const char * creat_title_text() {
	return ui_app.getTitle();
}



