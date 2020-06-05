#error unused

#ifndef _DRAW_WIFI_LIST_H_
#define _DRAW_WIFI_LIST_H_

#if defined(__cplusplus)
extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif

extern void draw_Wifi_list();
extern void Clear_Wifi_list();
extern void disp_wifi_list(void);
extern void cutWifiName(char *name, int len,char *outStr);

#if defined(__cplusplus)
}     /* Make sure we have C-declarations in C++ programs */
#endif

#endif
