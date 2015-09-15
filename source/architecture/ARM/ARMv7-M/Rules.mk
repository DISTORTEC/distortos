#
# file: Rules.mk
#
# author: Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
#
# date: 2015-09-15
#

#-----------------------------------------------------------------------------------------------------------------------
# compilation flags
#-----------------------------------------------------------------------------------------------------------------------

ASFLAGS_$(d)/ARMv7-M-Reset_Handler.S := -D__USES_TWO_STACKS -D__USES_CXX

CXXFLAGS_$(d) := $(CXXFLAGS_$(d)) -I$(OUTPUT)include
CXXFLAGS_$(d) := $(CXXFLAGS_$(d)) -Iinclude
CXXFLAGS_$(d) := $(CXXFLAGS_$(d)) -Isource/chip/STMicroelectronics/STM32F4/include
CXXFLAGS_$(d) := $(CXXFLAGS_$(d)) -Iexternal/CMSIS-STM32F4
CXXFLAGS_$(d) := $(CXXFLAGS_$(d)) -Iexternal/CMSIS
CXXFLAGS_$(d) := $(CXXFLAGS_$(d)) -Isource/architecture/ARM/ARMv7-M/include

#-----------------------------------------------------------------------------------------------------------------------
# standard footer
#-----------------------------------------------------------------------------------------------------------------------

include footer.mk
