#
# file: cmake/80-STM32.cmake
#
# author: Copyright (C) 2018 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
#
# Automatically generated file - do not edit!
#

distortosSetConfiguration(BOOLEAN
		CONFIG_CHIP_STM32
		ON
		INTERNAL)

distortosSetConfiguration(BOOLEAN
		CONFIG_CHIP_STM32L4
		ON
		INTERNAL)

distortosSetConfiguration(BOOLEAN
		CONFIG_CHIP_STM32L43
		ON
		INTERNAL)

distortosSetConfiguration(BOOLEAN
		CONFIG_CHIP_STM32L432
		ON
		INTERNAL)

distortosSetConfiguration(BOOLEAN
		CONFIG_CHIP_STM32L432K
		ON
		INTERNAL)

distortosSetConfiguration(STRING
		CONFIG_CHIP
		"STM32L432KC"
		INTERNAL
		OUTPUT_TYPES BOOLEAN STRING)

distortosSetConfiguration(BOOLEAN
		CONFIG_CHIP_STM32L4_RCC_HSE_CLOCK_BYPASS
		ON
		INTERNAL)

include("${CMAKE_CURRENT_SOURCE_DIR}/source/chip/STM32/STM32L4/distortos-sources.cmake")
