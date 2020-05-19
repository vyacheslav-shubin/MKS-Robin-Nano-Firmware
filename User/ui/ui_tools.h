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

#define img_page_up				"bmp_page_up.bin"
#define img_page_down			"bmp_page_down.bin"
#define img_page_back			"bmp_page_back.bin"

#define img_fan_state0		"bmp_fan_state0.bin"
#define img_fan_state1		"bmp_fan_state1.bin"
#define img_fan_state2		"bmp_fan_state2.bin"

#define img_state_extruder1  	"bmp_ext1_state.bin"
#define img_state_extruder2  	"bmp_ext2_state.bin"
#define img_state_bed  			"bmp_bed_state.bin"
#define img_state_z  			"bmp_zpos_state.bin"
#define img_state_time  		"bmp_time_state.bin"
#define img_state_speed  		"bmp_speed_state.bin"

#define img_print_pause  		"bmp_pause100.bin"
#define img_print_resume  		"bmp_resume100.bin"
#define img_print_stop  		"bmp_stop100.bin"
#define img_print_tools  		"bmp_operate100.bin"
#define img_print_auto_power_off	"bmp_autoOffEnabled100.bin"
#define img_print_manual_power_off	"bmp_autoOffDisabled100.bin"

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

#define img_x_plus				"bmp_xAdd.bin"
#define img_x_minus				"bmp_xDec.bin"
#define img_y_plus				"bmp_yAdd.bin"
#define img_y_minus				"bmp_yDec.bin"
#define img_z_plus				"bmp_zAdd.bin"
#define img_z_minus				"bmp_zDec.bin"


#define img_move				"bmp_mov.bin"
#define img_babystep			img_move
#define img_home				"bmp_zero.bin"
#define img_home_z				"bmp_zeroZ.bin"
#define img_leveling_manual		"bmp_leveling.bin"
#define img_leveling_auto		"bmp_autoleveling.bin"
#define img_filament			"bmp_extruct.bin"
#define img_more				"bmp_more.bin"
#define img_print				"bmp_printing.bin"
#define img_settings			"bmp_set.bin"
#define img_tools				"bmp_tool.bin"
#define img_continue_print		"bmp_breakpoint.bin"

#define img_lang_rus			"bmp_ru.bin"
#define img_lang_eng			"bmp_en.bin"
#define img_lang_spa			"bmp_sp.bin"
#define img_lang_ita			"bmp_it.bin"
#define img_lang_fra			"bmp_fr.bin"
#define img_lang_cn_simp		"bmp_cns.bin"
#define img_lang_cn_comp		"bmp_cnc.bin"

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

#define img_home_all		"bmp_zero.bin"
#define img_home_x			"bmp_zeroX.bin"
#define img_home_y			"bmp_zeroY.bin"
#define img_home_z			"bmp_zeroZ.bin"
#define img_stop_motor		"bmp_function1.bin"

#define img_filament_load		"bmp_in.bin"
#define img_filament_unload		"bmp_out.bin"

#define img_fan				"bmp_fan.bin"
#define img_speed			"bmp_speed.bin"

#define img_machine_settings			"bmp_MachineSetting.bin"
#define img_temperature_settings		"bmp_TemperatureSetting.bin"
#define img_motor_settings				"bmp_MotorSetting.bin"
#define img_advanced_settings			"bmp_AdvanceSetting.bin"

#define img_check_on 		 "bmp_enable.bin"
#define img_check_off 		 "bmp_disable.bin"

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

#define 	SUICIDE_WAIT 	60

typedef struct {
	volatile uint8_t enabled;
	volatile uint8_t do_execute;
	volatile uint8_t count_down;
} SUICIDE_CFG;


#define PREVIEW_CHECKED_BIT		0
#define PREVIEW_EXISTS_BIT		1
#define PREVIEW_CACHED_BIT		2
#define PREVIEW_CACHED 			((1<<PREVIEW_CHECKED_BIT) | (1<<PREVIEW_CACHED_BIT) | (1 << PREVIEW_EXISTS_BIT))

typedef struct {
	unsigned char	once; //printing
	SUICIDE_CFG		suicide;
	unsigned char 	rate;
	char			file_name[100];

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
	BUTTON_Handle button_check;
} UI_CHECK_PAIR;

typedef struct {
	BUTTON_Handle button_text;
	BUTTON_Handle button_arrow;
} UI_ARROW_PAIR;



extern GUI_BITMAP bmp_struct_100x80;

extern UI_PRINT_PROCESS ui_print_process;

extern void reset_file_info();

extern UPLOAD_INFO upload_file_info;

extern void print_time_to_str(PRINT_TIME * pt, char * buf);

extern void ui_push_disp_stack(DISP_STATE ui_id);
extern void ui_reset_disp_stack(DISP_STATE ui_id);
extern void ui_pop_disp_stack(void);

extern void ui_clear_screen(void);
extern void ui_set_encoding(void);
extern WM_HWIN ui_std_init_window(DISP_STATE ui_id, WM_CALLBACK* cb);
extern void ui_drop_window(WM_HWIN wnd);

//with placeholder
#define ui_std_col(ph_x) (INTERVAL_H + (BTN_X_PIXEL+INTERVAL_H)*(ph_x))
#define ui_std_row(ph_y) (INTERVAL_V + (BTN_Y_PIXEL + INTERVAL_V) * (ph_y))
#define ui_std_button(ph_x, ph_y, hwnd, img, title) ui_create_std_button(ui_std_col(ph_x),  ui_std_row(ph_y), hwnd, img, title)
#define ui_std_button_return(hwnd) ui_std_button(3, 1, hwnd, "bmp_return.bin", common_menu.text_back)
#define ui_std_window(cb) WM_CreateWindow(0, titleHeight, LCD_WIDTH, imgHeight, WM_CF_SHOW, cb, 0)


extern BUTTON_Handle ui_create_std_button(int x, int y, WM_HWIN hWinParent, const char *pFile, const char* text);
extern BUTTON_Handle ui_create_150_80_button(int x, int y, WM_HWIN hWinParent, const char *pFile, const char* text);
extern BUTTON_Handle ui_create_100_80_button(int x, int y, WM_HWIN hWinParent, const char *pFile);
extern BUTTON_Handle ui_create_state_button(int x, int y, WM_HWIN hWinParent, const char *pFile);
extern void ui_update_state_button(BUTTON_Handle btn, const char *pFile);

extern BUTTON_Handle ui_create_dialog_button(int x, int y, WM_HWIN hWinParent, const char* text);

extern BUTTON_Handle ui_create_check_button(int x, int y, WM_HWIN hWinParent, uint8_t state);
extern void ui_update_check_button(BUTTON_Handle  btn, uint8_t state);


extern TEXT_Handle ui_create_std_text_f(int x, int y, int w, int h, WM_HWIN hWinParent, int flags, char *text);
extern TEXT_Handle ui_create_std_text(int x, int y, int w, int h, WM_HWIN hWinParent, char *text);
extern TEXT_Handle ui_create_dialog_text(int x, int y, int w, int h, WM_HWIN hWinParent, char *text);
extern void ui_set_text_value(TEXT_Handle handle, char* val);

extern void ui_update_std_button(BUTTON_Handle handle, const char* file, char* title);

extern PROGBAR_Handle ui_create_std_progbar(int x, int y, int w, int h, WM_HWIN hWinParent);


extern void ui_draw_config_lines();
extern void ui_draw_config_half_lines();

extern void ui_make_page_navigator(WM_HWIN hWin, UI_PAGE_NAVIGATOR * navigator);
extern void ui_make_check_pair(int row, WM_HWIN hWin, UI_CHECK_PAIR * pair, char* title, uint8_t state);
extern void ui_make_half_row_check_pair(int col, int row, WM_HWIN hWin, UI_CHECK_PAIR * pair, char* title, uint8_t state);

#define ui_compare_arrow_pair(handle, pair) (((handle)==pair.button_text) || ((handle)==pair.button_arrow))
extern void ui_make_arrow_pair(int row, WM_HWIN hWin, UI_ARROW_PAIR * pair, char* title);



extern uint8_t ui_timing_flags;
#define F_UI_TIMING_HALF_SEC		1<<0
#define F_UI_TIMING_SEC				1<<1

#define is_ui_timing(FLAG) (ui_timing_flags & FLAG)
#define ui_timing_set(FLAG) (ui_timing_flags |= FLAG)
#define ui_timing_clear(FLAG) (ui_timing_flags &= (~FLAG))

extern void ui_timings(void);

extern void ui_start_print_process(void);


extern void ui_start_print_file();

extern char ascii2dec(char ascii);
extern uint8_t ui_file_with_preview(char *path, int * withoffset);
extern void ui_gcode_small_preview(char * file_name, int offset, int xpos_pixel,int ypos_pixel);

extern void ui_update_fan_button(BUTTON_Handle button, TEXT_Handle text);

#if defined(__cplusplus)
}
#endif

#endif
