#
# file: Rules.mk
#
# author: Copyright (C) 2015-2016 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
#

ifeq ($(CONFIG_TEST_APPLICATION_ENABLE),y)

#-----------------------------------------------------------------------------------------------------------------------
# subdirectories
#-----------------------------------------------------------------------------------------------------------------------

SUBDIRECTORIES += $(patsubst $(d)%/Rules.mk,%,$(wildcard $(d)*/Rules.mk))

#-----------------------------------------------------------------------------------------------------------------------
# final targets
#-----------------------------------------------------------------------------------------------------------------------

FILENAME_$(d) := $(OUTPUT)$(d)distortosTest
ELF_$(d) := $(FILENAME_$(d)).elf
HEX_$(d) := $(FILENAME_$(d)).hex
BIN_$(d) := $(FILENAME_$(d)).bin
DMP_$(d) := $(FILENAME_$(d)).dmp
LSS_$(d) := $(FILENAME_$(d)).lss

#-----------------------------------------------------------------------------------------------------------------------
# add final targets to list of generated files
#-----------------------------------------------------------------------------------------------------------------------

GENERATED := $(GENERATED) $(ELF_$(d)) $(ELF_$(d):%.elf=%.map) $(HEX_$(d)) $(BIN_$(d)) $(DMP_$(d)) $(LSS_$(d))

#-----------------------------------------------------------------------------------------------------------------------
# compilation flags
#-----------------------------------------------------------------------------------------------------------------------

CXXFLAGS_$(d) := $(CXXFLAGS_$(d)) -I$(d)
CXXFLAGS_$(d) := $(CXXFLAGS_$(d)) $(STANDARD_INCLUDES)
CXXFLAGS_$(d) := $(CXXFLAGS_$(d)) $(ARCHITECTURE_INCLUDES)
CXXFLAGS_$(d) := $(CXXFLAGS_$(d)) $(CHIP_INCLUDES)
CXXFLAGS_$(d) := $(CXXFLAGS_$(d)) $(BOARD_INCLUDES)

#-----------------------------------------------------------------------------------------------------------------------
# standard footer
#-----------------------------------------------------------------------------------------------------------------------

include $(DISTORTOS_PATH)footer.mk

#-----------------------------------------------------------------------------------------------------------------------
# .elf file dependencies - libdistortos.a, all objects from this folder tree, linker script and this Rules.mk
#-----------------------------------------------------------------------------------------------------------------------

$(ELF_$(d)): $(OUTPUT)libdistortos.a $(OBJECTS_$(d)) $(SUBDIRECTORIES_OBJECTS_$(d)) $(LDSCRIPT) $(d)Rules.mk

#-----------------------------------------------------------------------------------------------------------------------
# .hex, .bin, .dmp and .lss files depend on .elf file and this Rules.mk
#-----------------------------------------------------------------------------------------------------------------------

$(HEX_$(d)): $(ELF_$(d)) $(d)Rules.mk
$(BIN_$(d)): $(ELF_$(d)) $(d)Rules.mk
$(DMP_$(d)): $(ELF_$(d)) $(d)Rules.mk
$(LSS_$(d)): $(ELF_$(d)) $(d)Rules.mk

#-----------------------------------------------------------------------------------------------------------------------
# print size of generated .elf file
#-----------------------------------------------------------------------------------------------------------------------

size: $(ELF_$(d))
all: size

endif	# eq ($(CONFIG_TEST_APPLICATION_ENABLE),y)
