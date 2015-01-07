#
# file: Makefile
#
# author: Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
#
# date: 2015-01-07
#

#-----------------------------------------------------------------------------------------------------------------------
# toolchain configuration
#-----------------------------------------------------------------------------------------------------------------------

TOOLCHAIN = arm-none-eabi-

AS = $(TOOLCHAIN)gcc -x assembler-with-cpp
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

# output folder
OUTPUT = output/

# project name
PROJECT = distortos

# core type
COREFLAGS = -mcpu=cortex-m4 -mthumb

# linker flags related to linker script
LDFLAGS = -Lsource/chip/STMicroelectronics/STM32F4 -Lsource/architecture/ARM/ARMv7-M -TSTM32F4xxxG.ld

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

LDFLAGS += $(COREFLAGS)
LDFLAGS += -g -Wl,-Map=$(OUTPUT)$(PROJECT).map,--cref,--gc-sections

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
targets: $(OBJECTS) $(ELF) $(HEX) $(BIN) $(DMP) $(LSS) size

$(OBJECTS): Makefile

$(OUTPUT)%.o: %.S
	$(AS) $(ASFLAGS) $(ASFLAGS_$(<)) -c $< -o $@

$(OUTPUT)%.o: %.c
	$(CC) $(CFLAGS) $(CFLAGS_$(<)) -c $< -o $@

$(OUTPUT)%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(CXXFLAGS_$(<)) -c $< -o $@

$(OUTPUT)%.elf: $(OBJECTS) $(LDSCRIPTS)
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
