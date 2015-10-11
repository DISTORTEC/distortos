#
# file: Makefile
#
# author: Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
#
# date: 2015-10-11
#

#-----------------------------------------------------------------------------------------------------------------------
# make control/macros
#-----------------------------------------------------------------------------------------------------------------------
# defaults
mk_do_inc := 1

mk_simple_targets := clean configure distclean menuconfig help

# This macro checks, if the make target list MAKECMDGOALS contains the given simple target $1. If so, it
# executes mk_set_simple_target to set/clear some variables.
#
# parmeter 1 .. simple target to check
mk_check_simple_target = $(if $(findstring $(filter $(1),$(MAKECMDGOALS)),$(1)),$(call mk_set_simple_target,$(1)))
mk_set_simple_target = $(eval mk_do_inc := 0) \
                       $(eval mk_tgt_$(1) := 1)

# check all simple targets if present
$(eval $(foreach tgt,$(mk_simple_targets),$(call mk_check_simple_target,$(tgt))))

# remove all "
unquote_d = $(subst ",,$(1))
# " keep this line to improve the syntax highlighting of some editors

#-----------------------------------------------------------------------------------------------------------------------
# load configuration variables from distortosConfiguration.mk file selected by user
#-----------------------------------------------------------------------------------------------------------------------
ifeq ($(mk_do_inc),1)
   ifeq ($(wildcard selectedConfiguration.mk),)
      $(error Please run 'configure.sh' or 'make configure' first)
   endif

   # file with $(CONFIG_SELECTED_CONFIGURATION) variable
   include selectedConfiguration.mk

   # path to distortosConfiguration.mk file selected by $(CONFIG_SELECTED_CONFIGURATION) variable
   DISTORTOS_CONFIGURATION_MK = ./$(call unquote_d,$(CONFIG_SELECTED_CONFIGURATION))

   include $(DISTORTOS_CONFIGURATION_MK)
endif

#-----------------------------------------------------------------------------------------------------------------------
# toolchain configuration
#-----------------------------------------------------------------------------------------------------------------------

CONFIG_TOOLCHAIN_PREFIX := $(call unquote_d,$(CONFIG_TOOLCHAIN_PREFIX))

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

CONFIG_ARCHITECTURE_FLAGS := $(call unquote_d,$(CONFIG_ARCHITECTURE_FLAGS))
CONFIG_BUILD_OPTIMIZATION := $(call unquote_d,$(CONFIG_BUILD_OPTIMIZATION))

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
LDSCRIPT = $(OUTPUT)$(call unquote_d,$(CONFIG_CHIP)).ld

LDFLAGS += $(CONFIG_ARCHITECTURE_FLAGS)
LDFLAGS += -g -Wl,-Map=$(@:.elf=.map),--cref,--gc-sections

#-----------------------------------------------------------------------------------------------------------------------
# "constants" with include paths
#-----------------------------------------------------------------------------------------------------------------------

# "standard" includes
STANDARD_INCLUDES += -I$(OUTPUT)include -Iinclude

# architecture includes
ARCHITECTURE_INCLUDES += $(patsubst %,-I%,$(call unquote_d,$(CONFIG_ARCHITECTURE_INCLUDES)))

# chip includes
CHIP_INCLUDES += $(patsubst %,-I%,$(call unquote_d,$(CONFIG_CHIP_INCLUDES)))

#-----------------------------------------------------------------------------------------------------------------------
# build macros
#-----------------------------------------------------------------------------------------------------------------------

define DIRECTORY_DEPENDENCY
$(1): | $(dir $(1))
endef

define MAKE_DIRECTORY
$(1):
	mkdir -p $(1)
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

ifeq ($(mk_do_inc),1)
   # trigger parsing of all Rules.mk files
   include Rules.mk
endif

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
	$(AS) $(ASFLAGS) $(ASFLAGS_$(<)) -c $< -o $@

$(OUTPUT)%.o: %.c
	$(CC) $(CFLAGS) $(CFLAGS_$(<)) -c $< -o $@

$(OUTPUT)%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(CXXFLAGS_$(<)) -c $< -o $@

$(OUTPUT)%.a:
	rm -f $@
	$(AR) rcs $@ $(filter %.o,$(^))

$(OUTPUT)%.elf:
	$(eval ARCHIVES_$@ := -Wl,--whole-archive $(addprefix -l:,$(filter %.a,$(^))) -Wl,--no-whole-archive)
	$(LD) $(LDFLAGS) -T$(filter %.ld,$(^)) $(filter %.o,$(^)) $(ARCHIVES_$(@)) -o $@

$(OUTPUT)%.hex:
	$(OBJCOPY) -O ihex $(filter %.elf,$(^)) $@

$(OUTPUT)%.bin:
	$(OBJCOPY) -O binary $(filter %.elf,$(^)) $@

$(OUTPUT)%.dmp:
	$(OBJDUMP) -x --syms --demangle $(filter %.elf,$(^)) > $@

$(OUTPUT)%.lss:
	$(OBJDUMP) --demangle -S $(filter %.elf,$(^)) > $@

.PHONY: size
size:
	$(SIZE) -B $(filter %.elf,$(^))

.PHONY: clean
clean:
	$(RM) -r $(OUTPUT)

.PHONY: configure
configure:
	@./configure.sh $(CFG_PATH)

.PHONY: distclean
distclean:
	@./scripts/distclean.sh

.PHONY: menuconfig
menuconfig:
	@kconfig-mconf Kconfig

.PHONY: help
help:
	@echo ''
	@echo 'Available special targets:'
	@echo '  menuconfig .. to configure distortos'
	@echo '  configure  .. to execute configure.sh; use "make configure CFG_PATH=<path>"'
	@echo '                <path> .. the path where distortosConfiguration.mk can'
	@echo '                          be found; default "./"'
	@echo '  distclean  .. remove all generated files and configuration files'
	@echo '  clean      .. remove all generated files'
	@echo ''
