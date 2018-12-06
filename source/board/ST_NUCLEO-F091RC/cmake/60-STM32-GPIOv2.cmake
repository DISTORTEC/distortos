#
# file: cmake/60-STM32-GPIOv2.cmake
#
# author: Copyright (C) 2018 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
#
# Automatically generated file - do not edit!
#

distortosSetConfiguration(BOOLEAN
		CONFIG_CHIP_STM32_GPIOV2_HAS_4_AF_BITS
		OFF
		INTERNAL)

distortosSetConfiguration(BOOLEAN
		CONFIG_CHIP_STM32_GPIOV2_HAS_HIGH_SPEED
		OFF
		INTERNAL)

if(FORCE_STM32_GPIO_V2_GPIOA_ENABLE)
	set(value ON)
	set(internal INTERNAL)
else()
	set(value OFF)
	unset(internal)
endif()

distortosSetConfiguration(BOOLEAN
		distortos_Peripherals_GPIOA
		${value}
		${internal}
		HELP "Enable GPIOA."
		OUTPUT_NAME CONFIG_CHIP_STM32_GPIO_V2_GPIOA_ENABLE)

if(FORCE_STM32_GPIO_V2_GPIOB_ENABLE)
	set(value ON)
	set(internal INTERNAL)
else()
	set(value OFF)
	unset(internal)
endif()

distortosSetConfiguration(BOOLEAN
		distortos_Peripherals_GPIOB
		${value}
		${internal}
		HELP "Enable GPIOB."
		OUTPUT_NAME CONFIG_CHIP_STM32_GPIO_V2_GPIOB_ENABLE)

if(FORCE_STM32_GPIO_V2_GPIOC_ENABLE)
	set(value ON)
	set(internal INTERNAL)
else()
	set(value OFF)
	unset(internal)
endif()

distortosSetConfiguration(BOOLEAN
		distortos_Peripherals_GPIOC
		${value}
		${internal}
		HELP "Enable GPIOC."
		OUTPUT_NAME CONFIG_CHIP_STM32_GPIO_V2_GPIOC_ENABLE)

if(FORCE_STM32_GPIO_V2_GPIOD_ENABLE)
	set(value ON)
	set(internal INTERNAL)
else()
	set(value OFF)
	unset(internal)
endif()

distortosSetConfiguration(BOOLEAN
		distortos_Peripherals_GPIOD
		${value}
		${internal}
		HELP "Enable GPIOD."
		OUTPUT_NAME CONFIG_CHIP_STM32_GPIO_V2_GPIOD_ENABLE)

if(FORCE_STM32_GPIO_V2_GPIOF_ENABLE)
	set(value ON)
	set(internal INTERNAL)
else()
	set(value OFF)
	unset(internal)
endif()

distortosSetConfiguration(BOOLEAN
		distortos_Peripherals_GPIOF
		${value}
		${internal}
		HELP "Enable GPIOF."
		OUTPUT_NAME CONFIG_CHIP_STM32_GPIO_V2_GPIOF_ENABLE)

include("${CMAKE_CURRENT_SOURCE_DIR}/source/chip/STM32/peripherals/GPIOv2/distortos-sources.cmake")
