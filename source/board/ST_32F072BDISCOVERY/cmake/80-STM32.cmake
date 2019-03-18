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
		CONFIG_CHIP_STM32
		ON)

distortosSetFixedConfiguration(BOOLEAN
		CONFIG_CHIP_STM32F0
		ON)

distortosSetFixedConfiguration(BOOLEAN
		CONFIG_CHIP_STM32F07
		ON)

distortosSetFixedConfiguration(BOOLEAN
		CONFIG_CHIP_STM32F072
		ON)

distortosSetFixedConfiguration(BOOLEAN
		CONFIG_CHIP_STM32F072R
		ON)

distortosSetFixedConfiguration(BOOLEAN
		CONFIG_CHIP_STM32F072RB
		ON)

distortosSetFixedConfiguration(STRING
		CONFIG_CHIP
		"STM32F072RB")

distortosSetFixedConfiguration(INTEGER
		CONFIG_CHIP_STM32F0_VDD_MV
		3000)

distortosSetFixedConfiguration(BOOLEAN
		CONFIG_CHIP_STM32F0_RCC_HSE_CLOCK_BYPASS
		OFF)

distortosSetFixedConfiguration(INTEGER
		CONFIG_CHIP_STM32F0_RCC_HSE_FREQUENCY
		8000000)

include("${CMAKE_CURRENT_SOURCE_DIR}/source/chip/STM32/STM32F0/distortos-sources.cmake")
