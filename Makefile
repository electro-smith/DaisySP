TARGET = libdaisysp

MODULE_DIR = Source

# Each Module Directory is listed below with it's modules.
# Header only modules are listed commented out 
# below the others.

CONTROL_MOD_DIR = Control
CONTROL_MODULES = \
adenv \
adsr \
phasor \

DRUM_MOD_DIR = Drums
DRUM_MODULES = \
analogbassdrum \
analogsnaredrum \
hihat \
synthbassdrum \
synthsnaredrum \

DYNAMICS_MOD_DIR = Dynamics
DYNAMICS_MODULES = \
crossfade \
limiter \

EFFECTS_MOD_DIR = Effects
EFFECTS_MODULES = \
autowah \
chorus \
decimator \
flanger \
overdrive \
phaser \
sampleratereducer \
tremolo \
wavefolder \

FILTER_MOD_DIR = Filters
FILTER_MODULES = \
svf \
soap \

NOISE_MOD_DIR = Noise
NOISE_MODULES = \
clockednoise \
grainlet \
particle \

PHYSICAL_MODELING_MOD_DIR = PhysicalModeling
PHYSICAL_MODELING_MODULES = \
drip \
KarplusString \
modalvoice \
resonator \
stringvoice \

SAMPLING_MOD_DIR = Sampling
SAMPLING_MODULES = \
granularplayer

SYNTHESIS_MOD_DIR = Synthesis
SYNTHESIS_MODULES = \
fm2 \
formantosc \
oscillator \
oscillatorbank \
variablesawosc \
variableshapeosc \
vosim \
zoscillator \

UTILITY_MOD_DIR = Utility
UTILITY_MODULES = \
dcblock \
metro \

######################################
# source
######################################

CPP_SOURCES += $(addsuffix .cpp, $(MODULE_DIR)/$(CONTROL_MOD_DIR)/$(CONTROL_MODULES))
CPP_SOURCES += $(addsuffix .cpp, $(MODULE_DIR)/$(DRUM_MOD_DIR)/$(DRUM_MODULES))
CPP_SOURCES += $(addsuffix .cpp, $(MODULE_DIR)/$(DYNAMICS_MOD_DIR)/$(DYNAMICS_MODULES))
CPP_SOURCES += $(addsuffix .cpp, $(MODULE_DIR)/$(EFFECTS_MOD_DIR)/$(EFFECTS_MODULES))
CPP_SOURCES += $(addsuffix .cpp, $(MODULE_DIR)/$(FILTER_MOD_DIR)/$(FILTER_MODULES))
CPP_SOURCES += $(addsuffix .cpp, $(MODULE_DIR)/$(NOISE_MOD_DIR)/$(NOISE_MODULES))
CPP_SOURCES += $(addsuffix .cpp, $(MODULE_DIR)/$(PHYSICAL_MODELING_MOD_DIR)/$(PHYSICAL_MODELING_MODULES))
CPP_SOURCES += $(addsuffix .cpp, $(MODULE_DIR)/$(SAMPLING_MOD_DIR)/$(SAMPLING_MODULES))
CPP_SOURCES += $(addsuffix .cpp, $(MODULE_DIR)/$(SYNTHESIS_MOD_DIR)/$(SYNTHESIS_MODULES))
CPP_SOURCES += $(addsuffix .cpp, $(MODULE_DIR)/$(UTILITY_MOD_DIR)/$(UTILITY_MODULES))

######################################
# building variables
######################################
# debug build?
#DEBUG = 0
# optimization
#OPT = -Og
OPT = -O3

#######################################
# paths
#######################################

# Build path
BUILD_DIR = build
DAISYSP_LGPL_DIR = DaisySP-LGPL

#######################################
# binaries
#######################################
PREFIX = arm-none-eabi-
# The gcc compiler bin path can be either defined in make command via GCC_PATH variable (> make GCC_PATH=xxx)
# either it can be added to the PATH environment variable.
ifdef GCC_PATH
CC = $(GCC_PATH)/$(PREFIX)gcc
CXX = $(GCC_PATH)/$(PREFIX)g++
AS = $(GCC_PATH)/$(PREFIX)gcc -x assembler-with-cpp
CP = $(GCC_PATH)/$(PREFIX)objcopy
SZ = $(GCC_PATH)/$(PREFIX)size
AR = $(GCC_PATH)/$(PREFIX)ar
GDB = $(GCC_PATH)/$(PREFIX)gdb
else
CC = $(PREFIX)gcc
CXX = $(PREFIX)g++
AS = $(PREFIX)gcc -x assembler-with-cpp
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size
AR = $(PREFIX)ar
GDB = $(PREFIX)gdb
endif
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S

#######################################
# CFLAGS
#######################################

# language standards
C_STD = -std=gnu11
CPP_STD = -std=gnu++14

# cpu
CPU = -mcpu=cortex-m7

# fpu
FPU = -mfpu=fpv5-d16

# float-abi
FLOAT-ABI = -mfloat-abi=hard

# mcu
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)

# macros for gcc
# AS defines
AS_DEFS = 

# C defines
C_DEFS =  \
-DSTM32H750xx 

C_INCLUDES = \
-I$(MODULE_DIR) \
-I$(MODULE_DIR)/$(CONTROL_MOD_DIR) \
-I$(MODULE_DIR)/$(DRUM_MOD_DIR) \
-I$(MODULE_DIR)/$(DYNAMICS_MOD_DIR) \
-I$(MODULE_DIR)/$(EFFECTS_MOD_DIR) \
-I$(MODULE_DIR)/$(FILTER_MOD_DIR) \
-I$(MODULE_DIR)/$(NOISE_MOD_DIR) \
-I$(MODULE_DIR)/$(PHYSICAL_MODELING_MOD_DIR) \
-I$(MODULE_DIR)/$(SYNTHESIS_MOD_DIR) \
-I$(MODULE_DIR)/$(UTILITY_MOD_DIR) 

# compile gcc flags
ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

CFLAGS = $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -Werror -fdata-sections -ffunction-sections

ifeq ($(DEBUG), 1)
CFLAGS += -g -gdwarf-2
endif

# Generate dependency information
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)"

CPPFLAGS = $(CFLAGS)
CPPFLAGS += \
-fno-exceptions \
-finline-functions 

# default action: build all
all: $(BUILD_DIR)/$(TARGET).a lgpl

#######################################
# build the library
#######################################

# list of objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(CPP_SOURCES:.cpp=.o)))
vpath %.cpp $(sort $(dir $(CPP_SOURCES)))
# list of ASM program objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR)
	$(CC) -c $(CFLAGS) $(C_STD) -static -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.cpp Makefile | $(BUILD_DIR)
	$(CXX) -c $(CPPFLAGS) $(CPP_STD) -static -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.cpp=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	$(AS) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET).a: $(OBJECTS) Makefile
	$(AR) rcs $@ $(OBJECTS)

$(BUILD_DIR):
	mkdir $@

# build the lgpl submodule if it exists
lgpl:
	@if [ -f $(DAISYSP_LGPL_DIR)/Makefile ]; then\
		make -C $(DAISYSP_LGPL_DIR); \
		echo "Building DaisySP-LGPL";\
	fi

#######################################
# clean up
#######################################
clean:
	-rm -fR $(BUILD_DIR)
	-rm -fR $(DAISYSP_LGPL_DIR)/build
#######################################

# dependencies
#######################################
-include $(wildcard $(BUILD_DIR)/*.d)
