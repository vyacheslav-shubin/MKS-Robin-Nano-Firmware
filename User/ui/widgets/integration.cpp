/*
 * integration.c
 *
 *  Created on: May 13, 2020
 *      Author: shubin
 */

#include <ili9320.h>
#include "ui_tools.h"
#include "sh_tools.h"
#include "planner.h"
#include "mks_reprint.h"
#include "pic_manager.h"
#include "temperature.h"
#include "spi_flash.h"
#include "integration.h"
#include "Configuration_adv.h"
#include "configuration_store.h"

static u8 DAYS_PER_MOUNTH[]={31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

namespace shUI {

    unsigned int now = 0;

    void time_plus(unsigned short sec) {
        now+=sec;
    }

    void set_unix_time(unsigned int time) {
        now = time;
    }

    unsigned char isLeapYear(u16 year) {
        return ((year%400==0) || ((year%100!=0) && (year%4==0)));
    }

    char hasTime() {
        return now!=0;
    }

    void getTime(shUI::DateTime *time) {
        u32 lt=now+(gCfgItems.time_offset*60);
        memset(time, 0, sizeof(shUI::DateTime));
        time->sec = lt%60;
        lt/=60;
        time->min = lt%60;
        lt/=60;
        time->hour = lt%24;
        lt/=24;
        time->year=1970;
        time->wd=0;
        u32 d=lt;
        while (1) {
            u16 dc;
            dc=(isLeapYear(time->year))?366:365;
            if (d<dc)
                break;
            d-=dc;
            time->year++;
        }
        u8 i;
        bool ly=isLeapYear(time->year);
        time->month=1;
        for (i=0;i<12;i++) {
            u8 dc=DAYS_PER_MOUNTH[i];
            if ((i==1) && ly)
                dc++;
            if (d<dc) break;
            d-=dc;
            time->month++;
        }
        time->day=1+d;
    }

    char getTimeStr(char * buffer) {
        shUI::DateTime time;
        getTime(&time);
        sprintf(buffer, "%d%d:%d%d:%d%d", time.hour/10, time.hour%10, time.min/10, time.min%10,  time.sec/10, time.sec%10);
    }


    void powerOff() {
        SERIAL_ECHOLN("POWER OFF");
        GUI_Clear();
        enqueue_and_echo_commands_P(PSTR("M81"));
    }

    void powerOffForce() {
        SERIAL_ECHOLN("FORCE POWER OFF");
        lcd_light_off();
        suicide();
    }

    void suicide() {
        if (is_power_control_configured()) {
            MKS_PW_OFF_OP = (gCfgItems.power_control_flags & POWER_CONTROL_LOCK) ? 0 : 1;
        } else
            MKS_PW_OFF_OP = 0;
    }

    void power_hold() {
        if (is_power_control_configured()) {
            MKS_PW_OFF_OP = (gCfgItems.power_control_flags & POWER_CONTROL_LOCK) ? 1 : 0;
        } else
            MKS_PW_OFF_OP = 1;
    }

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
        settings.save();
	}

    extern void loadConfig() {
        settings.load();
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

	short _fix_feedrate_percentage(short value) {
		if (value > MAX_EXT_SPEED_PERCENT)
			value = MAX_EXT_SPEED_PERCENT;
		else if (value < MIN_EXT_SPEED_PERCENT)
			value = MIN_EXT_SPEED_PERCENT;
		return value;
	}

	short getFeedratePercentage() {
		return feedrate_percentage;
	}

	void setFeedratePercentage(short value) {
		feedrate_percentage = _fix_feedrate_percentage(value);
	}

	void addFeedratePercentage(short value) {
		setFeedratePercentage(getFeedratePercentage() + value);
	}

	short _fix_flow(short value) {
		if (value > MAX_EXT_SPEED_PERCENT)
			value = MAX_EXT_SPEED_PERCENT;
		else if (value < MIN_EXT_SPEED_PERCENT)
			value = MIN_EXT_SPEED_PERCENT;
		return value;
	}

	short getFlowPercentage(char index) {
		return planner.flow_percentage[index];
	}

	void addFlowPercentage(char index, short value) {
		setFlowPercentage(index, getFlowPercentage(index) + value);
	}

	void setFlowPercentage(char index, short value) {
		planner.flow_percentage[index] = _fix_flow(value);
		planner.e_factor[0]= planner.flow_percentage[0] * 0.01;
	}


	void pushGcode(const char * gcode) {
		enqueue_and_echo_commands_P(gcode);
	}

    void injectGcode(const char * gcode) {
        MYSERIAL.inject(cmd_code);
    }

	void getCurrentPosition(CURRENT_POSITION * value) {
		value->x = current_position[X_AXIS];
		value->y = current_position[Y_AXIS];
		value->z = current_position[Z_AXIS];
		value->e = filament_counter;//current_position[E_AXIS];
	}

	unsigned char isManualLeveling() {
		return gCfgItems.leveling_mode == 0;
	}

	unsigned char isMeshLeveling() {
		return BED_LEVELING_METHOD == MESH_BED_LEVELING;
	}

	void doCustomLeveling() {
        memset(cmd_code, 0, sizeof(cmd_code));
	    SPI_FLASH_BufferRead((u8 *)cmd_code, BUTTON_AUTOLEVELING_ADDR, 200);
        MYSERIAL.inject(cmd_code);
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
        switch(direction) {
            case 1: return gCfgItems.filamentchange.load.temper;
            case -1: return gCfgItems.filamentchange.unload.temper;
            default:
                return (gCfgItems.filamentchange.load.temper>gCfgItems.filamentchange.unload.temper)
                ?
                    gCfgItems.filamentchange.load.temper
                    : gCfgItems.filamentchange.unload.temper;
        }
	}

	void getFilamentLoadUnloadDistance(char direction, MOVE_DISTANCE * distance) {
		if (direction==-1) {
			distance->distance = -(float)gCfgItems.filamentchange.unload.length;
			distance->speed = gCfgItems.filamentchange.unload.speed;
		} else {
			distance->distance = gCfgItems.filamentchange.load.length;
			distance->speed = gCfgItems.filamentchange.load.speed;
		}
	}

	char isIdle() {
		return mksReprint.mks_printer_state == MKS_IDLE;
	}

	char isPaused() {
		return (mksReprint.mks_printer_state == MKS_PAUSING)
				|| (mksReprint.mks_printer_state == MKS_PAUSED)
				|| (mksReprint.mks_printer_state == MKS_REPRINTING)
				||(mksReprint.mks_printer_state == MKS_REPRINTED);
	}

	void setLanguage(unsigned char index) {
		gCfgItems.language = index;
		AT24CXX_Write(EPR_LANGUAGE, &index,1);
        make_lang_str();
	}


    void fan_commit() {
        MKS_FAN_TIM = fanSpeeds[0]*10000/255;
    }

	void fun_add_percentage(short value) {
		short p = fan_get_percent();
		p+=value;
		if (p<0) p = 0;
		if (p>100) p = 100;
		fan_set_percent(p);
	}

    void fan_inc() {
        if (fanSpeeds[0]>255)
            fanSpeeds[0] = 255;
        else {
            unsigned char percent = fan_get_percent();
            if (percent==100)
                fanSpeeds[0] = 255;
            else {
                unsigned char new_percent = percent;
                while (percent==new_percent) {
                    ++fanSpeeds[0];
                    new_percent = fan_get_percent();
                }
                if (new_percent==100)
                    fanSpeeds[0] = 255;

            }
        }
        fan_commit();
    }

    void fan_dec() {
        if (fanSpeeds[0]>255)
            fanSpeeds[0] = 255;
        else {
            unsigned char percent = fan_get_percent();
            if (percent==0) {
                fanSpeeds[0] = 0;
            } else {
                unsigned char new_percent = percent;
                while (percent==new_percent) {
                    --fanSpeeds[0];
                    new_percent = fan_get_percent();
                }
                if (new_percent==0)
                    fanSpeeds[0] = 0;
            }
        }
        fan_commit();
    }

    void fan_set_percent(unsigned char percent) {
        unsigned char fv = (int)((float)percent/100 * 255);
        while ((unsigned char)(((float)fv / 255) * 100) != percent)
            fv++;
        fanSpeeds[0] = fv;
        fan_commit();
    }

    unsigned char fan_get_percent() {
        return (unsigned char)(((float)fanSpeeds[0] / 255) * 100);
    }

    void fan_set_percent_double(double percent) {
        if (percent>100)
            percent = 100;
        if (percent<0)
            percent = 0;
        fan_set_percent((unsigned char)percent);
    }

}


