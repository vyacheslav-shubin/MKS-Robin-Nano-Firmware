/******************** (C) COPYRIGHT 2009 www.armjishu.com ************************
* File Name          : ARMJISHU_TouchScreen_ADS7843.c
* Author             : www.armjishu.com Team
* Version            : V3.0.1
* Date               : 03/20/2010
* Description        : 
                        ADS7843_CS   is PB1
                        ADS7843_INT  is PC1
*******************************************************************************/
#include "mks_touch_screen.h"
#include "stm32f1xx.h" //skyblue 2006-12-13
#include <stdio.h>
#include "stm32f1xx_it.h"
#include "ili9320.h"
#include "tim.h"
#include "stm32f10x_spi.h"
#include "spi_flash.h"
#include "mks_fastio.h"
#include "backui.h"

extern u16 DeviceCode;
static int16_t x_touch,  y_touch;
static int32_t touch_time = 0;
static uint8_t lastTouchState = PEN_UP;

#define  times  4

extern u16 DeviceCode;
#define	CHX 	0x90//0x90 	//ͨ��Y+��ѡ�������	//0x94
#define	CHY 	0xD0//0xd0	//ͨ��X+��ѡ�������	//0xD4

void ADS7843_CS_config(void){}
static void ADS7843_INT_config(void) { }
static void ADS7843_INT_EXIT_Init(void) {}


/*******************************************************************************
* Function Name  : InterruptConfig
* Description    : Configures the used IRQ Channels and sets their priority.NVIC_Configuration
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void ADS7843_InterruptConfig(void){ }


static void LCD_BIG_POINT(u16 xScreen, u16 yScreen) {
   ili9320_SetPoint(xScreen, yScreen, Magenta);
   ili9320_SetPoint(xScreen-1, yScreen, Magenta);
   ili9320_SetPoint(xScreen+1, yScreen, Magenta);
   ili9320_SetPoint(xScreen, yScreen-1, Magenta);
   ili9320_SetPoint(xScreen, yScreen+1, Magenta);
   ili9320_SetPoint(xScreen-1, yScreen-1, Magenta);
   ili9320_SetPoint(xScreen-1, yScreen+1, Magenta);
   ili9320_SetPoint(xScreen+1, yScreen-1, Magenta);
   ili9320_SetPoint(xScreen+1, yScreen+1, Magenta);
}

/*=====================================================================*/
u16 _AD2Y(u16 adx) {
	u16 sx=0;
	int r=adx-200;
	r*=240;
	sx=r/(4000-280);
	if (sx<=0 || sx>240)
		return 0;
	return sx;
}


u16 _AD2X(u16 ady){
  u16 sy=0;
  int r = ady - 260;
  r *= 320;
  sy=r/(3960 - 360);
  if (sy<=0 || sy>320)
    return 0;
  return sy;
}

void ADS7843_Init(void){ }
/*=====================================================================*/
/*=====================================================================*/


void SPI1_Config(void){ }

/*=====================================================================*/
void SPI1_Init_For_Byte(void) { }

/*=====================================================================*/
static void ADS7843_SPI_Start( void ) { }

/*=====================================================================*/
static void SPI_MOSI(u8 data) { }
/*=====================================================================*/
static void ADS7843_WrCmd(u8 cmd) { }

/*=====================================================================*/
static u16 ADS7843_Read(void) { }

/*=====================================================================*/

static void ADS7843_Rd_Addata(u16 *X_Addata,u16 *Y_Addata) {

	u16		i,j,k,x_addata[times],y_addata[times];
    int result;

        
	for(i=0;i<times;i++) {
		SPI2_SetSpeed(SPI_BaudRatePrescaler_64);
		ADS7843_CS = 0;
		SPI2_ReadWriteByte(CHX);
		y_addata[i] = SPI2_ReadWrite2Bytes();
		SPI2_ReadWriteByte(CHY);
		x_addata[i] = SPI2_ReadWrite2Bytes();
		ADS7843_CS = 1;
		SPI2_SetSpeed(SPI_BaudRatePrescaler_4);
	}

	result = x_addata[0];
	for(i=0;i<times;i++) {
		for(j = i + 1; j < times; j++) {
			if(x_addata[j] > x_addata[i]) {
				k = x_addata[j];
				x_addata[j] = x_addata[i];
				x_addata[i] = k;
			}
		}
	}
	if(x_addata[times / 2 -1] - x_addata[times / 2 ] > 50) {
		*X_Addata = 0;
		*Y_Addata = 0;
		return ;
	}
	*X_Addata = (x_addata[times / 2 -1] + x_addata[times / 2 ]) /2;
	
	result = y_addata[0];
	for(i=0;i<times;i++) {
		for(j = i + 1; j < times; j++) {
			if(y_addata[j] > y_addata[i]) {
				k = y_addata[j];
				y_addata[j] = y_addata[i];
				y_addata[i] = k;
			}
		}
	}
	
	if(y_addata[times / 2 -1] - y_addata[times / 2 ] > 50) {
		*X_Addata = 0;
		*Y_Addata = 0;
		return ;
    }
	*Y_Addata = (y_addata[times / 2 -1] + y_addata[times / 2 ]) /2;
}

/*=====================================================================*/
u16 distence(u16 data1,u16 data2) {
    if((data1 > data2) && (data1 > data2 + 10))
        return 0;

    if((data2 > data2) && (data2 > data2 + 10))
        return 0;
    return 1;    
}

/*=====================================================================*/
extern uint32_t TSC_Value_X;
extern uint32_t TSC_Value_Y;

void ARMJISHU_TouchScreen_ADS7843(void) { }

#define ADC_VALID_OFFSET	10

static uint8_t	TOUCH_PressValid(uint16_t _usX, uint16_t _usY) {
	if ((_usX <= ADC_VALID_OFFSET) || (_usY <= ADC_VALID_OFFSET)
		|| (_usX >= 4095 - ADC_VALID_OFFSET)
		|| (_usY >= 4095 - ADC_VALID_OFFSET))
	{
		return 0;
	} else {
		return 1;
	}
}

void getTouchEvent(TOUCH_EVENT *pTouchEvent) {
	uint32_t tmpTime, diffTime = 0;
	int16_t x, y;
	tmpTime = getTick();
	diffTime = getTickDiff(tmpTime, touch_time);
	if(diffTime > 20) {
		x = x_touch;
		y = y_touch;
		ADS7843_Rd_Addata(&x_touch, &y_touch);
		if(TOUCH_PressValid(x_touch, y_touch)) {
			if(abs(x - x_touch) > 50 || abs(y-y_touch) > 50)
				return;
			if (ui_app_touch(PEN_DOWN)) {
				pTouchEvent->x  = (x + x_touch) >> 1;
				pTouchEvent->y = (y + y_touch) >> 1;
				pTouchEvent->state = PEN_DOWN;
				touch_time = tmpTime;
			}
		} else {
			if (ui_app_touch(PEN_UP))
				pTouchEvent->state = PEN_UP;
		}
	}
}
/*=====================================================================*/
