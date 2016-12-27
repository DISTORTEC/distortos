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

ROM_ADDRESS := $(shell printf '0x%x' '$(shell echo $$(($(CONFIG_CHIP_ROM_ADDRESS) + $(CONFIG_LDSCRIPT_ROM_BEGIN))))')
ROM_SIZE := $(shell echo $$(($(CONFIG_LDSCRIPT_ROM_END) - $(CONFIG_LDSCRIPT_ROM_BEGIN))))

LD_SCRIPT_GENERATOR := $(DISTORTOS_PATH)source/architecture/ARM/ARMv6-M-ARMv7-M/ARMv6-M-ARMv7-M.ld.sh
MAIN_THREAD_STACK_SIZE := $(shell echo $$(((($(CONFIG_MAIN_THREAD_STACK_SIZE) + \
		$(CONFIG_ARCHITECTURE_STACK_ALIGNMENT) - 1) / $(CONFIG_ARCHITECTURE_STACK_ALIGNMENT) + \
		($(CONFIG_STACK_GUARD_SIZE) + $(CONFIG_ARCHITECTURE_STACK_ALIGNMENT) - 1) / \
		$(CONFIG_ARCHITECTURE_STACK_ALIGNMENT)) * $(CONFIG_ARCHITECTURE_STACK_ALIGNMENT))))
LD_SCRIPT_GENERATOR_ARGUMENTS := $(CONFIG_CHIP) \
		"$(ROM_ADDRESS),$(ROM_SIZE)" \
		"$(CONFIG_CHIP_STM32F0_SRAM_ADDRESS),$(CONFIG_CHIP_STM32F0_SRAM_SIZE)" \
		"$(CONFIG_ARCHITECTURE_ARMV6_M_ARMV7_M_MAIN_STACK_SIZE)" "$(MAIN_THREAD_STACK_SIZE)"

$(LDSCRIPT): $(DISTORTOS_CONFIGURATION_MK)
	$(call PRETTY_PRINT,"SH     " $(LD_SCRIPT_GENERATOR))
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
CXXFLAGS_$(d) := $(CXXFLAGS_$(d)) $(ARCHITECTURE_INCLUDES)
CXXFLAGS_$(d) := $(CXXFLAGS_$(d)) $(CHIP_INCLUDES)

#-----------------------------------------------------------------------------------------------------------------------
# standard footer
#-----------------------------------------------------------------------------------------------------------------------

include $(DISTORTOS_PATH)footer.mk

endif	# eq ($(CONFIG_CHIP_STM32F0),y)
