#
# file: Rules.mk
#
# author: Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
#
# date: 2015-08-28
#

#-----------------------------------------------------------------------------------------------------------------------
# linker script
#-----------------------------------------------------------------------------------------------------------------------

LDSCRIPT := $(OUTPUT)$(subst ",,$(CONFIG_CHIP)).ld
STM32F4_LD_SH := $(d)/STM32F4.ld.sh

$(LDSCRIPT): $(DISTORTOS_CONFIGURATION_MK)
	./$(STM32F4_LD_SH) "$<" > "$@"

#-----------------------------------------------------------------------------------------------------------------------
# rule to create output folder for linker script
#-----------------------------------------------------------------------------------------------------------------------

LDSCRIPT_DIRECTORY := $(dir $(LDSCRIPT))

$(LDSCRIPT): | $(LDSCRIPT_DIRECTORY)
$(LDSCRIPT_DIRECTORY):
	mkdir -p $(LDSCRIPT_DIRECTORY)

#-----------------------------------------------------------------------------------------------------------------------
# generated linker script depends on this Rules.mk, the script that generates it and the selectedConfiguration.mk file
#-----------------------------------------------------------------------------------------------------------------------

$(LDSCRIPT): $(d)/Rules.mk $(STM32F4_LD_SH) selectedConfiguration.mk

#-----------------------------------------------------------------------------------------------------------------------
# add generated linker script to list of generated files
#-----------------------------------------------------------------------------------------------------------------------

GENERATED := $(GENERATED) $(LDSCRIPT)

#-----------------------------------------------------------------------------------------------------------------------
# compilation flags
#-----------------------------------------------------------------------------------------------------------------------

CFLAGS_$(d) := $(CFLAGS_$(d)) -I$(OUTPUT)include
CFLAGS_$(d) := $(CFLAGS_$(d)) -Iinclude

#-----------------------------------------------------------------------------------------------------------------------
# linker scripts (used as explicit dependency of .elf file)
#-----------------------------------------------------------------------------------------------------------------------

LDSCRIPTS := $(LDSCRIPTS) $(d)/STM32F4xxxG.ld $(LDSCRIPT)

#-----------------------------------------------------------------------------------------------------------------------
# standard footer
#-----------------------------------------------------------------------------------------------------------------------

include footer.mk
