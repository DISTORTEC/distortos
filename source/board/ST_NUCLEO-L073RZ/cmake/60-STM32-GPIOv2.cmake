#
# file: cmake/60-STM32-GPIOv2.cmake
#
# author: Copyright (C) 2018-2019 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
#
# Automatically generated file - do not edit!
#

distortosSetFixedConfiguration(BOOLEAN
		DISTORTOS_CHIP_STM32_GPIOV2_HAS_4_AF_BITS
		OFF)

distortosSetFixedConfiguration(BOOLEAN
		DISTORTOS_CHIP_STM32_GPIOV2_HAS_HIGH_SPEED
		ON)

distortosSetConfiguration(BOOLEAN
		distortos_Peripherals_GPIOA
		OFF
		DEPENDENTS ${STM32_GPIO_V2_GPIOA_DEPENDENTS}
		HELP "Enable GPIOA."
		OUTPUT_NAME DISTORTOS_CHIP_STM32_GPIO_V2_GPIOA_ENABLE)

distortosSetConfiguration(BOOLEAN
		distortos_Peripherals_GPIOB
		OFF
		DEPENDENTS ${STM32_GPIO_V2_GPIOB_DEPENDENTS}
		HELP "Enable GPIOB."
		OUTPUT_NAME DISTORTOS_CHIP_STM32_GPIO_V2_GPIOB_ENABLE)

distortosSetConfiguration(BOOLEAN
		distortos_Peripherals_GPIOC
		OFF
		DEPENDENTS ${STM32_GPIO_V2_GPIOC_DEPENDENTS}
		HELP "Enable GPIOC."
		OUTPUT_NAME DISTORTOS_CHIP_STM32_GPIO_V2_GPIOC_ENABLE)

distortosSetConfiguration(BOOLEAN
		distortos_Peripherals_GPIOD
		OFF
		DEPENDENTS ${STM32_GPIO_V2_GPIOD_DEPENDENTS}
		HELP "Enable GPIOD."
		OUTPUT_NAME DISTORTOS_CHIP_STM32_GPIO_V2_GPIOD_ENABLE)

distortosSetConfiguration(BOOLEAN
		distortos_Peripherals_GPIOH
		OFF
		DEPENDENTS ${STM32_GPIO_V2_GPIOH_DEPENDENTS}
		HELP "Enable GPIOH."
		OUTPUT_NAME DISTORTOS_CHIP_STM32_GPIO_V2_GPIOH_ENABLE)

include("${CMAKE_CURRENT_SOURCE_DIR}/source/chip/STM32/peripherals/GPIOv2/distortos-sources.cmake")
