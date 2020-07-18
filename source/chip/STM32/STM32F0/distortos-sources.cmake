#
# file: distortos-sources.cmake
#
# author: Copyright (C) 2018-2019 Kamil Szczygiel https://distortec.com https://freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
#

distortosSetConfiguration(BOOLEAN
		distortos_Clocks_00_Standard_configuration_of_clocks
		ON
		HELP "Enable standard configuration of clocks.

		This will set values selected below and additionally configure appropriate FLASH latency before switching system
		clock to selected source.

		If disabled, no clock configuration will be done during chip initialization. The values entered below
		(frequencies, dividers, ...) will only be used to determine chip clocks. The user must configure the chip
		manually to match these settings."
		OUTPUT_NAME DISTORTOS_CHIP_STANDARD_CLOCK_CONFIGURATION_ENABLE)

if(distortos_Clocks_00_Standard_configuration_of_clocks)

	if(DISTORTOS_CHIP_RCC_HSE_FREQUENCY)

		if(DISTORTOS_CHIP_RCC_HSE_CLOCK_BYPASS)
			set(help "Enable HSE external user clock, ${DISTORTOS_CHIP_RCC_HSE_FREQUENCY} Hz.")
		else()
			set(help "Enable HSE crystal/ceramic resonator, ${DISTORTOS_CHIP_RCC_HSE_FREQUENCY} Hz.")
		endif()

		distortosSetConfiguration(BOOLEAN
				distortos_Clocks_01_HSE
				OFF
				HELP ${help}
				OUTPUT_NAME DISTORTOS_CHIP_RCC_HSE_ENABLE)

	endif(DISTORTOS_CHIP_RCC_HSE_FREQUENCY)

	if(DISTORTOS_CHIP MATCHES "STM32F0[49]|STM32F07[128]")

		distortosSetConfiguration(BOOLEAN
				distortos_Clocks_02_HSI48
				OFF
				HELP "Enable HSI48."
				OUTPUT_NAME DISTORTOS_CHIP_RCC_HSI48_ENABLE)

	endif(DISTORTOS_CHIP MATCHES "STM32F0[49]|STM32F07[128]")

	distortosSetConfiguration(BOOLEAN
			distortos_Clocks_03_PLL
			ON
			HELP "Enable PLL."
			OUTPUT_NAME DISTORTOS_CHIP_RCC_PLL_ENABLE)

	if(distortos_Clocks_03_PLL)

		if(DISTORTOS_CHIP MATCHES "STM32F0[49]|STM32F07[128]")
			set(pllClockSourceHsiPrediv HSIPREDIV)
		endif()
		if(distortos_Clocks_01_HSE)
			set(pllClockSourceHsePrediv HSEPREDIV)
		endif()
		if(distortos_Clocks_02_HSI48)
			set(pllClockSourceHsi48Prediv HSI48PREDIV)
		endif()

		distortosSetConfiguration(STRING
				distortos_Clocks_04_Clock_source_of_PLL
				HSIDIV2
				${pllClockSourceHsiPrediv}
				${pllClockSourceHsePrediv}
				${pllClockSourceHsi48Prediv}
				HELP "Select clock source of main PLL.

				PLL input frequency (PLLin) must be in [1 MHz; 24 MHz] range."
				OUTPUT_NAME DISTORTOS_CHIP_RCC_PLLSRC
				OUTPUT_TYPES BOOLEAN)

		if(distortos_Clocks_04_Clock_source_of_PLL MATCHES "PREDIV")

			distortosSetConfiguration(INTEGER
					distortos_Clocks_05_PREDIV
					2
					MIN 1
					MAX 16
					HELP "PREDIV value for main PLL.

					It is used to divide PREDIV input frequency (PREDIVin) before it is fed to main PLL.

					PLLin = PREDIVin / PREDIV"
					OUTPUT_NAME DISTORTOS_CHIP_RCC_PREDIV)

		endif(distortos_Clocks_04_Clock_source_of_PLL MATCHES "PREDIV")

		distortosSetConfiguration(INTEGER
				distortos_Clocks_06_PLLMUL
				12
				MIN 2
				MAX 16
				HELP "PLLMUL value for main PLL.

				It is used to multiply main PLL input frequency (PLLin). Resulting PLL output frequency (PLLout) must be
				in [16 MHz; 48 MHz] range.

				PLLout = PLLin * PLLMUL"
				OUTPUT_NAME DISTORTOS_CHIP_RCC_PLLMUL)

	endif(distortos_Clocks_03_PLL)

	if(distortos_Clocks_01_HSE)
		set(systemClockSourceHse HSE)
	endif()
	if(distortos_Clocks_03_PLL)
		set(systemClockSourcePll PLL DEFAULT)
	endif()
	if(distortos_Clocks_02_HSI48)
		set(systemClockSourceHsi48 HSI48)
	endif()

	distortosSetConfiguration(STRING
			distortos_Clocks_07_System_clock_source
			HSI
			${systemClockSourceHse}
			${systemClockSourcePll}
			${systemClockSourceHsi48}
			HELP "Select system clock source."
			OUTPUT_NAME DISTORTOS_CHIP_RCC_SYSCLK
			OUTPUT_TYPES BOOLEAN)

else(distortos_Clocks_00_Standard_configuration_of_clocks)

	distortosSetConfiguration(INTEGER
			distortos_Clocks_01_SYSCLK_frequency
			8000000
			MIN 1
			MAX 48000000
			HELP "Frequency of SYSCLK, Hz.

			RCC must be configured by user to achieve that frequency."
			OUTPUT_NAME DISTORTOS_CHIP_RCC_SYSCLK_FREQUENCY)

endif(distortos_Clocks_00_Standard_configuration_of_clocks)

distortosSetConfiguration(STRING
		distortos_Clocks_08_HPRE
		1
		2
		4
		8
		16
		64
		128
		256
		512
		HELP "AHB clock division factor.

		AHBclk = SYSclk / AHBdivider"
		OUTPUT_NAME DISTORTOS_CHIP_RCC_HPRE
		OUTPUT_TYPES INTEGER)

distortosSetConfiguration(STRING
		distortos_Clocks_09_PPRE
		1
		2
		4
		8
		16
		HELP "APB clock division factor.

		APBclk = AHBclk / APBdivider"
		OUTPUT_NAME DISTORTOS_CHIP_RCC_PPRE
		OUTPUT_TYPES INTEGER)

distortosSetConfiguration(BOOLEAN
		distortos_Memory_00_Flash_prefetch
		ON
		HELP "Enable flash prefetch option in FLASH->ACR register."
		OUTPUT_NAME DISTORTOS_CHIP_FLASH_PREFETCH_ENABLE)

target_include_directories(distortos PUBLIC
		${CMAKE_CURRENT_LIST_DIR}/../include
		${CMAKE_CURRENT_LIST_DIR}/include
		${CMAKE_CURRENT_LIST_DIR}/external/CMSIS-STM32F0)

target_sources(distortos PRIVATE
		${CMAKE_CURRENT_LIST_DIR}/STM32F0-chipLowLevelInitializer.cpp
		${CMAKE_CURRENT_LIST_DIR}/STM32F0-FLASH.cpp
		${CMAKE_CURRENT_LIST_DIR}/STM32F0-RCC.cpp)

doxygen(INPUT ${CMAKE_CURRENT_LIST_DIR} ${CMAKE_CURRENT_LIST_DIR}/../include
		INCLUDE_PATH ${CMAKE_CURRENT_LIST_DIR}/include ${CMAKE_CURRENT_LIST_DIR}/external/CMSIS-STM32F0
		EXCLUDE ${CMAKE_CURRENT_LIST_DIR}/external)
