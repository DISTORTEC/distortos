#
# file: Rules.mk
#
# author: Copyright (C) 2015-2017 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
#

ifeq ($(CONFIG_CHIP_STM32F4),y)

ifndef CONFIG_LDSCRIPT

#-----------------------------------------------------------------------------------------------------------------------
# linker script
#-----------------------------------------------------------------------------------------------------------------------

ROM_ADDRESS := $(shell printf '0x%x' '$(shell echo $$(($(CONFIG_CHIP_ROM_ADDRESS) + $(CONFIG_LDSCRIPT_ROM_BEGIN))))')
ROM_SIZE := $(shell echo $$(($(CONFIG_LDSCRIPT_ROM_END) - $(CONFIG_LDSCRIPT_ROM_BEGIN))))

ifdef CONFIG_CHIP_STM32F4_UNIFY_SRAM1_SRAM2
	SRAM2_UNIFIED := 1
	UNIFIED_RAM_SIZE := $(shell echo $$(($(CONFIG_CHIP_STM32F4_SRAM1_SIZE) + $(CONFIG_CHIP_STM32F4_SRAM2_SIZE))))
else ifdef CONFIG_CHIP_STM32F4_UNIFY_SRAM1_SRAM2_SRAM3
	SRAM2_UNIFIED := 1
	SRAM3_UNIFIED := 1
	UNIFIED_RAM_SIZE := $(shell echo $$(($(CONFIG_CHIP_STM32F4_SRAM1_SIZE) + $(CONFIG_CHIP_STM32F4_SRAM2_SIZE) + \
			$(CONFIG_CHIP_STM32F4_SRAM3_SIZE))))
else	# CONFIG_CHIP_STM32F4_UNIFY_NONE || CONFIG_CHIP_STM32F4_UNIFY_SRAM2_SRAM3
	UNIFIED_RAM_SIZE := $(CONFIG_CHIP_STM32F4_SRAM1_SIZE)
endif

LD_SCRIPT_GENERATOR := $(DISTORTOS_PATH)source/architecture/ARM/ARMv6-M-ARMv7-M/ARMv6-M-ARMv7-M.ld.sh
LD_SCRIPT_GENERATOR_ARGUMENTS := $(CONFIG_CHIP) \
		"$(ROM_ADDRESS),$(ROM_SIZE)" \
		"$(CONFIG_CHIP_STM32F4_SRAM1_ADDRESS),$(UNIFIED_RAM_SIZE)"

ifdef CONFIG_CHIP_STM32F4_BKPSRAM_ADDRESS
	LD_SCRIPT_GENERATOR_ARGUMENTS +=\
			"bkpsram,$(CONFIG_CHIP_STM32F4_BKPSRAM_ADDRESS),$(CONFIG_CHIP_STM32F4_BKPSRAM_SIZE)"
endif	# def CONFIG_CHIP_STM32F4_BKPSRAM_ADDRESS

ifdef CONFIG_CHIP_STM32F4_CCM_ADDRESS
	LD_SCRIPT_GENERATOR_ARGUMENTS += "ccm,$(CONFIG_CHIP_STM32F4_CCM_ADDRESS),$(CONFIG_CHIP_STM32F4_CCM_SIZE)"
endif	# def CONFIG_CHIP_STM32F4_CCM_ADDRESS

ifdef CONFIG_CHIP_STM32F4_UNIFY_SRAM2_SRAM3
	SRAM2_UNIFIED := 1
	SRAM3_UNIFIED := 1
	SRAM23_SIZE := $(shell echo $$(($(CONFIG_CHIP_STM32F4_SRAM2_SIZE) + $(CONFIG_CHIP_STM32F4_SRAM3_SIZE))))
	LD_SCRIPT_GENERATOR_ARGUMENTS += "sram23,$(CONFIG_CHIP_STM32F4_SRAM2_ADDRESS),$(SRAM23_SIZE)"
endif	# def CONFIG_CHIP_STM32F4_UNIFY_SRAM2_SRAM3

ifdef CONFIG_CHIP_STM32F4_SRAM2_ADDRESS
ifndef SRAM2_UNIFIED
	LD_SCRIPT_GENERATOR_ARGUMENTS += "sram2,$(CONFIG_CHIP_STM32F4_SRAM2_ADDRESS),$(CONFIG_CHIP_STM32F4_SRAM2_SIZE)"
endif	# ndef SRAM2_UNIFIED
endif	# def CONFIG_CHIP_STM32F4_SRAM2_ADDRESS

ifdef CONFIG_CHIP_STM32F4_SRAM3_ADDRESS
ifndef SRAM3_UNIFIED
	LD_SCRIPT_GENERATOR_ARGUMENTS += "sram3,$(CONFIG_CHIP_STM32F4_SRAM3_ADDRESS),$(CONFIG_CHIP_STM32F4_SRAM3_SIZE)"
endif	# ndef SRAM3_UNIFIED
endif	# def CONFIG_CHIP_STM32F4_SRAM3_ADDRESS

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
# standard footer
#-----------------------------------------------------------------------------------------------------------------------

include $(DISTORTOS_PATH)footer.mk

endif	# eq ($(CONFIG_CHIP_STM32F4),y)
