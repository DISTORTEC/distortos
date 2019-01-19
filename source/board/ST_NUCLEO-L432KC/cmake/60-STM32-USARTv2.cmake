#
# file: cmake/60-STM32-USARTv2.cmake
#
# author: Copyright (C) 2018-2019 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
#
# Automatically generated file - do not edit!
#

distortosSetFixedConfiguration(BOOLEAN
		CONFIG_CHIP_STM32_USARTV2_HAS_CR1_M1_BIT
		ON)

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

include("${CMAKE_CURRENT_SOURCE_DIR}/source/chip/STM32/peripherals/USARTv2/distortos-sources.cmake")
