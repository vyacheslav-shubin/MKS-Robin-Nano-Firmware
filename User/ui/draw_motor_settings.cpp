#include "gui.h"
#include "button.h"
#include "ui_tools.h"
#include "draw_ui.h"

#ifndef GUI_FLASH
#define GUI_FLASH
#endif

static GUI_HWIN hMotorSettingsWnd;

typedef struct{
	UI_ARROW_PAIR motor_dir;
	UI_ARROW_PAIR motor_steps;
	UI_ARROW_PAIR motor_feed_rate;
	UI_ARROW_PAIR motor_acceleration;
	UI_ARROW_PAIR motor_max_feed_rate;
	UI_ARROW_PAIR motor_jerk;

} UI_ELEMENTS;

UI_PAGE_NAVIGATOR navigator;

static UI_ELEMENTS ui;

void _draw_MotorSettings();

static void cbMotorSettingsWin(WM_MESSAGE * pMsg) {

    struct PressEvt *press_event;
    switch (pMsg->MsgId)
    {
    	case WM_PAINT:
    		ui_draw_config_lines();
    		break;
    	case WM_TOUCH:
    	 	press_event = (struct PressEvt *)pMsg->Data.p;
    		
    		break;
    	case WM_TOUCH_CHILD:
    		press_event = (struct PressEvt *)pMsg->Data.p;

    		break;
    		
    	case WM_NOTIFY_PARENT:
    		if(pMsg->Data.v == WM_NOTIFICATION_RELEASED) {
    			if(pMsg->hWinSrc == navigator.button_back) {
    				last_disp_state = MOTOR_SETTINGS_UI;
    				Clear_MotorSettings();
    				draw_return_ui();
    			} else if(pMsg->hWinSrc == navigator.button_next) {
    				navigator.page++;
    				if (navigator.page>=navigator.page_count)
    					navigator.page=0;
                    last_disp_state = MACHINE_SETTINGS_UI;
                    Clear_MachinePara();
                    _draw_MotorSettings();
                } else if(pMsg->hWinSrc == navigator.button_previous) {
    				navigator.page--;
    				if (navigator.page<0)
    					navigator.page=navigator.page_count-1;
                    last_disp_state = MACHINE_SETTINGS_UI;
                    Clear_MachinePara();
                    _draw_MotorSettings();
                } else if(ui_compare_arrow_pair(pMsg->hWinSrc,ui.motor_max_feed_rate)) {
    				last_disp_state = MOTOR_SETTINGS_UI;
    				Clear_MotorSettings();
    				draw_MaxFeedRate();
    			} else if(ui_compare_arrow_pair(pMsg->hWinSrc,ui.motor_acceleration)) {
    				last_disp_state = MOTOR_SETTINGS_UI;
    				Clear_MotorSettings();
    				draw_Acceleration();
    			} else if(ui_compare_arrow_pair(pMsg->hWinSrc, ui.motor_jerk)) {
    				last_disp_state = MOTOR_SETTINGS_UI;
    				Clear_MotorSettings();                    
                    draw_Jerk();
    			} else if(ui_compare_arrow_pair(pMsg->hWinSrc, ui.motor_steps)) {
    				last_disp_state = MOTOR_SETTINGS_UI;
    				Clear_MotorSettings();
                    draw_Steps();
    			} else if(ui_compare_arrow_pair(pMsg->hWinSrc, ui.motor_dir)) {
    				last_disp_state = MOTOR_SETTINGS_UI;
    				Clear_MotorSettings();
                    draw_MotorDir();
    			}else if(ui_compare_arrow_pair(pMsg->hWinSrc,ui.motor_feed_rate)) {
    				last_disp_state = MOTOR_SETTINGS_UI;
    				Clear_MotorSettings();
                    draw_HomeSpeed();
    			}
    		}
    		break;
    	default:
    		WM_DefaultProc(pMsg);
    }
}

void draw_MotorSettings() {
	navigator.page = 0;
	navigator.page_count = 2;
	_draw_MotorSettings();
}

void _draw_MotorSettings() {

	memset(&ui, 0, sizeof(ui));

    hMotorSettingsWnd = ui_std_init_window(MOTOR_SETTINGS_UI, cbMotorSettingsWin);
    ui_make_page_navigator(hMotorSettingsWnd, &navigator);

    if(navigator.page == 0) {
    	ui_make_arrow_pair(0, hMotorSettingsWnd, &ui.motor_dir, lang_str.motor_direction_settings);
    	ui_make_arrow_pair(1, hMotorSettingsWnd, &ui.motor_steps, lang_str.motor_steps_settings);
    	ui_make_arrow_pair(2, hMotorSettingsWnd, &ui.motor_feed_rate, lang_str.home_speed_settings);
    	ui_make_arrow_pair(3, hMotorSettingsWnd, &ui.motor_acceleration, lang_str.acceleration_settings);
    } else {
    	ui_make_arrow_pair(0, hMotorSettingsWnd, &ui.motor_max_feed_rate, lang_str.maximum_speed_settings);
    	ui_make_arrow_pair(1, hMotorSettingsWnd, &ui.motor_jerk, lang_str.jerk_settings);
        
    }
}


void Clear_MotorSettings() {
	ui_drop_window(hMotorSettingsWnd);
}




