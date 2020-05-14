#ifndef _PIC_H_
#define _PIC_H_

#if defined(__cplusplus)
extern "C" { 
#endif
#include "stdint.h"

#define VERSION_WITH_PIC	1
//extern unsigned char bmp_public_buf[16 * 1024];
extern unsigned char bmp_public_buf[8 * 1024];
extern GUI_BITMAP bmp_struct;
extern GUI_BITMAP bmp_struct_92;
extern GUI_BITMAP bmp_struct_50;
extern GUI_BITMAP bmp_struct_100;
extern GUI_BITMAP bmp_struct_150;
extern GUI_BITMAP bmp_struct_117x25;

extern GUI_BITMAP bmp_struct70X40;
extern GUI_BITMAP bmp_struct90X40;
extern GUI_BITMAP bmp_struct26X26;
extern GUI_BITMAP bmp_struct90X30;
extern GUI_BITMAP bmp_struct68X40;
extern GUI_BITMAP bmp_struct68X90;
extern GUI_BITMAP bmp_struct144X40;
extern GUI_BITMAP bmp_struct70X28;

extern GUI_BITMAP bmp_struct10x17;
extern GUI_BITMAP bmp_struct_50x50;
extern GUI_BITMAP bmp_struct_141x60;
extern uint8_t drawicon_preview(char *path,int xsize_small,int ysize_small,uint8_t part);


#if defined(__cplusplus)
}
#endif

#endif


