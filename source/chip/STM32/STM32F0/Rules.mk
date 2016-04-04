#
# file: Rules.mk
#
# author: Copyright (C) 2015-2016 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
#

ifeq ($(CONFIG_CHIP_STM32F0),y)

#-----------------------------------------------------------------------------------------------------------------------
# linker script
#-----------------------------------------------------------------------------------------------------------------------

LD_SCRIPT_GENERATOR := $(DISTORTOS_PATH)source/architecture/ARM/ARMv6-M-ARMv7-M/ARMv6-M-ARMv7-M.ld.sh
LD_SCRIPT_GENERATOR_ARGUMENTS := $(CONFIG_CHIP) \
		"$(CONFIG_CHIP_STM32F0_FLASH_ADDRESS),$(CONFIG_CHIP_STM32F0_FLASH_SIZE)" \
		"$(CONFIG_CHIP_STM32F0_SRAM_ADDRESS),$(CONFIG_CHIP_STM32F0_SRAM_SIZE)" \
		"$(CONFIG_ARCHITECTURE_ARMV6_M_ARMV7_M_MAIN_STACK_SIZE)" "$(CONFIG_MAIN_THREAD_STACK_SIZE)"

$(LDSCRIPT): $(DISTORTOS_CONFIGURATION_MK)
	$(call PRETTY_PRINT," SH     " $(LD_SCRIPT_GENERATOR))
	$(Q)./$(LD_SCRIPT_GENERATOR) $(LD_SCRIPT_GENERATOR_ARGUMENTS) > "$@"

#-----------------------------------------------------------------------------------------------------------------------
# generated linker script depends on this Rules.mk, the script that generates it and the selectedConfiguration.mk file
#-----------------------------------------------------------------------------------------------------------------------

$(LDSCRIPT): $(d)Rules.mk $(LD_SCRIPT_GENERATOR) selectedConfiguration.mk

#-----------------------------------------------------------------------------------------------------------------------
# add generated linker script to list of generated files
#-----------------------------------------------------------------------------------------------------------------------

GENERATED := $(GENERATED) $(LDSCRIPT)

#-----------------------------------------------------------------------------------------------------------------------
# compilation flags
#-----------------------------------------------------------------------------------------------------------------------

CXXFLAGS_$(d) := $(CXXFLAGS_$(d)) $(STANDARD_INCLUDES)
CXXFLAGS_$(d) := $(CXXFLAGS_$(d)) $(CHIP_INCLUDES)

#-----------------------------------------------------------------------------------------------------------------------
# standard footer
#-----------------------------------------------------------------------------------------------------------------------

include $(DISTORTOS_PATH)footer.mk

endif	# eq ($(CONFIG_CHIP_STM32F0),y)
