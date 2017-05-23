#
# file: Rules.mk
#
# author: Copyright (C) 2017 Cezary Gapinski cezary.gapinski@gmail.com
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
#

ifeq ($(CONFIG_CHIP_STM32L0),y)

#-----------------------------------------------------------------------------------------------------------------------
# subdirectories
#-----------------------------------------------------------------------------------------------------------------------

SUBDIRECTORIES += $(patsubst $(d)%/Rules.mk,%,$(wildcard $(d)*/Rules.mk))

#-----------------------------------------------------------------------------------------------------------------------
# standard footer
#-----------------------------------------------------------------------------------------------------------------------

include $(DISTORTOS_PATH)footer.mk

endif	# eq ($(CONFIG_CHIP_STM32L0),y)
