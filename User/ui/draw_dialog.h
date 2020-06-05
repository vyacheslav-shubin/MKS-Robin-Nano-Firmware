#ifndef _DRAW_DIALOG_H_
#define _DRAW_DIALOG_H_

#define DIALOG_TYPE_PRINT_FILE					1
#define DIALOG_TYPE_REPRINT_NO_FILE				2

#define DIALOG_TYPE_M80_FAIL					3
#define DIALOG_TYPE_MESSEGE_ERR1				4

#define DIALOG_TYPE_UPDATE_ESP_FIRMARE			5
#define DIALOG_TYPE_UPDATE_ESP_DATA				6
#define DIALOG_TYPE_UPLOAD_FILE					7
#define DIALOG_TYPE_UNBIND						8


#define DIALOG_TYPE_FILE_LOADING				17

#define DIALOG_TYPE_FILAMENT_NO_PRESS			18

#if defined(__cplusplus)
extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif

extern void refresh_dialog(void);
extern void draw_dialog(uint8_t type);
extern void Clear_dialog(void);
extern void wifi_scan_handle();
#if defined(__cplusplus)
}    /* Make sure we have C-declarations in C++ programs */
#endif


#endif
