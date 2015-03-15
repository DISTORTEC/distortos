#
# file: Rules.mk
#
# author: Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
#
# date: 2015-03-15
#

#-----------------------------------------------------------------------------------------------------------------------
# subdirectories
#-----------------------------------------------------------------------------------------------------------------------

SUBDIRECTORIES += source/allocators
SUBDIRECTORIES += source/architecture
SUBDIRECTORIES += source/chip/STMicroelectronics/STM32F4
SUBDIRECTORIES += source/clocks
SUBDIRECTORIES += source/scheduler
SUBDIRECTORIES += source/synchronization
SUBDIRECTORIES += source/syscalls
SUBDIRECTORIES += source/threads
SUBDIRECTORIES += test

#-----------------------------------------------------------------------------------------------------------------------
# final targets
#-----------------------------------------------------------------------------------------------------------------------

ELF := $(ELF) $(OUTPUT)$(PROJECT).elf
HEX := $(HEX) $(OUTPUT)$(PROJECT).hex
BIN := $(BIN) $(OUTPUT)$(PROJECT).bin
DMP := $(DMP) $(OUTPUT)$(PROJECT).dmp
LSS := $(LSS) $(OUTPUT)$(PROJECT).lss

#-----------------------------------------------------------------------------------------------------------------------
# .elf file depends on this Rules.mk
#-----------------------------------------------------------------------------------------------------------------------

$(ELF): Rules.mk

#-----------------------------------------------------------------------------------------------------------------------
# add final targets to list of generated files
#-----------------------------------------------------------------------------------------------------------------------

GENERATED := $(GENERATED) $(ELF) $(HEX) $(BIN) $(DMP) $(LSS) $(OUTPUT)$(PROJECT).map
