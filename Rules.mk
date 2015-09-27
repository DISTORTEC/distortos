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

SUBDIRECTORIES += source
SUBDIRECTORIES += test

#-----------------------------------------------------------------------------------------------------------------------
# generated headers
#-----------------------------------------------------------------------------------------------------------------------

DISTORTOS_CONFIGURATION_H := $(OUTPUT)include/distortos/distortosConfiguration.h

$(DISTORTOS_CONFIGURATION_H): $(DISTORTOS_CONFIGURATION_MK)
	./scripts/makeDistortosConfiguration.awk "$<" > "$@"

#-----------------------------------------------------------------------------------------------------------------------
# generated headers depend on this Rules.mk, the script that generates them and the selectedConfiguration.mk file
#-----------------------------------------------------------------------------------------------------------------------

$(DISTORTOS_CONFIGURATION_H): Rules.mk scripts/makeDistortosConfiguration.awk selectedConfiguration.mk

#-----------------------------------------------------------------------------------------------------------------------
# final targets
#-----------------------------------------------------------------------------------------------------------------------

ELF := $(OUTPUT)$(PROJECT).elf
HEX := $(OUTPUT)$(PROJECT).hex
BIN := $(OUTPUT)$(PROJECT).bin
DMP := $(OUTPUT)$(PROJECT).dmp
LSS := $(OUTPUT)$(PROJECT).lss

#-----------------------------------------------------------------------------------------------------------------------
# add generated headers and final targets to list of generated files
#-----------------------------------------------------------------------------------------------------------------------

GENERATED := $(GENERATED) $(DISTORTOS_CONFIGURATION_H) $(ELF) $(HEX) $(BIN) $(DMP) $(LSS) $(OUTPUT)$(PROJECT).map

#-----------------------------------------------------------------------------------------------------------------------
# standard footer
#-----------------------------------------------------------------------------------------------------------------------

include footer.mk

#-----------------------------------------------------------------------------------------------------------------------
# .elf file depends on all objects, linker script and this Rules.mk
#-----------------------------------------------------------------------------------------------------------------------

$(ELF): $(OBJECTS) $(LDSCRIPT) Rules.mk

#-----------------------------------------------------------------------------------------------------------------------
# .hex, .bin, .dmp and .lss files depends on .elf file and this Rules.mk
#-----------------------------------------------------------------------------------------------------------------------

$(HEX): $(ELF) Rules.mk
$(BIN): $(ELF) Rules.mk
$(DMP): $(ELF) Rules.mk
$(LSS): $(ELF) Rules.mk

#-----------------------------------------------------------------------------------------------------------------------
# print size of generated .elf file
#-----------------------------------------------------------------------------------------------------------------------

size: $(ELF)
