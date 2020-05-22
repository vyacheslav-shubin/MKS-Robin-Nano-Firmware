/*
 * backui.cpp
 *
 *  Created on: May 21, 2020
 *      Author: shubin
 */

#include "Application.h"
#include "backui.h"

char ui_app_touch(u8 action) {
	return ui_app.touch(action);
}


