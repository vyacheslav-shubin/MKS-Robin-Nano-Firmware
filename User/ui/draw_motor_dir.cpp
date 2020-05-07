#include "gui.h"
#include "button.h"
#include "draw_ui.h"
#include "ui_tools.h"
#include "stepper.h"

#ifndef GUI_FLASH
#define GUI_FLASH
#endif

static GUI_HWIN hMotorDirWnd;

typedef struct {
	UI_CHECK_PAIR x;
	UI_CHECK_PAIR y;
	UI_CHECK_PAIR z;
	UI_CHECK_PAIR e0;
	UI_CHECK_PAIR e1;
} UI_BUTTONS;

static UI_BUTTONS ui;
static UI_PAGE_NAVIGATOR navigator;


static void cbMotorDirWin(WM_MESSAGE * pMsg) {

    struct PressEvt *press_event;
    switch (pMsg->MsgId)
    {
    	case WM_PAINT:
    		ui_draw_config_half_lines();
    		break;

    	case WM_TOUCH:
    	case WM_TOUCH_CHILD:
    		break;
    		
    	case WM_NOTIFY_PARENT:
    		if(pMsg->Data.v == WM_NOTIFICATION_RELEASED) {
    			if(pMsg->hWinSrc == navigator.button_back) {
    				last_disp_state = MOTORDIR_UI;
    				Clear_MotorDir();
    				draw_return_ui();
    			} else if(pMsg->hWinSrc == ui.x.button_check) {
					if(mksCfg.invert_x_dir==1) {
						mksCfg.invert_x_dir=0;
					} else {
						mksCfg.invert_x_dir=1;
					}
					ui_update_check_button(ui.x.button_check, mksCfg.invert_x_dir==1);
					if((stepper.last_direction_bits & (1<<X_AXIS)) == 0)
						stepper.last_direction_bits = stepper.last_direction_bits |(1<<X_AXIS);
					else
						stepper.last_direction_bits = stepper.last_direction_bits & (~(1<<X_AXIS));
    				epr_write_data(EPR_INVERT_X_DIR, &mksCfg.invert_x_dir,1);
    			}
    			else if(pMsg->hWinSrc == ui.y.button_check) {
					if(mksCfg.invert_y_dir==1) {
						mksCfg.invert_y_dir=0;
					} else {
						mksCfg.invert_y_dir=1;
					}
					ui_update_check_button(ui.y.button_check, mksCfg.invert_y_dir==1);
					if((stepper.last_direction_bits & (1<<Y_AXIS)) == 0)
						stepper.last_direction_bits = stepper.last_direction_bits |(1<<Y_AXIS);
					else
						stepper.last_direction_bits = stepper.last_direction_bits & (~(1<<Y_AXIS));
    				epr_write_data(EPR_INVERT_Y_DIR, &mksCfg.invert_y_dir,1);
    				
    			} else if(pMsg->hWinSrc == ui.z.button_check) {
					if(mksCfg.invert_z_dir==1) {
						mksCfg.invert_z_dir=0;
					} else {
						mksCfg.invert_z_dir=1;
					}
					ui_update_check_button(ui.z.button_check, mksCfg.invert_z_dir==1);
					if((stepper.last_direction_bits & (1<<Z_AXIS)) == 0)
						stepper.last_direction_bits = stepper.last_direction_bits | (1<<Z_AXIS);
					else
						stepper.last_direction_bits = stepper.last_direction_bits & (~(1<<Z_AXIS));
    				epr_write_data(EPR_INVERT_Z_DIR, &mksCfg.invert_z_dir,1);
    			} else if(pMsg->hWinSrc == ui.e0.button_check) {
					if(mksCfg.invert_e0_dir==1) {
						mksCfg.invert_e0_dir=0;
					} else
					{
						mksCfg.invert_e0_dir=1;
					}
					ui_update_check_button(ui.e0.button_check, mksCfg.invert_e0_dir==1);
					if((stepper.last_direction_bits & (1<<E_AXIS)) == 0)
						stepper.last_direction_bits = stepper.last_direction_bits |(1<<E_AXIS);
					else
						stepper.last_direction_bits = stepper.last_direction_bits & (~(1<<E_AXIS));
    				epr_write_data(EPR_INVERT_E0_DIR, &mksCfg.invert_e0_dir,1);
    			} else if(pMsg->hWinSrc == ui.e1.button_check) {
					if(mksCfg.invert_e1_dir==1) {
						mksCfg.invert_e1_dir=0;
					} else {
						mksCfg.invert_e1_dir=1;
					}
					ui_update_check_button(ui.e1.button_check, mksCfg.invert_e1_dir==1);
					if((stepper.last_direction_bits & (1<<E_AXIS)) == 0)
						stepper.last_direction_bits = stepper.last_direction_bits |(1<<E_AXIS);
					else
						stepper.last_direction_bits = stepper.last_direction_bits & (~(1<<E_AXIS));
    				epr_write_data(EPR_INVERT_E1_DIR, &mksCfg.invert_e1_dir,1);
    			}  
    		}
    		break;
    		
    	default:
    		WM_DefaultProc(pMsg);
   	}
}


void draw_MotorDir() {
    hMotorDirWnd = ui_std_init_window(MOTORDIR_UI, cbMotorDirWin);
    navigator.page = 0;
    navigator.page_count = 1;
    ui_make_page_navigator(hMotorDirWnd, &navigator);
    
    ui_make_half_row_check_pair(0, 0, hMotorDirWnd, &ui.x, "X:", mksCfg.invert_x_dir);
    ui_make_half_row_check_pair(0, 1, hMotorDirWnd, &ui.y, "Y:", mksCfg.invert_y_dir);
    ui_make_half_row_check_pair(0, 2, hMotorDirWnd, &ui.z, "Z:", mksCfg.invert_z_dir);
    ui_make_half_row_check_pair(1, 0, hMotorDirWnd, &ui.e0, "E0:", mksCfg.invert_e0_dir);
    ui_make_half_row_check_pair(1, 1, hMotorDirWnd, &ui.e1, "E1:", mksCfg.invert_e1_dir);
}


void Clear_MotorDir() {
	ui_drop_window(hMotorDirWnd);
}







