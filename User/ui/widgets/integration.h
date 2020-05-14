/*
 * integration.h
 *
 *  Created on: May 13, 2020
 *      Author: shubin
 */

#ifndef USER_UI_WIDGETS_INTEGRATION_H_
#define USER_UI_WIDGETS_INTEGRATION_H_

namespace shUI {

typedef struct{
	float current;
	float target;
} SPRAYER_TEMP;

typedef struct{
	short current;
	short target;
} BED_TEMP;

typedef struct{
	float distance;
	short speed;
} MOVE_DISTANCE;

typedef struct{
	float x;
	float y;
	float z;
	float e;
} CURRENT_POSITION;

	extern void babystep(char * axe, float size);
	extern float babystepGetZ();

	extern char isDualExtruders();


	extern void getSprayerTemperature(unsigned char index, SPRAYER_TEMP * temp);
	extern void setSprayerTemperature(unsigned char index, float value);
	extern void addSprayerTemperature(unsigned char index, float value);

	extern void getBedTemperature(BED_TEMP * temp);
	extern void setBedTemperature(short value);
	extern void addBedTemperature(short value);

	extern short getFeedratePercentage();
	extern void setFeedratePercentage(short value);
	extern void addFeedratePercentage(short value);

	extern short getFlowPercentage(char index);
	extern void setFlowPercentage(char index, short value);
	extern void addFlowPercentage(char index, short value);

	extern void saveConfig();

	extern void pushGcode(const char * gcode);

	extern void getCurrentPosition(CURRENT_POSITION * value);

	extern unsigned char isManualLeveling();
	extern unsigned char isMeshLeveling();
	extern void doCustomLeveling();

	extern void doFilamentMove(unsigned char extruder, int size, int speed);
	extern short getFilamentExtrudeMinTempereture();
	extern short getFilamentLoadUnloadMinTempereture(char direction);
	extern void getFilamentLoadUnloadDistance(char direction, MOVE_DISTANCE * distance);

	extern char isPaused();
	extern char isIdle();


}



#endif /* USER_UI_WIDGETS_INTEGRATION_H_ */
