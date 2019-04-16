#
# file: cmake/80-STM32.cmake
#
# author: Copyright (C) 2018-2019 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
#
# Automatically generated file - do not edit!
#

distortosSetFixedConfiguration(BOOLEAN
		DISTORTOS_CHIP_STM32
		ON)

distortosSetFixedConfiguration(BOOLEAN
		DISTORTOS_CHIP_STM32F4
		ON)

distortosSetFixedConfiguration(BOOLEAN
		DISTORTOS_CHIP_STM32F42
		ON)

distortosSetFixedConfiguration(BOOLEAN
		DISTORTOS_CHIP_STM32F429
		ON)

distortosSetFixedConfiguration(BOOLEAN
		DISTORTOS_CHIP_STM32F429Z
		ON)

distortosSetFixedConfiguration(BOOLEAN
		DISTORTOS_CHIP_STM32F429ZI
		ON)

distortosSetFixedConfiguration(STRING
		DISTORTOS_CHIP
		"STM32F429ZI")

distortosSetFixedConfiguration(INTEGER
		DISTORTOS_CHIP_VDD_MV
		3300)

distortosSetFixedConfiguration(BOOLEAN
		DISTORTOS_CHIP_RCC_HSE_CLOCK_BYPASS
		ON)

distortosSetFixedConfiguration(INTEGER
		DISTORTOS_CHIP_RCC_HSE_FREQUENCY
		8000000)

include("${CMAKE_CURRENT_SOURCE_DIR}/source/chip/STM32/STM32F4/distortos-sources.cmake")
