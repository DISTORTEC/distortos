#
# file: Rules.mk
#
# author: Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
#
# date: 2015-09-27
#

#-----------------------------------------------------------------------------------------------------------------------
# subdirectories
#-----------------------------------------------------------------------------------------------------------------------

SUBDIRECTORIES += architecture/ARM/ARMv7-M
SUBDIRECTORIES += CallOnce
SUBDIRECTORIES += ConditionVariable
SUBDIRECTORIES += FifoQueue
SUBDIRECTORIES += MessageQueue
SUBDIRECTORIES += Mutex
SUBDIRECTORIES += RawFifoQueue
SUBDIRECTORIES += RawMessageQueue
SUBDIRECTORIES += Semaphore
SUBDIRECTORIES += Signals
SUBDIRECTORIES += SoftwareTimer
SUBDIRECTORIES += Thread

#-----------------------------------------------------------------------------------------------------------------------
# final targets
#-----------------------------------------------------------------------------------------------------------------------

ELF_$(d) := $(OUTPUT)$(d)/$(PROJECT).elf
HEX_$(d) := $(OUTPUT)$(d)/$(PROJECT).hex
BIN_$(d) := $(OUTPUT)$(d)/$(PROJECT).bin
DMP_$(d) := $(OUTPUT)$(d)/$(PROJECT).dmp
LSS_$(d) := $(OUTPUT)$(d)/$(PROJECT).lss

#-----------------------------------------------------------------------------------------------------------------------
# add final targets to list of generated files
#-----------------------------------------------------------------------------------------------------------------------

GENERATED := $(GENERATED) $(ELF_$(d)) $(ELF_$(d):.elf=.map) $(HEX_$(d)) $(BIN_$(d)) $(DMP_$(d)) $(LSS_$(d))

#-----------------------------------------------------------------------------------------------------------------------
# compilation flags
#-----------------------------------------------------------------------------------------------------------------------

CXXFLAGS_$(d) := $(CXXFLAGS_$(d)) -I$(d)
CXXFLAGS_$(d) := $(CXXFLAGS_$(d)) $(STANDARD_INCLUDES)
CXXFLAGS_$(d) := $(CXXFLAGS_$(d)) $(ARCHITECTURE_INCLUDES)

#-----------------------------------------------------------------------------------------------------------------------
# standard footer
#-----------------------------------------------------------------------------------------------------------------------

include footer.mk

#-----------------------------------------------------------------------------------------------------------------------
# .elf file dependencies - libdistortos.a, all objects from this folder tree, linker script and this Rules.mk
#-----------------------------------------------------------------------------------------------------------------------

$(ELF_$(d)): $(OUTPUT)libdistortos.a $(OBJECTS_$(d)) $(SUBDIRECTORIES_OBJECTS_$(d)) $(LDSCRIPT) $(d)/Rules.mk 

#-----------------------------------------------------------------------------------------------------------------------
# .hex, .bin, .dmp and .lss files depends on .elf file and this Rules.mk
#-----------------------------------------------------------------------------------------------------------------------

$(HEX_$(d)): $(ELF_$(d)) $(d)/Rules.mk
$(BIN_$(d)): $(ELF_$(d)) $(d)/Rules.mk
$(DMP_$(d)): $(ELF_$(d)) $(d)/Rules.mk
$(LSS_$(d)): $(ELF_$(d)) $(d)/Rules.mk

#-----------------------------------------------------------------------------------------------------------------------
# print size of generated .elf file
#-----------------------------------------------------------------------------------------------------------------------

size: $(ELF_$(d))
