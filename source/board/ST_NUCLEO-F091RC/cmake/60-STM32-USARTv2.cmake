#
# file: cmake/60-STM32-USARTv2.cmake
#
# author: Copyright (C) 2018 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
#
# Automatically generated file - do not edit!
#

distortosSetConfiguration(BOOLEAN
		CONFIG_CHIP_STM32_USARTV2_HAS_CR1_M1_BIT
		ON
		INTERNAL)

distortosSetConfiguration(BOOLEAN
		distortos_Peripherals_USART1
		OFF
		HELP "Enable USART1 low-level driver."
		OUTPUT_NAME CONFIG_CHIP_STM32_USARTV2_USART1_ENABLE)

if(distortos_Peripherals_USART1)
	set(ARCHITECTURE_ARMV6_M_ARMV7_M_NVIC_USART1_ENABLE ON)
endif()

distortosSetConfiguration(BOOLEAN
		distortos_Peripherals_USART2
		OFF
		HELP "Enable USART2 low-level driver."
		OUTPUT_NAME CONFIG_CHIP_STM32_USARTV2_USART2_ENABLE)

if(distortos_Peripherals_USART2)
	set(ARCHITECTURE_ARMV6_M_ARMV7_M_NVIC_USART2_ENABLE ON)
endif()

distortosSetConfiguration(BOOLEAN
		distortos_Peripherals_USART3
		OFF
		HELP "Enable USART3 low-level driver."
		OUTPUT_NAME CONFIG_CHIP_STM32_USARTV2_USART3_ENABLE)

if(distortos_Peripherals_USART3)
	set(ARCHITECTURE_ARMV6_M_ARMV7_M_NVIC_USART3_ENABLE ON)
endif()

distortosSetConfiguration(BOOLEAN
		distortos_Peripherals_USART4
		OFF
		HELP "Enable USART4 low-level driver."
		OUTPUT_NAME CONFIG_CHIP_STM32_USARTV2_USART4_ENABLE)

if(distortos_Peripherals_USART4)
	set(ARCHITECTURE_ARMV6_M_ARMV7_M_NVIC_USART4_ENABLE ON)
endif()

distortosSetConfiguration(BOOLEAN
		distortos_Peripherals_USART5
		OFF
		HELP "Enable USART5 low-level driver."
		OUTPUT_NAME CONFIG_CHIP_STM32_USARTV2_USART5_ENABLE)

if(distortos_Peripherals_USART5)
	set(ARCHITECTURE_ARMV6_M_ARMV7_M_NVIC_USART5_ENABLE ON)
endif()

distortosSetConfiguration(BOOLEAN
		distortos_Peripherals_USART6
		OFF
		HELP "Enable USART6 low-level driver."
		OUTPUT_NAME CONFIG_CHIP_STM32_USARTV2_USART6_ENABLE)

if(distortos_Peripherals_USART6)
	set(ARCHITECTURE_ARMV6_M_ARMV7_M_NVIC_USART6_ENABLE ON)
endif()

distortosSetConfiguration(BOOLEAN
		distortos_Peripherals_USART7
		OFF
		HELP "Enable USART7 low-level driver."
		OUTPUT_NAME CONFIG_CHIP_STM32_USARTV2_USART7_ENABLE)

if(distortos_Peripherals_USART7)
	set(ARCHITECTURE_ARMV6_M_ARMV7_M_NVIC_USART7_ENABLE ON)
endif()

distortosSetConfiguration(BOOLEAN
		distortos_Peripherals_USART8
		OFF
		HELP "Enable USART8 low-level driver."
		OUTPUT_NAME CONFIG_CHIP_STM32_USARTV2_USART8_ENABLE)

if(distortos_Peripherals_USART8)
	set(ARCHITECTURE_ARMV6_M_ARMV7_M_NVIC_USART8_ENABLE ON)
endif()

include("${CMAKE_CURRENT_SOURCE_DIR}/source/chip/STM32/peripherals/USARTv2/distortos-sources.cmake")
