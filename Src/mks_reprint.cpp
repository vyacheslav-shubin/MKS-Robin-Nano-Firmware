#include "at24cxx.h"
#include "mks_reprint.h"
#include "mks_cfg.h"
#include "UI.h"
#include "fatfs.h"
#include "usb_host.h"
#include "ili9320.h"
#include "draw_pause_ui.h"
extern "C" u16 DeviceCode;
extern uint8_t pause_resum;
extern uint32_t logo_tick1,logo_tick2;

uint8_t continue_print_error_flg = 0;

char *mks_pft_name = "mks_pft.sys"; 

void mks_ReadFromEpr_pwroff();
void mks_WriteToEpr_pwroff();

void mks_G28(char *g_command);
void mkstft_ui_load()
{
	//volatile uint32_t  valid_flag;
	//epr_read_data(EPR_INF_VALID_FLAG,(uint8_t *)&valid_flag,sizeof(valid_flag)); 
	//if(valid_flag == BAK_INF_VALID_FLAG)
	{
		epr_read_data(EPR_SCREEN_BKCOLOR,(uint8_t *)&gCfgItems.background_color,sizeof(gCfgItems.background_color)); 
		epr_read_data(EPR_TITIL_COLOR,(uint8_t *)&gCfgItems.title_color,sizeof(gCfgItems.title_color)); 
		//epr_read_data(EPR_STATE_BKCOLOR,(uint8_t *)&gCfgItems.state_background_color,sizeof(gCfgItems.state_background_color)); 
		epr_read_data(EPR_STATE_TEXTCOLOR,(uint8_t *)&gCfgItems.state_text_color,sizeof(gCfgItems.state_text_color)); 
		epr_read_data(EPR_FILENAME_BKCOLOR,(uint8_t *)&gCfgItems.filename_background_color,sizeof(gCfgItems.filename_background_color)); 
		epr_read_data(EPR_FILENAME_TEXTCOLOR,(uint8_t *)&gCfgItems.filename_color,sizeof(gCfgItems.filename_color)); 
		epr_read_data(EPR_BTN_BKCOLOR,(uint8_t *)&gCfgItems.btn_color,sizeof(gCfgItems.btn_color)); 
		epr_read_data(EPR_BTN_TEXTCOLOR,(uint8_t *)&gCfgItems.btn_textcolor,sizeof(gCfgItems.btn_textcolor)); 
		epr_read_data(EPR_STATE_BTN_BKCOLOR,(uint8_t *)&gCfgItems.btn_state_color,sizeof(gCfgItems.btn_state_color)); 
		epr_read_data(EPR_STATE_BTN_TEXTCOLOR,(uint8_t *)&gCfgItems.btn_state_textcolor,sizeof(gCfgItems.btn_state_textcolor)); 
		epr_read_data(EPR_BACK_BTN_BKCOLOR,(uint8_t *)&gCfgItems.back_btn_color,sizeof(gCfgItems.back_btn_color)); 
		epr_read_data(EPR_BACK_BTN_TEXTCOLOR,(uint8_t *)&gCfgItems.back_btn_textcolor,sizeof(gCfgItems.back_btn_textcolor)); 
		epr_read_data(EPR_SEL_BTN_BKCOLOR,(uint8_t *)&gCfgItems.btn_state_sel_color,sizeof(gCfgItems.btn_state_sel_color)); 
		epr_read_data(EPR_SEL_BTN_TEXTCOLOR,(uint8_t *)&gCfgItems.btn_state_sel_textcolor,sizeof(gCfgItems.btn_state_sel_textcolor)); 
		epr_read_data(EPR_DIALOG_BTN_BKCOLOR,(uint8_t *)&gCfgItems.dialog_btn_color,sizeof(gCfgItems.dialog_btn_color)); 
		epr_read_data(EPR_DIALOG_BTN_TEXTCOLOR,(uint8_t *)&gCfgItems.dialog_btn_textcolor,sizeof(gCfgItems.dialog_btn_textcolor)); 

		//epr_read_data(EPR_FILE_SYS,(uint8_t *)&gCfgItems.fileSysType,sizeof(gCfgItems.fileSysType)); 
		epr_read_data(EPR_MUTIL_LANGUAGE_FLG,(uint8_t *)&gCfgItems.multiple_language,sizeof(gCfgItems.multiple_language)); 
		epr_read_data(EPR_LANGUAGE,(uint8_t *)&gCfgItems.language,sizeof(gCfgItems.language)); 
		epr_read_data(EPR_MORE_ITEM_CNT,(uint8_t *)&gCfgItems.MoreItem_pic_cnt,sizeof(gCfgItems.MoreItem_pic_cnt)); 

		epr_read_data(EPR_FILAMENT_LOAD_LENGTH,(uint8_t *)&gCfgItems.filamentchange_load_length,sizeof(gCfgItems.filamentchange_load_length)); 
		epr_read_data(EPR_FILAMENT_LOAD_SPEED,(uint8_t *)&gCfgItems.filamentchange_load_speed,sizeof(gCfgItems.filamentchange_load_speed)); 
		epr_read_data(EPR_FILAMENT_LOAD_LIMIT_TEMPER,(uint8_t *)&gCfgItems.filament_load_limit_temper,sizeof(gCfgItems.filament_load_limit_temper)); 
		epr_read_data(EPR_FILAMENT_UNLOAD_LENGTH,(uint8_t *)&gCfgItems.filamentchange_unload_length,sizeof(gCfgItems.filamentchange_unload_length)); 
		epr_read_data(EPR_FILAMENT_UNLOAD_SPEED,(uint8_t *)&gCfgItems.filamentchange_unload_speed,sizeof(gCfgItems.filamentchange_unload_speed)); 
		epr_read_data(EPR_FILAMENT_UNLOAD_LIMIT_TEMPER,(uint8_t *)&gCfgItems.filament_unload_limit_temper,sizeof(gCfgItems.filament_unload_limit_temper)); 	
		epr_read_data(EPR_SETMENU_FUNC1_DISPLAY_FLG,(uint8_t *)&gCfgItems.func_btn1_display_flag,sizeof(gCfgItems.func_btn1_display_flag));	

		epr_read_data(EPR_SCREEN_DISPLAY_STYLE,(uint8_t *)&gCfgItems.display_style,sizeof(gCfgItems.display_style));	
		//epr_read_data(EPR_PWROFF_SAVE_MODE,(uint8_t *)&gCfgItems.pwroff_save_mode,sizeof(gCfgItems.pwroff_save_mode));	

		epr_read_data(EPR_INSERT_DET_MODULE_TYPE,(uint8_t *)&gCfgItems.insert_det_module,sizeof(gCfgItems.insert_det_module)); 
		epr_read_data(EPR_HAS_UPS,(uint8_t *)&gCfgItems.have_ups,sizeof(gCfgItems.have_ups));	
		epr_read_data(EPR_FILAMENT_DET0_LEVEL,(uint8_t *)&gCfgItems.filament_det0_level_flg,sizeof(gCfgItems.filament_det0_level_flg));	
		epr_read_data(EPR_FILAMENT_DET1_LEVEL,(uint8_t *)&gCfgItems.filament_det1_level_flg,sizeof(gCfgItems.filament_det1_level_flg));	
		epr_read_data(EPR_MASK_DET_FUNCTION,(uint8_t *)&gCfgItems.mask_det_Function,sizeof(gCfgItems.mask_det_Function));	

		epr_read_data(EPR_LEVELING_MODE,(uint8_t *)&gCfgItems.leveling_mode,sizeof(gCfgItems.leveling_mode));	
		epr_read_data(EPR_LEVELING_POINT_CNT,(uint8_t *)&gCfgItems.leveling_point_number,sizeof(gCfgItems.leveling_point_number));	

		epr_read_data(EPR_LEVELING_POINT1_X,(uint8_t *)&gCfgItems.leveling_points[0].x,sizeof(gCfgItems.leveling_points[0].x));
		epr_read_data(EPR_LEVELING_POINT1_Y,(uint8_t *)&gCfgItems.leveling_points[0].y,sizeof(gCfgItems.leveling_points[0].y));
		epr_read_data(EPR_LEVELING_POINT2_X,(uint8_t *)&gCfgItems.leveling_points[1].x,sizeof(gCfgItems.leveling_points[0].x));
		epr_read_data(EPR_LEVELING_POINT2_Y,(uint8_t *)&gCfgItems.leveling_points[1].y,sizeof(gCfgItems.leveling_points[0].y));
		epr_read_data(EPR_LEVELING_POINT3_X,(uint8_t *)&gCfgItems.leveling_points[2].x,sizeof(gCfgItems.leveling_points[0].x));
		epr_read_data(EPR_LEVELING_POINT3_Y,(uint8_t *)&gCfgItems.leveling_points[2].y,sizeof(gCfgItems.leveling_points[0].y));
		epr_read_data(EPR_LEVELING_POINT4_X,(uint8_t *)&gCfgItems.leveling_points[3].x,sizeof(gCfgItems.leveling_points[0].x));
		epr_read_data(EPR_LEVELING_POINT4_Y,(uint8_t *)&gCfgItems.leveling_points[3].y,sizeof(gCfgItems.leveling_points[0].y));
		epr_read_data(EPR_LEVELING_POINT5_X,(uint8_t *)&gCfgItems.leveling_points[4].x,sizeof(gCfgItems.leveling_points[0].x));
		epr_read_data(EPR_LEVELING_POINT5_Y,(uint8_t *)&gCfgItems.leveling_points[4].y,sizeof(gCfgItems.leveling_points[0].y));

		epr_read_data(EPR_AUTO_CLOSE_MACHINE,(uint8_t *)&gCfgItems.print_finish_close_machine_flg,sizeof(gCfgItems.print_finish_close_machine_flg));
		ui_print_process.suicide.enabled = gCfgItems.print_finish_close_machine_flg;
		epr_read_data(EPR_ENABLE_CLOUD,(uint8_t *)&gCfgItems.cloud_enable,sizeof(gCfgItems.cloud_enable));

		//sean 19.8.16
		epr_read_data(EPR_ENABLE_WIFI_SCAN,(uint8_t *)&gCfgItems.wifi_scan,sizeof(gCfgItems.wifi_scan));
		
		epr_read_data(EPR_DISABLE_WIFI,(uint8_t *)&gCfgItems.wifi_btn_state,sizeof(gCfgItems.wifi_btn_state));
		epr_read_data(EPR_PAUSE_UNLOAD_LEN,(uint8_t *)&gCfgItems.pause_unload_len,sizeof(gCfgItems.pause_unload_len ));
		epr_read_data(EPR_RESUME_LOAD_LEN,(uint8_t *)&gCfgItems.resume_load_len,sizeof(gCfgItems.resume_load_len ));
		epr_read_data(EPR_RESUME_SPEED,(uint8_t *)&gCfgItems.resume_speed,sizeof(gCfgItems.resume_speed));
		epr_read_data(EPR_SINGLE_NOZZLE,(uint8_t *)&gCfgItems.singleNozzle,sizeof(gCfgItems.singleNozzle));
		epr_read_data(EPR_STANDBY_MODE,(uint8_t *)&gCfgItems.standby_mode,sizeof(gCfgItems.standby_mode));
		epr_read_data(EPR_STANDBY_TIME,(uint8_t *)&gCfgItems.standby_time,sizeof(gCfgItems.standby_time));
		epr_read_data(EPR_PULSE_DELAY_TIME,(uint8_t *)&gCfgItems.pulseDelay,sizeof(gCfgItems.pulseDelay));
   		epr_read_data(EPR_PRINT_FINESH_COUNT,(uint8_t *)&gCfgItems.print_finish_count,sizeof(gCfgItems.print_finish_count));
	}
	/*
	else
	
	*/
	gCfgItems.preview_bk_color = rgb888_2_rgb565(gCfgItems.background_color);
}

void mkstft_ui_set_epr()
{
	volatile uint32_t  valid_flag;
	epr_read_data(EPR_INF_VALID_FLAG,(uint8_t *)&valid_flag,sizeof(valid_flag)); 
	if(valid_flag != BAK_INF_VALID_FLAG)
	{
		epr_write_data(EPR_SCREEN_BKCOLOR,(uint8_t *)&gCfgItems.background_color,sizeof(gCfgItems.background_color)); 
		epr_write_data(EPR_TITIL_COLOR,(uint8_t *)&gCfgItems.title_color,sizeof(gCfgItems.title_color)); 
		//epr_write_data(EPR_STATE_BKCOLOR,(uint8_t *)&gCfgItems.state_background_color,sizeof(gCfgItems.state_background_color)); 
		epr_write_data(EPR_STATE_TEXTCOLOR,(uint8_t *)&gCfgItems.state_text_color,sizeof(gCfgItems.state_text_color)); 
		epr_write_data(EPR_FILENAME_BKCOLOR,(uint8_t *)&gCfgItems.filename_background_color,sizeof(gCfgItems.filename_background_color)); 
		epr_write_data(EPR_FILENAME_TEXTCOLOR,(uint8_t *)&gCfgItems.filename_color,sizeof(gCfgItems.filename_color)); 
		epr_write_data(EPR_BTN_BKCOLOR,(uint8_t *)&gCfgItems.btn_color,sizeof(gCfgItems.btn_color)); 
		epr_write_data(EPR_BTN_TEXTCOLOR,(uint8_t *)&gCfgItems.btn_textcolor,sizeof(gCfgItems.btn_textcolor)); 
		epr_write_data(EPR_STATE_BTN_BKCOLOR,(uint8_t *)&gCfgItems.btn_state_color,sizeof(gCfgItems.btn_state_color)); 
		epr_write_data(EPR_STATE_BTN_TEXTCOLOR,(uint8_t *)&gCfgItems.btn_state_textcolor,sizeof(gCfgItems.btn_state_textcolor)); 
		epr_write_data(EPR_BACK_BTN_BKCOLOR,(uint8_t *)&gCfgItems.back_btn_color,sizeof(gCfgItems.back_btn_color)); 
		epr_write_data(EPR_BACK_BTN_TEXTCOLOR,(uint8_t *)&gCfgItems.back_btn_textcolor,sizeof(gCfgItems.back_btn_textcolor)); 
		epr_write_data(EPR_SEL_BTN_BKCOLOR,(uint8_t *)&gCfgItems.btn_state_sel_color,sizeof(gCfgItems.btn_state_sel_color)); 
		epr_write_data(EPR_SEL_BTN_TEXTCOLOR,(uint8_t *)&gCfgItems.btn_state_sel_textcolor,sizeof(gCfgItems.btn_state_sel_textcolor)); 
		epr_write_data(EPR_DIALOG_BTN_BKCOLOR,(uint8_t *)&gCfgItems.dialog_btn_color,sizeof(gCfgItems.dialog_btn_color)); 
		epr_write_data(EPR_DIALOG_BTN_TEXTCOLOR,(uint8_t *)&gCfgItems.dialog_btn_textcolor,sizeof(gCfgItems.dialog_btn_textcolor)); 

		//epr_write_data(EPR_FILE_SYS,(uint8_t *)&gCfgItems.fileSysType,sizeof(gCfgItems.fileSysType)); 
		epr_write_data(EPR_MUTIL_LANGUAGE_FLG,(uint8_t *)&gCfgItems.multiple_language,sizeof(gCfgItems.multiple_language)); 
		epr_write_data(EPR_LANGUAGE,(uint8_t *)&gCfgItems.language,sizeof(gCfgItems.language)); 
		epr_write_data(EPR_MORE_ITEM_CNT,(uint8_t *)&gCfgItems.MoreItem_pic_cnt,sizeof(gCfgItems.MoreItem_pic_cnt)); 

		epr_write_data(EPR_FILAMENT_LOAD_LENGTH,(uint8_t *)&gCfgItems.filamentchange_load_length,sizeof(gCfgItems.filamentchange_load_length)); 
		epr_write_data(EPR_FILAMENT_LOAD_SPEED,(uint8_t *)&gCfgItems.filamentchange_load_speed,sizeof(gCfgItems.filamentchange_load_speed)); 
		epr_write_data(EPR_FILAMENT_LOAD_LIMIT_TEMPER,(uint8_t *)&gCfgItems.filament_load_limit_temper,sizeof(gCfgItems.filament_load_limit_temper)); 
		epr_write_data(EPR_FILAMENT_UNLOAD_LENGTH,(uint8_t *)&gCfgItems.filamentchange_unload_length,sizeof(gCfgItems.filamentchange_unload_length)); 
		epr_write_data(EPR_FILAMENT_UNLOAD_SPEED,(uint8_t *)&gCfgItems.filamentchange_unload_speed,sizeof(gCfgItems.filamentchange_unload_speed)); 
		epr_write_data(EPR_FILAMENT_UNLOAD_LIMIT_TEMPER,(uint8_t *)&gCfgItems.filament_unload_limit_temper,sizeof(gCfgItems.filament_unload_limit_temper)); 	
		epr_write_data(EPR_SETMENU_FUNC1_DISPLAY_FLG,(uint8_t *)&gCfgItems.func_btn1_display_flag,sizeof(gCfgItems.func_btn1_display_flag));	

		epr_write_data(EPR_SCREEN_DISPLAY_STYLE,(uint8_t *)&gCfgItems.display_style,sizeof(gCfgItems.display_style));	
		//epr_write_data(EPR_PWROFF_SAVE_MODE,(uint8_t *)&gCfgItems.pwroff_save_mode,sizeof(gCfgItems.pwroff_save_mode));	

		epr_write_data(EPR_INSERT_DET_MODULE_TYPE,(uint8_t *)&gCfgItems.insert_det_module,sizeof(gCfgItems.insert_det_module));	
		epr_write_data(EPR_HAS_UPS,(uint8_t *)&gCfgItems.have_ups,sizeof(gCfgItems.have_ups));	
		epr_write_data(EPR_FILAMENT_DET0_LEVEL,(uint8_t *)&gCfgItems.filament_det0_level_flg,sizeof(gCfgItems.filament_det0_level_flg));	
		epr_write_data(EPR_FILAMENT_DET1_LEVEL,(uint8_t *)&gCfgItems.filament_det1_level_flg,sizeof(gCfgItems.filament_det1_level_flg));	
		epr_write_data(EPR_MASK_DET_FUNCTION,(uint8_t *)&gCfgItems.mask_det_Function,sizeof(gCfgItems.mask_det_Function));	

		epr_write_data(EPR_LEVELING_MODE,(uint8_t *)&gCfgItems.leveling_mode,sizeof(gCfgItems.leveling_mode));	
		epr_write_data(EPR_LEVELING_POINT_CNT,(uint8_t *)&gCfgItems.leveling_point_number,sizeof(gCfgItems.leveling_point_number));	

		epr_write_data(EPR_LEVELING_POINT1_X,(uint8_t *)&gCfgItems.leveling_points[0].x,sizeof(gCfgItems.leveling_points[0].x));
		epr_write_data(EPR_LEVELING_POINT1_Y,(uint8_t *)&gCfgItems.leveling_points[0].y,sizeof(gCfgItems.leveling_points[0].y));
		epr_write_data(EPR_LEVELING_POINT2_X,(uint8_t *)&gCfgItems.leveling_points[1].x,sizeof(gCfgItems.leveling_points[0].x));
		epr_write_data(EPR_LEVELING_POINT2_Y,(uint8_t *)&gCfgItems.leveling_points[1].y,sizeof(gCfgItems.leveling_points[0].y));
		epr_write_data(EPR_LEVELING_POINT3_X,(uint8_t *)&gCfgItems.leveling_points[2].x,sizeof(gCfgItems.leveling_points[0].x));
		epr_write_data(EPR_LEVELING_POINT3_Y,(uint8_t *)&gCfgItems.leveling_points[2].y,sizeof(gCfgItems.leveling_points[0].y));
		epr_write_data(EPR_LEVELING_POINT4_X,(uint8_t *)&gCfgItems.leveling_points[3].x,sizeof(gCfgItems.leveling_points[0].x));
		epr_write_data(EPR_LEVELING_POINT4_Y,(uint8_t *)&gCfgItems.leveling_points[3].y,sizeof(gCfgItems.leveling_points[0].y));
		epr_write_data(EPR_LEVELING_POINT5_X,(uint8_t *)&gCfgItems.leveling_points[4].x,sizeof(gCfgItems.leveling_points[0].x));
		epr_write_data(EPR_LEVELING_POINT5_Y,(uint8_t *)&gCfgItems.leveling_points[4].y,sizeof(gCfgItems.leveling_points[0].y));

		epr_write_data(EPR_AUTO_CLOSE_MACHINE,(uint8_t *)&gCfgItems.print_finish_close_machine_flg,sizeof(gCfgItems.print_finish_close_machine_flg));
		epr_write_data(EPR_ENABLE_CLOUD,(uint8_t *)&gCfgItems.cloud_enable,sizeof(gCfgItems.cloud_enable));
		//sean 19.8.16
		epr_write_data(EPR_ENABLE_WIFI_SCAN,(uint8_t *)&gCfgItems.wifi_scan,sizeof(gCfgItems.wifi_scan));
		epr_write_data(EPR_DISABLE_WIFI,(uint8_t *)&gCfgItems.wifi_btn_state,sizeof(gCfgItems.wifi_btn_state));
		epr_write_data(EPR_PAUSE_UNLOAD_LEN,(uint8_t *)&gCfgItems.pause_unload_len,sizeof(gCfgItems.pause_unload_len ));
		epr_write_data(EPR_RESUME_LOAD_LEN,(uint8_t *)&gCfgItems.resume_load_len,sizeof(gCfgItems.resume_load_len ));
		epr_write_data(EPR_RESUME_SPEED,(uint8_t *)&gCfgItems.resume_speed,sizeof(gCfgItems.resume_speed));
		epr_write_data(EPR_SINGLE_NOZZLE,(uint8_t *)&gCfgItems.singleNozzle,sizeof(gCfgItems.singleNozzle));
		epr_write_data(EPR_STANDBY_MODE,(uint8_t *)&gCfgItems.standby_mode,sizeof(gCfgItems.standby_mode));
		epr_write_data(EPR_STANDBY_TIME,(uint8_t *)&gCfgItems.standby_time,sizeof(gCfgItems.standby_time));
		epr_write_data(EPR_PULSE_DELAY_TIME,(uint8_t *)&gCfgItems.pulseDelay,sizeof(gCfgItems.pulseDelay));
		epr_write_data(EPR_PRINT_FINESH_COUNT,(uint8_t *)&gCfgItems.print_finish_count,sizeof(gCfgItems.print_finish_count));
		
		valid_flag = BAK_INF_VALID_FLAG;
		epr_write_data(EPR_INF_VALID_FLAG,(uint8_t *)&valid_flag,sizeof(valid_flag)); 

	}		
		
}

void mkstft_ui_init() {
	gCfgItems.value_bk_color = 0xFFFFFF;
	gCfgItems.value_text_color = 0x000000;

	gCfgItems.default_bk_color = 0xff5449;
	gCfgItems.default_text_color = 0xFFFFFF;
		
	gCfgItems.background_color = 0x494949;//
	gCfgItems.title_color = 0xFFFFFF;//
	gCfgItems.state_background_color = 0x1A1A1A;//
	gCfgItems.state_text_color = 0xFFFFFF;//
	gCfgItems.btn_color = 0X1A1A1A;
	gCfgItems.btn_textcolor = 0xffffff;
	gCfgItems.btn_state_color = 0X1A1A1A;//0x505050;
	gCfgItems.btn_state_textcolor = 0xffffff;
	gCfgItems.btn_state_sel_color = 0X1A1A1A;//0x9dfcff;
	gCfgItems.btn_state_sel_textcolor = 0xFFFFFF;
	gCfgItems.back_btn_color = 0xff5449;
	gCfgItems.back_btn_textcolor = 0xFFFFFF;
	gCfgItems.printing_bar_color_left= 0x00ff00;
	gCfgItems.printing_bar_color_right= 0xAAAAAA ;
	gCfgItems.printing_bar_text_color_left = 0x000000;
	gCfgItems.printing_bar_text_color_right = 0x000000;
	gCfgItems.dialog_btn_color = 0xff0000;
	gCfgItems.dialog_btn_textcolor = 0xffffff;
	gCfgItems.printfile_color = 0x000000;
	gCfgItems.printfile_textcolor = 0xffffff;

	gCfgItems.multiple_language = 1;
	gCfgItems.language = 3;

	gCfgItems.fileSysType = FILE_SYS_SD;
	gCfgItems.stepHeat = 10;
	gCfgItems.curSprayerChoose = 0;
	//gCfgItems.move_dist = 1;
	gCfgItems.moveSpeed = 3000;
	gCfgItems.extruSpeed = 1;
	gCfgItems.extruStep = 5;
	gCfgItems.stepPrintSpeed = 5;
	gCfgItems.printSpeed = 100;

	gCfgItems.printExtSpeed0 = 100;

	gCfgItems.leveling_mode = 0;
	gCfgItems.leveling_point_number = 4;

	set_xy_point(gCfgItems.leveling_points[0],50,50);
	set_xy_point(gCfgItems.leveling_points[1],180,50);
	set_xy_point(gCfgItems.leveling_points[2],180,180);
	set_xy_point(gCfgItems.leveling_points[3],50,180);
	set_xy_point(gCfgItems.leveling_points[4],115,115);

	gCfgItems.leveling_xy_speed = 3000;
	gCfgItems.leveling_z_speed = 1500;

	gCfgItems.insert_det_module = 0;
	gCfgItems.have_ups = 0;
	gCfgItems.filament_det0_level_flg=0;
	gCfgItems.filament_det1_level_flg=0;
	gCfgItems.mask_det_Function = 0;

	gCfgItems.print_finish_close_machine_flg = 0;
	ui_print_process.suicide.enabled = gCfgItems.print_finish_close_machine_flg;

	strcpy((char *)gCfgItems.z_display_pos,(char *)"0.000");
	gCfgItems.custom_bed_flag = 1;

	gCfgItems.filament_load_limit_temper = 200;
	gCfgItems.filamentchange_load_speed = 1000;
	gCfgItems.filamentchange_load_length = 100;
	gCfgItems.filament_unload_limit_temper = 200;
	gCfgItems.filamentchange_unload_speed = 1000;
	gCfgItems.filamentchange_unload_length = 100;

	gCfgItems.MoreItem_pic_cnt = 0;
	gCfgItems.func_btn1_display_flag = 1;

	/*wifi_cfg*/
	gCfgItems.wifi_mode_sel = 1;//1ΪAPģʽ������Ϊclientģʽ��Ĭ��ΪAPģʽ��
	memset(&wifiPara, 0, sizeof(wifiPara));
	memset(&ipPara, 0, sizeof(ipPara));
	strcpy(wifiPara.ap_name,WIFI_AP_NAME);
	strcpy(wifiPara.keyCode,WIFI_KEY_CODE);
	//clientģʽ��ʼֵ
	strcpy(ipPara.ip_addr,IP_ADDR);
	strcpy(ipPara.mask,IP_MASK);
	strcpy(ipPara.gate,IP_GATE);
	strcpy(ipPara.dns,IP_DNS);

	ipPara.dhcp_flag = IP_DHCP_FLAG;

	//APģʽ��ʼֵ
	strcpy(ipPara.dhcpd_ip,AP_IP_ADDR);
	strcpy(ipPara.dhcpd_mask,AP_IP_MASK);
	strcpy(ipPara.dhcpd_gate,AP_IP_GATE);
	strcpy(ipPara.dhcpd_dns,AP_IP_DNS);
	strcpy(ipPara.start_ip_addr,IP_START_IP);
	strcpy(ipPara.end_ip_addr,IP_END_IP);

	ipPara.dhcpd_flag = AP_IP_DHCP_FLAG;

	gCfgItems.wifi_type = ESP_WIFI;

	strcpy((char*)gCfgItems.cloud_hostUrl, "baizhongyun.cn");
	gCfgItems.cloud_port = 10086;


	gCfgItems.cloud_enable = 1;

	gCfgItems.btn_text_offset = 23;//23;
	gCfgItems.display_style = 0;//1;

	gCfgItems.pwroff_save_mode = 0;

	gCfgItems.breakpoint_reprint_flg = 0;
	gCfgItems.breakpoint_flg=0;
	gCfgItems.wifi_scan = 0;
	gCfgItems.wifi_btn_state = 0;
	gCfgItems.pause_unload_len = 0;
	gCfgItems.resume_load_len = 0;
	gCfgItems.resume_speed = 1000;
	gCfgItems.singleNozzle=0;
	gCfgItems.overturn_180 = 0;
	gCfgItems.standby_mode=0;
	gCfgItems.standby_time=0;
	gCfgItems.pulseDelay=5;
	gCfgItems.print_finish_count=180;
	mkstft_ui_set_epr();
	BMP_PIC_X = BTN_PIC_X_OFFSET;
	BMP_PIC_Y = BTN_PIC_Y_OFFSET;
}

DATA_REPRINT_ITMES mksReprint;
void mks_initPrint()
{
	mksReprint.mks_pausePrint_x = mksCfg.filament_change_x_pos;//FILAMENT_CHANGE_X_POS;
	mksReprint.mks_pausePrint_y = mksCfg.filament_change_y_pos;//FILAMENT_CHANGE_Y_POS;
	mksReprint.mks_pausePrint_z = mksCfg.filament_change_z_add;//FILAMENT_CHANGE_Z_ADD;
	mksReprint.mks_pausePrint_e = 5.0;
	mksReprint.waitEndMoves = 0;
	mksReprint.mks_printer_state = MKS_IDLE;

	mksReprint.sdpos = 0;

	memset(mksReprint.dirname,0,sizeof(mksReprint.dirname));
	memset(mksReprint.filename,0,sizeof(mksReprint.filename));

	mksReprint.sdprinting = 0;

	mksReprint.target_temperature_0 = 0;
 	mksReprint.target_temperature_bed = 0;

	mksReprint.resume = MKS_RESUME_IDLE;

	mksReprint.refresh = false;
	
}

uint32_t t1,t2;
char t12[10];

void mks_WriteToEpr()        //��ͣ���ϵ�ʱ�������ݣ��Ա��ϵ������
{
    //t1 = getTick();    
	//�����ļ�λ��
	mksReprint.sdpos = card.getsdpos();
	epr_write_data(EPR_SAV_SDPOS, (uint8_t *)&mksReprint.sdpos,sizeof(mksReprint.sdpos));	
	
	//�����ӡͷ�¶�
	mksReprint.target_temperature_0 = thermalManager.target_temperature[0];
	epr_write_data(EPR_SAV_TARGET_TEMP_0, (uint8_t *)&mksReprint.target_temperature_0,sizeof(mksReprint.target_temperature_0));
	if(mksCfg.extruders == 2)
	{
		mksReprint.target_temperature_1 = thermalManager.target_temperature[1];
		epr_write_data(EPR_SAV_TARGET_TEMP_1, (uint8_t *)&mksReprint.target_temperature_1,sizeof(mksReprint.target_temperature_1));
		
	}
	//�����ȴ��¶�
	if(HAS_TEMP_BED)
	{
		mksReprint.target_temperature_bed = thermalManager.target_temperature_bed;
		epr_write_data(EPR_SAV_TARGET_TEMP_BED, (uint8_t *)&mksReprint.target_temperature_bed,sizeof(mksReprint.target_temperature_bed));
	}
	
	//���浱ǰλ��
	epr_write_data(EPR_SAV_CUR_X, (uint8_t *)&mksReprint.current_position[0],sizeof(mksReprint.current_position[0]));
	epr_write_data(EPR_SAV_CUR_Y, (uint8_t *)&mksReprint.current_position[1],sizeof(mksReprint.current_position[1]));
	epr_write_data(EPR_SAV_CUR_Z, (uint8_t *)&mksReprint.current_position[2],sizeof(mksReprint.current_position[2]));
	epr_write_data(EPR_SAV_CUR_E, (uint8_t *)&mksReprint.current_position[3],sizeof(mksReprint.current_position[3]));
	//����Ŀ��λ��
	epr_write_data(EPR_SAV_DST_X, (uint8_t *)&mksReprint.destination[0],sizeof(mksReprint.destination[0]));
	epr_write_data(EPR_SAV_DST_Y, (uint8_t *)&mksReprint.destination[1],sizeof(mksReprint.destination[1]));
	epr_write_data(EPR_SAV_DST_Z, (uint8_t *)&mksReprint.destination[2],sizeof(mksReprint.destination[2]));
	epr_write_data(EPR_SAV_DST_E, (uint8_t *)&mksReprint.destination[3],sizeof(mksReprint.destination[3]));

#if tan_mask        
	//������Ⱥ�ʱ��
	print_job_timer.getTime(&mksReprint.accumulator,&mksReprint.startTimestamp,&mksReprint.stopTimestamp);
	mksReprint.uwTick = millis();
	
	epr_write_data(EPR_SAV_ACCU, (uint8_t *)&mksReprint.accumulator,sizeof(mksReprint.accumulator));
	epr_write_data(EPR_SAV_STARTIME, (uint8_t *)&mksReprint.startTimestamp,sizeof(mksReprint.startTimestamp));
	epr_write_data(EPR_SAV_STOPTIME, (uint8_t *)&mksReprint.stopTimestamp,sizeof(mksReprint.stopTimestamp));
	epr_write_data(EPR_SAV_TICK, (uint8_t *)&mksReprint.uwTick,sizeof(mksReprint.uwTick));
#endif
	epr_write_data(EPR_SAV_HOUR, (uint8_t *)&print_time.hours,sizeof(print_time.hours));
	epr_write_data(EPR_SAV_MINUTE, (uint8_t *)&print_time.minutes,sizeof(print_time.minutes));
	epr_write_data(EPR_SAV_SECOND, (uint8_t *)&print_time.seconds,sizeof(print_time.seconds));


	//��������ٶ�
	epr_write_data(EPR_SAV_FAN, (uint8_t *)&mksReprint.fanSpeeds_0,sizeof(mksReprint.fanSpeeds_0));

	//����feedrate
	epr_write_data(EPR_SAV_F, (uint8_t *)&mksReprint.feedrate_mm_s,sizeof(mksReprint.feedrate_mm_s));

	epr_write_data(EPR_SAV_CUR_EXTRUDE, (uint8_t *)&active_extruder,sizeof(active_extruder));

	//t2 = getTick();
	//sprintf(t12,"%d",t2-t1);
	//GUI_DispStringAt(t12,0,0);
}


void mks_ReadFromEpr()        //��ͣ���ϵ�ʱ��ȡ����
{
	//��ȡ�ļ���
	epr_read_data((int)EPR_SAV_FILENAME, (uint8_t*)&mksReprint.filename[0],sizeof(mksReprint.filename));  
	//��ȡ�ļ�ƫ��
	if(gCfgItems.pwroff_save_mode != 1)
	{
		epr_read_data((int)EPR_SAV_SDPOS, (uint8_t*)&mksReprint.sdpos, sizeof(mksReprint.sdpos));	
	}
	else
	{
		epr_read_data((int)EPR_SAV_SDPOS, (uint8_t*)&mksReprint.sdpos_from_epr, sizeof(mksReprint.sdpos_from_epr));
		epr_read_data((int)EPR_SAV_SDPOS_BAK, (uint8_t*)&mksReprint.sdpos_bak, sizeof(mksReprint.sdpos_bak));//���zλ��
	}
	//��ȡ��ӡͷ�¶�
	epr_read_data((int)EPR_SAV_TARGET_TEMP_0, (uint8_t*)&mksReprint.target_temperature_0, sizeof(mksReprint.target_temperature_0));
    if(mksCfg.extruders == 2)
    {
		epr_read_data(EPR_SAV_TARGET_TEMP_1, (uint8_t *)&mksReprint.target_temperature_1,sizeof(mksReprint.target_temperature_1));
    }

    //��ȡ�ȴ��¶�
	if(HAS_TEMP_BED)
	{
		epr_read_data((int)EPR_SAV_TARGET_TEMP_BED, (uint8_t*)&mksReprint.target_temperature_bed, sizeof(mksReprint.target_temperature_bed));
	}
	//��ȡ��ǰλ��
	epr_read_data((int)EPR_SAV_CUR_X, (uint8_t*)&mksReprint.current_position[0], sizeof(mksReprint.current_position[0]));
	epr_read_data((int)EPR_SAV_CUR_Y, (uint8_t*)&mksReprint.current_position[1], sizeof(mksReprint.current_position[1]));
	epr_read_data((int)EPR_SAV_CUR_Z, (uint8_t*)&mksReprint.current_position[2], sizeof(mksReprint.current_position[2]));
	epr_read_data((int)EPR_SAV_CUR_E, (uint8_t*)&mksReprint.current_position[3], sizeof(mksReprint.current_position[3]));
	//��ȡĿ��λ��
	epr_read_data((int)EPR_SAV_DST_X, (uint8_t*)&mksReprint.destination[0], sizeof(mksReprint.destination[0]));
	epr_read_data((int)EPR_SAV_DST_Y, (uint8_t*)&mksReprint.destination[1], sizeof(mksReprint.destination[1]));
	epr_read_data((int)EPR_SAV_DST_Z, (uint8_t*)&mksReprint.destination[2], sizeof(mksReprint.destination[2]));
	epr_read_data((int)EPR_SAV_DST_E, (uint8_t*)&mksReprint.destination[3], sizeof(mksReprint.destination[3]));

#if tan_mask
	//��ȡ���Ⱥ�ʱ��
	epr_read_data((int)EPR_SAV_ACCU, (uint8_t*)&mksReprint.accumulator, sizeof(mksReprint.accumulator));
	epr_read_data((int)EPR_SAV_STARTIME, (uint8_t*)&mksReprint.startTimestamp, sizeof(mksReprint.startTimestamp));
	epr_read_data((int)EPR_SAV_STOPTIME, (uint8_t*)&mksReprint.stopTimestamp, sizeof(mksReprint.stopTimestamp));
	epr_read_data((int)EPR_SAV_TICK, (uint8_t*)&mksReprint.uwTick, sizeof(mksReprint.uwTick));
#endif
	epr_read_data((int)EPR_SAV_HOUR, (uint8_t *)&print_time.hours,sizeof(print_time.hours));
	epr_read_data((int)EPR_SAV_MINUTE, (uint8_t *)&print_time.minutes,sizeof(print_time.minutes));
	epr_read_data((int)EPR_SAV_SECOND, (uint8_t *)&print_time.seconds,sizeof(print_time.seconds));

	//��ȡ�����ٶ�
	epr_read_data((int)EPR_SAV_FAN, (uint8_t*)&mksReprint.fanSpeeds_0, sizeof(mksReprint.fanSpeeds_0));

	//��ȡfeedrate
	epr_read_data((int)EPR_SAV_F, (uint8_t*)&mksReprint.feedrate_mm_s, sizeof(mksReprint.feedrate_mm_s));

	epr_read_data((int)EPR_SAV_CUR_EXTRUDE, (uint8_t *)&active_extruder,sizeof(active_extruder));
	gCfgItems.curSprayerChoose=active_extruder;
}


void mks_resumePrint()
{
	if(mksReprint.mks_printer_state == MKS_PAUSED)
		{
		mksReprint.mks_printer_state = MKS_WORKING;
		
		//��������
		mks_preExtrude(mksReprint.mks_pausePrint_e);

		//�ƶ�X,Y

		mks_moveXY(mksReprint.destination[0],mksReprint.destination[1]);
		//Z���½�
		mks_moveZ(0);
		//lcd_setstatus("Resume print.");
		//lcd_setstatus(MSG_RESUME_PRINT);
		lcd_setstatus(mksReprint.filename);
		
		}
	
	if(mksReprint.mks_printer_state == MKS_REPRINTING)
		{
		mksReprint.mks_printer_state = MKS_REPRINTED;
		}
}



void mks_pausePrint()
{
	stepper.synchronize();

	mksReprint.current_position[0] = current_position[X_AXIS];
	mksReprint.current_position[1] = current_position[Y_AXIS];
	mksReprint.current_position[2] = current_position[Z_AXIS];
	mksReprint.current_position[3] = current_position[E_AXIS];

	mksReprint.destination[0]=destination[X_AXIS];
	mksReprint.destination[1]=destination[Y_AXIS];
	mksReprint.destination[2]=destination[Z_AXIS];
	mksReprint.destination[3]=destination[E_AXIS];

        mksReprint.fanSpeeds_0 = fanSpeeds[0];
        mksReprint.feedrate_mm_s = feedrate_mm_s;

	//E �س�
	//mks_preExtrude(MKS_RETROVERSION); 
	mks_preExtrude(gCfgItems.pause_unload_len); 
	//Z������
	mks_moveZ(mksReprint.mks_pausePrint_z);
    //�ƶ�X,Y
    if(mksReprint.mks_pausePrint_x != (float)-1  && mksReprint.mks_pausePrint_y != (float)-1)
	mks_moveXY(mksReprint.mks_pausePrint_x,mksReprint.mks_pausePrint_y);

	mks_WriteToEpr();
	//lcd_setstatus("Pause print.");
#if ENABLED(ULTRA_LCD)         
	lcd_setstatus(MSG_PAUSE_PRINT);
#endif
}

volatile uint8_t has_adjust_speed=0;
volatile uint16_t resume_printed_time=0;

uint8_t print_finish_start_timer=TIMER_STOP;
uint32_t print_finish_timer_count=0;
uint8_t print_finish_close_machine=0;

void mks_PrintStatePolling()
{
	if(mksReprint.mks_printer_state == MKS_PAUSING)
		{
		if( !planner.blocks_queued() &&  card.getsdpos()>MIN_FILE_PRINTED) 	//���� �ļ��� M109��M190ָ�� 
			mksReprint.waitEndMoves++;
		
		if(mksReprint.waitEndMoves > 20)
			{
			mksReprint.mks_printer_state = MKS_PAUSED;
            if(gCfgItems.pwroff_save_mode != 1)
			    epr_write_data(EPR_SAV_FLAG, (uint8_t *)&mksReprint.mks_printer_state,sizeof(mksReprint.mks_printer_state));  //��ͣ��־��λ
			mksReprint.waitEndMoves = 0;
			mks_pausePrint();

			}
		}
	else
		{
			mksReprint.waitEndMoves = 0;
		}
/*
	Cartesian���� ��ͣ��LCD �ƶ�X,Y,Z����
	DELTA ���� ��ͣ��X,Y,Z�����㣬���账��
*/
	//if(mksReprint.mks_printer_state == MKS_PAUSED && MACHINETPYE != DELTA)
	if(mksReprint.mks_printer_state == MKS_PAUSED)		
		{
		if( abs(mksReprint.current_position[0] - current_position[X_AXIS]) > 0.0001 )
			{
			mksReprint.current_position[0] = current_position[X_AXIS];
			epr_write_data(EPR_SAV_CUR_X, (uint8_t *)&mksReprint.current_position[0],sizeof(mksReprint.current_position[0]));
			}
		if( abs(mksReprint.current_position[1] - current_position[Y_AXIS]) > 0.0001 )
			{
			mksReprint.current_position[1] = current_position[Y_AXIS];
			epr_write_data(EPR_SAV_CUR_Y, (uint8_t *)&mksReprint.current_position[1],sizeof(mksReprint.current_position[1]));
			}
		if( abs(mksReprint.current_position[2] - current_position[Z_AXIS]) > 0.0001 )
			{
			mksReprint.current_position[2] = current_position[Z_AXIS];
			epr_write_data(EPR_SAV_CUR_Z, (uint8_t *)&mksReprint.current_position[2],sizeof(mksReprint.current_position[2]));
			}
		}

/*
	�ļ�������ӡ��󣬱�־λ����
*/
    if(mksReprint.mks_printer_state == MKS_RESUMING)
    {
		if(pause_resum == 1)//��ͣ�ָ�
		{
			pause_resum = 0;
			//mks_resumePrint();
			//mks_preExtrude(mksReprint.mks_pausePrint_e);
			mks_preExtrude(gCfgItems.resume_load_len);
			/*if(gCfgItems.resume_speed != 0)
			{
				char buf[10];
				memset(mksReprint.command_queue,0,MAX_CMD_SIZE);
				memset(buf,0,sizeof(buf));
				strcpy(mksReprint.command_queue,"G1 F");
				sprintf(buf,"%d",gCfgItems.resume_speed);
				strcat(mksReprint.command_queue,buf);
			       parser.parse(mksReprint.command_queue);
				gcode_G0_G1();
				stepper.synchronize();
			}*/
			//�ƶ�X,Y
			mks_moveXY(mksReprint.destination[0],mksReprint.destination[1]);
            //����ֹͣ��֮�󣬺���Ķ�������Ҫִ�С�
            if(mksReprint.mks_printer_state == MKS_STOP)
            {
                card.sdprinting = false;
                return;
            }
			//mks_preExtrude(gCfgItems.resume_load_len);
			
			//Z���½�
			mks_moveZ(0);

			mks_adjust_extrude_speed();
			
			card.startFileprint();
			print_job_timer.start();

		}        
    }
    
	if(mksReprint.mks_printer_state == MKS_WORKING)
	{
	/*
		if(pause_resum == 1)//��ͣ�ָ�
		{
			pause_resum = 0;
			//mks_resumePrint();
			mks_preExtrude(mksReprint.mks_pausePrint_e);
			//�ƶ�X,Y
			mks_moveXY(mksReprint.destination[0],mksReprint.destination[1]);
            //����ֹͣ��֮�󣬺���Ķ�������Ҫִ�С�
            if(mksReprint.mks_printer_state == MKS_STOP)
            {
                card.sdprinting = false;
                return;
            }
			//Z���½�
			mks_moveZ(0);

			card.startFileprint();
			print_job_timer.start();

		}
		*/
		if(card.sdprinting == 0)
			mksReprint.sdprinting++;
		else
			mksReprint.sdprinting = 0;

		if(mksReprint.sdprinting > 20)
		{
			mksReprint.sdprinting = 0;
			mksReprint.mks_printer_state = MKS_IDLE;
            if(gCfgItems.pwroff_save_mode != 1)
            {
			    epr_write_data(EPR_SAV_FLAG, (uint8_t *)&mksReprint.mks_printer_state,sizeof(mksReprint.mks_printer_state));  
			//if(gCfgItems.pwroff_save_mode != 1)
				mks_clearFile();
            }
            //����ػ�
            
            if((gCfgItems.print_finish_close_machine_flg == 1)&&(IsChooseAutoShutdown==1))
            {
                //stepper.synchronize();
			    //Close_machine_display();
			       if(card.eof());
				print_finish_start_timer=TIMER_START;
            }

		}
	}
	//ֹͣ��ӡ
	if(mksReprint.mks_printer_state == MKS_STOP)
	{
		mksReprint.mks_printer_state = MKS_IDLE;
        if(gCfgItems.pwroff_save_mode != 1)
		    epr_write_data(EPR_SAV_FLAG, (uint8_t *)&mksReprint.mks_printer_state,sizeof(mksReprint.mks_printer_state));  
	
		clear_command_queue();
        quickstop_stepper();
		//Z������
		mksReprint.destination[2]=destination[Z_AXIS];
		mks_moveZ(mksReprint.mks_pausePrint_z);
		mks_G28("G28 X0 Y0");
		//gcode_M998();
		//card.stopSDPrint();
		//clear_command_queue();
		
		print_job_timer.stop();
		thermalManager.disable_all_heaters();
	#if FAN_COUNT > 0
		  for (uint8_t i = 0; i < FAN_COUNT; i++) 
		  {
		  	fanSpeeds[i] = 0;
			MKS_FAN_TIM = 0 ;
		   }
	#endif
		wait_for_heatup = false;	

		if(gCfgItems.pwroff_save_mode != 1)
			mks_clearFile();		
	}
	//��ͣ,�ϵ�����
	if(mksReprint.mks_printer_state == MKS_REPRINTED)
		{
		switch(MACHINETPYE)
			{
			case Cartesian:
			case COREXY:
			case COREYX:	
				if(mksReprint.resume == MKS_RESUME_PAUSE)
					mks_contiuePrintPause();
				else if(mksReprint.resume == MKS_RESUME_PWDWN) 
					mks_contiuePrintPwdwn();
			break;
			case DELTA:
				mks_contiuePrintDelta();
				break;
			default: 
				break;				
			}
			
		}



	//SD�ļ����� "��" ��Ϣ
	
	if(mksReprint.mks_printer_state == MKS_WORKING && card.sdprinting == true)
		{
			if(mksReprint.refresh)
			{
				if(card.getsdpos()>MIN_FILE_PRINTED)
				{
					mks_WriteToFile();
				}
				
				mksReprint.refresh = false;
			}
		}
	//sean
	if(has_adjust_speed==1)
	{
		if(resume_printed_time >= 10000){
			resume_printed_time=0;
			mks_resume_extrude_speed();
		}
	}
	if(mksReprint.mks_printer_state == MKS_IDLE && print_finish_close_machine == true)
	{
		print_finish_close_machine = false;
		stepper.synchronize();
		Close_machine_display();
	}
	
}



void mks_setTemperature()
{
	char string[20];
	
		//�����ȴ��¶� M140
		if(HAS_TEMP_BED && mksReprint.target_temperature_bed != 0)
			{
			memset(mksReprint.command_queue,0,MAX_CMD_SIZE);
			strcpy(mksReprint.command_queue,"M140 S");	
			//itoa(mksReprint.target_temperature_bed,string,10);
			sprintf(string,"%d",mksReprint.target_temperature_bed);
			strcat(mksReprint.command_queue,string);
			//current_command_args 
			//parser.string_arg = &mksReprint.command_queue[5];
			parser.parse(mksReprint.command_queue);
			gcode_M140();
			}
	
	
			//���ô�ӡͷ�¶�
		if(mksReprint.target_temperature_0 != 0)
		{
			if((mksCfg.extruders ==2))
			{
				memset(mksReprint.command_queue,0,MAX_CMD_SIZE);
				strcpy(mksReprint.command_queue,"M104 T1 S");	
			//	itoa(mksReprint.target_temperature_0,string,10);
				sprintf(string,"%d",mksReprint.target_temperature_1);
				strcat(mksReprint.command_queue,string);
				//current_command_args 
				//parser.string_arg = &mksReprint.command_queue[5];
				parser.parse(mksReprint.command_queue);
				gcode_M104();			
			}
			memset(mksReprint.command_queue,0,MAX_CMD_SIZE);
			strcpy(mksReprint.command_queue,"M109 T0 S");	
		//	itoa(mksReprint.target_temperature_0,string,10);
			sprintf(string,"%d",mksReprint.target_temperature_0);
			strcat(mksReprint.command_queue,string);
			//current_command_args = 
			//parser.string_arg = &mksReprint.command_queue[5];
			parser.parse(mksReprint.command_queue);
			gcode_M109();
		}
		if((mksCfg.extruders ==2)&&(mksReprint.target_temperature_1 != 0))
		{
			memset(mksReprint.command_queue,0,MAX_CMD_SIZE);
			strcpy(mksReprint.command_queue,"M109 T1 S");	
		//	itoa(mksReprint.target_temperature_0,string,10);
			sprintf(string,"%d",mksReprint.target_temperature_1);
			strcat(mksReprint.command_queue,string);
			//current_command_args = 
			//parser.string_arg = &mksReprint.command_queue[5];
			parser.parse(mksReprint.command_queue);
			gcode_M109();
		}	
		//�����ȴ��¶�
		if(HAS_TEMP_BED && mksReprint.target_temperature_bed != 0)
		{
			memset(mksReprint.command_queue,0,MAX_CMD_SIZE);
			strcpy(mksReprint.command_queue,"M190 S");	
			//itoa(mksReprint.target_temperature_bed,string,10);
			sprintf(string,"%d",mksReprint.target_temperature_bed);
			strcat(mksReprint.command_queue,string);
			//current_command_args 
			//parser.string_arg = &mksReprint.command_queue[5];
			parser.parse(mksReprint.command_queue);
			gcode_M190();
		}


}

void mks_setPositionZ()
{
 	char string[20];
   
	//����Z��ǰλ��
	memset(mksReprint.command_queue,0,MAX_CMD_SIZE);
	strcpy(mksReprint.command_queue,"G92 Z");
	memset(string,0,sizeof(string));
	sprintf(string,"%f",mksReprint.current_position[2]);
	strcat(mksReprint.command_queue,string);
	//current_command_args
	//parser.string_arg = &mksReprint.command_queue[4];
	parser.parse(mksReprint.command_queue);
	gcode_G92();
	soft_endstop_min[2] = 0;
}
void mks_ResetPositionZ(float z)
{
 	char string[20];
   
	//����Z��ǰλ��
	memset(mksReprint.command_queue,0,MAX_CMD_SIZE);
	strcpy(mksReprint.command_queue,"G92 Z");
	memset(string,0,sizeof(string));
	sprintf(string,"%f",z);
	strcat(mksReprint.command_queue,string);
	parser.parse(mksReprint.command_queue);
	gcode_G92();
	soft_endstop_min[2] = 0;
}


void mks_setFeedrate( )
{
	//����feedrate
	feedrate_mm_s = mksReprint.feedrate_mm_s;
}


void mks_preExtrude(float e)
{
	//��������

	char string[20];
	relative_mode = true;

	memset(mksReprint.command_queue,0,MAX_CMD_SIZE);
	strcpy(mksReprint.command_queue,"G1 E");
	memset(string,0,sizeof(string));
	sprintf(string,"%f",e);
	strcat(mksReprint.command_queue,string);
	//current_command_args = 
	//parser.command_ptr = &mksReprint.command_queue[3];
    parser.parse(mksReprint.command_queue);
	gcode_G0_G1();
	stepper.synchronize();

	relative_mode = false;


//����E��ǰλ��
	memset(mksReprint.command_queue,0,MAX_CMD_SIZE);
	strcpy(mksReprint.command_queue,"G92 E");
	memset(string,0,sizeof(string));
	//sprintf(string,"%f",mksReprint.current_position[3]);
	sprintf(string,"%f",mksReprint.destination[3]);	
	strcat(mksReprint.command_queue,string);
	//current_command_args = 
	//parser.string_arg = &mksReprint.command_queue[4];
	parser.parse(mksReprint.command_queue);
	gcode_G92();

}

void mks_moveXY(float X,float Y)
{
      char string[20];
    //�ƶ���ӡͷX,Yλ��
	memset(mksReprint.command_queue,0,MAX_CMD_SIZE);
	strcpy(mksReprint.command_queue,"G1 X");
	memset(string,0,sizeof(string));
	sprintf(string,"%f",X);
	strcat(mksReprint.command_queue,string);

	strcat(mksReprint.command_queue," Y");
	memset(string,0,sizeof(string));
	sprintf(string,"%f",Y);
	strcat(mksReprint.command_queue,string);
	
	//current_command_args = 
	//parser.string_arg = &mksReprint.command_queue[3];
	parser.parse(mksReprint.command_queue);
	gcode_G0_G1();
	stepper.synchronize();

}
void mks_moveZ(float Z)
{
      char string[20];
    //�ƶ���ӡͷZλ��
	memset(mksReprint.command_queue,0,MAX_CMD_SIZE);
	strcpy(mksReprint.command_queue,"G1 Z");
	memset(string,0,sizeof(string));
	sprintf(string,"%f",mksReprint.destination[2]+Z);
	strcat(mksReprint.command_queue,string);
	//current_command_args = 
	//parser.command_ptr = &mksReprint.command_queue[0];
	parser.parse(mksReprint.command_queue);
	gcode_G0_G1();
	stepper.synchronize();
}
void mks_moveZ_relative(float Z)
{
      char string[20];
      relative_mode = true;
      
    //�ƶ���ӡͷZλ��
	memset(mksReprint.command_queue,0,MAX_CMD_SIZE);
	strcpy(mksReprint.command_queue,"G1 Z");
	memset(string,0,sizeof(string));
	sprintf(string,"%f",Z);
	strcat(mksReprint.command_queue,string);
	//current_command_args = 
	//parser.command_ptr = &mksReprint.command_queue[0];
	parser.parse(mksReprint.command_queue);
	gcode_G0_G1();
	stepper.synchronize();
    
    relative_mode = false;
}

void mks_G28(char *g_command)
{
	memset(mksReprint.command_queue,0,MAX_CMD_SIZE);
	strcpy(mksReprint.command_queue,g_command);
	//current_command_args
	//parser.string_arg = &mksReprint.command_queue[4];
	parser.parse(mksReprint.command_queue);
	gcode_G28(false);
	
	if((BED_LEVELING_METHOD & HAS_LEVELING) && (BED_LEVELING_METHOD != AUTO_BED_LEVELING_UBL))
		{
		  set_bed_leveling_enabled(true);
	#if ENABLED(ENABLE_LEVELING_FADE_HEIGHT)
		if((MACHINETPYE != DELTA) && (BED_LEVELING_METHOD&HAS_FADE))	//mks_delta
		 { if (parser.seen('Z')) set_z_fade_height(parser.value_linear_units()/*code_value_linear_units()*/);}
	#endif

                }
}


void mks_contiuePrintPause()
{
	 
	char string[20];

	card.sdprinting = true;

	mks_setTemperature();

	// X,Y����
	mks_G28("G28 X0 Y0");
    //����ֹͣ��֮�󣬺���Ķ�������Ҫִ�С�
    if(mksReprint.mks_printer_state == MKS_STOP)
     {
        card.sdprinting = false;
        return;
     }

	//��������
	mks_preExtrude(mksReprint.mks_pausePrint_e);

	
	//�ƶ���ӡͷX,Yλ��
	mks_moveXY(mksReprint.destination[0],mksReprint.destination[1]);	
	//����Z��ǰλ��
	mks_setPositionZ();

	//�ƶ���ӡͷZλ��
	mks_moveZ(0);


	//����feedrate
	mks_setFeedrate();

	//sean
	mks_adjust_extrude_speed();

	//���÷����ٶ�
	fanSpeeds[0] = mksReprint.fanSpeeds_0;
	MKS_FAN_TIM = fanSpeeds[0]*10000/255;
    //����ֹͣ��֮�󣬺���Ķ�������Ҫִ�С�
    if(mksReprint.mks_printer_state == MKS_STOP)
     {
        card.sdprinting = false;
        return;
     }

	//lcd_setstatus("Resume printing...");
	//lcd_setstatus(mksReprint.filename);
	mksReprint.mks_printer_state = MKS_WORKING;
	gcode_M24(); 


}

void mks_contiuePrintPwdwn()
{
	char string[20];

	card.sdprinting = true;

	//HAL_GPIO_WritePin(Z_ENA_GPIO_Port, GPIO_PIN_8, GPIO_PIN_RESET);

	mks_setTemperature();

	//����Z��ǰλ��
	mks_setPositionZ();
	//E �س�
	//mks_preExtrude(MKS_RETROVERSION);
	//�ƶ���ӡͷZλ��
	mks_moveZ(mksReprint.mks_pausePrint_z);

	// X,Y����
	mks_G28("G28 X0 Y0");
    //����ֹͣ��֮�󣬺���Ķ�������Ҫִ�С�
    if(mksReprint.mks_printer_state == MKS_STOP)
     {
        card.sdprinting = false;
        return;
     }
	if(BED_LEVELING_METHOD==AUTO_BED_LEVELING_BILINEAR)
    {
        mks_ResetPositionZ(mksReprint.current_position[2]+mksReprint.mks_pausePrint_z);
    }

	delay(3000);
	
	//��������
	mks_preExtrude(mksReprint.mks_pausePrint_e);

	//�ƶ���ӡͷX,Yλ��
	mks_moveXY(mksReprint.destination[0],mksReprint.destination[1]);	
	//�ƶ���ӡͷZλ��
	mks_moveZ(0);
	
	//����feedrate
	mks_setFeedrate();

	//sean19.8.21
	mks_adjust_extrude_speed();

	//���÷����ٶ�
	fanSpeeds[0] = mksReprint.fanSpeeds_0;
	MKS_FAN_TIM = fanSpeeds[0]*10000/255;

	//lcd_setstatus("Resume printing...");
	//lcd_setstatus(mksReprint.filename);
    //����ֹͣ��֮�󣬺���Ķ�������Ҫִ�С�
    if(mksReprint.mks_printer_state == MKS_STOP)
     {
        card.sdprinting = false;
        return;
     }	
	mksReprint.mks_printer_state = MKS_WORKING;
	gcode_M24(); 
	
	mksReprint.refresh = false;
}


void mks_contiuePrintDelta()
{
	char string[20];

	card.sdprinting = true;

	mks_setTemperature();

	
	// X,Y,Z����
	mks_G28("G28");

	
	//�ƶ���ӡͷZλ��
	mks_moveZ(mksReprint.mks_pausePrint_z);

	//��������
	mks_preExtrude(mksReprint.mks_pausePrint_e);

	//�ƶ���ӡͷX,Yλ��
	mks_moveXY(mksReprint.destination[0],mksReprint.destination[1]);	

	//�ƶ���ӡͷZλ��
	mks_moveZ(0);

	//����feedrate
	mks_setFeedrate();

	//���÷����ٶ�
	fanSpeeds[0] = mksReprint.fanSpeeds_0;
	MKS_FAN_TIM = fanSpeeds[0]*10000/255;

	//lcd_setstatus("Resume printing...");
	//lcd_setstatus(mksReprint.filename);
	
	mksReprint.mks_printer_state = MKS_WORKING;
	gcode_M24(); 

	mksReprint.refresh = false;

}

static uint8_t first_resu=1;

void mks_getPositionXYZE()
{
    volatile char *ZPOS_TEMP;
    volatile char z_dest[20];
    volatile char k=0;
    
	//�����ļ�ƫ��
	if(gCfgItems.pwroff_save_mode != 1)
	{
		card.setIndex(mksReprint.sdpos);

	
		card.sdprinting = true;
		get_sdcard_commands();
		card.sdprinting = false;
		mksReprint.refresh = false;

		LOOP_XYZE(i) 
		{
			destination[i] = current_position[i] = 0;
		}

		LOOP_XYZE(i) 
		{
			//current_command_args
			//parser.string_arg = &command_queue[i][0];
			//�ϵ�z���һ��ָ���������Ҫ���⴦��
          /*
			if(i!=0)
            {   
                parser.parse(&command_queue[i][0]);
                gcode_get_destination();
            }
            else
            {
               ZPOS_TEMP = strchr(&command_queue[i][0],'Z');
               while((*ZPOS_TEMP++ != 0))
               {
                    z_dest[k++]=*ZPOS_TEMP;
                    if(k>20)break;
               }
               destination[2]=atof((char*)z_dest);
            }
           */
                if((i==0)&&(first_resu==1))
                {
                   first_resu = 0;
                   ZPOS_TEMP = strchr(&command_queue[i][0],'Z');
                   if(ZPOS_TEMP)
                   {
                     while((*ZPOS_TEMP++ != 0))
                     {
                          z_dest[k++]=*ZPOS_TEMP;
                          if(k>20)break;
                     }
                     destination[2]=atof((char*)z_dest);                  
                   }
                }
                else
                {            
				    parser.parse(&command_queue[i][0]);
				    gcode_get_destination();                 
                }          
			
			

			LOOP_XYZE(j)
			{
				if(abs(mksReprint.destination[j])< 0.0001 && abs(destination[j]) > 0.0001)
					mksReprint.destination[j] = destination[j];
			}
		}

		mksReprint.current_position[2] = mksReprint.destination[2];
		mksReprint.current_position[3] = mksReprint.destination[3];
	}
	else
	{
		LOOP_XYZE(i) 
		{
			destination[i] = current_position[i] = 0;
		}
		card.setIndex(mksReprint.sdpos_bak);
		while(card.getsdpos() < mksReprint.sdpos_from_epr)
		{
			commands_in_queue = 0;	
			card.sdprinting = true;
			get_sdcard_commands();

			LOOP_XYZE(i) 
			{
				//current_command_args
				//parser.string_arg = &command_queue[i][0];
                /*
				if(i != 0)
                {            
				    parser.parse(&command_queue[i][0]);
				    gcode_get_destination();
                }
                else
                {
                    ZPOS_TEMP = strchr(&command_queue[i][0],'Z');
                    while((*ZPOS_TEMP++ != 0))
                    {
                        z_dest[k++]=*ZPOS_TEMP;
                        if(k>20)break;
                    }
                    destination[2]=atof((char*)z_dest);
                }
                */
                if((i==0)&&(first_resu==1))
                {
                   first_resu = 0;
                   ZPOS_TEMP = strchr(&command_queue[i][0],'Z');
                   if(ZPOS_TEMP)
                   {
                     while((*ZPOS_TEMP++ != 0))
                     {
                          z_dest[k++]=*ZPOS_TEMP;
                          if(k>20)break;
                     }
                     destination[2]=atof((char*)z_dest);  
                   }
                   
                }
                else
                {            
				    parser.parse(&command_queue[i][0]);
				    gcode_get_destination();                 
                }
                
				LOOP_XYZE(j)
				{
					if(abs(mksReprint.destination[j])< 0.0001 && abs(destination[j]) > 0.0001)
						mksReprint.destination[j] = destination[j];

						if(j!=2&&(abs(destination[j]> 0.0001)))
						{
							mksReprint.destination[j] = destination[j];
						}
					
				}
			}
			mksReprint.current_position[2] = mksReprint.destination[2];
			mksReprint.current_position[3] = mksReprint.destination[3];			
		}

		
		card.sdprinting = false;
		mksReprint.refresh = false;

	}





	LOOP_XYZE(i) 
	{
	 	destination[i] = current_position[i] = 0;
	}
	for(int i=0;i<BUFSIZE;i++)
		memset(&command_queue[i][0],0,MAX_CMD_SIZE);
	clear_command_queue();
	

}

void mks_clearDir()
{
	char *tmp_index = NULL;
	char string[30];

// ��mksReprint.filename�ַ����е�Ŀ¼����ȥ����ֻ�����ļ���
	while(1)
		{
		tmp_index = (char *)strstr(mksReprint.filename, "/");
		if(tmp_index)
			{
			strcpy(string,tmp_index+1);
			strcpy(mksReprint.filename,string);
			}
		else
			break;
		}
	
}
extern void Beeper(uint32_t cnt);

void mks_contiuePrint_UI() {
	lcd_setstatus("Resume print?");

    SERIAL_PROTOCOLLN("CONTINUE PRINT");
    ui_print_process.preview_state_flags = 0;
	card.openFile(mksReprint.filename, true);
	strcpy(ui_print_process.file_name,mksReprint.filename);
	if(!card.isFileOpen()) {
        Beeper(1200);
		draw_dialog(DIALOG_TYPE_REPRINT_NO_FILE);
	} else {
	    if((mksReprint.sdpos > MIN_FILE_PRINTED)||(mksReprint.sdpos_from_epr>MIN_FILE_PRINTED)) {
    		epr_write_data(EPR_SAV_FILENAME, (uint8_t *)&mksReprint.filename[0],sizeof(mksReprint.filename)); 
    		card.sdprinting = 0;

    		if(mksReprint.resume == MKS_RESUME_PWDWN) 
    			mks_getPositionXYZE();
    		
    		if(gCfgItems.pwroff_save_mode != 1)
    			card.setIndex(mksReprint.sdpos);
    		else
    			card.setIndex(mksReprint.sdpos_from_epr);
    		current_position[X_AXIS] = mksReprint.current_position[0];
    		current_position[Y_AXIS] = mksReprint.current_position[1];
    		current_position[Z_AXIS] = mksReprint.current_position[2];
    		if(gCfgItems.pwroff_save_mode != 1)
    			mks_clearDir();
    		while(1) {
    			logo_tick2 = getTick();
    			if((getTickDiff(logo_tick2, logo_tick1)>=3000) || (gCfgItems.fileSysType == FILE_SYS_USB))
    				break;
    		}
			printing_ui.show();
        } else {
            mksReprint.resume = MKS_RESUME_IDLE;
            mksReprint.mks_printer_state = MKS_IDLE;
            if(gCfgItems.pwroff_save_mode != 1)
                epr_write_data(EPR_SAV_FLAG, (uint8_t *)&mksReprint.mks_printer_state,sizeof(mksReprint.mks_printer_state));  //
            continue_print_error_flg = 1;
        }
	}
}

/*
void mks_rePrintCheck() ʵ��
��ͣ -> �ϵ� -> �ϵ�����
ֱ�Ӷϵ� -> �ϲ�����
��ͣ������Բ���:
1.M32 *.gcode --�򿪲���ӡ�ļ�
2.M25		  -- ��ͣ��ӡ
3.�ϵ縴λ
4.M24		  -- ����
�ϵ�������Բ���:
1.M32 *.gcode --�򿪲���ӡ�ļ�
2.�ϵ縴λ
3.M24		  -- ����
*/



void mks_rePrintCheck()
{
	bool pft_ok = false;
	epr_read_data((int)EPR_SAV_FLAG, (uint8_t*)&mksReprint.mks_printer_state, sizeof(mksReprint.mks_printer_state));
	if(mksReprint.mks_printer_state == MKS_PAUSED)		//��ͣ����
	{
		mksReprint.mks_printer_state = MKS_REPRINTING;
		mks_ReadFromEpr();		//��EERPOM��ȡ �������
		mksReprint.resume = MKS_RESUME_PAUSE;
		mks_contiuePrint_UI();

	}
	
	else if(mksReprint.mks_printer_state == MKS_WORKING)	//�ϵ�����
	{
		if(gCfgItems.pwroff_save_mode == 1)
		{
			mksReprint.mks_printer_state = MKS_REPRINTING;
			mks_ReadFromEpr_pwroff();		//��EERPOM��ȡ �������
			//if(mksReprint.sdpos_from_epr > MIN_FILE_PRINTED)
			{
				mksReprint.resume = MKS_RESUME_PWDWN;
				mks_contiuePrint_UI();

			}
		}
		else
		{

			mksReprint.mks_printer_state = MKS_REPRINTING;
			//��ȡ�ļ���
			epr_read_data((int)EPR_SAV_FILENAME, (uint8_t*)&mksReprint.filename[0],sizeof(mksReprint.filename)); 
			pft_ok = mks_ReadFromFile();	//��mks_pft.sys�ļ���ȡ�������
			//if(pft_ok && mksReprint.sdpos > MIN_FILE_PRINTED )
			{
				mksReprint.resume = MKS_RESUME_PWDWN;
				mks_contiuePrint_UI();
			}
		}
	}
	else
	{
		mksReprint.mks_printer_state = MKS_IDLE;
        if(gCfgItems.pwroff_save_mode != 1)
		    epr_write_data(EPR_SAV_FLAG, (uint8_t *)&mksReprint.mks_printer_state,sizeof(mksReprint.mks_printer_state));

 		continue_print_error_flg = 1;

	}

	if(continue_print_error_flg == 1)
	{
		continue_print_error_flg = 0;
		while(1)
		{
		    if(mksTmp.cfg_hardware_test_enable==1)break;
            
			logo_tick2 = getTick();
			if((getTickDiff(logo_tick2, logo_tick1)>=3000))
			{
				ui_app.showMainWidget();
				break;
			}
		}		
	}
}


void mks_WriteToFile()
{
	char string[20];
	//mksReprint.card.openFile(MKS_PFT_NAME, false);
	//if (mksReprint.card.isFileOpen()) 
	char name[30]={0};
	
	if(card.cardOK)
		strcat(name,SD_Path);
	else
    {
#if unused
		strcat(name,USBH_Path);
#endif
    }

	strcat(name,mks_pft_name);
        
	if (f_open(&mksReprint.pft, (const TCHAR *)name, FA_OPEN_EXISTING | FA_READ | FA_WRITE) == FR_OK)
		{
		memset(string,0,sizeof(string));
		strcpy(string,"start\n");
		//mksReprint.card.write_command(string);	//"start"
		f_printf(&mksReprint.pft,string);

		//�����ļ�λ��
		memset(string,0,sizeof(string));
		sprintf(string,"%d",mksReprint.sdpos);
		//mksReprint.card.write_command(string);
		strcat(string,"\n");
		f_printf(&mksReprint.pft,string);
		//�����ӡͷ�¶�
		memset(string,0,sizeof(string));
		sprintf(string,"%d",thermalManager.target_temperature[0]);
		strcat(string,"\n");
		f_printf(&mksReprint.pft,string);
		
		memset(string,0,sizeof(string));
		sprintf(string,"%d",thermalManager.target_temperature[1]);
		strcat(string,"\n");
		f_printf(&mksReprint.pft,string);		
		
		//�����ȴ��¶�
		//if(HAS_TEMP_BED)
		{
			memset(string,0,sizeof(string));
			sprintf(string,"%d",thermalManager.target_temperature_bed);
			//mksReprint.card.write_command(string);
			strcat(string,"\n");
			f_printf(&mksReprint.pft,string);
			
		}

		//������Ⱥ�ʱ��
		print_job_timer.getTime(&mksReprint.accumulator,&mksReprint.startTimestamp,&mksReprint.stopTimestamp);
		mksReprint.uwTick = millis();
		if(mksReprint.startTimestamp > mksReprint.stopTimestamp  ) mksReprint.stopTimestamp = mksReprint.uwTick;

		memset(string,0,sizeof(string));					//accumulator
		sprintf(string,"%d",mksReprint.accumulator);
		//mksReprint.card.write_command(string);
		strcat(string,"\n");
		f_printf(&mksReprint.pft,string);
#if tan_mask
		memset(string,0,sizeof(string));					//startTimestamp
		sprintf(string,"%d",mksReprint.startTimestamp);
		//mksReprint.card.write_command(string);
		strcat(string,"\n");
		f_printf(&mksReprint.pft,string);

		memset(string,0,sizeof(string));					//stopTimestamp
		sprintf(string,"%d",mksReprint.stopTimestamp);
		//mksReprint.card.write_command(string);
		strcat(string,"\n");
		f_printf(&mksReprint.pft,string);

		memset(string,0,sizeof(string));					//uwTick
		sprintf(string,"%d",mksReprint.uwTick);
		//mksReprint.card.write_command(string);
		strcat(string,"\n");
		f_printf(&mksReprint.pft,string);
#endif
		//��ӡʱ��:hour
		memset(string,0,sizeof(string));					//uwTick
		sprintf(string,"%d",print_time.hours);
		//mksReprint.card.write_command(string);
		strcat(string,"\n");
		f_printf(&mksReprint.pft,string);
		
		//��ӡʱ��:min
		memset(string,0,sizeof(string));					//uwTick
		sprintf(string,"%d",print_time.minutes);
		//mksReprint.card.write_command(string);
		strcat(string,"\n");
		f_printf(&mksReprint.pft,string);
		
		//��ӡʱ��:sec
		memset(string,0,sizeof(string));					//uwTick
		sprintf(string,"%d",print_time.seconds);
		//mksReprint.card.write_command(string);
		strcat(string,"\n");
		f_printf(&mksReprint.pft,string);	
		
		//�������
		memset(string,0,sizeof(string));
		sprintf(string,"%d",fanSpeeds[0]);
		//mksReprint.card.write_command(string);
		strcat(string,"\n");
		f_printf(&mksReprint.pft,string);
		
		//����feedrate
		memset(string,0,sizeof(string));
		sprintf(string,"%f",feedrate_mm_s);
		//mksReprint.card.write_command(string);
		strcat(string,"\n");
		f_printf(&mksReprint.pft,string);
		
		//���浱ǰ����ͷ
		memset(string,0,sizeof(string));
		sprintf(string,"%d",active_extruder);
		//mksReprint.card.write_command(string);
		strcat(string,"\n");
		f_printf(&mksReprint.pft,string);


		memset(string,0,sizeof(string));
		strcpy(string,"end");						//"end"
		//mksReprint.card.write_command(string);	
		strcat(string,"\n");
		f_printf(&mksReprint.pft,string);
		
		//mksReprint.card.closefile(true);
		f_close(&mksReprint.pft);
		}
	

}

void mks_WriteToEpr_pwroff()        //�ϵ�ʱ�������ݣ��Ա��ϵ������
{ 
    //��ӡ״̬
    epr_write_data(EPR_SAV_FLAG, (uint8_t *)&mksReprint.mks_printer_state,sizeof(mksReprint.mks_printer_state));

	//�����ļ�λ��
	epr_write_data(EPR_SAV_SDPOS_BAK, (uint8_t *)&mksReprint.sdpos_bak,sizeof(mksReprint.sdpos_bak));
	
	mksReprint.sdpos = card.getsdpos();
	epr_write_data(EPR_SAV_SDPOS, (uint8_t *)&mksReprint.sdpos,sizeof(mksReprint.sdpos));	
	
	//�����ӡͷ�¶�
	mksReprint.target_temperature_0 = thermalManager.target_temperature[0];
	epr_write_data(EPR_SAV_TARGET_TEMP_0, (uint8_t *)&mksReprint.target_temperature_0,sizeof(mksReprint.target_temperature_0));
	if(mksCfg.extruders == 2)
	{
		mksReprint.target_temperature_1 = thermalManager.target_temperature[1];
		epr_write_data(EPR_SAV_TARGET_TEMP_1, (uint8_t *)&mksReprint.target_temperature_1,sizeof(mksReprint.target_temperature_1));
		
	}
	//�����ȴ��¶�
	if(HAS_TEMP_BED)
	{
		mksReprint.target_temperature_bed = thermalManager.target_temperature_bed;
		epr_write_data(EPR_SAV_TARGET_TEMP_BED, (uint8_t *)&mksReprint.target_temperature_bed,sizeof(mksReprint.target_temperature_bed));
	}

	epr_write_data(EPR_SAV_HOUR, (uint8_t *)&print_time.hours,sizeof(print_time.hours));
	epr_write_data(EPR_SAV_MINUTE, (uint8_t *)&print_time.minutes,sizeof(print_time.minutes));
	epr_write_data(EPR_SAV_SECOND, (uint8_t *)&print_time.seconds,sizeof(print_time.seconds));

	//��������ٶ�
	mksReprint.fanSpeeds_0 = fanSpeeds[0];
	epr_write_data(EPR_SAV_FAN, (uint8_t *)&mksReprint.fanSpeeds_0,sizeof(mksReprint.fanSpeeds_0));

	//����feedrate
	mksReprint.feedrate_mm_s = feedrate_mm_s;
	epr_write_data(EPR_SAV_F, (uint8_t *)&mksReprint.feedrate_mm_s,sizeof(mksReprint.feedrate_mm_s));

	epr_write_data(EPR_SAV_CUR_EXTRUDE, (uint8_t *)&active_extruder,sizeof(active_extruder));

}



void mks_ReadFromEpr_pwroff()        //�ϵ�ʱ��ȡ����
{
	//��ȡ�ļ���
	epr_read_data((int)EPR_SAV_FILENAME, (uint8_t*)&mksReprint.filename[0],sizeof(mksReprint.filename));  
	//��ȡ�ļ�ƫ��

	epr_read_data((int)EPR_SAV_SDPOS, (uint8_t*)&mksReprint.sdpos_from_epr, sizeof(mksReprint.sdpos_from_epr));
	epr_read_data((int)EPR_SAV_SDPOS_BAK, (uint8_t*)&mksReprint.sdpos_bak, sizeof(mksReprint.sdpos_bak));//���zλ��
	
	//��ȡ��ӡͷ�¶�
	epr_read_data((int)EPR_SAV_TARGET_TEMP_0, (uint8_t*)&mksReprint.target_temperature_0, sizeof(mksReprint.target_temperature_0));
	if(mksCfg.extruders == 2)
		epr_read_data((int)EPR_SAV_TARGET_TEMP_1, (uint8_t*)&mksReprint.target_temperature_1, sizeof(mksReprint.target_temperature_1));

	//��ȡ�ȴ��¶�
	if(HAS_TEMP_BED)
	{
		epr_read_data((int)EPR_SAV_TARGET_TEMP_BED, (uint8_t*)&mksReprint.target_temperature_bed, sizeof(mksReprint.target_temperature_bed));
	}
#if tan_mask
	//��ȡ���Ⱥ�ʱ��
	epr_read_data((int)EPR_SAV_ACCU, (uint8_t*)&mksReprint.accumulator, sizeof(mksReprint.accumulator));
	epr_read_data((int)EPR_SAV_STARTIME, (uint8_t*)&mksReprint.startTimestamp, sizeof(mksReprint.startTimestamp));
	epr_read_data((int)EPR_SAV_STOPTIME, (uint8_t*)&mksReprint.stopTimestamp, sizeof(mksReprint.stopTimestamp));
	epr_read_data((int)EPR_SAV_TICK, (uint8_t*)&mksReprint.uwTick, sizeof(mksReprint.uwTick));
#endif
	epr_read_data((int)EPR_SAV_HOUR, (uint8_t *)&print_time.hours,sizeof(print_time.hours));
	epr_read_data((int)EPR_SAV_MINUTE, (uint8_t *)&print_time.minutes,sizeof(print_time.minutes));
	epr_read_data((int)EPR_SAV_SECOND, (uint8_t *)&print_time.seconds,sizeof(print_time.seconds));

	//��ȡ�����ٶ�
	epr_read_data((int)EPR_SAV_FAN, (uint8_t*)&mksReprint.fanSpeeds_0, sizeof(mksReprint.fanSpeeds_0));

	//��ȡfeedrate
	epr_read_data((int)EPR_SAV_F, (uint8_t*)&mksReprint.feedrate_mm_s, sizeof(mksReprint.feedrate_mm_s));

	epr_read_data((int)EPR_SAV_CUR_EXTRUDE, (uint8_t *)&active_extruder,sizeof(active_extruder));
	gCfgItems.curSprayerChoose=active_extruder;
}


int16_t pft_get() {
	BYTE readByte;
	UINT rc;
	if (f_read(&mksReprint.pft, &readByte, 1, &rc) != FR_OK)				   
	{
		readByte = -1;
	}
	else
	{
		mksReprint.pftpos += rc;
	}
	return (int16_t) readByte;
};



bool mks_get_commands() {
	char string[20];
	char commands_count = 0;
  uint16_t sd_count = 0;
//  bool card_eof = false;
	bool end_ok = false;
  
  //while (!card_eof ) 
  while(mksReprint.pftpos < mksReprint.filesize)
  	{
	const int16_t n = pft_get();
	if(n == -1)	break;
	char sd_char = (char)n;
	
	
	if ( sd_char == '\n') 	//"start\r\n"
		{
		string[sd_count-1] = 0;		//ȥ�� "\r"
		switch(commands_count)
			{
			case 0:			//start
				break;
			case 1:			//��ȡ�ļ�λ��
				mksReprint.sdpos = atoi(string);
				break;
			case 2:			//��ȡ��ӡͷ�¶�
				mksReprint.target_temperature_0 = atoi(string);
				break;
			case 3:			//��ȡ��ӡͷ�¶�
				mksReprint.target_temperature_1 = atoi(string);
				break;				
			case 4:			//��ȡ�ȴ��¶�
				mksReprint.target_temperature_bed = atoi(string);
				break;
			case 5:			//accumulator
				mksReprint.accumulator = atoi(string);
				break;
#if tan_mask
			case 5:			//startTimestamp
				mksReprint.startTimestamp = atoi(string);
				break;
			case 6:			//stopTimestamp
				mksReprint.stopTimestamp = atoi(string);
				break;
			case 7:			//uwTick
				mksReprint.uwTick = atoi(string);
				break;
#endif	
			case 6:			//hour
				print_time.hours= atoi(string);
				break;
			case 7:			//min
				print_time.minutes= atoi(string);
				break;
			case 8:			//second
				print_time.seconds = atoi(string);
				break;

			case 9:			//��ȡ����
				mksReprint.fanSpeeds_0 = atoi(string);
				break;
			case 10:		//��ȡfeedrate
				mksReprint.feedrate_mm_s = atof(string);
				break;
			case 11:		//��ȡ��ǰ����ͷ
				gCfgItems.curSprayerChoose = atof(string);
                active_extruder=gCfgItems.curSprayerChoose;
				break;				
			case 12:		//end
				if(strcmp(string,"end") == 0)
					end_ok = true;
				break;
			default: break;				
				
			}

	  	sd_count = 0;
		memset(string,0,sizeof(string));
		commands_count++;
		}
	else 
		{
		  string[sd_count++] = sd_char;
          if(sd_count>=20)//��ֹ����������
          {
            break;
          }
		}
		
  }

  return(end_ok);
}



bool mks_ReadFromFile()
{
	bool get_ok = false;
	//mksReprint.card.openFile(MKS_PFT_NAME, true);
	//if (mksReprint.card.isFileOpen()) 
       	char name[30]={0};

	if(card.cardOK)
		strcat(name,SD_Path);
	else
    {
#if unused
		strcat(name,USBH_Path);
#endif
    }
	strcat(name,mks_pft_name);
        
	if (f_open(&mksReprint.pft, (const TCHAR *)name, FA_OPEN_EXISTING | FA_READ) == FR_OK)		
	{
		mksReprint.pftpos = 0;
		mksReprint.filesize = f_size(&mksReprint.pft);
		get_ok = mks_get_commands();

		//mksReprint.card.closefile(true);
		f_close(&mksReprint.pft);
	}
	
	return(get_ok);

}

void mks_clearFile()
{
#if 1
	char string[20]="clear";
	char name[30]={0};
	
	if(card.cardOK)
		strcat(name,SD_Path);
	else
    {
#if unused
		strcat(name,USBH_Path);
#endif    
    }
	strcat(name,mks_pft_name);

	
	if (f_open(&mksReprint.pft, (const TCHAR *)name, FA_CREATE_ALWAYS |  FA_WRITE) == FR_OK)
        {
          f_printf(&mksReprint.pft,string);
          f_close(&mksReprint.pft);
        }
    #endif
}

void mks_saveFileName(char *name)
{
#if 1
		memset(mksReprint.filename,0,sizeof(mksReprint.filename));
	  
	//�����ļ���
		strcpy(mksReprint.filename,name);
		epr_write_data(EPR_SAV_FILENAME, (uint8_t *)&mksReprint.filename[0],sizeof(mksReprint.filename)); 
	//��� mks_pft.sys�ļ��еļ�¼
		if((mksReprint.mks_printer_state == MKS_IDLE)
			&&(gCfgItems.pwroff_save_mode != 1))	/*�����ļ�ʱ���������¼������ʱ�����*/
                  mks_clearFile();
#endif
}

void mks_adjust_extrude_speed()
{
	//sean 19.8.20
	if(gCfgItems.resume_speed != 0 && has_adjust_speed !=1)
	{
		if(planner.flow_percentage[0]< MAX_EXT_SPEED_PERCENT - gCfgItems.resume_speed)
		{
			planner.flow_percentage[0] += gCfgItems.resume_speed;
			
		}
		else
		{
			planner.flow_percentage[0] = MAX_EXT_SPEED_PERCENT;
		}
	        planner.e_factor[0]= planner.flow_percentage[0]*0.01;
		planner.flow_percentage[1] = planner.flow_percentage[0];
	       planner.e_factor[1]= planner.flow_percentage[1]*0.01;

		   has_adjust_speed=1;
		   resume_printed_time=0;
	}
}
void mks_resume_extrude_speed()
{
	if(gCfgItems.resume_speed != 0)
	{
		//sean 19.8.20
		planner.flow_percentage[0] -= gCfgItems.resume_speed;
	        planner.e_factor[0]= planner.flow_percentage[0]*0.01;
		planner.flow_percentage[1] = planner.flow_percentage[0];
	       planner.e_factor[1]= planner.flow_percentage[1]*0.01;

	       has_adjust_speed=0;
	}
}

extern "C" void EXTI9_5_IRQHandler(void);

void EXTI9_5_IRQHandler(void)
{
  //cli(); // Stop interrupts
  Lcd_Light_OFF;
  if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_5) != RESET)
  {
    __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_5);
	
	if(mksReprint.mks_printer_state == MKS_IDLE)
	{
		FALA_CTRL = 0;
		return;
	}
	if((mksReprint.mks_printer_state == MKS_REPRINTING)
		||(mksReprint.mks_printer_state == MKS_REPRINTING))
	{
		FALA_CTRL = 0;
		return;	
	}
	if((mksReprint.mks_printer_state == MKS_PAUSING)
		||(mksReprint.mks_printer_state == MKS_PAUSED))
	{
		FALA_CTRL = 0;
		return;	
	}
	
	if(gCfgItems.pwroff_save_mode == 1)
	{
		EXT_CTRL = 0;
    	mks_WriteToEpr_pwroff();
		delay(1000);
		FALA_CTRL = 0;
		while(1);
	}
  }
  sei(); // ���� interrupts
}


uint16_t rgb888_2_rgb565(int32_t  color_rgb888)
{
	uint8_t r5,g6,b5;
	uint16_t color_rgb565;
	r5 = (uint8_t)((color_rgb888  & 0x00ff0000)>>19);
	g6 = (uint8_t)((color_rgb888  & 0x0000ff00)>>10);
	b5 = (uint8_t)((color_rgb888  & 0x000000ff)>>3);

	color_rgb565 = (((uint16_t)(r5<<3) & 0x00ff)<<8);
	color_rgb565 = color_rgb565 | (((uint16_t)(g6<<2))<<3);
	color_rgb565 = color_rgb565 | ((uint16_t)(b5));

	return color_rgb565;
}

void excute_m290(char *g_command)
{
	memset(mksReprint.command_queue,0,MAX_CMD_SIZE);
	strcpy(mksReprint.command_queue,g_command);
	parser.parse(mksReprint.command_queue);
	gcode_M290();
}

void excute_m500()
{
	gcode_M500();
}

