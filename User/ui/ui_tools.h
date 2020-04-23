#ifndef _UI_TOOLS
#define _UI_TOOLS

#include "draw_ui.h"
#include "GUI.h"
#include "BUTTON.h"
#include "TEXT.h"

#if defined(__cplusplus)
extern "C" {
#endif

extern uint8_t	ui_suicide_enabled;

extern void ui_push_disp_stack(DISP_STATE ui_id);
extern void ui_reset_disp_stack(DISP_STATE ui_id);
extern void ui_pop_disp_stack(void);
extern void ui_clear_screen(void);
extern void ui_drop_window(WM_HWIN wnd);
extern void ui_initialize_screen_gui(void);

//with placeholder
#define ui_std_button(ph_x, ph_y, hwnd, img, title) ui_create_std_button(INTERVAL_H + (BTN_X_PIXEL+INTERVAL_H)*ph_x,  INTERVAL_V + (BTN_Y_PIXEL + INTERVAL_V) * ph_y, hwnd, img, title)
#define ui_std_button_return(hwnd) ui_std_button(3, 1, hwnd, "bmp_return.bin", common_menu.text_back)
#define ui_std_window(cb) WM_CreateWindow(0, titleHeight, LCD_WIDTH, imgHeight, WM_CF_SHOW, cb, 0)


extern BUTTON_Handle ui_create_std_button(int x, int y, WM_HWIN hWinParent, char *pFile, const char* text);
extern BUTTON_Handle ui_create_150_80_button(int x, int y, WM_HWIN hWinParent, char *pFile, const char* text);
extern BUTTON_Handle ui_create_state_button(int x, int y, WM_HWIN hWinParent, char *pFile);
extern BUTTON_Handle ui_create_state_button_id(int x, int y, WM_HWIN hWinParent, char *pFile, uint32_t id);

extern TEXT_Handle ui_create_std_text(int x, int y, int w, int h, WM_HWIN hWinParent, char *text);

extern void ui_set_text_value(TEXT_Handle handle, char* val);



extern uint8_t ui_timing_flags;
#define F_UI_TIMING_HALF_SEC		1<<0
#define F_UI_TIMING_SEC				1<<1

#define is_ui_timing(FLAG) (ui_timing_flags & FLAG)
#define ui_timing_set(FLAG) (ui_timing_flags |= FLAG)
#define ui_timing_clear(FLAG) (ui_timing_flags &= (~FLAG))

extern void ui_timings(void);



#if defined(__cplusplus)
}
#endif

#endif
