/*
 * PrintProcessUI.h
 *
 *  Created on: May 19, 2020
 *      Author: shubin
 */

#ifndef USER_UI_WIDGETS_FILEINFOBASEUI_H_
#define USER_UI_WIDGETS_FILEINFOBASEUI_H_

#include "StdWidget.h"

class FileInfoBaseUI  : public StdWidget{
protected:
	unsigned char preview_done = 0;

	virtual void createControls() {
		this->preview_done = 0;
	}
public:
	virtual void refresh();
	FileInfoBaseUI(DISP_STATE id) : StdWidget(id) {};
};

#endif /* USER_UI_WIDGETS_FILEINFOBASEUI_H_ */
