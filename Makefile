#
# file: Makefile
#
# author: Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
#
# date: 2015-09-18
#

#-----------------------------------------------------------------------------------------------------------------------
# toolchain configuration
#-----------------------------------------------------------------------------------------------------------------------

TOOLCHAIN = arm-none-eabi-

AS = $(TOOLCHAIN)gcc
CC = $(TOOLCHAIN)gcc
CXX = $(TOOLCHAIN)g++
LD = $(TOOLCHAIN)g++
OBJCOPY = $(TOOLCHAIN)objcopy
OBJDUMP = $(TOOLCHAIN)objdump
SIZE = $(TOOLCHAIN)size
RM = rm -f

#-----------------------------------------------------------------------------------------------------------------------
# project configuration
#-----------------------------------------------------------------------------------------------------------------------

# file with $(CONFIG_SELECTED_CONFIGURATION) variable
include selectedConfiguration.mk

# path to distortosConfiguration.mk file selected by $(CONFIG_SELECTED_CONFIGURATION) variable
DISTORTOS_CONFIGURATION_MK = ./$(subst ",,$(CONFIG_SELECTED_CONFIGURATION))/distortosConfiguration.mk

# output folder
OUTPUT = output/

# project name
PROJECT = distortos

# core type
COREFLAGS = -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16

# global assembler flags
ASFLAGS =

# global C flags
CFLAGS =

# global C++ flags
CXXFLAGS =

# optimization flags ("-O0" - no optimization, "-O1" - optimize, "-O2" - optimize even more, "-Os" - optimize for size
# or "-O3" - optimize yet more)
OPTIMIZATION = -O2

# define warning options here
CXXWARNINGS = -Wall -Wextra -Wshadow
CWARNINGS = -Wall -Wstrict-prototypes -Wextra -Wshadow

# C++ language standard ("c++98", "gnu++98" - default, "c++11", "gnu++11")
CXXSTD = -std=gnu++11

# C language standard ("c89" / "iso9899:1990", "iso9899:199409", "c99" / "iso9899:1999", "gnu89" - default, "gnu99")
CSTD = -std=gnu99

# debug flags
DBGFLAGS = -g -ggdb3

# linker flags
LDFLAGS =

#-----------------------------------------------------------------------------------------------------------------------
# load configuration variables from distortosConfiguration.mk file selected by user
#-----------------------------------------------------------------------------------------------------------------------

include $(DISTORTOS_CONFIGURATION_MK)

#-----------------------------------------------------------------------------------------------------------------------
# compilation flags
#-----------------------------------------------------------------------------------------------------------------------

ASFLAGS += $(COREFLAGS)
ASFLAGS += $(DBGFLAGS)
ASFLAGS += -MD -MP

CFLAGS += $(COREFLAGS)
CFLAGS += $(OPTIMIZATION)
CFLAGS += $(CWARNINGS)
CFLAGS += $(CSTD)
CFLAGS += $(DBGFLAGS)
CFLAGS += -ffunction-sections -fdata-sections -MD -MP

CXXFLAGS += $(COREFLAGS)
CXXFLAGS += $(OPTIMIZATION)
CXXFLAGS += $(CXXWARNINGS)
CXXFLAGS += $(CXXSTD)
CXXFLAGS += $(DBGFLAGS)
CXXFLAGS += -ffunction-sections -fdata-sections -fno-rtti -fno-exceptions -MD -MP

# path to linker script (generated automatically)
LDSCRIPT = $(OUTPUT)$(subst ",,$(CONFIG_CHIP)).ld

LDFLAGS += -T$(LDSCRIPT)
LDFLAGS += $(COREFLAGS)
LDFLAGS += -g -Wl,-Map=$(OUTPUT)$(PROJECT).map,--cref,--gc-sections

#-----------------------------------------------------------------------------------------------------------------------
# "constants" with include paths
#-----------------------------------------------------------------------------------------------------------------------

# "standard" includes
STANDARD_INCLUDES += -I$(OUTPUT)include -Iinclude

# architecture includes
ARCHITECTURE_INCLUDES += $(patsubst %,-I%,$(subst ",,$(CONFIG_ARCHITECTURE_INCLUDES)))

# chip includes
CHIP_INCLUDES += $(patsubst %,-I%,$(subst ",,$(CONFIG_CHIP_INCLUDES)))

#-----------------------------------------------------------------------------------------------------------------------
# build macros
#-----------------------------------------------------------------------------------------------------------------------

define PARSE_SUBDIRECTORY
ifdef d
dir := $$(d)/$(1)
else
dir := $(1)
endif
sp := $$(sp).x
dirstack_$$(sp) := $$(d)
d := $$(dir)
SUBDIRECTORIES :=
include $$(dir)/Rules.mk
$$(call PARSE_SUBDIRECTORIES,$$(SUBDIRECTORIES))
-include $$(DEPENDENCIES_$$(d))
d := $$(dirstack_$$(sp))
sp := $$(basename $$(sp))
endef

define PARSE_SUBDIRECTORIES
$(foreach subdirectory,$(1),$(eval $(call PARSE_SUBDIRECTORY,$(subdirectory))))
endef

#-----------------------------------------------------------------------------------------------------------------------
# build targets
#-----------------------------------------------------------------------------------------------------------------------

.PHONY: all
all: targets

# trigger parsing of all Rules.mk files
include Rules.mk
$(call PARSE_SUBDIRECTORIES,$(SUBDIRECTORIES))

.PHONY: targets
targets: $(OUTPUT)include/distortos/distortosConfiguration.h $(OBJECTS) $(ELF) $(HEX) $(BIN) $(DMP) $(LSS) size

$(OBJECTS): $(OUTPUT)include/distortos/distortosConfiguration.h

$(GENERATED): Makefile

$(OUTPUT)%.o: %.S
	$(AS) $(ASFLAGS) $(ASFLAGS_$(<)) -c $< -o $@

$(OUTPUT)%.o: %.c
	$(CC) $(CFLAGS) $(CFLAGS_$(<)) -c $< -o $@

$(OUTPUT)%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(CXXFLAGS_$(<)) -c $< -o $@

$(OUTPUT)%.elf: $(OBJECTS) $(LDSCRIPT)
	$(LD) $(LDFLAGS) $(OBJECTS) -o $@

$(OUTPUT)%.hex: $(ELF)
	$(OBJCOPY) -O ihex $< $@

$(OUTPUT)%.bin: $(ELF)
	$(OBJCOPY) -O binary $< $@

$(OUTPUT)%.dmp: $(ELF)
	$(OBJDUMP) -x --syms --demangle $< > $@

$(OUTPUT)%.lss: $(ELF)
	$(OBJDUMP) --demangle -S $< > $@

.PHONY: size
size: $(ELF)
	$(SIZE) -B $^

.PHONY: clean
clean:
	$(RM) $(GENERATED)
