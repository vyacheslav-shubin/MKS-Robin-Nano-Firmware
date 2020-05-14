/*
 * integration.c
 *
 *  Created on: May 13, 2020
 *      Author: shubin
 */

#include "ui_tools.h"
#include "mks_reprint.h"
#include "temperature.h"
#include "integration.h"
#include "Configuration_adv.h"

namespace shUI {
	void babystep(char * axe, float size) {
		sprintf(ui_buf1_80, "M290 %s%.3f", axe,  size);
		excute_m290(ui_buf1_80);
	}
	float babystepGetZ() {
		float ofs = zprobe_zoffset;
		if (abs(ofs)<0.001)
			ofs = 0;
		return ofs;
	}

	void saveConfig() {
		excute_m500();
	}

	char isDualExtruders() {
		return (mksCfg.extruders == 2) && (gCfgItems.singleNozzle == 0);
	}

	float _fix_sprayer_temp_bound(float value) {
		if (value<0) value = 0;
		float limit = mksCfg.heater_0_maxtemp - (WATCH_TEMP_INCREASE + TEMP_HYSTERESIS + 1);
		if (value > limit)
			value = limit;
		return value;
	}

	void addSprayerTemperature(unsigned char index, float value) {
		setSprayerTemperature(index,_fix_sprayer_temp_bound(thermalManager.target_temperature[index] + value));
	}

	void getSprayerTemperature(unsigned char index, SPRAYER_TEMP * temp) {
		temp->current = thermalManager.current_temperature[index];
		temp->target = thermalManager.target_temperature[index];
	}

	void setSprayerTemperature(unsigned char index, float value) {
		thermalManager.target_temperature[index] = _fix_sprayer_temp_bound(value);
		thermalManager.start_watching_heater(index);
	}


	short _fix_bed_temp_bound(short value) {
		if (value<0) value = 0;
		float limit = mksCfg.bed_maxtemp - (WATCH_BED_TEMP_INCREASE + TEMP_BED_HYSTERESIS + 1);
		if (value > limit)
			value = limit;
		return value;
	}

	void addBedTemperature(short value) {
		setBedTemperature(thermalManager.target_temperature_bed + value);
	}

	void setBedTemperature(short value) {
		thermalManager.target_temperature_bed = _fix_bed_temp_bound(value);
		thermalManager.start_watching_bed();
	}

	void getBedTemperature(BED_TEMP * temp) {
		temp->current = thermalManager.current_temperature_bed;
		temp->target = thermalManager.target_temperature_bed;
	}

	void pushGcode(const char * gcode) {
		enqueue_and_echo_commands_P(gcode);
	}

	void getCurrentPosition(CURRENT_POSITION * value) {
		value->x = current_position[X_AXIS];
		value->y = current_position[Y_AXIS];
		value->z = current_position[Z_AXIS];
		value->e = current_position[E_AXIS];
	}

	unsigned char isManualLeveling() {
		return gCfgItems.leveling_mode == 0;
	}

	unsigned char isMeshLeveling() {
		return BED_LEVELING_METHOD & MESH_BED_LEVELING;
	}

	void doCustomLeveling() {
	    //SPI_FLASH_BufferRead((u8 *)cmd_code,BUTTON_AUTOLEVELING_ADDR,201);
	    //codebufpoint = cmd_code;
	}

	void doFilamentMove(unsigned char extruder, int size, int speed) {
		active_extruder = extruder;
		sprintf(ui_buf1_80, "G91 G1 E%d F%d\n", size, speed);
		enqueue_and_echo_commands_P(ui_buf1_80);
		enqueue_and_echo_commands_P("G90");
	}

	short getFilamentExtrudeMinTempereture() {
		return mksCfg.extrude_mintemp;
	}

	extern short getFilamentLoadUnloadMinTempereture(char direction) {
		return direction==-1?gCfgItems.filament_unload_limit_temper:gCfgItems.filament_load_limit_temper;
	}


	void getFilamentLoadUnloadDistance(char direction, MOVE_DISTANCE * distance) {
		if (direction==-1) {
			distance->distance = -(float)gCfgItems.filamentchange_unload_length;
			distance->speed = gCfgItems.filamentchange_unload_speed;
		} else {
			distance->distance = gCfgItems.filamentchange_load_length;
			distance->speed = gCfgItems.filamentchange_load_speed;
		}
	}

	char isIdle() {
		return mksReprint.mks_printer_state == MKS_IDLE;
	}

	char isPaused() {
		return (mksReprint.mks_printer_state == MKS_PAUSING)
				|| (mksReprint.mks_printer_state == MKS_PAUSED)
				|| (mksReprint.mks_printer_state ==MKS_REPRINTING)
				||(mksReprint.mks_printer_state ==MKS_REPRINTED);
	}

}


