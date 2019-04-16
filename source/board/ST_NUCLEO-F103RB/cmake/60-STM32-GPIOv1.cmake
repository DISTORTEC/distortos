#
# file: cmake/60-STM32-GPIOv1.cmake
#
# author: Copyright (C) 2018-2019 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
#
# Automatically generated file - do not edit!
#

distortosSetConfiguration(BOOLEAN
		distortos_Peripherals_GPIOA
		OFF
		DEPENDENTS ${STM32_GPIO_V1_GPIOA_DEPENDENTS}
		HELP "Enable GPIOA."
		OUTPUT_NAME DISTORTOS_CHIP_GPIOA_ENABLE)

distortosSetConfiguration(BOOLEAN
		distortos_Peripherals_GPIOB
		OFF
		DEPENDENTS ${STM32_GPIO_V1_GPIOB_DEPENDENTS}
		HELP "Enable GPIOB."
		OUTPUT_NAME DISTORTOS_CHIP_GPIOB_ENABLE)

distortosSetConfiguration(BOOLEAN
		distortos_Peripherals_GPIOC
		OFF
		DEPENDENTS ${STM32_GPIO_V1_GPIOC_DEPENDENTS}
		HELP "Enable GPIOC."
		OUTPUT_NAME DISTORTOS_CHIP_GPIOC_ENABLE)

distortosSetConfiguration(BOOLEAN
		distortos_Peripherals_GPIOD
		OFF
		DEPENDENTS ${STM32_GPIO_V1_GPIOD_DEPENDENTS}
		HELP "Enable GPIOD."
		OUTPUT_NAME DISTORTOS_CHIP_GPIOD_ENABLE)

include("${CMAKE_CURRENT_SOURCE_DIR}/source/chip/STM32/peripherals/GPIOv1/distortos-sources.cmake")
