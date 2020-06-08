#ifndef _DRAW_DIALOG_H_
#define _DRAW_DIALOG_H_

#define DIALOG_TYPE_REPRINT_NO_FILE				2

#define DIALOG_TYPE_MESSEGE_ERR1				4

#define DIALOG_TYPE_UPLOAD_FILE					7
#define DIALOG_TYPE_UNBIND						8


#if defined(__cplusplus)
extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif

extern void draw_dialog(uint8_t type);
extern void Clear_dialog(void);

#if defined(__cplusplus)
}    /* Make sure we have C-declarations in C++ programs */
#endif


#endif
