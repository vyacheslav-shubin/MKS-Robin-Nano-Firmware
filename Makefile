TOOLS=/home/shubin/electronic/tools/stm32/tools/bin/
#TOOLS=/opt/st/stm32cubeide_1.1.0/plugins/com.st.stm32cube.ide.mcu.externaltools.gnu-arm-embedded.7-2018-q2-update.linux64_1.0.0.201904120901/tools/bin/
TOOL_PREFIX=arm-none-eabi

#include/c++/7.3.1/arm-none-eabi/thumb/v7-m/bits/c++config.h

EWARM_TOOL=./src-list.pl

MODULES		= Middlewares Src User Drivers
BUILD_BASE	= build

SRC_DIR		:= $(MODULES)
C_SRC		:= $(shell $(EWARM_TOOL) src | grep .c$$)
CPP_SRC		:= $(shell $(EWARM_TOOL) src | grep .cpp$$)
ASM_SRC		:= Startup/startup_stm32f103vetx.s

ASM_OBJ			:= $(patsubst %.s,$(BUILD_BASE)/%.o,$(ASM_SRC))
C_OBJ			:= $(patsubst %.c,$(BUILD_BASE)/%.o,$(C_SRC))
CPP_OBJ			:= $(patsubst %.cpp,$(BUILD_BASE)/%.o,$(CPP_SRC))
OBJ_LIST_FILE	:=  $(BUILD_BASE)/obj.list

LIBS		:= $(shell $(EWARM_TOOL) lib)


INCLUDE		:= $(shell $(EWARM_TOOL) include)
PATCH_DIR	:= $(BUILD_BASE)/patch
INCLUDE		:= $(INCLUDE) $(PATCH_DIR)
DEFINES		:= $(shell $(EWARM_TOOL) define)

INCLIDE_OPT	:= $(addprefix -I,$(INCLUDE))
DEFINE_OPT	:= $(addprefix -D,$(DEFINES))

FIRMWARE 	:= $(BUILD_BASE)/firmware

MKS_BIN_FILE	:=robin_nano35.bin
MKS_FIRMWARE 	:= $(BUILD_BASE)/$(MKS_BIN_FILE)

LD_SCRIPT=MKS_ROBIN.ld

SD_CARD=/media/shubin/5DB9-A287

#LD_SCRIPT=STM32F103VETX_FLASH.ld

CXX = $(TOOLS)$(TOOL_PREFIX)-g++
GCC = $(TOOLS)$(TOOL_PREFIX)-gcc
OBJCOPY = $(TOOLS)$(TOOL_PREFIX)-objcopy

COMMON_ARGS = -Os -mcpu=cortex-m3 -mthumb -fsigned-char -fno-move-loop-invariants -fno-strict-aliasing --specs=nano.specs --specs=nosys.specs -MMD -MP -DTARGET_STM32F1
CXX_ARGS = -fabi-version=0 -fno-use-cxa-atexit -fno-threadsafe-statics


prepare:
	mkdir -p $(BUILD_BASE)
	mkdir -p $(PATCH_DIR)
	ln -s -f ../../Middlewares/GUI/GUI.h $(PATCH_DIR)/gui.h
	ln -s -f ../../Middlewares/GUI/BUTTON.h $(PATCH_DIR)/button.h
	ln -s -f ../../Middlewares/GUI/TEXT.h $(PATCH_DIR)/text.h
	ln -s -f ../../Middlewares/Third_Party/Marlin/Marlin.h $(PATCH_DIR)/marlin.h
	ln -s -f ../../User/ui/draw_ready_print.h $(PATCH_DIR)/Draw_ready_print.h
	ln -s -f ../../Middlewares/Third_Party/Marlin/Configuration_adv.h $(PATCH_DIR)/configuration_adv.h
	ln -s -f ../../Drivers/BSP/variant/fastio_Robin2.h $(PATCH_DIR)/fastio_robin2.h
	ln -s -f ../../Drivers/BSP/Components/w25qxx/spi_flash.h $(PATCH_DIR)/SPI_Flash.h
	ln -s -f ../../User/ui/draw_Sprayer.h $(PATCH_DIR)/draw_sprayer.h
	ln -s -f ../../Middlewares/Third_Party/Marlin/Conditionals_post.h $(PATCH_DIR)/conditionals_post.h
	ln -s -f ../../Drivers/STM32F10x_StdPeriph_Driver/inc/stm32f10x_gpio.h $(PATCH_DIR)/stm32F10x_gpio.h
	ln -s -f ../../Drivers/STM32F10x_StdPeriph_Driver/inc/stm32f10x_tim.h $(PATCH_DIR)/stm32F10x_tim.h
	ln -s -f ../../Drivers/BSP/variant/mks_variant.h $(PATCH_DIR)/Mks_variant.h
	ln -s -f ../../User/ui/draw_LevelingSettings.h $(PATCH_DIR)/draw_levelingsettings.h
	ln -s -f ../../User/ui/draw_doubleZ.h $(PATCH_DIR)/draw_doublez.h
	ln -s -f ../../Middlewares/Third_Party/Marlin/Configuration.h $(PATCH_DIR)/configuration.h
	ln -s -f ../../User/ui/QRENCODE/QR_Encode.h $(PATCH_DIR)/qr_encode.h
	ln -s -f ../../Middlewares/Third_Party/FatFs/src/ff.h $(PATCH_DIR)/Ff.h


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

$(FIRMWARE).elf: $(OBJ_LIST_FILE)
	$(CXX) -Os -o $(FIRMWARE).elf $(ASM_OBJ) $(C_OBJ) $(CPP_OBJ) $(LIBS) -mcpu=cortex-m3 -T"$(LD_SCRIPT)" -Wl,-Map="$(FIRMWARE).map" -Wl,--gc-sections -static  --specs=nano.specs --specs=nosys.specs  -u_printf_float -mfloat-abi=soft -mthumb -Wl,--start-group -lc -lm -lstdc++ -lsupc++ -Wl,--end-group
	#$(CXX) -o $(FIRMWARE).elf $(ASM_OBJ) $(C_OBJ) $(CPP_OBJ) $(LIBS) -T"$(LD_SCRIPT)" -Wl,-Map="$(FIRMWARE).map" $(COMMON_ARGS)b -Wl,--start-group -lc -lm -lstdc++ -lsupc++ -Wl,--end-group


$(OBJ_LIST_FILE): $(ASM_OBJ) $(C_OBJ) $(CPP_OBJ)
	echo $(ASM_OBJ) $(C_OBJ) $(CPP_OBJ)>$(OBJ_LIST_FILE)

obj: $(OBJ_LIST_FILE)

$(foreach src,$(ASM_SRC), $(eval $(call compile-asm-objects, $(patsubst %.s, $(BUILD_BASE)/%.o, $(src)), $(src) )))

$(foreach src,$(C_SRC), $(eval $(call compile-c-objects, $(patsubst %.c, $(BUILD_BASE)/%.o, $(src)), $(src) )))

$(foreach src,$(CPP_SRC), $(eval $(call compile-cpp-objects, $(patsubst %.cpp, $(BUILD_BASE)/%.o, $(src)), $(src) )))

.PHONY: all 


clean: remove_all prepare

remove_all: 
	rm -r $(BUILD_BASE)

copy_fw: ren_exists copy_bin

$(SD_CARD)/$(MKS_BIN_FILE): $(MKS_FIRMWARE)
	rm -f $(SD_CARD)/ROBIN_NANO35.CUR
	if [ -d "$(SD_CARD)" ]; then cp $(MKS_FIRMWARE) $(SD_CARD)/$(MKS_BIN_FILE); fi

make_sd_bin: $(SD_CARD)/$(MKS_BIN_FILE)

make_sd: make_sd_bin mks
	if [ -d "$(SD_CARD)/bak_font" ]; then mv $(SD_CARD)/bak_font  $(SD_CARD)/mks_font; fi
	if [ -d "$(SD_CARD)/bak_pic" ]; then mv $(SD_CARD)/bak_pic  $(SD_CARD)/mks_pic; fi
	if [ -d "$(SD_CARD)/robin_nano35_cfg.CUR" ]; then mv $(SD_CARD)/robin_nano35_cfg.CUR $(SD_CARD)/robin_nano35_cfg.txt; fi
	rm -f /media/shubin/5DB9-A287/ROBIN_NANO35.CUR


#picocom --echo --baud 115200 /dev/ttyUSB0

