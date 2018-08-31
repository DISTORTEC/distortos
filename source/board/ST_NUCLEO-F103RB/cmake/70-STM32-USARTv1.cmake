#
# file: cmake/70-STM32-USARTv1.cmake
#
# author: Copyright (C) 2018 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
#
# Automatically generated file - do not edit!
#

distortosSetConfiguration(BOOLEAN
		CONFIG_CHIP_STM32_USARTV1_HAS_CR1_OVER8_BIT
		OFF
		INTERNAL)

distortosSetConfiguration(BOOLEAN
		distortos_Peripherals_USART1
		OFF
		HELP "Enable USART1 low-level driver."
		OUTPUT_NAME CONFIG_CHIP_STM32_USARTV1_USART1_ENABLE)

distortosSetConfiguration(BOOLEAN
		distortos_Peripherals_USART2
		OFF
		HELP "Enable USART2 low-level driver."
		OUTPUT_NAME CONFIG_CHIP_STM32_USARTV1_USART2_ENABLE)

distortosSetConfiguration(BOOLEAN
		distortos_Peripherals_USART3
		OFF
		HELP "Enable USART3 low-level driver."
		OUTPUT_NAME CONFIG_CHIP_STM32_USARTV1_USART3_ENABLE)

include("${CMAKE_CURRENT_SOURCE_DIR}/source/chip/STM32/peripherals/USARTv1/distortos-sources.cmake")
