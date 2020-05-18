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
#include "Marlin.h"
#include "MarlinConfig.h"
#include "fatfs.h"
#include "mks_cfg.h"
#include "mks_reprint.h"
#include "usb_host.h"

#include "draw_ui.h"

#if ENABLED(SDSUPPORT)

#include "cardreader.h"
#include "sdio_sdcard.h"


//#define LONGEST_FILENAME (longFilename[0] ? longFilename : filename)

CardReader::CardReader() {
    sdmode = 0;			
	sdprinting = false;
	cardOK = false;	
	usbOK = false;
    savetosd = false;
}

void CardReader::automount()	{}
void CardReader::checkautostart(bool force) {}

void CardReader::checkFilesys(unsigned char filesys) {
	switch(filesys) {
		case FILE_SYS_SD: {
			if (SD_DET_IP != SD_DETECT_INVERTED) {
				if(cardOK || sdprinting) {  // Card removed
					SERIAL_ECHO_START();
					SERIAL_ECHOLNPGM("SD card removed");
					cardOK = false;
				}
			} else {
				if(!cardOK) {
					SERIAL_ECHO_START();
					SERIAL_ECHOLNPGM("Select SD file system");
					initsd();
					usbOK = false;
				}
			}
		} break;
		case FILE_SYS_USB: break;
		default:break;
	}
}

void CardReader::initsd() {
	FRESULT mksMountState = FR_DISK_ERR;
	cardOK = false;
	if( SD_DET_IP != SD_DETECT_INVERTED)
		return;

	if(f_mount(&fs, (TCHAR const*)SD_Path, 0) != FR_OK) {
		SERIAL_ECHO_START();
		SERIAL_ECHOLNPGM(MSG_SD_INIT_FAIL);
	} else {
		SERIAL_ECHO_START();
		SERIAL_ECHOLNPGM(MSG_SD_CARD_OK);
		cardOK = true;
	}
}

void CardReader::initusb() { }

void CardReader::mount() {
    sdmode = false;	sdprinting = false;
    initsd();
}

void CardReader::unmount() {
    sdmode = false;		sdprinting = false;
	cardOK = false;
    savetosd = false;	
}

void CardReader::release() {
    sdmode = false;		
	sdprinting = false;	
	cardOK = false;
    savetosd = false;	
}

void CardReader::startPrint() {
    if(!cardOK) return;
    sdmode = true;	
	sdprinting = true;
}

void CardReader::startFileprint() {
	if (cardOK || usbOK) {
		sdprinting = true;
		sdmode = true;
		#if ENABLED(SDCARD_SORT_ALPHA)
			flush_presort();
		#endif

		mksReprint.mks_printer_state = MKS_WORKING;
		if(gCfgItems.pwroff_save_mode != 1)
			epr_write_data(EPR_SAV_FLAG, (uint8_t *)&mksReprint.mks_printer_state,sizeof(mksReprint.mks_printer_state));
	}
}

void CardReader::pausePrint(bool intern) {
    if(!card.cardOK)
    	return;
    sdmode = 2; // finish running line
}

void CardReader::pauseSDPrint() {
	if(sdprinting)
		sdprinting = false;	sdmode = 2;
}


void CardReader::continuePrint(bool intern) {
    if(!card.cardOK)
    	return;
}

void CardReader::stopSDPrint() {
	sdprinting = false;
	if (isFileOpen())
		closefile();
}

void CardReader::stopPrint() {
    if(!card.cardOK)
    	return;
    sdmode = 0;
    sdprinting = false;
}

void CardReader::operatePrint() {}

char *CardReader::createFilename(char *buffer,const dir_t &p) {
    char *pos = buffer,*src = (char*)p.name;
    for (uint8_t i = 0; i < 11; i++,src++) {
        if (*src == ' ') continue;
        if (i == 8)
            *pos++ = '.';
        *pos++ = *src;
    }
    *pos = 0;
    return pos;
}

bool CardReader::showFilename(const uint8_t *name) {
    if (*name == DIR_NAME_DELETED || *name == '.')
    	return false;
    return true;
}

int8_t RFstricmp(const char* s1, const char* s2) {
    while(*s1 && (tolower(*s1) == tolower(*s2)))
        s1++,s2++;
    return (const uint8_t)tolower(*s1)-(const uint8_t)tolower(*s2);
}

int8_t RFstrnicmp(const char* s1, const char* s2, size_t n) {
    while(n--) {
        if(tolower(*s1)!=tolower(*s2))
            return (uint8_t)tolower(*s1) - (uint8_t)tolower(*s2);
        s1++;
        s2++;
    }
    return 0;
}

void CardReader::ls() {
    if ((SD_DET_IP == SD_DETECT_INVERTED)&&(usbOK == false))
    	get_file_list(SD_Path);
}

bool CardReader::selectFile(char *filename, bool silent){}

bool CardReader::reopenfile(char* filename) {
    f_close(&curFile);
    if(f_open(&curFile, (const TCHAR *)filename, FA_OPEN_EXISTING | FA_READ) == FR_OK) {
        filesize = curFile.fsize;
        return true;
    } else
        return false;
}

bool CardReader::openFile(char* filename,bool silent, bool replace_current/*=true*/) {
    SdBaseFile parent;
    char *oldP;
    boolean bFound;
    char newname[100]={0};
	if(sdprinting)
		return false;
	f_close(&curFile);

	if(filename[2] != '/') {
		if(card.cardOK)
			strcat(newname,SD_Path);
		else
			return false;
	}
	strcat(newname,filename);
	mks_saveFileName(newname);

	if(f_open(&curFile, (const TCHAR *)newname, FA_OPEN_EXISTING | FA_READ) == FR_OK) {
        sdpos = 0;
        filesize = curFile.fsize;
        SERIAL_PROTOCOLPAIR(MSG_SD_FILE_OPENED, filename);
		SERIAL_PROTOCOLPGM(MSG_SD_SIZE);
		SERIAL_PROTOCOL(filesize);
        SERIAL_EOL();
		SERIAL_PROTOCOLLNPGM(MSG_SD_FILE_SELECTED);
        return true;
    } else {
		SERIAL_PROTOCOLPAIR(MSG_SD_OPEN_FILE_FAIL, filename);
		SERIAL_PROTOCOLCHAR('.');
		SERIAL_EOL();
    	return false;
    }
}

void CardReader::printStatus() { }

void CardReader::getStatus() {
	if(cardOK || usbOK){
		SERIAL_PROTOCOLPGM(MSG_SD_PRINTING_BYTE);
		SERIAL_PROTOCOL(sdpos);
		SERIAL_PROTOCOLPGM("/");
		SERIAL_PROTOCOLLN(filesize);
	} else
		SERIAL_PROTOCOLLNPGM(MSG_SD_NOT_PRINTING);
}


void CardReader::startWrite(char *filename) {
	if(!cardOK && ! usbOK)
		return;
	f_close(&card.curFile);
    sdmode = false;
    sdprinting = false;
	if(f_open(&curFile, (const TCHAR *)filename, FA_CREATE_ALWAYS | FA_WRITE | FA_READ)  != FR_OK) {
    } else
        savetosd = true;
}

void CardReader::finishWrite() {
    if(!savetosd)
    	return;
    f_sync(&curFile);
	f_close(&curFile);
    savetosd = false;
}


void CardReader::deleteFile(char *filename)	{}

void CardReader::removeFile(char* filename)  {
    if(!cardOK && !usbOK)
    	return;
    sdmode = false;
    sdprinting = false;
    f_close(&curFile);
    if(f_unlink((const TCHAR *)filename) == FR_OK) {
    	SERIAL_PROTOCOLPGM("File deleted:");
    	SERIAL_PROTOCOLLN(filename);
    	sdpos = 0;
    } else {
		SERIAL_PROTOCOLPGM("Deletion failed, File: ");
		SERIAL_PROTOCOL(filename);
		SERIAL_PROTOCOLLNPGM(".");
	}
}

void CardReader::makeDirectory(char *filename) {
    if(!cardOK && !usbOK)
    	return;
    sdmode = false;
    sdprinting = false;
    f_close(&curFile);
    if(f_mkdir((const TCHAR *)filename) == FR_OK) {
 		SERIAL_PROTOCOLLN("Directory created");
    } else {
    	SERIAL_PROTOCOLLN("Creation failed");
    }
}

void CardReader::get_file_list(char *path) {
	if( path == 0)
		return;
	Explore_Disk(path, 0);
}

uint8_t CardReader::Explore_Disk(char* path , uint8_t recu_level) {
	FILINFO fno;
	DIR dir;
	TCHAR *fn;
	char tmp[200];
	char Fstream[200];
	int local_offset;
	int file_offset = 0;
	FRESULT res;
	#if _USE_LFN
    	static TCHAR lfn[_MAX_LFN + 1];
    	fno.lfname = lfn;
    	fno.lfsize = sizeof(lfn);
	#endif
	if(path == 0)
		return 0;
	f_mount(&fs, (char *)path, 0);
        
	for(;;) {
		local_offset = 0;
		if (f_opendir(&dir, (const TCHAR *)path) == FR_OK) {
		    while(1) {
				res = f_readdir(&dir, &fno);
				if (res != FR_OK || fno.fname[0] == 0)
					return;
				if (fno.fname[0] == '.')
					continue;
				if(local_offset >= file_offset) {
					file_offset++;
					break;
				}
				local_offset++;
		    }
		    if ((fno.lfname[0] == 0) || (fno.lfname == 0))
				fn = fno.fname;
			else
				fn = fno.lfname;
				
		     if((strstr((const char *)fn, ".gco")) || (strstr((const char *)fn, ".GCO")) || (fno.fattrib & AM_DIR)) {
		    	 tmp[0] = '\0';
		    	 strcat((char *)tmp, (char *)fn);
		    	 memset(Fstream, 0, sizeof(Fstream));
		    	 strcpy(Fstream, tmp);
		    	 if((fno.fattrib & AM_DIR)&&(recu_level <= 10)) {
		    		 strcat(Fstream, ".DIR\r\n");
		    		 SERIAL_PROTOCOL(Fstream);
		    	 } else {
		    		 strcat(Fstream, "\r\n");
		    		 SERIAL_PROTOCOL(Fstream);
		    	 }
		     }
		} else
			break;
	}
	return res;
}

int CardReader::ascii2dec(char *ascii, char width) {
	int i = 0;
	int result = 0;
	if(ascii == 0)
		return 0;
	
	while(i < width) {
		result = result << 4;
		if(*(ascii + i) >= '0' && *(ascii + i) <= '9')
			result += *(ascii + i) - '0';
		else if(*(ascii + i) >= 'a' && *(ascii + i) <= 'f')
			result += *(ascii + i) - 'a' + 0x0a;
		else if(*(ascii + i) >= 'A' && *(ascii + i) <= 'F')
			result += *(ascii + i) - 'A' + 0x0a;
		else
			return 0;
		i++;
	}
	return result;
}

/*---------------------------------mks add begin-------------------------------------*/
uint32_t CardReader::getsdpos() {
	return sdpos;
}

void CardReader::printingHasFinished() {
    stepper.synchronize();
	f_close(&curFile);
    sdprinting = false;
	sdmode = 0;
    if (SD_FINISHED_STEPPERRELEASE)
    	enqueue_and_echo_commands_P(PSTR(SD_FINISHED_RELEASECOMMAND));
    print_job_timer.stop();
    if (print_job_timer.duration() > 60)
      enqueue_and_echo_commands_P(PSTR("M31"));
}


void CardReader::closefile(bool store_location) {
	f_close(&curFile);
	saving = false;
	logging = false;
}

void CardReader::openLogFile(char* name) {
	logging = true;
	openFile(name, false);
}

void CardReader::write_command(char *buf) {
	unsigned int lastBufferEntry = 0;
	FRESULT writeStatus;
	char* begin = buf;
	char* npos = 0;
	char* end = buf + strlen(buf) - 1;
	memset(&writeStatus, 0, sizeof(FRESULT));

	if((npos = strchr(buf, 'N')) != NULL) {
		begin = strchr(npos, ' ') + 1;
		end = strchr(npos, '*') - 1;
	}
	end[1] = '\r';
	end[2] = '\n';
	writeStatus = f_write(&curFile, begin, &(end[2]) - begin + 1, &lastBufferEntry);
	if(
			(writeStatus != FR_OK)
			|| (lastBufferEntry != (unsigned int)(&(end[2]) - begin + 1)))
	{
		SERIAL_ERROR_START();
		SERIAL_ERRORLNPGM(MSG_SD_ERR_WRITE_TO_FILE);
	}
}


#endif // SDSUPPORT
