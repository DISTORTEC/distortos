#
# file: Rules.mk
#
# author: Copyright (C) 2015-2017 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
#

ifeq ($(or $(CONFIG_ARCHITECTURE_ARMV6_M),$(CONFIG_ARCHITECTURE_ARMV7_M)),y)

#-----------------------------------------------------------------------------------------------------------------------
# linker script fragment with symbols
#-----------------------------------------------------------------------------------------------------------------------

SYMBOLS_LD_GENERATOR := $(DISTORTOS_PATH)scripts/symbols.ld.sh
SYMBOLS_LD_GENERATOR_ARGUMENTS := "__main_stack_size = ALIGN($(CONFIG_ARCHITECTURE_ARMV6_M_ARMV7_M_MAIN_STACK_SIZE), \
		$(CONFIG_ARCHITECTURE_STACK_ALIGNMENT))" "__process_stack_size = ALIGN($(CONFIG_MAIN_THREAD_STACK_SIZE), \
		$(CONFIG_ARCHITECTURE_STACK_ALIGNMENT)) + ALIGN($(CONFIG_STACK_GUARD_SIZE), \
		$(CONFIG_ARCHITECTURE_STACK_ALIGNMENT))"

$(SYMBOLS_LD): $(DISTORTOS_CONFIGURATION_MK)
	$(call PRETTY_PRINT,"SH     " $(SYMBOLS_LD_GENERATOR))
	$(Q)./$(SYMBOLS_LD_GENERATOR) $(SYMBOLS_LD_GENERATOR_ARGUMENTS) > "$@"

#-----------------------------------------------------------------------------------------------------------------------
# generated linker script fragment with symbols depends on this Rules.mk, the script that generates it and the
# selectedConfiguration.mk file
#-----------------------------------------------------------------------------------------------------------------------

$(SYMBOLS_LD): $(d)Rules.mk $(SYMBOLS_LD_GENERATOR) selectedConfiguration.mk

#-----------------------------------------------------------------------------------------------------------------------
# add generated linker script fragment with symbols to list of generated files
#-----------------------------------------------------------------------------------------------------------------------

GENERATED := $(GENERATED) $(SYMBOLS_LD)

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

endif	# eq ($(or $(CONFIG_ARCHITECTURE_ARMV6_M),$(CONFIG_ARCHITECTURE_ARMV7_M)),y)
