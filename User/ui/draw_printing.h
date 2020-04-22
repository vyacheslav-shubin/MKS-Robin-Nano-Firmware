#ifndef _DRAW_PRINTING_H_
#define _DRAW_PRINTING_H_
#include "stdint.h"


#if defined(__cplusplus)
extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif
extern int once_flag;

extern void draw_printing();
extern void clear_printing();
extern void refresh_printing();


extern void setProBarValue(int added);
#if tan_mask
extern void setProBarRate(int rate);
#else
extern void setProBarRate();
#endif


#if defined(__cplusplus)
}     /* Make sure we have C-declarations in C++ programs */
#endif

#endif

