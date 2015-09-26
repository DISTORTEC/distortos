#
# file: Rules.mk
#
# author: Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
#
# date: 2015-09-26
#

#-----------------------------------------------------------------------------------------------------------------------
# subdirectories
#-----------------------------------------------------------------------------------------------------------------------

SUBDIRECTORIES += allocators
SUBDIRECTORIES += architecture
SUBDIRECTORIES += chip/STMicroelectronics/STM32F4
SUBDIRECTORIES += clocks
SUBDIRECTORIES += scheduler
SUBDIRECTORIES += synchronization
SUBDIRECTORIES += syscalls
SUBDIRECTORIES += threads

#-----------------------------------------------------------------------------------------------------------------------
# standard footer
#-----------------------------------------------------------------------------------------------------------------------

include footer.mk
