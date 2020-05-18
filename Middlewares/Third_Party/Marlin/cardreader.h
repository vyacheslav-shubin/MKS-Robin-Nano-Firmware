/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef _CARDREADER_H_
#define _CARDREADER_H_


#define SD_DETECT_INVERTED false
/*-----------------------2017-11-01 add 1---------------------------*/
#define PGM_P	const char *
#define FSTRINGPARAM(var) PGM_P var
#define PSTR(s) s
#define pgm_read_byte(x) (*(char*)x)
    
#define SDPOS_BUF_LEN   20       //delta 6; Cartesian 18   

//#define card.release    card.unmount
/*-----------------------2017-11-01 add 1---------------------------*/


//#include "MarlinConfig.h"

#if ENABLED(SDSUPPORT)

#include "Marlin.h"
//#include "ultralcd.h"
#include "stepper.h"
#include "temperature.h"
#include "language.h"
#include "configuration_store.h"

#include "ff.h"  //for FATFS

#include <ctype.h> //for call to tolower function

//#define MAX_DIR_DEPTH 10          // Maximum folder depth

//#include "SdFile.h"
//#include "types.h"
//#include "enum.h"

#define LONG_FILENAME_LENGTH (13*MAX_VFAT_ENTRIES+1)
#define SD_MAX_FOLDER_DEPTH 2

extern char tempLongFilename[LONG_FILENAME_LENGTH+1];
extern char fullName[LONG_FILENAME_LENGTH*SD_MAX_FOLDER_DEPTH+SD_MAX_FOLDER_DEPTH+1];
#define SHORT_FILENAME_LENGTH 14
#include "SdFat.h"
#if defined(TFT35)
#define    MAX_FILES_ONE_LEVER	   6
#else
#define    MAX_FILES_ONE_LEVER	   5//30
#endif
typedef struct
{
	TCHAR fileName[MAX_FILES_ONE_LEVER][100];//fileName[MAX_FILES_ONE_LEVER][50];
	uint8_t fileAttr[MAX_FILES_ONE_LEVER];	//0?agcode???t?��o?1?a????
	uint8_t index;
	uint8_t listVaild;	//flag of valid
	
} fileNameList;


class CardReader {
public:
#if JSON_OUTPUT
			GCodeFileInfo fileInfo;
#endif
	FIL curFile;
	FATFS fs;
	uint32_t filesize;
	uint32_t sdpos;
	char *shortname; // Pointer to start of filename itself
	char *pathend; // File to char where pathname in fullname ends
	uint8_t sdmode;  // true if we are printing from sd card, 2 = stop accepting new commands
	bool savetosd;
	SdBaseFile parentFound;
	uint32_t Sd_file_offset;
	uint32_t Sd_file_cnt;
	TCHAR gCurDir[100];
	fileNameList gcodeFileList;
	CardReader();

	void initsd();
	void initusb();
    bool selectFile(char *filename,bool silent=false);
    void mount();	
    void unmount();		void release();
    void startPrint();
    void pausePrint(bool intern = false);       //?Y����䨰��?
    void continuePrint(bool intern = false);    //?��D?�䨰��?
    void stopPrint();                           //����?1�䨰��?
    void operatePrint();  
		
  // Files auto[0-9].g on the sd card are performed in sequence.
  // This is to delay autostart and hence the initialisation of
  // the sd card to some seconds after the normal init, so the
  // device is available soon after a reset.
  /*-----------------------2017-11-01 add 2---------------------------*/
	bool sdprinting;
	bool saving;
	bool logging;
	bool cardOK ;
	bool usbOK;

	void printingHasFinished();

	void checkautostart(bool x);
	void checkFilesys(unsigned char filesys);
	bool reopenfile(char* filename);
	bool openFile(char* name,bool read,bool replace_current=true);
	void startFileprint();
	void pauseSDPrint();
	void getStatus();
	void closefile(bool store_location=false);
	void removeFile(char* name);
	void openLogFile(char* name);
	void write_command(char *buf);
	void stopSDPrint();
	FORCE_INLINE uint32_t fileLength() { return filesize; }
	FORCE_INLINE bool isFileOpen() { return (bool)curFile.fs; }
	FORCE_INLINE bool eof() { return sdpos>=filesize ;};
	FORCE_INLINE int16_t get() {
		char readByte;
		UINT rc;
		if (f_read(&curFile, &readByte, 1, &rc) != FR_OK){
			readByte = -1;
		} else {
			sdpos += rc;
		}
		return (int16_t) readByte;
	};
	FORCE_INLINE void setIndex(long index) {sdpos = index;f_lseek(&curFile, index);};
	FORCE_INLINE uint8_t percentDone(){if(!isFileOpen()) return 0; if(filesize) return sdpos/((filesize+99)/100); else return 0;};
	
#ifdef USE_MKS_WIFI  
		FORCE_INLINE uint32_t getFileLength() { return filesize; }
	
		FORCE_INLINE bool renameFile(const char * newPath){ file.rename(curDir, newPath);}
	
		 FORCE_INLINE bool lastOpenOk(){return lastOpen;}; //whether last open is ok
	
		int write_data(char *buf);
	
		int read_data(char *buf, int size);
#endif

	void mksConfiguration();
	void mksEepromRefresh();
	void mksReset();
	void mksLoad();
	uint32_t getsdpos();
	void printStatus();
	void ls();
	void startWrite(char *filename);
    void deleteFile(char *filename);
    void finishWrite();
    char *createFilename(char *buffer,const dir_t &p);
    void makeDirectory(char *filename);
    bool showFilename(const uint8_t *name);
    void automount();
	void get_file_list(char *path);
	uint8_t Explore_Disk (char* path , uint8_t recu_level);
	void ShowSDFiles(void);
	int ascii2dec(char *ascii, char width);
	void get_sd_cfg(void);
	#ifdef GLENN_DEBUG
		void writeToFile();
	#endif
	private:
		uint8_t lsRecursive(SdBaseFile *parent,uint8_t level,char *findFilename);
};
extern CardReader card;


#if ENABLED(SD_DETECT_INVERTED)
  #define IS_SD_INSERTED (READ(SD_DETECT_PIN) != 0)
#else
  #define IS_SD_INSERTED (READ(SD_DETECT_PIN) == 0)
#endif


#endif // SDSUPPORT

#if ENABLED(SDSUPPORT)
	#define IS_SD_PRINTING (card.sdprinting)
	#define IS_SD_FILE_OPEN (card.isFileOpen())
#else
	#define IS_SD_PRINTING (false)
	#define IS_SD_FILE_OPEN (false)
#endif

#endif // _CARDREADER_H_
