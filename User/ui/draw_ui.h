#ifndef _DRAW_UI_
#define _DRAW_UI_
#include <stdlib.h>

#include "stdint.h"
#include "string.h"
#include "main.h"
#include "GUI.h"
#include "BUTTON.h"
#include "PROGBAR.h"
#include "fontLib.h"
#include "LISTBOX.h"
#include "TEXT.h"

#include "mks_cfg.h"

#include "id_manage.h"
#include "draw_dialog.h"
#include "id_manage.h"
#include "ff.h"
#include "string_deal.h"
#include "pic.h"
#include "Multi_language.h"
#include "lang_common.h"
#include "draw_meshleveling.h"
#include "draw_deltaLevelPare.h"
#include "draw_MaxFeedRate.h"
#include "draw_steps.h"
#include "draw_acceleration.h"
#include "draw_jerk.h"
#include "draw_nozzle_config.h"
#include "draw_hotbed_config.h"
#include "draw_doubleZ.h"
#include "draw_EnablePin_Invert.h"
#include "draw_number_key.h"

extern float delta_height;
extern float delta_radius;
extern float delta_diagonal_rod;
extern float delta_calibration_radius;

extern float zprobe_zoffset;
extern void gcode_M500();
extern void gcode_M501();

typedef enum {

	xoffset,
	yoffset,
	zoffset,
	xyspeed,
	zspeed,

	DeltaRadius,
	DeltaDiagonalRod,
	PrintableRadius,
	DeltaHeight,
	SmoothRodOffset,
	EffectorOffset,
	CalibrationRadius,

	XMaxFeedRate,
	YMaxFeedRate,
	ZMaxFeedRate,
	E0MaxFeedRate,
	E1MaxFeedRate,
	
	PrintAcceleration,
	RetractAcceleration,
	TravelAcceleration,
	XAcceleration,
	YAcceleration,
	ZAcceleration,
	E0Acceleration,
	E1Acceleration,

	XJerk,
	YJerk,
	ZJerk,
	EJerk,

	Xstep,
	Ystep,
	Zstep,
	E0step,
	E1step,

	XYspeed,
	Zspeed,

	Nozzle_Cnt,
	Nozzle_min,
	Nozzle_max,
	Extrude_min_temper,
	Nozzle_pid_p,
	Nozzle_pid_i,
	Nozzle_pid_d,
	
	HotBed_min,
	HotBed_max,
	HotBed_pid_p,
	HotBed_pid_i,
	HotBed_pid_d

}value_state;

extern value_state value;


#define VERSION_WITH_PIC	1

//10460
#define PREVIEW_LITTLE_PIC_SIZE		((200+9)*50+9+1)
//172270
#define PREVIEW_SIZE			(PREVIEW_LITTLE_PIC_SIZE+(800+9)*200+9+1)


#define VALUE_DEFAULT_X				70
#define VALUE_DEFAULT_Y				28


#define LCD_WIDTH			480
#define LCD_HEIGHT		320

#define MAIN_PAGE_X		32
#define MAIN_PAGE_Y		90

#define TOP_BLANK			30
#define TOP_WIDTH			160

#define ICON_GAP_H	 2
#define ICON_GAP_V  2

#define SUB_PAGE_X		2
#define SUB_PAGE_Y		0

#define SUB_BTN_X_PIXEL 	100
#define SUB_BTN_Y_PIXEL  40

#define titleHeight	36//TFT_screen.title_high

#define imgHeight		(LCD_HEIGHT-titleHeight)//(TFT_screen.high-TFT_screen.title_high)

#define SIMPLE_MAIN_PAGE_GAP	32

#define TITLE_XPOS	 3		//TFT_screen.title_xpos//
#define TITLE_YPOS		5		//TFT_screen.title_ypos//

#define MARGIN_PIXEL  3

#define INTERVAL_H	2		//TFT_screen.gap_h
#define INTERVAL_V	2		//TFT_screen.gap_v

#define BTN_X_PIXEL	117		//TFT_screen.btn_x_pixel//
#define BTN_Y_PIXEL	140//TFT_screen.btn_y_pixel//

#define BTN_PIC_X_PIXEL	117
#define BTN_PIC_Y_PIXEL	140

#define BTN_PIC_X_OFFSET		0
#define BTN_PIC_Y_OFFSET		0

#define BTN_TEXT_OFFSET		8

#define OTHER_BTN_XPIEL		117
#define OTHER_BTN_YPIEL		92

#define STATE_PIC_X_PIXEL	45
#define STATE_PIC_Y_PIXEL	45

#define FILE_PRE_PIC_X_OFFSET	8
#define FILE_PRE_PIC_Y_OFFSET	5

#define	DIALOG_BTN_XPIEL		140
#define DIALOG_BTN_YPIEL		50

#define SIMPLE_FIRST_PAGE_GRAP	32	

#define GUI_PURPLE			0x300018

//#define GUI_STATE_COLOR         0x646400 
//#define GUI_STATE_COLOR         0x806480 
#define GUI_RET_BUTTON_COLOR			GUI_BLACK//GUI_DARKGREEN//0x00a800
#define GUI_STATE_COLOR        			GUI_BLACK//GUI_DARKBLUE
#define GUI_BK_CLOLOR					GUI_BLACK
#define GUI_TITLE_TEXT_COLOR			GUI_WHITE
#define GUI_STATE_TEXT_COLOR			GUI_WHITE
#define GUI_FILE_NAME_TEXT_COLOR		GUI_WHITE
#define GUI_FILE_NAME_BK_COLOR			GUI_BLACK//GUI_BLUE
#define GUI_PRINTING_STATE_BK_COLOR		GUI_BLACK//GUI_DARKBLUE
#define GUI_PRINTING_STATE_TEXT_COLOR		GUI_WHITE

#define GUI_BUTTON_COLOR		GUI_BLACK // GUI_BLUE
#define GUI_FOCUS_CLOLOR		GUI_RET_BUTTON_COLOR

#define GUI_DARKGREEN2         GUI_DARKGREEN

extern char BMP_PIC_X ;
extern char BMP_PIC_Y;  // 0// 17

#define BMP_PIC_X_H		0
#define BMP_PIC_Y_H    0

#define FILE_SYS_USB	0
#define FILE_SYS_SD		1


struct PressEvt {
	int x;
	int y;
	unsigned char pressed;
};

typedef struct {
	int16_t  		days;
	uint16_t    	hours;
	uint8_t    		minutes;
	volatile int8_t    seconds;
	int8_t			ms_10;
	int8_t			start;
} PRINT_TIME;

extern PRINT_TIME  print_time;

typedef enum {
	MAIN_UI,
	PRINT_READY_UI,
	PRINTING_UI,
	MOVE_MOTOR_UI,
	OPERATE_UI,
	FILAMENT_UI,
	FAN_UI,
	PRE_HEAT_UI,
	CHANGE_SPEED_UI,
	TEMP_UI,
	SET_UI,
	ZERO_UI,
	SPRAYER_UI,
	MACHINE_UI,
	LANGUAGE_UI,
	ABOUT_UI,
	LOG_UI,
	DISK_UI,
	CALIBRATE_UI,
	DIALOG_UI,
	WIFI_UI,
	MORE_UI,
	FILETRANSFER_UI,
	FILETRANSFERSTATE_UI,
	LEVELING_UI,
	MESHLEVELING_UI,
	BIND_UI,
	ZOFFSET_UI,
	TOOL_UI,
	HARDWARE_TEST_UI,
	WIFI_LIST_UI,
	KEY_BOARD_UI,
	TIPS_UI,
	MACHINE_PARA_UI,
	MACHINE_SETTINGS_UI,
	TEMPERATURE_SETTINGS_UI,
	MOTOR_SETTINGS_UI,
	MACHINETYPE_UI,
	STROKE_UI,
	HOME_DIR_UI,
	ENDSTOP_TYPE_UI,
    FILAMENT_SETTINGS_UI,
    HOME_SETTINGS_UI,
	LEVELING_SETTIGNS_UI,
	LEVELING_PARA_UI,
	DELTA_LEVELING_PARA_UI,
	XYZ_LEVELING_PARA_UI,
	MAXFEEDRATE_UI,
	STEPS_UI,
	ACCELERATION_UI,
	JERK_UI,
	MOTORDIR_UI,
	HOMESPEED_UI,
	NOZZLE_CONFIG_UI,
	HOTBED_CONFIG_UI,
	ADVANCED_UI,
	DOUBLE_Z_UI,
	ENABLE_INVERT_UI,
	NUMBER_KEY_UI,
	BABY_STEP_UI,
	FILE_BROWSER_UI,
	FILE_INFO_UI,
	CUSTOM_DIALOG_UI,

} DISP_STATE;

typedef struct {
	DISP_STATE _disp_state[100];
	char       _disp_index;
} DISP_STATE_STACK;

#define  CB_EVENT_DEPTH	20

typedef struct {
	int16_t event[CB_EVENT_DEPTH];
	int8_t  r_idx;
	int8_t  w_idx;
	
} CB_EVENT_STACK;

typedef enum {
	SD_NOT_INIT = 1,
	SD_INIT_OK,
	SD_FILE_RET_BEGIN,
	SD_FILE_RET_END,
	SD_SEL_FILE
	
} SD_STATE;


typedef struct {
	BUTTON_Handle 	btnHandle;
	GUI_BITMAP 		btnPic;
} BUTTON_STRUCT;

#if defined(__cplusplus)
extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif

extern CFG_ITMES gCfgItems;

extern uint8_t temperature_change_frequency;

extern DISP_STATE disp_state;
extern DISP_STATE last_disp_state;
extern DISP_STATE_STACK disp_state_stack;

extern volatile uint32_t TimeIncrease;
extern uint16_t temperature_change_frequency_cnt;
extern uint8_t temperature_change_frequency;
extern uint8_t from_flash_pic;

extern void start_print_time();
extern void stop_print_time();
extern void reset_print_time();

extern void _draw_logo();
extern int8_t get_printing_rate(FIL *fileHandle);
extern void draw_return_ui();

extern void clear_cur_ui(void);
extern void gui_view_init(void);
extern void push_cb_stack(int16_t event_id);
extern void GUI_callback(void);

extern void GUI_RefreshPage(void);

extern char * creat_title_text();

#if defined(__cplusplus)
}
#endif

#endif
