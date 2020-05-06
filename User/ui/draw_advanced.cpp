#include "GUI.h"
#include "BUTTON.h"
#include "draw_ui.h"
#include "ui_tools.h"

#ifndef GUI_FLASH
#define GUI_FLASH
#endif

static GUI_HWIN hAdvancedWnd;

static BUTTON_Handle PowerOff_text,PowerOff_Sel;
static BUTTON_Handle button_next,button_previous,button_back;


static UI_PAGE_NAVIGATOR navigator;

static void btnHandle_rest() {
    PowerOff_text = 0;
    PowerOff_Sel = 0;
    button_previous = 0;
    button_next = 0;
    button_back = 0;
}

void _inner_draw_Advanced(void);

static void cbAdvancedWin(WM_MESSAGE * pMsg) {
    switch (pMsg->MsgId) {
    	case WM_PAINT:
    		GUI_SetColor(0xff5449);
    		GUI_FillRect(10, 50, 470, 50);
    		GUI_FillRect(10, 100, 470, 100);
    		GUI_FillRect(10, 150, 470, 150);
    		GUI_FillRect(10, 200, 470, 200);
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
                } else if(pMsg->hWinSrc == PowerOff_Sel) {
					if(gCfgItems.print_finish_close_machine_flg==1) {
						gCfgItems.print_finish_close_machine_flg=0;
					} else {
						gCfgItems.print_finish_close_machine_flg=1;
					}
					ui_update_check_button(PowerOff_Sel, gCfgItems.print_finish_close_machine_flg==1);
					epr_write_data(EPR_AUTO_CLOSE_MACHINE, (uint8_t *)&gCfgItems.print_finish_close_machine_flg,1);
    		
    			}
    		}
    		break;
    	default:
    		WM_DefaultProc(pMsg);
    }
}

#define ROW(idx) (10+50*idx)
#define X_TEXT 10
#define X_RADIO 370
#define X_ARROW 460


void _inner_draw_Advanced() {
    hAdvancedWnd = ui_std_init_window(ADVANCED_UI, cbAdvancedWin);
	PowerOff_text = BUTTON_CreateEx(X_TEXT,ROW(0),240,40,hAdvancedWnd, BUTTON_CF_SHOW, 0, alloc_win_id());
    BUTTON_SetText(PowerOff_text, machine_menu.PwrOffAfterPrint);

	PowerOff_Sel = ui_create_check_button(X_RADIO, ROW(0), hAdvancedWnd, gCfgItems.print_finish_close_machine_flg==1);
    BUTTON_SetTextAlign(PowerOff_text,GUI_TA_LEFT|GUI_TA_VCENTER );
    ui_make_page_navigator(hAdvancedWnd, &navigator);
}

void draw_Advanced() {
	navigator.page_count = 1;
	navigator.page = 0;
	_inner_draw_Advanced();
}


void Clear_Advanced() {
	ui_drop_window(hAdvancedWnd);
}






