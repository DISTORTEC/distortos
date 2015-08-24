#
# file: Rules.mk
#
# author: Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
#
# date: 2015-08-24
#

#-----------------------------------------------------------------------------------------------------------------------
# compilation flags
#-----------------------------------------------------------------------------------------------------------------------

CFLAGS_$(d) := $(CFLAGS_$(d)) -I$(OUTPUT)include
CFLAGS_$(d) := $(CFLAGS_$(d)) -Iinclude

#-----------------------------------------------------------------------------------------------------------------------
# linker scripts (used as explicit dependency of .elf file)
#-----------------------------------------------------------------------------------------------------------------------

LDSCRIPTS := $(LDSCRIPTS) $(d)/STM32F4xxxG.ld

#-----------------------------------------------------------------------------------------------------------------------
# standard footer
#-----------------------------------------------------------------------------------------------------------------------

include footer.mk
