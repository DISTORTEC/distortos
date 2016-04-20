#
# file: Makefile
#
# author: Copyright (C) 2015-2016 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
#

#-----------------------------------------------------------------------------------------------------------------------
# project configuration
#-----------------------------------------------------------------------------------------------------------------------

# global assembler flags
ASFLAGS =

# global C flags
CFLAGS = -Wall -Wstrict-prototypes -Wextra -Wshadow -std=gnu99

# global C++ flags
CXXFLAGS = -Wall -Wextra -Wshadow -std=gnu++11

# linker flags
LDFLAGS =

# build mode (0 - non-verbose, 1 - verbose)
VERBOSE ?= 0

#-----------------------------------------------------------------------------------------------------------------------
# output folder
#-----------------------------------------------------------------------------------------------------------------------

OUTPUT = output/

#-----------------------------------------------------------------------------------------------------------------------
# support for building distortos as a submodule
#-----------------------------------------------------------------------------------------------------------------------

ifneq ($(DISTORTOS_PATH),)
	# strip trailing slash (if any) and add a single one
	DISTORTOS_PATH := $(DISTORTOS_PATH:%/=%)/
endif

#-----------------------------------------------------------------------------------------------------------------------
# make control
#-----------------------------------------------------------------------------------------------------------------------

DO_INCLUDE := 1
SIMPLE_TARGETS := clean configure distclean help menuconfig

# This macro checks, if the make target list MAKECMDGOALS contains the given simple target $1. If so, it executes
# SET_SIMPLE_TARGETS to set/clear some variables.
# parmeter 1 .. simple target to check
CHECK_SIMPLE_TARGETS = $(if $(findstring $(filter $(1),$(MAKECMDGOALS)),$(1)),$(call SET_SIMPLE_TARGETS,$(1)))
SET_SIMPLE_TARGETS = $(eval DO_INCLUDE := 0)

# check all simple targets if present
$(eval $(foreach target,$(SIMPLE_TARGETS),$(call CHECK_SIMPLE_TARGETS,$(target))))

#-----------------------------------------------------------------------------------------------------------------------
# load configuration variables from distortosConfiguration.mk file selected by user
#-----------------------------------------------------------------------------------------------------------------------

DISTORTOS_CONFIGURATION_MK = distortosConfiguration.mk

ifneq ($(wildcard selectedConfiguration.mk),)

    # file with $(CONFIG_SELECTED_CONFIGURATION) variable
    include selectedConfiguration.mk

    # path to distortosConfiguration.mk file selected by $(CONFIG_SELECTED_CONFIGURATION) variable
    DISTORTOS_CONFIGURATION_MK = ./$(subst ",,$(CONFIG_SELECTED_CONFIGURATION))

endif

ifeq ($(DO_INCLUDE),1)

    ifeq ($(wildcard selectedConfiguration.mk),)
        $(error Please run first 'make configure [CONFIG_PATH=<path-to-distortosConfiguration.mk>]')
    endif

    include $(DISTORTOS_CONFIGURATION_MK)

endif

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
# add obligatory compilation flags
#-----------------------------------------------------------------------------------------------------------------------

CONFIG_ARCHITECTURE_FLAGS := $(subst ",,$(CONFIG_ARCHITECTURE_FLAGS))
CONFIG_BUILD_OPTIMIZATION := $(subst ",,$(CONFIG_BUILD_OPTIMIZATION))
CONFIG_DEBUGGING_INFORMATION_COMPILATION := $(subst ",,$(CONFIG_DEBUGGING_INFORMATION_COMPILATION))

ASFLAGS += $(CONFIG_DEBUGGING_INFORMATION_COMPILATION)
ASFLAGS += $(CONFIG_ARCHITECTURE_FLAGS)
ASFLAGS += -MD -MP

CFLAGS += $(CONFIG_DEBUGGING_INFORMATION_COMPILATION)
CFLAGS += $(CONFIG_ARCHITECTURE_FLAGS)
CFLAGS += $(CONFIG_BUILD_OPTIMIZATION)
CFLAGS += -ffunction-sections -fdata-sections -MD -MP

CXXFLAGS += $(CONFIG_DEBUGGING_INFORMATION_COMPILATION)
CXXFLAGS += $(CONFIG_ARCHITECTURE_FLAGS)
CXXFLAGS += $(CONFIG_BUILD_OPTIMIZATION)
CXXFLAGS += -ffunction-sections -fdata-sections -fno-rtti -fno-exceptions -MD -MP

# path to linker script (generated automatically)
LDSCRIPT = $(OUTPUT)$(subst ",,$(CONFIG_CHIP)).ld

LDFLAGS += $(subst ",,$(CONFIG_DEBUGGING_INFORMATION_LINKING))
LDFLAGS += $(CONFIG_ARCHITECTURE_FLAGS)
LDFLAGS += -Wl,-Map=$(@:%.elf=%.map),--cref,--gc-sections

#-----------------------------------------------------------------------------------------------------------------------
# "constants" with include paths
#-----------------------------------------------------------------------------------------------------------------------

# "standard" includes
STANDARD_INCLUDES += -I$(OUTPUT)include -I$(DISTORTOS_PATH)include

# architecture includes
ARCHITECTURE_INCLUDES += $(patsubst %,-I$(DISTORTOS_PATH)%,$(subst ",,$(CONFIG_ARCHITECTURE_INCLUDES)))

# chip includes
CHIP_INCLUDES += $(patsubst %,-I$(DISTORTOS_PATH)%,$(subst ",,$(CONFIG_CHIP_INCLUDES)))

# board includes
BOARD_INCLUDES += $(patsubst %,-I$(DISTORTOS_PATH)%,$(subst ",,$(CONFIG_BOARD_INCLUDES)))

#-----------------------------------------------------------------------------------------------------------------------
# build macros
#-----------------------------------------------------------------------------------------------------------------------

ifndef ECHO
	I := i
	TARGET_COUNTER = $(words $(I)) $(eval I += i)
	TOTAL_TARGETS := $(shell $(MAKE) $(MAKECMDGOALS) --dry-run --file=$(firstword $(MAKEFILE_LIST)) \
			--no-print-directory --no-builtin-rules --no-builtin-variables ECHO="COUNTTHIS" | grep -c "COUNTTHIS")
	ECHO = echo "[`expr "  \`expr ${TARGET_COUNTER} '*' 100 / ${TOTAL_TARGETS}\`" : '.*\(...\)$$'`%]"
endif

ifeq ($(VERBOSE),0)
	Q = @
	PRETTY_PRINT = @$(ECHO) $(1)
else
	Q =
	PRETTY_PRINT =
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
	ifeq ($$(d),./)
		NEXT_DIRECTORY := $(1)/
	else
		NEXT_DIRECTORY := $$(d)$(1)/
	endif
else
	NEXT_DIRECTORY := $(1)/
endif
STACK_POINTER := $$(STACK_POINTER).x
DIRECTORY_STACK_$$(STACK_POINTER) := $$(d)
d := $$(NEXT_DIRECTORY)
SUBDIRECTORIES :=
include $$(d)Rules.mk
d := $$(DIRECTORY_STACK_$$(STACK_POINTER))
STACK_POINTER := $$(basename $$(STACK_POINTER))
endef

define PARSE_SUBDIRECTORIES
$(foreach subdirectory,$(sort $(1)),$(eval $(call PARSE_SUBDIRECTORY,$(subdirectory))))
endef

#-----------------------------------------------------------------------------------------------------------------------
# build targets
#-----------------------------------------------------------------------------------------------------------------------

# clear all implicit suffix rules
.SUFFIXES:

.PHONY: all
all: targets

ifeq ($(DO_INCLUDE),1)
    # trigger parsing of all Rules.mk files
    $(call PARSE_SUBDIRECTORIES,.)
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

$(GENERATED): $(DISTORTOS_PATH)Makefile

$(OUTPUT)%.o: %.S
	$(call PRETTY_PRINT,"AS     " $<)
	$(Q)$(AS) $(ASFLAGS) $(ASFLAGS_$(<)) -c $< -o $@

$(OUTPUT)%.o: %.c
	$(call PRETTY_PRINT,"CC     " $<)
	$(Q)$(CC) $(CFLAGS) $(CFLAGS_$(<)) -c $< -o $@

$(OUTPUT)%.o: %.cpp
	$(call PRETTY_PRINT,"CXX    " $<)
	$(Q)$(CXX) $(CXXFLAGS) $(CXXFLAGS_$(<)) -c $< -o $@

$(OUTPUT)%.a:
	$(Q)$(RM) $@
	$(call PRETTY_PRINT,"AR     " $@)
	$(Q)$(AR) rcs $@ $(filter %.o,$(^))

$(OUTPUT)%.elf:
	$(call PRETTY_PRINT,"LD     " $@)
	$(eval ARCHIVES_$@ := -Wl,--whole-archive $(addprefix -l:,$(filter %.a,$(^))) -Wl,--no-whole-archive)
	$(Q)$(LD) $(LDFLAGS) -T$(filter %.ld,$(^)) $(filter %.o,$(^)) $(ARCHIVES_$(@)) -o $@

$(OUTPUT)%.hex:
	$(call PRETTY_PRINT,"HEX    " $@)
	$(Q)$(OBJCOPY) -O ihex $(filter %.elf,$(^)) $@

$(OUTPUT)%.bin:
	$(call PRETTY_PRINT,"BIN    " $@)
	$(Q)$(OBJCOPY) -O binary $(filter %.elf,$(^)) $@

$(OUTPUT)%.dmp:
	$(call PRETTY_PRINT,"DMP    " $@)
	$(Q)$(OBJDUMP) -x --syms --demangle $(filter %.elf,$(^)) > $@

$(OUTPUT)%.lss:
	$(call PRETTY_PRINT,"LSS    " $@)
	$(Q)$(OBJDUMP) --demangle -S $(filter %.elf,$(^)) > $@

.PHONY: size
size:
	$(Q)$(SIZE) -B $(filter %.elf,$(^))

.PHONY: clean
clean:
	$(RM) -r $(OUTPUT)

.PHONY: configure
configure:
	./$(DISTORTOS_PATH)scripts/configure.sh $(CONFIG_PATH)

.PHONY: distclean
distclean:
	./$(DISTORTOS_PATH)scripts/distclean.sh

.PHONY: doxygen
doxygen: all
	$(eval EXCLUDE_STRING := EXCLUDE =)
	$(eval EXCLUDE_STRING += $(DISTORTOS_PATH)external)
	$(eval HTML_FOOTER_STRING := HTML_FOOTER =)
	$(eval HTML_FOOTER_STRING += $(HTML_FOOTER))
	$(eval IMAGE_PATH_STRING := IMAGE_PATH =)
	$(eval IMAGE_PATH_STRING += $(DISTORTOS_PATH)documentation/images)
	$(eval INCLUDE_PATH_STRING := INCLUDE_PATH =)
	$(eval INCLUDE_PATH_STRING += $(patsubst -I%,%,$(STANDARD_INCLUDES)))
	$(eval INCLUDE_PATH_STRING += $(patsubst -I%,%,$(ARCHITECTURE_INCLUDES)))
	$(eval INCLUDE_PATH_STRING += $(patsubst -I%,%,$(CHIP_INCLUDES)))
	$(eval INCLUDE_PATH_STRING += $(patsubst -I%,%,$(BOARD_INCLUDES)))
	$(eval INCLUDE_PATH_STRING += $(DISTORTOS_PATH)test)
	$(eval PROJECT_NUMBER_STRING := PROJECT_NUMBER =)
	$(eval PROJECT_NUMBER_STRING += `git describe --dirty 2>/dev/null || date +%Y%m%d%H%M%S`)
	(cat $(DISTORTOS_PATH)Doxyfile; echo $(EXCLUDE_STRING); echo $(HTML_FOOTER_STRING); echo $(IMAGE_PATH_STRING); \
			echo $(INCLUDE_PATH_STRING); echo $(PROJECT_NUMBER_STRING)) | doxygen -

define NEWLINE


endef

define HELP_TEXT

Available special targets:
all - build current configuration
clean - remove the build output
configure [CONFIG_PATH=<path>] - to select the configuration of your choice;
  <path> .. the path where distortosConfiguration.mk can be found; default "."
distclean - remove the build output, doxygen documentation and file created by
  "make configure"
doxygen - generate doxygen API reference for current configuration
menuconfig - to create/edit configuration of distortos
olddefconfig - update currently selected configuration with default values of
  new options

endef

.PHONY: help
help:
	@echo -e '$(subst $(NEWLINE),\n,${HELP_TEXT})'

.PHONY: menuconfig
menuconfig:
	$(DISTORTOS_PATH)scripts/generateKconfig.sh "$(OUTPUT)"
	DISTORTOS_PATH=.$(DISTORTOS_PATH:%/=/%) KCONFIG_CONFIG=$(DISTORTOS_CONFIGURATION_MK) OUTPUT=.$(OUTPUT:%/=/%) \
			kconfig-mconf $(DISTORTOS_PATH)Kconfig

.PHONY: olddefconfig
olddefconfig:
	$(DISTORTOS_PATH)scripts/generateKconfig.sh "$(OUTPUT)"
	DISTORTOS_PATH=.$(DISTORTOS_PATH:%/=/%) KCONFIG_CONFIG=$(DISTORTOS_CONFIGURATION_MK) OUTPUT=.$(OUTPUT:%/=/%) \
			kconfig-conf $(DISTORTOS_PATH)Kconfig --olddefconfig
