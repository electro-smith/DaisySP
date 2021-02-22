# Project Name
TARGET = tst_fir

# Library Locations
LIBDAISY_DIR ?= ../../../libdaisy
DAISYSP_DIR ?= ../../../DaisySP
CMSIS_DIR ?= $(LIBDAISY_DIR)/Drivers/CMSIS


# Sources
CPP_SOURCES = tst_fir.cpp	\

C_SOURCES = $(CMSIS_DIR)/DSP/Source/FilteringFunctions/arm_fir_f32.c   \
			$(CMSIS_DIR)/DSP/Source/FilteringFunctions/arm_fir_init_f32.c  

C_INCLUDES = -I./ -I../util/


# Options

#OPT ?= -O3

# Note: USE_ARM_DSP line may be commented out to disable the ARM-specific code
C_DEFS += -DNDEBUG	\
-DUSE_ARM_DSP






# Core location, and generic Makefile.
SYSTEM_FILES_DIR = $(LIBDAISY_DIR)/core
include $(SYSTEM_FILES_DIR)/Makefile

