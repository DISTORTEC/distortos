#
# file: Rules.mk
#
# author: Copyright (C) 2015-2018 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
#

#-----------------------------------------------------------------------------------------------------------------------
# sources
#-----------------------------------------------------------------------------------------------------------------------

CSOURCES_$(d) ?= $(wildcard $(d)littlefs/*.c) 

#-----------------------------------------------------------------------------------------------------------------------
# compilation flags
#-----------------------------------------------------------------------------------------------------------------------

CFLAGS_$(d) := $(CFLAGS_$(d)) -DLFS_NO_DEBUG
CFLAGS_$(d) := $(CFLAGS_$(d)) -DLFS_NO_WARN
CFLAGS_$(d) := $(CFLAGS_$(d)) -DLFS_NO_ERROR

ifndef CONFIG_ASSERT_ENABLE
	CFLAGS_$(d) := $(CFLAGS_$(d)) -DLFS_NO_ASSERT
endif

#-----------------------------------------------------------------------------------------------------------------------
# standard footer
#-----------------------------------------------------------------------------------------------------------------------

include $(DISTORTOS_PATH)footer.mk
