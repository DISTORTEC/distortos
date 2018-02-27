#
# file: Rules.mk
#
# author: Copyright (C) 2017-2018 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
#

ifeq ($(CONFIG_CHIP_STM32L4),y)

ifndef CONFIG_LDSCRIPT

#-----------------------------------------------------------------------------------------------------------------------
# linker script
#-----------------------------------------------------------------------------------------------------------------------

ROM_ADDRESS := $(shell printf '0x%x' '$(shell echo $$(($(CONFIG_CHIP_ROM_ADDRESS) + $(CONFIG_LDSCRIPT_ROM_BEGIN))))')
ROM_SIZE := $(shell echo $$(($(CONFIG_LDSCRIPT_ROM_END) - $(CONFIG_LDSCRIPT_ROM_BEGIN))))

LD_SCRIPT_GENERATOR := $(DISTORTOS_PATH)source/architecture/ARM/ARMv6-M-ARMv7-M/ARMv6-M-ARMv7-M.ld.sh
LD_SCRIPT_GENERATOR_ARGUMENTS := $(CONFIG_CHIP) \
		"$(ROM_ADDRESS),$(ROM_SIZE)"

ifdef CONFIG_CHIP_STM32L4_UNIFY_SRAM1_SRAM2
	UNIFIED_RAM_SIZE := $(shell echo $$(($(CONFIG_CHIP_STM32L4_SRAM1_SIZE) + $(CONFIG_CHIP_STM32L4_SRAM2_SIZE))))
	LD_SCRIPT_GENERATOR_ARGUMENTS += "$(CONFIG_CHIP_STM32L4_SRAM1_ADDRESS),$(UNIFIED_RAM_SIZE)"
else
	LD_SCRIPT_GENERATOR_ARGUMENTS += "$(CONFIG_CHIP_STM32L4_SRAM1_ADDRESS),$(CONFIG_CHIP_STM32L4_SRAM1_SIZE)" \
			"sram2,$(CONFIG_CHIP_STM32L4_SRAM2_ADDRESS),$(CONFIG_CHIP_STM32L4_SRAM2_SIZE)"
endif

$(RAW_LDSCRIPT): $(DISTORTOS_CONFIGURATION_MK)
	$(call PRETTY_PRINT,"SH     " $(LD_SCRIPT_GENERATOR))
	$(Q)./$(LD_SCRIPT_GENERATOR) $(LD_SCRIPT_GENERATOR_ARGUMENTS) > "$(@)"

#-----------------------------------------------------------------------------------------------------------------------
# generated linker script depends on this Rules.mk, the script that generates it and the selectedConfiguration.mk file
#-----------------------------------------------------------------------------------------------------------------------

$(RAW_LDSCRIPT): $(d)Rules.mk $(LD_SCRIPT_GENERATOR) selectedConfiguration.mk

#-----------------------------------------------------------------------------------------------------------------------
# add generated linker script to list of generated files
#-----------------------------------------------------------------------------------------------------------------------

GENERATED := $(GENERATED) $(RAW_LDSCRIPT)

endif	# ndef CONFIG_LDSCRIPT

#-----------------------------------------------------------------------------------------------------------------------
# compilation flags
#-----------------------------------------------------------------------------------------------------------------------

CXXFLAGS_$(d) := $(CXXFLAGS_$(d)) $(STANDARD_INCLUDES)
CXXFLAGS_$(d) := $(CXXFLAGS_$(d)) $(ARCHITECTURE_INCLUDES)
CXXFLAGS_$(d) := $(CXXFLAGS_$(d)) $(CHIP_INCLUDES)

#-----------------------------------------------------------------------------------------------------------------------
# doxygen
#-----------------------------------------------------------------------------------------------------------------------

DOXYGEN_EXCLUDE := $(DOXYGEN_EXCLUDE) $(d)external

#-----------------------------------------------------------------------------------------------------------------------
# standard footer
#-----------------------------------------------------------------------------------------------------------------------

include $(DISTORTOS_PATH)footer.mk

endif	# eq ($(CONFIG_CHIP_STM32L4),y)
