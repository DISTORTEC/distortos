#
# file: Rules.mk
#
# author: Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
#
# date: 2015-09-18
#

#-----------------------------------------------------------------------------------------------------------------------
# linker script
#-----------------------------------------------------------------------------------------------------------------------

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

CFLAGS_$(d) := $(CFLAGS_$(d)) $(STANDARD_INCLUDES)

CXXFLAGS_$(d) := $(CXXFLAGS_$(d)) $(STANDARD_INCLUDES)
CXXFLAGS_$(d) := $(CXXFLAGS_$(d)) -Isource/architecture/ARM/ARMv7-M/include
CXXFLAGS_$(d) := $(CXXFLAGS_$(d)) -Isource/chip/STMicroelectronics/STM32F4/include
CXXFLAGS_$(d) := $(CXXFLAGS_$(d)) -Iexternal/CMSIS-STM32F4
CXXFLAGS_$(d) := $(CXXFLAGS_$(d)) -Iexternal/CMSIS

#-----------------------------------------------------------------------------------------------------------------------
# standard footer
#-----------------------------------------------------------------------------------------------------------------------

include footer.mk
