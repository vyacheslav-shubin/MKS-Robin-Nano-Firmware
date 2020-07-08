#ifndef _UI_TOOLS
#define _UI_TOOLS

#include "draw_ui.h"
#include "GUI.h"
#include "BUTTON.h"
#include "PROGBAR.h"
#include "TEXT.h"

#if defined(__cplusplus)
extern "C" {
#endif

extern GUI_BITMAP bmp_struct_96x80;
extern GUI_BITMAP bmp_struct_50x50;


#define img_fan_state0		"bmp_fan_state0.bin"
#define img_fan_state1		"bmp_fan_state1.bin"
#define img_fan_state2		"bmp_fan_state2.bin"

#define img_state_extruder1  	"bmp_ext1_state.bin"
#define img_state_extruder2  	"bmp_ext2_state.bin"
#define img_state_bed  			"bmp_bed_state.bin"
#define img_state_z  			"bmp_zpos_state.bin"
#define img_state_time  		"bmp_time_state.bin"
#define img_state_mesh_point	"bmp_mesh_point.bin"
#define img_state_speed  		"bmp_speed_state.bin"

//96x80 images
#define img_print_pause  		"bmp_pause.bin"
#define img_print_resume  		"bmp_play.bin"
#define img_print_stop  		"bmp_stop.bin"
#define img_print_tools  		"bmp_tools96.bin"
#define img_print_auto_power_off	"bmp_auto_off.bin"
#define img_print_manual_power_off	"bmp_manual_off.bin"
#define img_ok					"bmp_ok.bin"
#define img_cancel				"bmp_cancel.bin"
#define img_Aa				    "bmp_Aa.bin"
#define img_page_up				"bmp_page_up.bin"
#define img_page_down			"bmp_page_down.bin"
#define img_page_back			"bmp_page_back.bin"
#define img_file_delete  		"bmp_delete.bin"
#define img_info  		        "bmp_info96.bin"


#define img_degree_step1	"bmp_step1_degree.bin"
#define img_degree_step5  	"bmp_step5_degree.bin"
#define img_degree_step10  	"bmp_step10_degree.bin"

#define img_percent_step1  	"bmp_step1_percent.bin"
#define img_percent_step5  	"bmp_step5_percent.bin"
#define img_percent_step10 	"bmp_step10_percent.bin"


#define img_temperature_off		"bmp_speed0.bin"

#define img_preset_pla  	"bmp_pla.bin"
#define img_preset_sbs  	"bmp_sbs.bin"
#define img_preset_petg  	"bmp_petg.bin"

#define img_plus  			"bmp_Add.bin"
#define img_minus  			"bmp_Dec.bin"


#define img_fan_speed0			"bmp_speed0.bin"
#define img_fan_speed50			"bmp_speed127.bin"
#define img_fan_speed100		"bmp_speed255.bin"

#define img_move_step_1			"bmp_moveStep1.bin"
#define img_move_step_2			"bmp_moveStep2.bin"
#define img_move_step_3			"bmp_moveStep3.bin"
#define img_move_step_huge		"bmp_moveStepHuge.bin"

#define img_preheat				"bmp_preHeat.bin"
#define img_back				"bmp_return.bin"

#define img_wifi				"bmp_wifi.bin"
#define img_cloud				"bmp_cloud.bin"
#define img_about				"bmp_about.bin"
#define img_machine_settings_root	"bmp_machinePara.bin"

#define img_x_plus				"bmp_xAdd.bin"
#define img_x_minus				"bmp_xDec.bin"
#define img_y_plus				"bmp_yAdd.bin"
#define img_y_minus				"bmp_yDec.bin"
#define img_z_plus				"bmp_zAdd.bin"
#define img_z_minus				"bmp_zDec.bin"


#define img_move				"bmp_mov.bin"
#define img_babystep			img_move
#define img_leveling		    "bmp_leveling.bin"
#define img_filament			"bmp_extruct.bin"
#define img_more				"bmp_more.bin"
#define img_print				"bmp_printing.bin"
#define img_settings			"bmp_set.bin"
#define img_power			    "bmp_power.bin"
#define img_tools				"bmp_tool.bin"
#define img_continue_print		"bmp_breakpoint.bin"


#define img_dir 				"bmp_dir.bin"
#define img_file 				"bmp_file.bin"
#define img_file_wpv			"bmp_file_WPV.bin"

#define img_leveling_point1		"bmp_leveling1.bin"
#define img_leveling_point2		"bmp_leveling2.bin"
#define img_leveling_point3		"bmp_leveling3.bin"
#define img_leveling_point4		"bmp_leveling4.bin"
#define img_leveling_point5		"bmp_leveling5.bin"

#define img_heating_selector_bed		"bmp_bed.bin"
#define img_heating_selector_ext1		"bmp_extru1.bin"
#define img_heating_selector_ext2		"bmp_extru2.bin"

#define img_speed_selector_ext1		img_heating_selector_ext1
#define img_speed_selector_ext2		img_heating_selector_ext2
#define img_speed_selector_move		img_move

#define img_filament_step1		"bmp_step1_mm.bin"
#define img_filament_step2		"bmp_step5_mm.bin"
#define img_filament_step3		"bmp_step10_mm.bin"

#define img_speed1			"bmp_speed_slow.bin"
#define img_speed2			"bmp_speed_normal.bin"
#define img_speed3			"bmp_speed_high.bin"

#define img_filament_change "bmp_filamentchange.bin"

#define img_home_all		"bmp_homeAll.bin"
#define img_home_x			"bmp_homeX.bin"
#define img_home_y			"bmp_homeY.bin"
#define img_home_z			"bmp_homeZ.bin"
#define img_home_t			"bmp_homeT.bin"
#define img_stop_motor		"bmp_function1.bin"

#define img_filament_load		"bmp_in.bin"
#define img_filament_unload		"bmp_out.bin"

#define img_fan				"bmp_fan.bin"
#define img_speed			"bmp_speed.bin"

#define img_check_on 		 "bmp_enable.bin"
#define img_check_off 		 "bmp_disable.bin"

#define img_dialog_confirm 		 "bmp_confirm.bin"
#define img_dialog_info 		 "bmp_info.bin"
#define img_arrow		        "bmp_arrow.bin"

#define img_navigator_back		        "bmp_back70x40.bin"
#define img_navigator_next		        "bmp_next70x40.bin"
#define img_navigator_prev		        "bmp_prev70x40.bin"

#define img_calc_button		        "bmp_calc_b.bin"
#define img_value_blank		        "bmp_value_blank.bin"
#define img_value_default		    "bmp_default.bin"

#define img_radio		            "bmp_radio.bin"
#define img_radio_sel		        "bmp_radio_sel.bin"


#define is_dual_extruders() (mksCfg.extruders == 2 && gCfgItems.singleNozzle == 0)
//#define is_dual_extruders() (1)

extern	FATFS fs;

extern const char* FAN_STATES[3];
extern char ui_buf1_100[100];
#define ui_buf1_20	ui_buf1_100
#define ui_buf1_80	ui_buf1_100

typedef struct{
	const char* pic;
	const char** title;
} BUTTON_META;

typedef struct{
	float step;
	char* pic;
} STEP_INFO;

typedef struct {
	volatile uint32_t time;
	volatile uint32_t size;
}
UPLOAD_INFO;

#define 	SUICIDE_WAIT 	    60
#define 	HW_SUICIDE_WAIT 	3

#define PREVIEW_CHECKED_BIT			0
#define PREVIEW_EXISTS_BIT			1
#define PREVIEW_CACHED_BIT			2
#define PREVIEW_CACHED 			((1<<PREVIEW_CHECKED_BIT) | (1<<PREVIEW_CACHED_BIT) | (1 << PREVIEW_EXISTS_BIT))

typedef struct {
	float minv;
	float maxv;
} UI_MIN_MAX;


typedef enum {
    PREVIEW_NONE,
    PREVIEW_50,
    PREVIEW_100
} PREVIEW_MODE;

typedef struct{
    PREVIEW_MODE mode;
    unsigned int offset;
} PREVIEW_META;


typedef struct {
	unsigned char	once; //printing
	unsigned char	suicide_enabled;
	unsigned char 	rate;
	char			file_name[100];

	UI_MIN_MAX		mmx;
	UI_MIN_MAX		mmy;
	UI_MIN_MAX		mmz;
	float			filament_used;
	int				time;
	float			layer_height;
	int				layer_count;
	unsigned long	size;
	PREVIEW_META    meta;
	unsigned char	preview_row;
	int				preview_offset;
	unsigned char	preview_state_flags;
} UI_PRINT_PROCESS;

typedef struct {

} UI_CURENT_FILE_INFO;

typedef struct {
	BUTTON_Handle button_next;
	BUTTON_Handle button_previous;
	BUTTON_Handle button_back;
	unsigned char page;
	unsigned char page_count;
} UI_PAGE_NAVIGATOR;

typedef struct {
	BUTTON_Handle button_text;
	BUTTON_Handle button_arrow;
} UI_ARROW_PAIR;



extern GUI_BITMAP bmp_struct_50x50;
extern GUI_BITMAP bmp_struct_70x50;
extern GUI_BITMAP bmp_struct_70x28;
extern GUI_BITMAP bmp_struct_30x30;

extern UI_PRINT_PROCESS ui_print_process;

extern void reset_file_info();

extern UPLOAD_INFO upload_file_info;

extern void print_time_to_str(PRINT_TIME * pt, char * buf);

extern void ui_push_disp_stack(DISP_STATE ui_id);
extern void ui_reset_disp_stack(DISP_STATE ui_id);

extern void ui_clear_screen(void);
extern void ui_set_encoding(void);
extern void ui_drop_window(WM_HWIN wnd);

//with placeholder
#define ui_std_col(ph_x) (INTERVAL_H + (BTN_X_PIXEL+INTERVAL_H)*(ph_x))
#define ui_std_row(ph_y) (INTERVAL_V + (BTN_Y_PIXEL + INTERVAL_V) * (ph_y))
#define ui_std_window(cb) WM_CreateWindow(0, titleHeight, LCD_WIDTH, LCD_HEIGHT - titleHeight, WM_CF_SHOW, cb, 0)


extern BUTTON_Handle ui_create_std_button(int x, int y, WM_HWIN hWinParent, const char *pFile, const char* text);
extern BUTTON_Handle ui_create_96_80_button(int x, int y, WM_HWIN hWinParent, const char *pFile);
extern BUTTON_Handle ui_create_state_button(int x, int y, WM_HWIN hWinParent, const char *pFile);
extern void ui_update_state_button(BUTTON_Handle btn, const char *pFile);

extern BUTTON_Handle ui_create_dialog_button(int x, int y, WM_HWIN hWinParent, const char* text);


extern TEXT_Handle ui_create_std_text_f(int x, int y, int w, int h, WM_HWIN hWinParent, int flags, const char *text);
extern TEXT_Handle ui_create_std_text(int x, int y, int w, int h, WM_HWIN hWinParent, const char *text);
extern TEXT_Handle ui_create_dialog_text(int x, int y, int w, int h, WM_HWIN hWinParent, const char *text);
extern void ui_set_text_value(TEXT_Handle handle, char* val);

extern void ui_update_std_button(BUTTON_Handle handle, const char* file, const char* title);

extern PROGBAR_Handle ui_create_std_progbar(int x, int y, int w, int h, WM_HWIN hWinParent);

extern char ascii2dec(char ascii);

extern unsigned char ui_file_with_preview(char *path, int *withoffset);

extern void ui_file_check_preview(char *path, PREVIEW_META *meta);

extern void ui_gcode_small_preview(char * file_name, PREVIEW_META * meta, int xpos_pixel,int ypos_pixel);

extern void ui_update_fan_button(BUTTON_Handle button, TEXT_Handle text);
#if defined(__cplusplus)
}
#endif

#endif
