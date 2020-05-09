/*
 * Application.cpp
 *
 *  Created on: May 8, 2020
 *      Author: shubin
 */

#include "Application.h"
#include "draw_ui.h"
#include "ui_tools.h"
#include "MainUI.h"

Application ui_app;

void Application::showMainWidget() {
	main_ui.show();
}

void Application::push(Widget * widget) {
	if (widget==&main_ui)
		reset_stack(widget);
	else {
		ui_push_disp_stack(widget->id);
	}
}

void Application::reset_stack(Widget * widget) {
	ui_reset_disp_stack(widget->id);
}

void Application::back_ui() {
	draw_return_ui();
}
