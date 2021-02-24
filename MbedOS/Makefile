ifeq ($(shell echo $$OS),$$OS)
    MAKEDIR = if not exist "$(1)" mkdir "$(1)"
    RM = rmdir /S /Q "$(1)"
else
    MAKEDIR = '$(SHELL)' -c "mkdir -p \"$(1)\""
    RM = '$(SHELL)' -c "rm -rf \"$(1)\""
endif

OBJDIR := BUILD
# Move to the build directory
ifeq (,$(filter $(OBJDIR),$(notdir $(CURDIR))))
.SUFFIXES:
mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
MAKETARGET = '$(MAKE)' --no-print-directory -C $(OBJDIR) -f '$(mkfile_path)' \
		'SRCDIR=$(CURDIR)' $(MAKECMDGOALS)
.PHONY: $(OBJDIR) clean
all:
	+@$(call MAKEDIR,$(OBJDIR))
	+@$(MAKETARGET)
$(OBJDIR): all
Makefile : ;
% :: $(OBJDIR) ; :
clean :
	rm ../MbedOS_FE.o
	rm ../RTOS_BE.o
	$(call RM,$(OBJDIR))

else

# trick rules into thinking we are in the root, when we are in the bulid dir
VPATH = ..

# Boiler-plate
###############################################################################
# Project settings

PROJECT := MbedOS

OBJECTS += ../../MbedOS_FE.o
OBJECTS += ../../RTOS_BE.o
INCLUDE_PATHS += -I../../

# Project settings
###############################################################################
# Objects and Paths

OBJECTS += ./MQTT/MQTTPacket/MQTTConnectClient.o
OBJECTS += ./MQTT/MQTTPacket/MQTTConnectServer.o
OBJECTS += ./MQTT/MQTTPacket/MQTTDeserializePublish.o
OBJECTS += ./MQTT/MQTTPacket/MQTTFormat.o
OBJECTS += ./MQTT/MQTTPacket/MQTTPacket.o
OBJECTS += ./MQTT/MQTTPacket/MQTTSerializePublish.o
OBJECTS += ./MQTT/MQTTPacket/MQTTSubscribeClient.o
OBJECTS += ./MQTT/MQTTPacket/MQTTSubscribeServer.o
OBJECTS += ./MQTT/MQTTPacket/MQTTUnsubscribeClient.o
OBJECTS += ./MQTT/MQTTPacket/MQTTUnsubscribeServer.o
OBJECTS += ./main.o
OBJECTS += ./mbed-os/features/frameworks/mbed-client-cli/source/ns_cmdline.o
OBJECTS += ./mbed-os/features/frameworks/mbed-client-randlib/source/randLIB.o
OBJECTS += ./mbed-os/features/frameworks/mbed-coap/source/sn_coap_builder.o
OBJECTS += ./mbed-os/features/frameworks/mbed-coap/source/sn_coap_header_check.o
OBJECTS += ./mbed-os/features/frameworks/mbed-coap/source/sn_coap_parser.o
OBJECTS += ./mbed-os/features/frameworks/mbed-coap/source/sn_coap_protocol.o
OBJECTS += ./mbed-os/features/frameworks/nanostack-libservice/source/libList/ns_list.o
OBJECTS += ./mbed-os/features/netsocket/TCPSocket.o
OBJECTS += ./mbed-os/hal/mbed_critical_section_api.o
OBJECTS += ./mbed-os/hal/mbed_gpio.o
OBJECTS += ./mbed-os/hal/mbed_ticker_api.o
OBJECTS += ./mbed-os/hal/mbed_us_ticker_api.o
OBJECTS += ./mbed-os/platform/FileBase.o
OBJECTS += ./mbed-os/platform/FileHandle.o
OBJECTS += ./mbed-os/platform/FilePath.o
OBJECTS += ./mbed-os/platform/internal/mbed_atomic_impl.o
OBJECTS += ./mbed-os/platform/mbed_assert.o
OBJECTS += ./mbed-os/platform/mbed_board.o
OBJECTS += ./mbed-os/platform/mbed_critical.o
OBJECTS += ./mbed-os/platform/mbed_error.o
OBJECTS += ./mbed-os/platform/mbed_poll.o
OBJECTS += ./mbed-os/platform/mbed_retarget.o
OBJECTS += ./mbed-os/platform/mbed_wait_api_no_rtos.o
OBJECTS += ./mbed-os/rtos/EventFlags.o
OBJECTS += ./mbed-os/rtos/Kernel.o
OBJECTS += ./mbed-os/rtos/Mutex.o
OBJECTS += ./mbed-os/rtos/RtosTimer.o
OBJECTS += ./mbed-os/rtos/Semaphore.o
OBJECTS += ./mbed-os/rtos/TARGET_CORTEX/SysTimer.o
OBJECTS += ./mbed-os/rtos/TARGET_CORTEX/TOOLCHAIN_GCC_ARM/mbed_boot_gcc_arm.o
OBJECTS += ./mbed-os/rtos/TARGET_CORTEX/mbed_boot.o
OBJECTS += ./mbed-os/rtos/TARGET_CORTEX/mbed_rtos_rtx.o
OBJECTS += ./mbed-os/rtos/TARGET_CORTEX/mbed_rtx_handlers.o
OBJECTS += ./mbed-os/rtos/TARGET_CORTEX/mbed_rtx_idle.o
OBJECTS += ./mbed-os/rtos/TARGET_CORTEX/rtx4/cmsis_os1.o
OBJECTS += ./mbed-os/rtos/TARGET_CORTEX/rtx5/RTX/Config/RTX_Config.o
OBJECTS += ./mbed-os/rtos/TARGET_CORTEX/rtx5/RTX/Source/rtx_delay.o
OBJECTS += ./mbed-os/rtos/TARGET_CORTEX/rtx5/RTX/Source/rtx_evflags.o
OBJECTS += ./mbed-os/rtos/TARGET_CORTEX/rtx5/RTX/Source/rtx_evr.o
OBJECTS += ./mbed-os/rtos/TARGET_CORTEX/rtx5/RTX/Source/rtx_kernel.o
OBJECTS += ./mbed-os/rtos/TARGET_CORTEX/rtx5/RTX/Source/rtx_lib.o
OBJECTS += ./mbed-os/rtos/TARGET_CORTEX/rtx5/RTX/Source/rtx_memory.o
OBJECTS += ./mbed-os/rtos/TARGET_CORTEX/rtx5/RTX/Source/rtx_mempool.o
OBJECTS += ./mbed-os/rtos/TARGET_CORTEX/rtx5/RTX/Source/rtx_msgqueue.o
OBJECTS += ./mbed-os/rtos/TARGET_CORTEX/rtx5/RTX/Source/rtx_mutex.o
OBJECTS += ./mbed-os/rtos/TARGET_CORTEX/rtx5/RTX/Source/rtx_semaphore.o
OBJECTS += ./mbed-os/rtos/TARGET_CORTEX/rtx5/RTX/Source/rtx_system.o
OBJECTS += ./mbed-os/rtos/TARGET_CORTEX/rtx5/RTX/Source/rtx_thread.o
OBJECTS += ./mbed-os/rtos/TARGET_CORTEX/rtx5/RTX/Source/rtx_timer.o
OBJECTS += ./mbed-os/rtos/TARGET_CORTEX/rtx5/Source/os_systick.o
OBJECTS += ./mbed-os/rtos/TARGET_CORTEX/rtx5/Source/os_tick_ptim.o
OBJECTS += ./mbed-os/rtos/ThisThread.o
OBJECTS += ./mbed-os/rtos/Thread.o
OBJECTS += ./mbed-os/targets/TARGET_Freescale/TARGET_MCUXpresso_MCUS/TARGET_MCU_K64F/device/system_MK64F12.o
OBJECTS += ./mbed-os/targets/TARGET_Freescale/TARGET_MCUXpresso_MCUS/api/gpio_api.o

INCLUDE_PATHS += -I../.
INCLUDE_PATHS += -I.././MQTT
INCLUDE_PATHS += -I.././MQTT/FP
INCLUDE_PATHS += -I.././MQTT/MQTTPacket
INCLUDE_PATHS += -I.././mbed-os
INCLUDE_PATHS += -I.././mbed-os/cmsis
INCLUDE_PATHS += -I.././mbed-os/cmsis/TARGET_CORTEX_M
INCLUDE_PATHS += -I.././mbed-os/drivers
INCLUDE_PATHS += -I.././mbed-os/events
INCLUDE_PATHS += -I.././mbed-os/events/equeue
INCLUDE_PATHS += -I.././mbed-os/features
INCLUDE_PATHS += -I.././mbed-os/features/frameworks/mbed-client-cli
INCLUDE_PATHS += -I.././mbed-os/features/frameworks/mbed-client-cli/mbed-client-cli
INCLUDE_PATHS += -I.././mbed-os/features/frameworks/mbed-client-randlib/mbed-client-randlib
INCLUDE_PATHS += -I.././mbed-os/features/frameworks/mbed-coap
INCLUDE_PATHS += -I.././mbed-os/features/frameworks/mbed-coap/mbed-coap
INCLUDE_PATHS += -I.././mbed-os/features/frameworks/mbed-coap/source
INCLUDE_PATHS += -I.././mbed-os/features/frameworks/mbed-coap/source/include
INCLUDE_PATHS += -I.././mbed-os/features/frameworks/mbed-trace
INCLUDE_PATHS += -I.././mbed-os/features/frameworks/nanostack-libservice
INCLUDE_PATHS += -I.././mbed-os/features/frameworks/nanostack-libservice/mbed-client-libservice
INCLUDE_PATHS += -I.././mbed-os/features/netsocket

INCLUDE_PATHS += -I.././mbed-os/hal
INCLUDE_PATHS += -I.././mbed-os/platform
INCLUDE_PATHS += -I.././mbed-os/rtos
INCLUDE_PATHS += -I.././mbed-os/rtos/TARGET_CORTEX
INCLUDE_PATHS += -I.././mbed-os/rtos/TARGET_CORTEX/rtx4
INCLUDE_PATHS += -I.././mbed-os/rtos/TARGET_CORTEX/rtx5/Include
INCLUDE_PATHS += -I.././mbed-os/rtos/TARGET_CORTEX/rtx5/RTX/Config
INCLUDE_PATHS += -I.././mbed-os/rtos/TARGET_CORTEX/rtx5/RTX/Include
INCLUDE_PATHS += -I.././mbed-os/rtos/TARGET_CORTEX/rtx5/RTX/Source
INCLUDE_PATHS += -I.././mbed-os/targets/TARGET_Freescale
INCLUDE_PATHS += -I.././mbed-os/targets/TARGET_Freescale/TARGET_MCUXpresso_MCUS/TARGET_MCU_K64F
INCLUDE_PATHS += -I.././mbed-os/targets/TARGET_Freescale/TARGET_MCUXpresso_MCUS/TARGET_MCU_K64F/TARGET_FRDM
INCLUDE_PATHS += -I.././mbed-os/targets/TARGET_Freescale/TARGET_MCUXpresso_MCUS/TARGET_MCU_K64F/device
INCLUDE_PATHS += -I.././mbed-os/targets/TARGET_Freescale/TARGET_MCUXpresso_MCUS/TARGET_MCU_K64F/drivers
INCLUDE_PATHS += -I.././mbed-os/targets/TARGET_Freescale/TARGET_MCUXpresso_MCUS/api

INCLUDE_PATHS += -I.././mbed-os

LIBRARY_PATHS :=
LIBRARIES :=
LINKER_SCRIPT :=
ELF2BIN = objcopy
AS = AFL_USE_ASAN=1 afl-clang-fast -m32
CC = AFL_USE_ASAN=1 afl-clang-fast -m32
CPP = AFL_USE_ASAN=1 afl-clang-fast -m32
LD = AFL_USE_ASAN=1 afl-clang-fast -m32
C_FLAGS += -include
C_FLAGS += mbed_config.h
C_FLAGS += -DTARGET_KSDK2_MCUS
C_FLAGS += -DTARGET_PSA
C_FLAGS += -D__MBED__=1
C_FLAGS += -D__FPU_PRESENT=0
C_FLAGS += -DTARGET_Freescale
C_FLAGS += -DTARGET_RTOS_M4_M7
C_FLAGS += -DTARGET_NAME=K64F
C_FLAGS += -D__CMSIS_RTOS
C_FLAGS += -DFSL_RTOS_MBED
C_FLAGS += -DTARGET_KPSDK_MCUS
C_FLAGS += -DTOOLCHAIN_GCC
C_FLAGS += -DARM_MATH_CM4
C_FLAGS += -DTARGET_DEBUG
C_FLAGS += -DTARGET_LIKE_CORTEX_M4
C_FLAGS += -DTARGET_M4
C_FLAGS += -DTARGET_K64F
C_FLAGS += -DTARGET_Freescale_EMAC
C_FLAGS += -DTARGET_CORTEX
C_FLAGS += -D__CORTEX_M4
C_FLAGS += -DMBED_BUILD_TIMESTAMP=1564594286.95
C_FLAGS += -DTARGET_LIKE_MBED
C_FLAGS += -DTARGET_FF_ARDUINO
C_FLAGS += -DTARGET_KPSDK_CODE
C_FLAGS += -DMBED_SPLIT_HEAP
C_FLAGS += -DDEVICE_TRNG=1
C_FLAGS += -D__MBED_CMSIS_RTOS_CM
C_FLAGS += -DTOOLCHAIN_GCC_ARM
C_FLAGS += -DCPU_MK64FN1M0VMD12
C_FLAGS += -DTARGET_FRDM
C_FLAGS += -DTARGET_MCUXpresso_MCUS
C_FLAGS += -DTARGET_MCU_K64F
C_FLAGS += -DTARGET_CORTEX_M
C_FLAGS += -include
C_FLAGS += mbed_config.h
C_FLAGS += -c
C_FLAGS += -O0
C_FLAGS += -g
C_FLAGS += -DMBED_DEBUG
C_FLAGS += -DMBED_TRAP_ERRORS_ENABLED=1
CXX_FLAGS += -include
CXX_FLAGS += mbed_config.h
CXX_FLAGS += -DTARGET_KSDK2_MCUS
CXX_FLAGS += -DTARGET_PSA
CXX_FLAGS += -D__MBED__=1
CXX_FLAGS += -D__FPU_PRESENT=0
CXX_FLAGS += -DTARGET_Freescale
CXX_FLAGS += -DTARGET_RTOS_M4_M7
CXX_FLAGS += -DTARGET_NAME=K64F
CXX_FLAGS += -D__CMSIS_RTOS
CXX_FLAGS += -DFSL_RTOS_MBED
CXX_FLAGS += -DTARGET_KPSDK_MCUS
CXX_FLAGS += -DTOOLCHAIN_GCC
CXX_FLAGS += -DARM_MATH_CM4
CXX_FLAGS += -DTARGET_DEBUG
CXX_FLAGS += -DTARGET_LIKE_CORTEX_M4
CXX_FLAGS += -DTARGET_M4
CXX_FLAGS += -DTARGET_K64F
CXX_FLAGS += -DTARGET_Freescale_EMAC
CXX_FLAGS += -DTARGET_CORTEX
CXX_FLAGS += -D__CORTEX_M4
CXX_FLAGS += -DMBED_BUILD_TIMESTAMP=1564594286.95
CXX_FLAGS += -DTARGET_LIKE_MBED
CXX_FLAGS += -DTARGET_FF_ARDUINO
CXX_FLAGS += -DTARGET_KPSDK_CODE
CXX_FLAGS += -DMBED_SPLIT_HEAP
CXX_FLAGS += -DDEVICE_TRNG=1
CXX_FLAGS += -D__MBED_CMSIS_RTOS_CM
CXX_FLAGS += -DTOOLCHAIN_GCC_ARM
CXX_FLAGS += -DCPU_MK64FN1M0VMD12
CXX_FLAGS += -DTARGET_FRDM
CXX_FLAGS += -DTARGET_MCUXpresso_MCUS
CXX_FLAGS += -DTARGET_MCU_K64F
CXX_FLAGS += -DTARGET_CORTEX_M
CXX_FLAGS += -include
CXX_FLAGS += mbed_config.h
CXX_FLAGS += -fno-rtti
CXX_FLAGS += -c
CXX_FLAGS += -fno-exceptions
CXX_FLAGS += -O0
CXX_FLAGS += -g
CXX_FLAGS += -DMBED_DEBUG
CXX_FLAGS += -DMBED_TRAP_ERRORS_ENABLED=1

LD_FLAGS := -Wl,--wrap,main -Wl,--wrap,_malloc_r -Wl,--wrap,_free_r -Wl,--wrap,_realloc_r -Wl,--wrap,_memalign_r -Wl,--wrap,_calloc_r -Wl,--wrap,exit -Wl,--wrap,atexit
LD_SYS_LIBS := -lgcc

# Tools and Flags
###############################################################################
# Rules

.PHONY: all lst size


all: $(PROJECT).bin $(PROJECT).hex size


.s.o:
	+@$(call MAKEDIR,$(dir $@))
	+@echo "Assemble: $(notdir $<)"
  
	@$(AS) -c $(ASM_FLAGS) -o $@ $<
  


.S.o:
	+@$(call MAKEDIR,$(dir $@))
	+@echo "Assemble: $(notdir $<)"
  
	@$(AS) -c $(ASM_FLAGS) -o $@ $<
  

.c.o:
	+@$(call MAKEDIR,$(dir $@)) 
	+@echo "Compile: $(notdir $<)"
	@$(CC) $(C_FLAGS) -pthread $(INCLUDE_PATHS) -o $@ $<

.cpp.o:
	+@$(call MAKEDIR,$(dir $@))
	+@echo "Compile: $(notdir $<)"
	@$(CPP) $(CXX_FLAGS) -pthread $(INCLUDE_PATHS) -o $@ $<

$(PROJECT).elf: $(OBJECTS)
	+@echo "link: $(notdir $@)"
	$(LD) $(LD_FLAGS) $(LIBRARY_PATHS) -pthread --output $@ $(filter %.o, $^) $(LIBRARIES) $(LD_SYS_LIBS)


$(PROJECT).bin: $(PROJECT).elf
	$(ELF2BIN) -O binary $< $@
	+@echo "===== bin file ready to flash: $(OBJDIR)/$@ =====" 

$(PROJECT).hex: $(PROJECT).elf
	$(ELF2BIN) -O ihex $< $@


# Rules
###############################################################################
# Dependencies

DEPS = $(OBJECTS:.o=.d)
-include $(DEPS)
endif

# Dependencies
###############################################################################
