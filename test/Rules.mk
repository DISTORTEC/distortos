#
# file: Rules.mk
#
# author: Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
#
# date: 2015-01-07
#

#-----------------------------------------------------------------------------------------------------------------------
# subdirectories
#-----------------------------------------------------------------------------------------------------------------------

SUBDIRECTORIES += ConditionVariable
SUBDIRECTORIES += FifoQueue
SUBDIRECTORIES += Mutex
SUBDIRECTORIES += RawFifoQueue
SUBDIRECTORIES += Semaphore
SUBDIRECTORIES += SoftwareTimer
SUBDIRECTORIES += Thread

#-----------------------------------------------------------------------------------------------------------------------
# compilation flags
#-----------------------------------------------------------------------------------------------------------------------

CXXFLAGS_$(d) := -I$(d)
CXXFLAGS_$(d) := $(CXXFLAGS_$(d)) -Iinclude
CXXFLAGS_$(d) := $(CXXFLAGS_$(d)) -Isource/architecture/ARM/ARMv7-M/include

#-----------------------------------------------------------------------------------------------------------------------
# standard footer
#-----------------------------------------------------------------------------------------------------------------------

include footer.mk
