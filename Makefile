TOOLS=/home/shubin/electronic/tools/stm32/tools/bin/
#TOOLS=/opt/st/stm32cubeide_1.1.0/plugins/com.st.stm32cube.ide.mcu.externaltools.gnu-arm-embedded.7-2018-q2-update.linux64_1.0.0.201904120901/tools/bin/
TOOL_PREFIX=arm-none-eabi

#include/c++/7.3.1/arm-none-eabi/thumb/v7-m/bits/c++config.h

#EWARM_TOOL=python ./src-list.py 

BUILD_BASE	= build

C_SRC		:= $(shell cat src.list | grep .c$$)
CPP_SRC		:= $(shell cat src.list| grep .cpp$$)	 
ASM_SRC		:= Startup/startup_stm32f103vetx.s


ASM_OBJ			:= $(patsubst %.s,$(BUILD_BASE)/%.o,$(ASM_SRC))
C_OBJ			:= $(patsubst %.c,$(BUILD_BASE)/%.o,$(C_SRC))
CPP_OBJ			:= $(patsubst %.cpp,$(BUILD_BASE)/%.o,$(CPP_SRC))

OBJ_LIST_FILE	:= $(BUILD_BASE)/obj.list

PIC_LIST_FILE	:= $(BUILD_BASE)/pic.list
PIC_INPUT		:= res/ui
PIC_OUTPUT		:=	$(BUILD_BASE)/mks_pic

PICS		:= $(patsubst $(PIC_INPUT)/%,%,$(shell find $(PIC_INPUT) -name *.png))
BIN_PICS	:= $(patsubst %.png,$(BUILD_BASE)/mks_pic/bmp_%.bin,$(PICS))

LIBS		:= Middlewares/GUI/GUI.a

INCLUDE		:= $(shell cat include.list)
#PATCH_DIR	:= $(BUILD_BASE)/patch
#INCLUDE		:= $(INCLUDE)
DEFINES		:= USE_HAL_DRIVER STM32F103xE STM32F10X_HD USE_HAL_LIB MKS_ROBIN_NANO TFT35 BUILD_DATE='"$(shell date +'%Y.%m.%d')"'
 
INCLIDE_OPT	:= $(addprefix -I,$(INCLUDE))
DEFINE_OPT	:= $(addprefix -D,$(DEFINES))

FIRMWARE 	:= $(BUILD_BASE)/firmware

MKS_BIN_FILE	:=robin_nano35.bin
SNAPSHOT_FILE	:=robin_nano35.zip
SNAPSHOT_PIC_FILE	:=mks_pic.zip
SNAPSHOT_DIR 	:=../mks-robin-nano35-binary/snapshot
MKS_FIRMWARE 	:= $(BUILD_BASE)/$(MKS_BIN_FILE)

LD_SCRIPT=MKS_ROBIN.ld

#SD_CARD=/media/shubin/5DB9-A287
SD_CARD=/media/shubin/sd

#LD_SCRIPT=STM32F103VETX_FLASH.ld

CXX = $(TOOLS)$(TOOL_PREFIX)-g++
GCC = $(TOOLS)$(TOOL_PREFIX)-gcc
OBJCOPY = $(TOOLS)$(TOOL_PREFIX)-objcopy

COMMON_ARGS = -Os -mcpu=cortex-m3 -mthumb -fsigned-char -fno-move-loop-invariants -fno-strict-aliasing --specs=nano.specs --specs=nosys.specs -MMD -MP -DTARGET_STM32F1
CXX_ARGS = -fabi-version=0 -fno-use-cxa-atexit -fno-threadsafe-statics

print_date:
	echo $(BUILD_DATE)

print_pic_src:
	echo $(PICS)

print_pic_out:
	echo $(BIN_PICS)

prepare:
	mkdir -p $(BUILD_BASE)
	mkdir -p $(BUILD_BASE)/mks_pic
	

$(FIRMWARE).bin: $(FIRMWARE).elf
	$(OBJCOPY) -O binary $(FIRMWARE).elf $(FIRMWARE).bin
	
$(MKS_FIRMWARE): $(FIRMWARE).bin
	python mks_robin.py $(FIRMWARE).bin $(MKS_FIRMWARE) 
	 
elf: $(FIRMWARE).elf
bin: $(FIRMWARE).bin
mks: $(MKS_FIRMWARE)


define compile-asm-objects
$1: $2
	mkdir -p $(dir $1)
	$(CXX) -MMD -c $2 -o $1
endef


define compile-c-objects
$1: $2
	mkdir -p $(dir $1)
	$(GCC) $2 -mcpu=cortex-m3 -g3 $(DEFINE_OPT) -c $(INCLIDE_OPT) -Os -fsigned-char -fno-move-loop-invariants -fno-strict-aliasing -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MT"$@"  --specs=nano.specs --specs=nosys.specs  -mfloat-abi=soft -mthumb -o $1
	#$(GCC) -std=gnu11 -ffunction-sections -fdata-sections $2 $(COMMON_ARGS) $(DEFINE_OPT) -c $(INCLIDE_OPT) -o $1
endef


define compile-cpp-objects
$1: $2
	mkdir -p $(dir $1)
	$(CXX) $2 -mcpu=cortex-m3 -std=gnu++14 -g3 $(DEFINE_OPT) -c $(INCLIDE_OPT) -Os  -fsigned-char -fno-move-loop-invariants -fno-strict-aliasing -ffunction-sections -fdata-sections -fno-exceptions  -fno-use-cxa-atexit -fno-threadsafe-statics -fno-rtti -fno-threadsafe-statics -fpermissive -fno-use-cxa-atexit -fstack-usage -MMD -MP -MT"$1" -MT"$1.d"  --specs=nano.specs --specs=nosys.specs  -mfloat-abi=soft -mthumb -o $1
	#$(GCC) -fpermissive -ffunction-sections -fdata-sections -std=gnu++14 $2 $(COMMON_ARGS) $(CXX_ARGS) $(DEFINE_OPT) -c $(INCLIDE_OPT) -o $1
endef

define compile-pics
$1: $2
	python pic-build.py $2 $1
endef

$(FIRMWARE).elf: $(OBJ_LIST_FILE)
	$(CXX) -Os -o $(FIRMWARE).elf $(ASM_OBJ) $(C_OBJ) $(CPP_OBJ) $(LIBS) -mcpu=cortex-m3 -T"$(LD_SCRIPT)" -Wl,-Map="$(FIRMWARE).map" -Wl,--gc-sections -static  --specs=nano.specs --specs=nosys.specs  -Wl,-u,_printf_float -mfloat-abi=soft -mthumb -Wl,--start-group -lc -lm -lstdc++ -lsupc++ -Wl,--end-group
	#$(CXX) -o $(FIRMWARE).elf $(ASM_OBJ) $(C_OBJ) $(CPP_OBJ) $(LIBS) -T"$(LD_SCRIPT)" -Wl,-Map="$(FIRMWARE).map" $(COMMON_ARGS)b -Wl,--start-group -lc -lm -lstdc++ -lsupc++ -Wl,--end-group


$(OBJ_LIST_FILE): $(ASM_OBJ) $(C_OBJ) $(CPP_OBJ)
	echo $(ASM_OBJ) $(C_OBJ) $(CPP_OBJ)>$(OBJ_LIST_FILE)

obj: $(OBJ_LIST_FILE)

$(PIC_LIST_FILE): $(BIN_PICS)
	echo $(BIN_PICS)>$(PIC_LIST_FILE)
	
pics: $(PIC_LIST_FILE)

$(foreach src,$(PICS), $(eval $(call compile-pics, $(patsubst %.png, $(PIC_OUTPUT)/bmp_%.bin, $(src)), $(PIC_INPUT)/$(src) )))

$(foreach src,$(ASM_SRC), $(eval $(call compile-asm-objects, $(patsubst %.s, $(BUILD_BASE)/%.o, $(src)), $(src) )))

$(foreach src,$(C_SRC), $(eval $(call compile-c-objects, $(patsubst %.c, $(BUILD_BASE)/%.o, $(src)), $(src) )))

$(foreach src,$(CPP_SRC), $(eval $(call compile-cpp-objects, $(patsubst %.cpp, $(BUILD_BASE)/%.o, $(src)), $(src) )))

.PHONY: all prepare


clean: remove_all prepare

remove_all: 
	rm -r $(BUILD_BASE)

copy_fw: ren_exists copy_bin

$(SD_CARD)/$(MKS_BIN_FILE): $(MKS_FIRMWARE)
	rm -f $(SD_CARD)/ROBIN_NANO35.CUR
	#mountpoint -q /media/shubin/sd || udisksctl mount -b /dev/sdc1
	cp $(MKS_FIRMWARE) $(SD_CARD)/$(MKS_BIN_FILE)

$(SNAPSHOT_DIR):
	mkdir $(SNAPSHOT_DIR)

$(SNAPSHOT_DIR)/$(SNAPSHOT_FILE): $(MKS_FIRMWARE) $(SNAPSHOT_DIR)
	zip -9 -j $(SNAPSHOT_DIR)/$(SNAPSHOT_FILE) $(MKS_FIRMWARE)

$(SNAPSHOT_DIR)/$(SNAPSHOT_PIC_FILE) : pics $(SNAPSHOT_DIR)
	zip -9 -j $(SNAPSHOT_DIR)/$(SNAPSHOT_PIC_FILE) $(PIC_OUTPUT)/*
 	 

sd_bin: $(SD_CARD)/$(MKS_BIN_FILE)

snapshot: $(SNAPSHOT_DIR)/$(SNAPSHOT_FILE) $(SNAPSHOT_DIR)/$(SNAPSHOT_PIC_FILE)

make_sd: make_sd_bin mks
	if [ -d "$(SD_CARD)/bak_font" ]; then mv $(SD_CARD)/bak_font  $(SD_CARD)/mks_font; fi
	if [ -d "$(SD_CARD)/bak_pic" ]; then mv $(SD_CARD)/bak_pic  $(SD_CARD)/mks_pic; fi
	if [ -d "$(SD_CARD)/robin_nano35_cfg.CUR" ]; then mv $(SD_CARD)/robin_nano35_cfg.CUR $(SD_CARD)/robin_nano35_cfg.txt; fi
	rm -f $(SD_CARD)/ROBIN_NANO35.CUR


sync_pics:
	if [ -d "$(SD_CARD)/bak_pic" ]; then mv $(SD_CARD)/bak_pic  $(SD_CARD)/mks_pic; fi
	cp -r -u -v $(PIC_OUTPUT) $(SD_CARD)


#picocom --imap lfcrlf --echo --baud 115200 /dev/ttyUSB0



