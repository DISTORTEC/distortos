#
# file: Makefile
#
# author: Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
#
# date: 2015-10-08
#

#-----------------------------------------------------------------------------------------------------------------------
# load configuration variables from distortosConfiguration.mk file selected by user
#-----------------------------------------------------------------------------------------------------------------------

# file with $(CONFIG_SELECTED_CONFIGURATION) variable
include selectedConfiguration.mk

# path to distortosConfiguration.mk file selected by $(CONFIG_SELECTED_CONFIGURATION) variable
DISTORTOS_CONFIGURATION_MK = ./$(subst ",,$(CONFIG_SELECTED_CONFIGURATION))

include $(DISTORTOS_CONFIGURATION_MK)

#-----------------------------------------------------------------------------------------------------------------------
# toolchain configuration
#-----------------------------------------------------------------------------------------------------------------------

CONFIG_TOOLCHAIN_PREFIX := $(subst ",,$(CONFIG_TOOLCHAIN_PREFIX))

AS = $(CONFIG_TOOLCHAIN_PREFIX)gcc
CC = $(CONFIG_TOOLCHAIN_PREFIX)gcc
CXX = $(CONFIG_TOOLCHAIN_PREFIX)g++
AR = $(CONFIG_TOOLCHAIN_PREFIX)ar
LD = $(CONFIG_TOOLCHAIN_PREFIX)g++
OBJCOPY = $(CONFIG_TOOLCHAIN_PREFIX)objcopy
OBJDUMP = $(CONFIG_TOOLCHAIN_PREFIX)objdump
SIZE = $(CONFIG_TOOLCHAIN_PREFIX)size
RM = rm -f

#-----------------------------------------------------------------------------------------------------------------------
# project configuration
#-----------------------------------------------------------------------------------------------------------------------

# output folder
OUTPUT = output/

# project name
PROJECT = distortos

# global assembler flags
ASFLAGS =

# global C flags
CFLAGS =

# global C++ flags
CXXFLAGS =

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
# compilation flags
#-----------------------------------------------------------------------------------------------------------------------

CONFIG_ARCHITECTURE_FLAGS := $(subst ",,$(CONFIG_ARCHITECTURE_FLAGS))
CONFIG_BUILD_OPTIMIZATION := $(subst ",,$(CONFIG_BUILD_OPTIMIZATION))

ASFLAGS += $(CONFIG_ARCHITECTURE_FLAGS)
ASFLAGS += $(DBGFLAGS)
ASFLAGS += -MD -MP

CFLAGS += $(CONFIG_ARCHITECTURE_FLAGS)
CFLAGS += $(CONFIG_BUILD_OPTIMIZATION)
CFLAGS += $(CWARNINGS)
CFLAGS += $(CSTD)
CFLAGS += $(DBGFLAGS)
CFLAGS += -ffunction-sections -fdata-sections -MD -MP

CXXFLAGS += $(CONFIG_ARCHITECTURE_FLAGS)
CXXFLAGS += $(CONFIG_BUILD_OPTIMIZATION)
CXXFLAGS += $(CXXWARNINGS)
CXXFLAGS += $(CXXSTD)
CXXFLAGS += $(DBGFLAGS)
CXXFLAGS += -ffunction-sections -fdata-sections -fno-rtti -fno-exceptions -MD -MP

# path to linker script (generated automatically)
LDSCRIPT = $(OUTPUT)$(subst ",,$(CONFIG_CHIP)).ld

LDFLAGS += $(CONFIG_ARCHITECTURE_FLAGS)
LDFLAGS += -g -Wl,-Map=$(@:.elf=.map),--cref,--gc-sections

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
#verbose mode
VERBOSE ?= 0

ifeq ($(VERBOSE), 0)
Q = @
else
Q =
endif

define DIRECTORY_DEPENDENCY
$(1): | $(dir $(1))
endef

define MAKE_DIRECTORY
$(1):
	$(Q)mkdir -p $(1)
endef

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

# generated files depend (order-only) on their directories
$(foreach file,$(GENERATED),$(eval $(call DIRECTORY_DEPENDENCY,$(file))))

# create rules to make missing output directories
DIRECTORIES = $(sort $(dir $(GENERATED)))
MISSING_DIRECTORIES = $(filter-out $(wildcard $(DIRECTORIES)),$(DIRECTORIES))
$(foreach directory,$(MISSING_DIRECTORIES),$(eval $(call MAKE_DIRECTORY,$(directory))))

.PHONY: targets
targets: $(GENERATED)

$(OBJECTS): $(OUTPUT)include/distortos/distortosConfiguration.h

$(GENERATED): Makefile

$(OUTPUT)%.o: %.S
	@echo " AS     " $< 
	$(Q)$(AS) $(ASFLAGS) $(ASFLAGS_$(<)) -c $< -o $@

$(OUTPUT)%.o: %.c
	@echo " CC     " $<
	$(Q)$(CC) $(CFLAGS) $(CFLAGS_$(<)) -c $< -o $@

$(OUTPUT)%.o: %.cpp
	@echo " CXX    " $<
	$(Q)$(CXX) $(CXXFLAGS) $(CXXFLAGS_$(<)) -c $< -o $@

$(OUTPUT)%.a:
	$(Q)rm -f $@
	@echo " AR     " $@
	$(Q)$(AR) rcs $@ $(filter %.o,$(^))

$(OUTPUT)%.elf:
	@echo " LD     " $@
	$(eval ARCHIVES_$@ := -Wl,--whole-archive $(addprefix -l:,$(filter %.a,$(^))) -Wl,--no-whole-archive)
	$(Q)$(LD) $(LDFLAGS) -T$(filter %.ld,$(^)) $(filter %.o,$(^)) $(ARCHIVES_$(@)) -o $@

$(OUTPUT)%.hex:
	@echo " HEX    " $@
	$(Q)$(OBJCOPY) -O ihex $(filter %.elf,$(^)) $@

$(OUTPUT)%.bin:
	@echo " BIN    " $@
	$(Q)$(OBJCOPY) -O binary $(filter %.elf,$(^)) $@

$(OUTPUT)%.dmp:
	@echo " DMP    " $@
	$(Q)$(OBJDUMP) -x --syms --demangle $(filter %.elf,$(^)) > $@

$(OUTPUT)%.lss:
	@echo " LSS    " $@
	$(Q)$(OBJDUMP) --demangle -S $(filter %.elf,$(^)) > $@

.PHONY: size
size:
	$(Q)$(SIZE) -B $(filter %.elf,$(^))

.PHONY: clean
clean:
	$(RM) -r $(OUTPUT)
