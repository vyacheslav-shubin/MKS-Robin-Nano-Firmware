#include "GUI.h"
#include "BUTTON.h"
#include "PROGBAR.h"
#include "CHECKBOX.h"
#include "draw_printing.h"
#include "draw_ui.h"
#include "fontLib.h"
#include "LISTBOX.h"
#include "text.h"
#include "draw_operate.h"
#include "ui_tools.h"
#include "pic_manager.h"

#include "draw_ready_print.h"
//#include "others.h"
//#include "mks_tft_com.h"

#include "draw_print_file.h"
#include "pic.h"
#include "marlin.h"
#include "mks_reprint.h"
#include "stm32f10x_gpio.h"



#ifndef GUI_FLASH
#define GUI_FLASH
#endif
extern GUI_CONST_STORAGE GUI_FONT GUI_FontD36x48;

extern FIL *srcfp;

extern volatile uint8_t temper_error_flg;

int8_t curFilePath[30];

int once_flag = 0;

//////////FIL *srcfp;

static GUI_HWIN hPrintingWnd, hRetDlgWnd;
static PROGBAR_Handle printingBar;
static TEXT_Handle printTimeLeft, printSpeed, sprayTem1, sprayTem2, BedTem, fanSpeed;
static BUTTON_STRUCT button1, button2,button3,button4,button5,buttonE1, buttonE2, buttonFan, buttonBed, buttonOk, buttonCancle, btnRetMainPage;
static TEXT_Handle printRetDlgText;

static BUTTON_STRUCT buttonPause,buttonStop,buttonOperat,buttonExt1, buttonExt2, buttonFanstate, buttonBedstate,buttonPrintSpeed,buttonTime,buttonZpos,buttonAutoclose;
static TEXT_Handle printZposRight,E1_Temp, E2_Temp, Fan_Pwm, Bed_Temp,Printing_speed,Zpos,Autoclose;
static CHECKBOX_Handle auto_close;
///////static FIL curFile;

static FIL curFile;
extern GUI_FLASH const GUI_FONT GUI_FontHZ_fontHz18;
extern GUI_BITMAP bmp_struct;

extern volatile int16_t logo_time;
//extern PR_STATUS printerStaus;
extern FILE_PRINT_STATE gCurFileState;
extern int X_ADD;
extern int X_INTERVAL;   //**ͼƬ���

uint8_t print_start_flg = 0;

uint8_t pause_resum=0;


extern uint8_t gcode_preview_over;

extern uint8_t from_flash_pic;
extern volatile uint8_t pause_from_high_level;
extern volatile uint8_t pause_from_low_level;
extern uint8_t button_disp_pause_state;

void update_printing_1s(void);

static void cbPrintingWin(WM_MESSAGE * pMsg) {

struct PressEvt *press_event;
switch (pMsg->MsgId)
{
	case WM_PAINT:
		break;
	case WM_TOUCH:
	 	press_event = (struct PressEvt *)pMsg->Data.p;
		
		break;
	case WM_TOUCH_CHILD:
		press_event = (struct PressEvt *)pMsg->Data.p;

		break;
		
	case WM_NOTIFY_PARENT:
		if(pMsg->Data.v == WM_NOTIFICATION_RELEASED)
		{
			if(pMsg->hWinSrc == buttonOperat.btnHandle)
			{
				if(gcode_preview_over != 1)
				{
					last_disp_state = PRINTING_UI;
					clear_printing();
					if((mksReprint.mks_printer_state == MKS_IDLE)  &&  (gCurFileState.totalSend == 100) )
					{
						f_close(srcfp);

						reset_file_info();
						draw_ready_print();
					}
					else
					{
						draw_operate();
					}
				}
			}
			else if(pMsg->hWinSrc == buttonPause.btnHandle)
			{
				if(gcode_preview_over != 1)
				{
					if(mksReprint.mks_printer_state == MKS_WORKING)
					{
						stop_print_time();
						if(mksCfg.extruders==2)
						{
							gCfgItems.curSprayerChoose_bak= active_extruder;
							gCfgItems.moveSpeed_bak = feedrate_mm_s;
						}
      						card.pauseSDPrint();
      						print_job_timer.pause();
						mksReprint.mks_printer_state = MKS_PAUSING;
 
						BUTTON_SetBmpFileName(buttonPause.btnHandle, "bmp_resume.bin",1);
						BUTTON_SetBitmapEx(buttonPause.btnHandle, 0, &bmp_struct_150, 0, 0);
						if(gCfgItems.multiple_language != 0)
						{
								BUTTON_SetText(buttonPause.btnHandle, printing_menu.resume);	
						}						
					}
					else if(mksReprint.mks_printer_state == MKS_PAUSED)
					{
				   
		                        	 if(mksCfg.extruders==2)
			                        {
			                              if(pause_from_high_level==1)
			                              {
			                                if(((MKS_MT_DET1_OP == Bit_SET)&&(gCfgItems.filament_det0_level_flg == 1))
			                                    ||((MKS_MT_DET2_OP == Bit_SET)&&(gCfgItems.filament_det1_level_flg == 1)))
			                                	{
			                                		last_disp_state = PRINTING_UI;
			                    			  clear_printing();
			                    			  draw_dialog(DIALOG_TYPE_FILAMENT_NO_PRESS);  
			                                	}
							else	if(((MKS_MT_DET1_OP == Bit_RESET)&&(gCfgItems.filament_det0_level_flg == 0))
			                                    ||((MKS_MT_DET2_OP == Bit_RESET)&&(gCfgItems.filament_det1_level_flg == 0)))
			                                	{
			                                		last_disp_state = PRINTING_UI;
			                    			  clear_printing();
			                    			  draw_dialog(DIALOG_TYPE_FILAMENT_NO_PRESS);  
			                                	}
								else
			                                {
			                                        pause_from_high_level=0;
			                                        start_print_time();
			                                        pause_resum = 1;
			                                        mksReprint.mks_printer_state = MKS_RESUMING;//MKS_WORKING;
								BUTTON_SetBmpFileName(buttonPause.btnHandle, "bmp_pause.bin",1);
								BUTTON_SetBitmapEx(buttonPause.btnHandle, 0, &bmp_struct_150, 0, 0);
								if(gCfgItems.multiple_language != 0)
								{
									BUTTON_SetText(buttonPause.btnHandle, printing_menu.pause);	
								}
			                                }
			                                 
			                              }
							   else if(pause_from_low_level==1)
							   {
							   	if(((MKS_MT_DET1_OP == Bit_SET)&&(gCfgItems.filament_det0_level_flg == 1))
			                                    ||((MKS_MT_DET2_OP == Bit_SET)&&(gCfgItems.filament_det1_level_flg == 1)))
							   	{
							   		last_disp_state = PRINTING_UI;
			                    			  clear_printing();
			                    			  draw_dialog(DIALOG_TYPE_FILAMENT_NO_PRESS); 
							   	}
								else if(((MKS_MT_DET1_OP == Bit_RESET)&&(gCfgItems.filament_det0_level_flg == 0))
			                                    ||((MKS_MT_DET2_OP == Bit_RESET)&&(gCfgItems.filament_det1_level_flg == 0)))
							   	{
							   		last_disp_state = PRINTING_UI;
			                    			  clear_printing();
			                    			  draw_dialog(DIALOG_TYPE_FILAMENT_NO_PRESS); 
							   	}
								else
			                                {
			                                        pause_from_low_level=0;
			                                        start_print_time();
			                                        pause_resum = 1;
			                                        mksReprint.mks_printer_state = MKS_RESUMING;//MKS_WORKING;
								BUTTON_SetBmpFileName(buttonPause.btnHandle, "bmp_pause.bin",1);
								BUTTON_SetBitmapEx(buttonPause.btnHandle, 0, &bmp_struct_150, 0, 0);
								if(gCfgItems.multiple_language != 0)
								{
									BUTTON_SetText(buttonPause.btnHandle, printing_menu.pause);	
								}
			                                }
			                                 
							   }
			                              else
			                              {
			                                    start_print_time();
			                                    pause_resum = 1;
			                                    mksReprint.mks_printer_state = MKS_RESUMING;//MKS_WORKING;
								BUTTON_SetBmpFileName(buttonPause.btnHandle, "bmp_pause.bin",1);
								BUTTON_SetBitmapEx(buttonPause.btnHandle, 0, &bmp_struct_150, 0, 0);
								if(gCfgItems.multiple_language != 0)
								{
									BUTTON_SetText(buttonPause.btnHandle, printing_menu.pause);	
								}
			                              }
			                        }
		                        	else
			                        {
			                            //if(gCfgItems.filament_det0_level_flg == 1)
			                            //{
			                                if(pause_from_high_level==1)
			                                {
			                                    if(MKS_MT_DET1_OP == Bit_RESET)
			                                    {
			                                        pause_from_high_level=0;
			                                        start_print_time();
			                                        pause_resum = 1;
			                                        mksReprint.mks_printer_state = MKS_RESUMING;//MKS_WORKING;
								   	BUTTON_SetBmpFileName(buttonPause.btnHandle, "bmp_pause.bin",1);
									BUTTON_SetBitmapEx(buttonPause.btnHandle, 0, &bmp_struct_150, 0, 0);
									if(gCfgItems.multiple_language != 0)
									{
										BUTTON_SetText(buttonPause.btnHandle, printing_menu.pause);	
									}
			                                    }
			                                    else
			                                    {
					                                     last_disp_state = PRINTING_UI;
					                    			clear_printing();
			                    					draw_dialog(DIALOG_TYPE_FILAMENT_NO_PRESS);   
			                                    }
			                                }
							     else if(pause_from_low_level==1)
			                                {
			                                    if(MKS_MT_DET1_OP == Bit_SET)
			                                    {
			                                        pause_from_low_level=0;
			                                        start_print_time();
			                                        pause_resum = 1;
			                                        mksReprint.mks_printer_state = MKS_RESUMING;//MKS_WORKING;
								   	BUTTON_SetBmpFileName(buttonPause.btnHandle, "bmp_pause.bin",1);
									BUTTON_SetBitmapEx(buttonPause.btnHandle, 0, &bmp_struct_150, 0, 0);
									if(gCfgItems.multiple_language != 0)
									{
										BUTTON_SetText(buttonPause.btnHandle, printing_menu.pause);	
									}
			                                    }
			                                    else
			                                    {
					                                     last_disp_state = PRINTING_UI;
					                    			clear_printing();
			                    					draw_dialog(DIALOG_TYPE_FILAMENT_NO_PRESS);   
			                                    }
			                                }
			                                else
			                                {
			                                    start_print_time();
			                                    pause_resum = 1;
			                                    mksReprint.mks_printer_state = MKS_RESUMING;//MKS_WORKING;
							        BUTTON_SetBmpFileName(buttonPause.btnHandle, "bmp_pause.bin",1);
								BUTTON_SetBitmapEx(buttonPause.btnHandle, 0, &bmp_struct_150, 0, 0);
								if(gCfgItems.multiple_language != 0)
								{
									BUTTON_SetText(buttonPause.btnHandle, printing_menu.pause);	
								}
			                                }
			                        }
					}
					else if(mksReprint.mks_printer_state == MKS_REPRINTING)
					{
						start_print_time();
						mksReprint.mks_printer_state = MKS_REPRINTED;
						BUTTON_SetBmpFileName(buttonPause.btnHandle, "bmp_pause.bin",1);
						BUTTON_SetBitmapEx(buttonPause.btnHandle, 0, &bmp_struct_150, 0, 0);
						if(gCfgItems.multiple_language != 0)
						{
							BUTTON_SetText(buttonPause.btnHandle, printing_menu.pause);	
						}
					#if tan_mask
						#ifdef SAVE_FROM_SD
						if(gCfgItems.pwroff_save_mode != 0)
						#endif
						{
							rePrintProcess();
						}
						#ifdef SAVE_FROM_SD
						else
						{
							if((RE_PRINT_STATUS)gCfgItems.rePrintFlag == printer_pwdwn_reprint)
							{
								rePrintProcess_pwrdwn();
							}
							else
							{
								rePrintProcess();
							}
						}
						#endif

						printerStaus = pr_working;
						start_print_time();
						MX_I2C1_Init(400000);//�ָ���ӡ�����ٶȸĳ�400k
						BUTTON_SetBmpFileName(buttonPause.btnHandle, "bmp_pause.bin",1);
						#if defined(TFT70)
						BUTTON_SetBitmapEx(buttonPause.btnHandle, 0, &bmp_struct, 48, 21);	
						#elif defined(TFT35)
						BUTTON_SetBitmapEx(buttonPause.btnHandle, 0, &bmp_struct_92, 0, 0);	
						
						#endif
					#endif
					}
				}
			}
			else if(pMsg->hWinSrc == buttonStop.btnHandle)
			{	
				if(gcode_preview_over != 1)
				{
					if(mksReprint.mks_printer_state != MKS_IDLE)
					{
						last_disp_state = PRINTING_UI;
						clear_printing();
						draw_dialog(DIALOG_TYPE_STOP);
					}
				}
			}
		}
		break;
		
	default:
		WM_DefaultProc(pMsg);
	}
}

void reset_file_info()
{
	gCurFileState.fileSize = 0;
	gCurFileState.totalRead = 0;
	gCurFileState.totalSend = 0;
	gCurFileState.bytesHaveRead = 0;
	gCurFileState.bufPoint = 0;
	gCurFileState.file_open_flag = 0;
}
#define PB_HEIGHT	25
#define SB_OFFSET	(PB_HEIGHT + 10)
#define ROW_SIZE	40
#define COL(x) (200 + 5 + ((140+5) * x))
#define COL_T(x) COL(x) + STATE_PIC_X_PIXEL
#define ROW(y) (SB_OFFSET + (ROW_SIZE*y))
#define TEXT_L(phx, phy) ui_create_std_text(COL_T(phx), ROW(phy), 80, STATE_PIC_Y_PIXEL, hPrintingWnd, 0)
#define BUTTON_L(phx, phy, file) ui_create_state_button(COL(phx), ROW(phy), hPrintingWnd, file);

static void update_pause_button() {
	char * fn;
	char * tn;
	if(gCfgItems.standby_mode==1 && mksReprint.mks_printer_state == MKS_REPRINTED && button_disp_pause_state==1) {
		fn="bmp_pause.bin";
		tn=printing_menu.pause;
	} else {
		if(
				(mksReprint.mks_printer_state == MKS_REPRINTING)
				|| (mksReprint.mks_printer_state == MKS_PAUSING)
				|| (mksReprint.mks_printer_state == MKS_PAUSED)) {
			fn="bmp_resume.bin";
			tn=printing_menu.resume;
			BUTTON_SetBmpFileName(buttonPause.btnHandle, "bmp_resume.bin",1);
		} else {
			fn="bmp_pause.bin";
			tn=printing_menu.pause;
		}
	}
	BUTTON_SetBmpFileName(buttonPause.btnHandle, fn, 1);
	if(gCfgItems.multiple_language != 0)
		BUTTON_SetText(buttonPause.btnHandle, tn);
}

#define is_dual_extruders() (mksCfg.extruders == 2 && gCfgItems.singleNozzle == 0)
//#define is_dual_extruders() (1)

void draw_printing()
{
	int dual_extrude;
	dual_extrude = is_dual_extruders();

	FRESULT res;
	int i;
	ui_reset_disp_stack(PRINTING_UI);
	ui_clear_screen();
	ui_initialize_screen_gui();

	hPrintingWnd = ui_std_window(cbPrintingWin);


	buttonTime.btnHandle = BUTTON_L(0,0,"bmp_time_state.bin");
	printTimeLeft = TEXT_L(0, 0);

	buttonExt1.btnHandle = BUTTON_L(0, 1, "bmp_ext1_state.bin");
	E1_Temp = TEXT_L(0, 1);

	if (dual_extrude) {
		buttonExt2.btnHandle = BUTTON_L(1, 1, "bmp_ext2_state.bin");
		E2_Temp = TEXT_L(1, 1);
	}
	buttonBedstate.btnHandle = BUTTON_L(0, 2, "bmp_bed_state.bin");
	Bed_Temp = TEXT_L(0, 2);

	buttonFanstate.btnHandle = BUTTON_L(1, 2, "bmp_fan_state.bin");
	Fan_Pwm = TEXT_L(1, 2);

	buttonZpos.btnHandle = BUTTON_L(1, 0, "bmp_zpos_state.bin");
	Zpos = TEXT_L(1, 0);

	printingBar = PROGBAR_CreateEx(COL(0), 0, 270, PB_HEIGHT, hPrintingWnd, WM_CF_SHOW, 0, 0);
	PROGBAR_SetBarColor(printingBar, 0, gCfgItems.printing_bar_color_left);
	PROGBAR_SetBarColor(printingBar, 1, gCfgItems.printing_bar_color_right);
	PROGBAR_SetTextColor(printingBar, 0, gCfgItems.printing_bar_text_color_left);
	PROGBAR_SetTextColor(printingBar, 1, gCfgItems.printing_bar_text_color_right);
	PROGBAR_SetFont(printingBar, &FONT_TITLE);

	buttonPause.btnHandle = ui_create_150_80_button(5, 204, hPrintingWnd, 0, 0);
	buttonStop.btnHandle = ui_create_150_80_button(165,204, hPrintingWnd, "bmp_stop.bin", printing_menu.stop);
	buttonOperat.btnHandle = ui_create_150_80_button(325,204, hPrintingWnd, "bmp_operate.bin", printing_menu.option);
	update_pause_button();
	update_printing_1s();
}


#if 0
void draw_printing()
{
	FRESULT res;
	int i;

	disp_state_stack._disp_index = 0;
	memset(disp_state_stack._disp_state, 0, sizeof(disp_state_stack._disp_state));
	disp_state_stack._disp_state[disp_state_stack._disp_index] = PRINTING_UI;

	disp_state = PRINTING_UI;

	GUI_SetBkColor(gCfgItems.background_color);
	GUI_SetColor(gCfgItems.title_color);
	GUI_Clear();


	GUI_UC_SetEncodeNone();
	GUI_SetFont(&GUI_FontHZ16);
	GUI_DispStringAt(creat_title_text(),  TITLE_XPOS, TITLE_YPOS);

    if((gCfgItems.language == LANG_SIMPLE_CHINESE)||(gCfgItems.language == LANG_COMPLEX_CHINESE))
    {
      GUI_SetFont(&GUI_FontHZ16);
      BUTTON_SetDefaultFont(&GUI_FontHZ16);
      TEXT_SetDefaultFont(&GUI_FontHZ16);  
      GUI_UC_SetEncodeNone();
    }
    else
    {
      GUI_SetFont(&FONT_TITLE);
      BUTTON_SetDefaultFont(&FONT_TITLE);
      TEXT_SetDefaultFont(&FONT_TITLE);                    
      GUI_UC_SetEncodeUTF8();
    }

    
		hPrintingWnd = WM_CreateWindow(0, titleHeight, LCD_WIDTH, imgHeight, WM_CF_SHOW, cbPrintingWin, 0);
		
		buttonExt1.btnHandle = BUTTON_CreateEx(205,100,STATE_PIC_X_PIXEL,STATE_PIC_Y_PIXEL,hPrintingWnd, BUTTON_CF_SHOW, 0, 301);
			E1_Temp = TEXT_CreateEx(205+STATE_PIC_X_PIXEL, 100, 80, STATE_PIC_Y_PIXEL, hPrintingWnd, WM_CF_SHOW, TEXT_CF_LEFT|TEXT_CF_VCENTER,  GUI_ID_TEXT0, " ");
			if(mksCfg.extruders == 2 && gCfgItems.singleNozzle == 0)
			{
				buttonExt2.btnHandle = BUTTON_CreateEx(340+10,100,STATE_PIC_X_PIXEL,STATE_PIC_Y_PIXEL,hPrintingWnd, BUTTON_CF_SHOW, 0, 302);
				E2_Temp = TEXT_CreateEx(340+STATE_PIC_X_PIXEL+10,100,80,STATE_PIC_Y_PIXEL, hPrintingWnd, WM_CF_SHOW, TEXT_CF_LEFT|TEXT_CF_VCENTER,  GUI_ID_TEXT0, " ");
			}
				buttonBedstate.btnHandle = BUTTON_CreateEx(205,150,STATE_PIC_X_PIXEL,STATE_PIC_Y_PIXEL,hPrintingWnd, BUTTON_CF_SHOW, 0, 303);
				Bed_Temp = TEXT_CreateEx(205+STATE_PIC_X_PIXEL,150,80,STATE_PIC_Y_PIXEL, hPrintingWnd, WM_CF_SHOW, TEXT_CF_LEFT|TEXT_CF_VCENTER,	GUI_ID_TEXT0, " ");
			buttonFanstate.btnHandle = BUTTON_CreateEx(340+10,150,STATE_PIC_X_PIXEL,STATE_PIC_Y_PIXEL,hPrintingWnd, BUTTON_CF_SHOW, 0, 304);
			Fan_Pwm = TEXT_CreateEx(340+STATE_PIC_X_PIXEL+10, 150, 80, STATE_PIC_Y_PIXEL, hPrintingWnd, WM_CF_SHOW, TEXT_CF_LEFT|TEXT_CF_VCENTER,  GUI_ID_TEXT0, " ");
		
			printingBar = PROGBAR_CreateEx(205,0, 270, 40, hPrintingWnd, WM_CF_SHOW, 0, 0);

			buttonTime.btnHandle = BUTTON_CreateEx(205,50,STATE_PIC_X_PIXEL,STATE_PIC_Y_PIXEL,hPrintingWnd, BUTTON_CF_SHOW, 0, 305);		
			printTimeLeft = TEXT_CreateEx(205+STATE_PIC_X_PIXEL,50,80, STATE_PIC_Y_PIXEL, hPrintingWnd, WM_CF_SHOW, TEXT_CF_LEFT|TEXT_CF_VCENTER,	GUI_ID_TEXT0, " ");
		
			buttonZpos.btnHandle = BUTTON_CreateEx(340+10,50,STATE_PIC_X_PIXEL,STATE_PIC_Y_PIXEL,hPrintingWnd, BUTTON_CF_SHOW, 0, 306);
			Zpos = TEXT_CreateEx(340+STATE_PIC_X_PIXEL+10, 50, 80, STATE_PIC_Y_PIXEL, hPrintingWnd, WM_CF_SHOW, TEXT_CF_LEFT|TEXT_CF_VCENTER, GUI_ID_TEXT0, " ");
		
			PROGBAR_SetBarColor(printingBar, 0, gCfgItems.printing_bar_color_left);
			PROGBAR_SetBarColor(printingBar, 1, gCfgItems.printing_bar_color_right);
			PROGBAR_SetTextColor(printingBar, 0, gCfgItems.printing_bar_text_color_left);
			PROGBAR_SetTextColor(printingBar, 1, gCfgItems.printing_bar_text_color_right);
			
			PROGBAR_SetFont(printingBar, &FONT_TITLE);
			
			buttonPause.btnHandle = BUTTON_CreateEx(5,204,150,80,hPrintingWnd, BUTTON_CF_SHOW, 0, 306);//alloc_win_id());
			buttonStop.btnHandle = BUTTON_CreateEx(165,204,150,80,hPrintingWnd, BUTTON_CF_SHOW, 0, 307);//alloc_win_id());
			buttonOperat.btnHandle = BUTTON_CreateEx(325,204,150,80,hPrintingWnd, BUTTON_CF_SHOW, 0, 305);//alloc_win_id());

			BUTTON_SetBmpFileName(buttonTime.btnHandle, "bmp_time_state.bin",0);
			BUTTON_SetBmpFileName(buttonZpos.btnHandle, "bmp_zpos_state.bin",0);
			

		BUTTON_SetBmpFileName(buttonExt1.btnHandle, "bmp_ext1_state.bin",0);
		if(mksCfg.extruders == 2 && gCfgItems.singleNozzle == 0)
		{
			BUTTON_SetBmpFileName(buttonExt2.btnHandle, "bmp_ext2_state.bin",0);
		}
			BUTTON_SetBmpFileName(buttonBedstate.btnHandle, "bmp_bed_state.bin",0);
		BUTTON_SetBmpFileName(buttonFanstate.btnHandle, "bmp_fan_state.bin",0);
		
		BUTTON_SetBitmapEx(buttonTime.btnHandle, 0, &bmp_struct_50, 0, 0);
		BUTTON_SetBitmapEx(buttonZpos.btnHandle, 0, &bmp_struct_50, 0, 0);				
		BUTTON_SetBitmapEx(buttonExt1.btnHandle, 0, &bmp_struct_50, 0, 0);	
		if(mksCfg.extruders == 2 && gCfgItems.singleNozzle == 0)
		{
			BUTTON_SetBitmapEx(buttonExt2.btnHandle, 0, &bmp_struct_50, 0, 0);
		}
			BUTTON_SetBitmapEx(buttonBedstate.btnHandle, 0, &bmp_struct_50, 0, 0);
		BUTTON_SetBitmapEx(buttonFanstate.btnHandle, 0, &bmp_struct_50, 0, 0);
		BUTTON_SetBitmapEx(buttonZpos.btnHandle, 0, &bmp_struct_50, 0, 0);
		if(gCfgItems.standby_mode==1 && mksReprint.mks_printer_state == MKS_REPRINTED && button_disp_pause_state==1)
		{
			BUTTON_SetBmpFileName(buttonPause.btnHandle, "bmp_pause.bin",1);
		}
		else
		{
			if((mksReprint.mks_printer_state == MKS_REPRINTING)
				||(mksReprint.mks_printer_state == MKS_PAUSING)
				||(mksReprint.mks_printer_state == MKS_PAUSED))
			{
				BUTTON_SetBmpFileName(buttonPause.btnHandle, "bmp_resume.bin",1);
			}
			else
			{
				BUTTON_SetBmpFileName(buttonPause.btnHandle, "bmp_pause.bin",1);
			}
		}
		BUTTON_SetBmpFileName(buttonStop.btnHandle, "bmp_stop.bin",1);
		BUTTON_SetBmpFileName(buttonOperat.btnHandle, "bmp_operate.bin",1);
		
		BUTTON_SetBkColor(buttonPause.btnHandle, BUTTON_CI_PRESSED, gCfgItems.btn_color);
		BUTTON_SetBkColor(buttonPause.btnHandle, BUTTON_CI_UNPRESSED, gCfgItems.btn_color);
		BUTTON_SetBkColor(buttonStop.btnHandle, BUTTON_CI_PRESSED, gCfgItems.btn_color);
		BUTTON_SetBkColor(buttonStop.btnHandle, BUTTON_CI_UNPRESSED, gCfgItems.btn_color);
		BUTTON_SetBkColor(buttonOperat.btnHandle, BUTTON_CI_PRESSED, gCfgItems.btn_color);
		BUTTON_SetBkColor(buttonOperat.btnHandle, BUTTON_CI_UNPRESSED, gCfgItems.btn_color);
		
		BUTTON_SetTextColor(buttonPause.btnHandle, BUTTON_CI_PRESSED, gCfgItems.btn_textcolor);
		BUTTON_SetTextColor(buttonPause.btnHandle, BUTTON_CI_UNPRESSED, gCfgItems.btn_textcolor);
		BUTTON_SetTextColor(buttonStop.btnHandle, BUTTON_CI_PRESSED, gCfgItems.btn_textcolor);
		BUTTON_SetTextColor(buttonStop.btnHandle, BUTTON_CI_UNPRESSED, gCfgItems.btn_textcolor);
		BUTTON_SetTextColor(buttonOperat.btnHandle, BUTTON_CI_PRESSED, gCfgItems.btn_textcolor);
		BUTTON_SetTextColor(buttonOperat.btnHandle, BUTTON_CI_UNPRESSED, gCfgItems.btn_textcolor);

	
		BUTTON_SetBitmapEx(buttonPause.btnHandle, 0, &bmp_struct_150, 0, 0); 
		BUTTON_SetBitmapEx(buttonStop.btnHandle, 0, &bmp_struct_150, 0, 0);
		BUTTON_SetBitmapEx(buttonOperat.btnHandle, 0, &bmp_struct_150, 0, 0);
	
		TEXT_SetBkColor(Zpos,  gCfgItems.background_color);
		TEXT_SetTextColor(Zpos, gCfgItems.title_color);
		TEXT_SetBkColor(printTimeLeft,	gCfgItems.background_color);
		TEXT_SetTextColor(printTimeLeft, gCfgItems.title_color);	
		TEXT_SetBkColor(E1_Temp,  gCfgItems.background_color);
		TEXT_SetTextColor(E1_Temp, gCfgItems.title_color);
        if(mksCfg.extruders == 2 && gCfgItems.singleNozzle == 0){
		    TEXT_SetBkColor(E2_Temp,  gCfgItems.background_color);
		    TEXT_SetTextColor(E2_Temp, gCfgItems.title_color);
        }
		TEXT_SetBkColor(Bed_Temp,  gCfgItems.background_color);
		TEXT_SetTextColor(Bed_Temp, gCfgItems.title_color);
		TEXT_SetBkColor(Fan_Pwm,  gCfgItems.background_color);
		TEXT_SetTextColor(Fan_Pwm, gCfgItems.title_color);
		
		BUTTON_SetTextAlign(buttonOperat.btnHandle,GUI_TA_VCENTER | GUI_CUSTOM_POS);
		BUTTON_SetTextAlign(buttonStop.btnHandle,GUI_TA_VCENTER | GUI_CUSTOM_POS);
		BUTTON_SetTextAlign(buttonPause.btnHandle,GUI_TA_VCENTER | GUI_CUSTOM_POS);		
		if(gCfgItems.multiple_language != 0)
		{
			BUTTON_SetText(buttonOperat.btnHandle, printing_menu.option);
			BUTTON_SetText(buttonStop.btnHandle,printing_menu.stop);
            if((mksReprint.mks_printer_state == MKS_REPRINTING)
                ||(mksReprint.mks_printer_state == MKS_PAUSING)
                ||(mksReprint.mks_printer_state == MKS_PAUSED))

                BUTTON_SetText(buttonPause.btnHandle, printing_menu.resume);
			else
				BUTTON_SetText(buttonPause.btnHandle, printing_menu.pause);		
		}
		
		disp_sprayer_tem_printing();
		disp_bed_temp_printing();
		disp_fan_speed_printing();
}

#endif

extern uint32_t rcv_ok_nums;

void print_time_to_str(PRINT_TIME * pt, char * buf) {
	sprintf(buf, "%d%d:%d%d:%d%d", pt->hours/10, pt->hours%10, pt->minutes/10, pt->minutes%10,  pt->seconds/10, pt->seconds%10);
}

static int8_t fan_dir = 0;


void disp_printing_speed()
{
	char buf[30] = {0};
	
	TEXT_SetBkColor(Printing_speed,0x000000);
	TEXT_SetTextColor(Printing_speed,gCfgItems.title_color);


	memset(buf, 0, sizeof(buf));
	sprintf(buf, "%3d%%", gCfgItems.printSpeed);
	TEXT_SetText(Printing_speed, buf);
}

void setProBarValue(int added)
{
	if(added <= 0)
		return;
}


void setProBarRate()
{
	int rate;
	volatile long long rate_tmp_r;
	
	if(from_flash_pic != 1) {
		rate_tmp_r =(long long) card.sdpos * 100;
		rate = rate_tmp_r / card.filesize;
	} else {
		rate_tmp_r =(long long)card.sdpos;
		rate = (rate_tmp_r-(PREVIEW_SIZE+To_pre_view))* 100 / (card.filesize-(PREVIEW_SIZE+To_pre_view));
	}
	gCurFileState.totalSend = rate;
	if(rate <= 0)
		return;


	if(disp_state == PRINTING_UI) {

		char buf[30] = {0};
		memset(buf, 0, sizeof(buf));

		if (rate==0) {
			strcpy(buf, "??:??:??");
		} else {
			int total = print_time.seconds + print_time.minutes * 60 + print_time.hours * 3600;
			total = (total * 100 / rate) - total;
			PRINT_TIME pt;
			pt.seconds = total % 60;
			total = total / 60;
			pt.minutes = total % 60;
			pt.hours = total / 60;
			print_time_to_str(&pt, buf);
		}
	    SERIAL_ECHOPAIR("PB_V:", rate);
	    SERIAL_EOL();
	    SERIAL_ECHOPAIR("PB_T", buf);
	    SERIAL_EOL();

		PROGBAR_SetValue(printingBar, rate);
		PROGBAR_SetText(printingBar, buf);

		if((mksReprint.mks_printer_state == MKS_IDLE)  &&  (rate == 100) )
		{
			//memset((char *)gCfgItems.z_display_pos,0,sizeof(gCfgItems.z_display_pos));
			if(once_flag == 0)
			{					
				stop_print_time();				
				#if VERSION_WITH_PIC	
				flash_preview_begin = 0;
				default_preview_flg = 0;
				clear_printing();
				draw_dialog(DIALOG_TYPE_FINISH_PRINT);
				if(gCfgItems.multiple_language != 0)
				{
					BUTTON_SetText(buttonOperat.btnHandle, common_menu.text_back);	
				}				
				#endif
				once_flag = 1;
			}					
		}
		
	}	
}

void update_printing_1s(void) {
	char buf[30] = {0};
	memset(buf, 0, sizeof(buf));
	sprintf(buf, printing_menu.temp1, (int)thermalManager.current_temperature[0], (int)thermalManager.target_temperature[0]);
	ui_set_text_value(E1_Temp, buf);
	if(is_dual_extruders()){
		memset(buf,0,sizeof(buf));
		sprintf(buf, printing_menu.temp2, (int)thermalManager.current_temperature[1], (int)thermalManager.target_temperature[1]);
		ui_set_text_value(E2_Temp, buf);
	}

	memset(buf, 0, sizeof(buf));
	sprintf(buf, printing_menu.bed_temp, (int)thermalManager.current_temperature_bed,  (int)thermalManager.target_temperature_bed);
	ui_set_text_value(Bed_Temp, buf);


	memset(buf, 0, sizeof(buf));
	print_time_to_str(&print_time, buf);
	ui_set_text_value(printTimeLeft, buf);

	memset(buf, 0, sizeof(buf));
	sprintf(buf,"%.3f",current_position[Z_AXIS]);
	ui_set_text_value(Zpos, buf);

	memset(buf, 0, sizeof(buf));
	sprintf(buf, "%3d", fanSpeeds[0]);
	ui_set_text_value(Fan_Pwm, buf);
}

void refresh_printing() {
	if (is_ui_timing(F_UI_TIMING_SEC)) {
		ui_timing_clear(F_UI_TIMING_SEC);
		update_printing_1s();

		if(gcode_preview_over == 0)
			setProBarRate();
	}
}

void clear_printing()
{
	ui_drop_window(hPrintingWnd);
}
