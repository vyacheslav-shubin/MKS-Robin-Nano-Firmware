#include "GUI.h"
#include "BUTTON.h"
#include "draw_ui.h"
#include "ui_tools.h"
#include "lang_common.h"
#include "UI.h"

#ifndef GUI_FLASH
#define GUI_FLASH
#endif

static GUI_HWIN hAdvancedWnd;

static UI_CHECK_PAIR power_off;
static UI_CHECK_PAIR main_ui_type;
static BUTTON_Handle button_next,button_previous,button_back;


static UI_PAGE_NAVIGATOR navigator;

static void btnHandle_rest() {
	power_off.button_check = 0;
	power_off.button_text = 0;
    button_previous = 0;
    button_next = 0;
    button_back = 0;
}

void _inner_draw_Advanced(void);

static void cbAdvancedWin(WM_MESSAGE * pMsg) {
    switch (pMsg->MsgId) {
    	case WM_PAINT:
    		ui_draw_config_lines();
    		break;
    	case WM_TOUCH:
    	case WM_TOUCH_CHILD:
    		break;
    		
    	case WM_NOTIFY_PARENT:
    		if(pMsg->Data.v == WM_NOTIFICATION_RELEASED) {
    			if(pMsg->hWinSrc == navigator.button_back) {
    				last_disp_state = ADVANCED_UI;
    				Clear_Advanced();
    				draw_return_ui();
    			} else if(pMsg->hWinSrc == navigator.button_next) {
                    last_disp_state = ADVANCED_UI;
                    Clear_Advanced();
                    navigator.page++;
                    if (navigator.page>=navigator.page_count)
                    	navigator.page = 0;
                    _inner_draw_Advanced();
                } else if(pMsg->hWinSrc == navigator.button_previous) {
                    last_disp_state = ADVANCED_UI;
                    Clear_Advanced();
                    navigator.page--;
                    if (navigator.page<0)
                    	navigator.page = navigator.page_count - 1;
                    _inner_draw_Advanced();
                } else if(pMsg->hWinSrc == power_off.button_check) {
					if(gCfgItems.print_finish_close_machine_flg==0) {
						gCfgItems.print_finish_close_machine_flg=1;
					} else {
						gCfgItems.print_finish_close_machine_flg=0;
					}
					ui_update_check_button(power_off.button_check, gCfgItems.print_finish_close_machine_flg==1);
					epr_write_data(EPR_AUTO_CLOSE_MACHINE, (uint8_t *)&gCfgItems.print_finish_close_machine_flg,sizeof(gCfgItems.print_finish_close_machine_flg));
    		
    			}  else if(pMsg->hWinSrc == main_ui_type.button_check) {
					if(gCfgItems.display_style==0) {
						gCfgItems.display_style=1;
					} else {
						gCfgItems.display_style=0;
					}
					ui_update_check_button(main_ui_type.button_check, gCfgItems.display_style==1);
					epr_write_data(EPR_SCREEN_DISPLAY_STYLE, (uint8_t *)&gCfgItems.display_style,sizeof(gCfgItems.display_style));
    			}
    		}
    		break;
    	default:
    		WM_DefaultProc(pMsg);
    }
}



void _inner_draw_Advanced() {
    hAdvancedWnd = ui_std_init_window(ADVANCED_UI, cbAdvancedWin);
    ui_make_check_pair(0, hAdvancedWnd, &power_off, lang_str.config_ui.power_off_after_print, gCfgItems.print_finish_close_machine_flg==1);
    ui_make_check_pair(1, hAdvancedWnd, &main_ui_type, lang_str.config_ui.simple_main_ui, gCfgItems.display_style==1);

    ui_make_page_navigator(hAdvancedWnd, &navigator);
}

void draw_Advanced() {
	advanced_config_ui.show();
	return;
	navigator.page_count = 1;
	navigator.page = 0;
	_inner_draw_Advanced();
}


void Clear_Advanced() {
	advanced_config_ui.hide();
	return;
	ui_drop_window(hAdvancedWnd);
}






