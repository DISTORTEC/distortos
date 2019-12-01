#
# file: cmake/60-STM32-device-electronic-signature.cmake
#
# author: Copyright (C) 2019 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
#
# Automatically generated file - do not edit!
#

distortosSetFixedConfiguration(BOOLEAN
		DISTORTOS_CHIP_HAS_UNIQUEDEVICEID
		ON)

distortosSetFixedConfiguration(BOOLEAN
		DISTORTOS_CHIP_UNIQUEDEVICEID_FRAGMENTED
		ON)
