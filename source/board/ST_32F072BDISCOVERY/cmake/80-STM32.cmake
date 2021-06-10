#
# file: cmake/80-STM32.cmake
#
# author: Copyright (C) 2018-2021 Kamil Szczygiel https://distortec.com https://freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
#
# Automatically generated file - do not edit!
#

distortosSetFixedConfiguration(BOOLEAN
		DISTORTOS_CHIP_STM32
		ON)

distortosSetFixedConfiguration(BOOLEAN
		DISTORTOS_CHIP_STM32F0
		ON)

distortosSetFixedConfiguration(BOOLEAN
		DISTORTOS_CHIP_STM32F07
		ON)

distortosSetFixedConfiguration(BOOLEAN
		DISTORTOS_CHIP_STM32F072
		ON)

distortosSetFixedConfiguration(BOOLEAN
		DISTORTOS_CHIP_STM32F072R
		ON)

distortosSetFixedConfiguration(BOOLEAN
		DISTORTOS_CHIP_STM32F072RB
		ON)

distortosSetFixedConfiguration(STRING
		DISTORTOS_CHIP
		"STM32F072RB")

distortosSetFixedConfiguration(INTEGER
		DISTORTOS_CHIP_VDD_MV
		3000)

distortosSetFixedConfiguration(BOOLEAN
		DISTORTOS_CHIP_RCC_HSE_CLOCK_BYPASS
		OFF)

distortosSetFixedConfiguration(INTEGER
		DISTORTOS_CHIP_RCC_HSE_FREQUENCY
		8000000)

include("${CMAKE_CURRENT_SOURCE_DIR}/source/chip/STM32/STM32F0/distortos-sources.cmake")
