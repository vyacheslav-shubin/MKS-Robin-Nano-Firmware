#include "GUI.h"
#include "BUTTON.h"
#include "TEXT.h"
#include "draw_ui.h"
#include "ui_tools.h"

#include "draw_manual_leveling.h"
#include "spi_flash.h"
#include "Marlin.h"
#include "mks_reprint.h"

static GUI_HWIN hLevelingWnd;

uint8_t leveling_first_time=0;
extern char cmd_code[201];
extern char x[1];
extern volatile char *codebufpoint;

extern GUI_FLASH const GUI_FONT GUI_FontHZ_fontHz18;

static BUTTON_Handle buttonNext,buttonRet,buttonAutoleveling,buttonleveling1,buttonleveling2,buttonleveling3,buttonleveling4,buttonleveling5;
static BUTTON_Handle buttonAllZero, buttonZZero;
static TEXT_Handle textLevelingMsg;

uint8_t next_cnt=0;
uint8_t leveling_finish_flag = 0;
uint8_t leveling_start_flg = 0;

void manual_leveling(uint8_t point)
{
	char level_buf[80]={0};

	memset(level_buf,0,sizeof(level_buf));
	sprintf(level_buf,"G91 G1 Z10 F%d\n",gCfgItems.leveling_z_speed);
	enqueue_and_echo_commands_P(PSTR(level_buf));

	memset(level_buf,0,sizeof(level_buf));
	sprintf(level_buf,"G90 G1 X%d Y%d F%d\n", gCfgItems.leveling_points[point].x, gCfgItems.leveling_points[point].y, gCfgItems.leveling_xy_speed);
	enqueue_and_echo_commands_P(PSTR(level_buf));

	enqueue_and_echo_commands_P(PSTR("G1 Z0"));
}

static void cbLevelingWin(WM_MESSAGE * pMsg) {

	char level_buf[80]={0};
	
	switch (pMsg->MsgId)
	{
		case WM_PAINT:
			break;
		case WM_TOUCH:
			break;
		case WM_TOUCH_CHILD:
			break;
		case WM_NOTIFY_PARENT:
			if(pMsg->Data.v == WM_NOTIFICATION_RELEASED) {
				if(pMsg->hWinSrc == buttonRet) {
					leveling_first_time=0;
					Clear_Leveling();
					draw_return_ui();
				} else if(pMsg->hWinSrc == buttonleveling1) {
					manual_leveling(0);
				} else if(pMsg->hWinSrc == buttonleveling2) {
					manual_leveling(1);
				} else if(pMsg->hWinSrc == buttonleveling3){
					manual_leveling(2);
				} else if(pMsg->hWinSrc == buttonleveling4) {
					manual_leveling(3);
				} else if(pMsg->hWinSrc == buttonleveling5) {
					manual_leveling(4);
				}				
			}
			break;
		default:
			WM_DefaultProc(pMsg);
	}
}

char * leveliling_button_title(char * buf, uint8_t sz, uint8_t point) {
	memset(buf,0,sz);
	sprintf(buf,"X:%d Y:%d",gCfgItems.leveling_points[point].x, gCfgItems.leveling_points[point].y);
	return buf;
}

void draw_leveling()
{
	ui_push_disp_stack(LEVELING_UI);
	ui_clear_screen();
	hLevelingWnd = ui_std_window(cbLevelingWin);

	char buf[80]={0};

	buttonleveling1 = ui_std_button(0, 1, hLevelingWnd, "bmp_leveling1.bin", leveliling_button_title(buf, sizeof(buf), 0));
	buttonleveling2 = ui_std_button(1, 1, hLevelingWnd, "bmp_leveling2.bin", leveliling_button_title(buf, sizeof(buf), 1));
	buttonleveling3 = ui_std_button(1, 0, hLevelingWnd, "bmp_leveling3.bin", leveliling_button_title(buf, sizeof(buf), 2));
	buttonleveling4 = ui_std_button(0, 0, hLevelingWnd, "bmp_leveling4.bin", leveliling_button_title(buf, sizeof(buf), 3));
	buttonleveling5 = ui_std_button(2, 1, hLevelingWnd, "bmp_leveling5.bin", leveliling_button_title(buf, sizeof(buf), 4));

	buttonAllZero = ui_std_button(3, 0, hLevelingWnd, "bmp_zero.bin", home_menu.home_all);
	buttonZZero = ui_std_button(2, 0, hLevelingWnd, "bmp_zeroZ.bin", home_menu.home_z);
	buttonRet = ui_std_button(3, 1, hLevelingWnd, "bmp_return.bin", common_menu.text_back);
}

void Clear_Leveling()
{
	ui_drop_window(hLevelingWnd);
}
