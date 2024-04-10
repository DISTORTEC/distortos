#
# file: distortos-sources.cmake
#
# author: Copyright (C) 2024 Kamil Szczygiel https://distortec.com https://freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
#

distortosSetConfiguration(INTEGER
		distortos_Clocks_00_SYSCLK_frequency
		16000000
		MIN 1
		MAX 64000000
		HELP "Frequency of SYSCLK, Hz.

		RCC must be configured by user to achieve that frequency."
		OUTPUT_NAME DISTORTOS_CHIP_RCC_SYSCLK_FREQUENCY)

distortosSetConfiguration(STRING
		distortos_Clocks_01_HPRE
		1
		2
		4
		8
		16
		64
		128
		256
		512
		HELP "AHB clock division factor

		AHBclk = SYSclk / AHBdivider"
		OUTPUT_NAME DISTORTOS_CHIP_RCC_HPRE
		OUTPUT_TYPES INTEGER)

distortosSetConfiguration(STRING
		distortos_Clocks_02_PPRE
		1
		2
		4
		8
		16
		HELP "APB clock division factor.

		APBclk = AHBclk / APBdivider"
		OUTPUT_NAME DISTORTOS_CHIP_RCC_PPRE
		OUTPUT_TYPES INTEGER)

target_include_directories(distortos PUBLIC
		${CMAKE_CURRENT_LIST_DIR}/../include
		${CMAKE_CURRENT_LIST_DIR}/include
		${CMAKE_CURRENT_LIST_DIR}/external/CMSIS-STM32G0)

target_sources(distortos PRIVATE
		${CMAKE_CURRENT_LIST_DIR}/STM32G0-chipLowLevelInitializer.cpp)

doxygen(INPUT ${CMAKE_CURRENT_LIST_DIR} ${CMAKE_CURRENT_LIST_DIR}/../include
		INCLUDE_PATH ${CMAKE_CURRENT_LIST_DIR}/include ${CMAKE_CURRENT_LIST_DIR}/external/CMSIS-STM32G0
		EXCLUDE ${CMAKE_CURRENT_LIST_DIR}/external)
