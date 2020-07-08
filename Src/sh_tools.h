#ifndef _SH_TOOLS_H
#define _SH_TOOLS_H

#include "stdint.h"

#ifdef __cplusplus
 extern "C" {
#endif

#define POWER_CONTROL_TEMPERATURE_MAX           80

#define POWER_CONTROL_SUNCIDE                   (1<<0)
#define POWER_CONTROL_WAIT_HOTEND               (1<<1)
#define POWER_CONTROL_HARDWARE_AS_SOFTWARE      (1<<2)


extern unsigned char is_filament_fail(void);

#ifdef __cplusplus
}
#endif
#endif
