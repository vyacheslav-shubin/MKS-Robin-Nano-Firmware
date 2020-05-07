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

const char img_fan_state0 = "bmp_fan_state0.bin";
const char img_fan_state1 = "bmp_fan_state1.bin";
const char img_fan_state2 = "bmp_fan_state2.bin";

#define is_dual_extruders() (mksCfg.extruders == 2 && gCfgItems.singleNozzle == 0)
//#define is_dual_extruders() (1)

extern const char* FAN_STATES[3];
extern char ui_buf1_20[20];

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


typedef struct {
	uint8_t		once; //printing
	SUICIDE_CFG	suicide;
	uint8_t 	rate;
} UI_PRINT_PROCESS;

typedef struct {
	BUTTON_Handle button_next;
	BUTTON_Handle button_previous;
	BUTTON_Handle button_back;
	uint8_t	page;
	uint8_t	page_count;
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
extern void ui_make_title(void);
extern void ui_init_page(void);
extern WM_HWIN ui_std_init_window(DISP_STATE ui_id, WM_CALLBACK* cb);
extern void ui_drop_window(WM_HWIN wnd);

//with placeholder
#define ui_std_col(ph_x) (INTERVAL_H + (BTN_X_PIXEL+INTERVAL_H)*(ph_x))
#define ui_std_row(ph_y) (INTERVAL_V + (BTN_Y_PIXEL + INTERVAL_V) * (ph_y))
#define ui_std_button(ph_x, ph_y, hwnd, img, title) ui_create_std_button(ui_std_col(ph_x),  ui_std_row(ph_y), hwnd, img, title)
#define ui_std_button_return(hwnd) ui_std_button(3, 1, hwnd, "bmp_return.bin", common_menu.text_back)
#define ui_std_window(cb) WM_CreateWindow(0, titleHeight, LCD_WIDTH, imgHeight, WM_CF_SHOW, cb, 0)


extern BUTTON_Handle ui_create_std_button(int x, int y, WM_HWIN hWinParent, char *pFile, const char* text);
extern BUTTON_Handle ui_create_150_80_button(int x, int y, WM_HWIN hWinParent, char *pFile, const char* text);
extern BUTTON_Handle ui_create_100_80_button(int x, int y, WM_HWIN hWinParent, char *pFile, const char* text);
extern BUTTON_Handle ui_create_state_button(int x, int y, WM_HWIN hWinParent, char *pFile);
extern void ui_update_state_button(BUTTON_Handle btn, const char *pFile);

extern BUTTON_Handle ui_create_dialog_button(int x, int y, WM_HWIN hWinParent, const char* text);

extern BUTTON_Handle ui_create_check_button(int x, int y, WM_HWIN hWinParent, uint8_t state);
extern void ui_update_check_button(BUTTON_Handle  btn, uint8_t state);


extern TEXT_Handle ui_create_std_text_f(int x, int y, int w, int h, WM_HWIN hWinParent, int flags, char *text);
extern TEXT_Handle ui_create_std_text(int x, int y, int w, int h, WM_HWIN hWinParent, char *text);
extern TEXT_Handle ui_create_dialog_text(int x, int y, int w, int h, WM_HWIN hWinParent, char *text);
extern void ui_set_text_value(TEXT_Handle handle, char* val);

extern void ui_update_std_button(BUTTON_Handle handle, char* file, char* title);

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

extern uint8_t ui_file_with_preview(char *path, int * withoffset);
extern void ui_gcode_small_preview(char * file_name, int offset, int xpos_pixel,int ypos_pixel);

extern void ui_update_fan_button(BUTTON_Handle button, TEXT_Handle text);

#if defined(__cplusplus)
}
#endif

#endif
