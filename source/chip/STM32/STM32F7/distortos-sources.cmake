#
# file: distortos-sources.cmake
#
# author: Copyright (C) 2018 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
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
		OUTPUT_NAME CONFIG_CHIP_STM32F7_STANDARD_CLOCK_CONFIGURATION_ENABLE)

if(distortos_Clocks_00_Standard_configuration_of_clocks)

	distortosSetConfiguration(INTEGER
			distortos_Clocks_01_Voltage_scale_mode
			1
			MIN 1
			MAX 3
			HELP "Select voltage scale mode."
			OUTPUT_NAME CONFIG_CHIP_STM32F7_PWR_VOLTAGE_SCALE_MODE)

	if(distortos_Clocks_01_Voltage_scale_mode LESS_EQUAL 2 AND CONFIG_CHIP_STM32F7_VDD_MV GREATER_EQUAL 2100)

		distortosSetConfiguration(BOOLEAN
				distortos_Clocks_02_Over_drive_mode
				OFF
				HELP "Enable over-drive mode."
				OUTPUT_NAME CONFIG_CHIP_STM32F7_PWR_OVER_DRIVE_ENABLE)

	endif(distortos_Clocks_01_Voltage_scale_mode LESS_EQUAL 2 AND CONFIG_CHIP_STM32F7_VDD_MV GREATER_EQUAL 2100)

	if(CONFIG_CHIP_STM32F7_RCC_HSE_FREQUENCY)

		if(CONFIG_CHIP_STM32F7_RCC_HSE_CLOCK_BYPASS)
			set(help "Enable HSE external user clock, ${CONFIG_CHIP_STM32F7_RCC_HSE_FREQUENCY} Hz.")
		else()
			set(help "Enable HSE crystal/ceramic resonator, ${CONFIG_CHIP_STM32F7_RCC_HSE_FREQUENCY} Hz.")
		endif()

		distortosSetConfiguration(BOOLEAN
				distortos_Clocks_03_HSE
				OFF
				HELP ${help}
				OUTPUT_NAME CONFIG_CHIP_STM32F7_RCC_HSE_ENABLE)

	endif(CONFIG_CHIP_STM32F7_RCC_HSE_FREQUENCY)

	distortosSetConfiguration(BOOLEAN
			distortos_Clocks_04_PLL
			ON
			HELP "Enable PLL."
			OUTPUT_NAME CONFIG_CHIP_STM32F7_RCC_PLL_ENABLE)

	if(distortos_Clocks_04_PLL)

		if(distortos_Clocks_03_HSE)
			set(pllClockSourceHse HSE)
		endif()

		distortosSetConfiguration(STRING
				distortos_Clocks_05_Clock_source_of_PLLs
				HSI
				${pllClockSourceHse}
				HELP "Select clock source of main and audio PLLs."
				OUTPUT_NAME CONFIG_CHIP_STM32F7_RCC_PLLSRC
				OUTPUT_TYPES BOOLEAN)

		distortosSetConfiguration(INTEGER
				distortos_Clocks_06_PLLM
				16
				MIN 2
				MAX 63
				HELP "PLLM value for main and audio PLLs.

				It is used to divide PLL input clock (PLLin) before it is fed to VCO. VCO input frequency (VCOin) must
				be in the range [1 MHz; 2 MHz], 2 MHz is recommended to limit PLL jitter.

				VCOin = PLLin / PLLM"
				OUTPUT_NAME CONFIG_CHIP_STM32F7_RCC_PLLM)

		distortosSetConfiguration(INTEGER
				distortos_Clocks_07_PLLN
				336
				MIN 50
				MAX 432
				HELP "PLLN value for main PLL.

				It is used to multiply VCO input frequency (VCOin). Resulting VCO output frequency (VCOout) must be in
				the range [100 MHz; 432 MHz].

				VCOout = VCOin * PLLN = PLLin / PLLM * PLLN"
				OUTPUT_NAME CONFIG_CHIP_STM32F7_RCC_PLLN)

		distortosSetConfiguration(STRING
				distortos_Clocks_08_PLLP
				2
				4
				6
				8
				HELP "PLLP value for main PLL.

				It is used to divide VCO output frequency (VCOout). PLL output frequency (PLLout) must not exceed:
				- 216 MHz (with over-drive mode) or 180 MHz (otherwise) in voltage scale 1 mode,
				- 180 MHz (with over-drive mode) or 168 MHz (otherwise) in voltage scale 2 mode,
				- 144 MHz in voltage scale 3 mode.

				PLLout = VCOout / PLLP = PLLin / PLLM * PLLN / PLLP"
				OUTPUT_NAME CONFIG_CHIP_STM32F7_RCC_PLLP
				OUTPUT_TYPES INTEGER)

		distortosSetConfiguration(INTEGER
				distortos_Clocks_09_PLLQ
				7
				MIN 2
				MAX 15
				HELP "PLLQ value for main PLL.

				It is used to divide VCO output frequency (VCOout) to produce clock for USB OTG FS, SDMMC and RNG
				(PLLQout). USB OTG FS requires a 48 MHz clock for correct operation, while SDMMC and RNG need a
				frequency lower than or equal to 48 MHz to work correctly.

				PLLQout = VCOout / PLLQ = PLLin / PLLM * PLLN / PLLQ"
				OUTPUT_NAME CONFIG_CHIP_STM32F7_RCC_PLLQ)

		if(CONFIG_CHIP MATCHES "STM32F7[67]")

			distortosSetConfiguration(INTEGER
					distortos_Clocks_10_PLLR
					7
					MIN 2
					MAX 7
					HELP "PLLR value for main PLL.

					It is used to divide VCO output frequency (VCOout) to produce clock for DSI (PLLRout).

					PLLRout = VCOout / PLLR = PLLin / PLLM * PLLN / PLLR"
					OUTPUT_NAME CONFIG_CHIP_STM32F7_RCC_PLLR)

		endif(CONFIG_CHIP MATCHES "STM32F7[67]")

	endif(distortos_Clocks_04_PLL)

	if(distortos_Clocks_03_HSE)
		set(systemClockSourceHse HSE)
	endif()
	if(distortos_Clocks_04_PLL)
		set(systemClockSourcePll PLL DEFAULT)
	endif()

	distortosSetConfiguration(STRING
			distortos_Clocks_11_System_clock_source
			HSI
			${systemClockSourceHse}
			${systemClockSourcePll}
			HELP "Select system clock source."
			OUTPUT_NAME CONFIG_CHIP_STM32F7_RCC_SYSCLK
			OUTPUT_TYPES BOOLEAN)

else(distortos_Clocks_00_Standard_configuration_of_clocks)

	distortosSetConfiguration(INTEGER
			distortos_Clocks_01_SYSCLK_frequency
			16000000
			MIN 1
			MAX 216000000
			HELP "Frequency of SYSCLK, Hz.

			RCC must be configured by user to achieve that frequency. SYSCLK frequency must not exceed:
			- 216 MHz (with over-drive mode) or 180 MHz (otherwise) in voltage scale 1 mode,
			- 180 MHz (with over-drive mode) or 168 MHz (otherwise) in voltage scale 2 mode,
			- 144 MHz in voltage scale 3 mode."
			OUTPUT_NAME CONFIG_CHIP_STM32F7_RCC_SYSCLK_FREQUENCY)

endif(distortos_Clocks_00_Standard_configuration_of_clocks)

distortosSetConfiguration(STRING
		distortos_Clocks_12_HPRE
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
		OUTPUT_NAME CONFIG_CHIP_STM32F7_RCC_HPRE
		OUTPUT_TYPES INTEGER)

distortosSetConfiguration(STRING
		distortos_Clocks_13_PPRE1
		1
		2
		4 DEFAULT
		8
		16
		HELP "APB1 (low speed) clock division factor.

		APB1 clock must not exceed 54 MHz (with over-drive mode) or 45 MHz (otherwise).

		APB1clk = AHBclk / APB1divider"
		OUTPUT_NAME CONFIG_CHIP_STM32F7_RCC_PPRE1
		OUTPUT_TYPES INTEGER)

distortosSetConfiguration(STRING
		distortos_Clocks_14_PPRE2
		1
		2 DEFAULT
		4
		8
		16
		HELP "APB2 (high speed) clock division factor.

		APB2 clock must not exceed 108 MHz (with over-drive mode) or 90 MHz (otherwise).

		APB2clk = AHBclk / APB2divider"
		OUTPUT_NAME CONFIG_CHIP_STM32F7_RCC_PPRE2
		OUTPUT_TYPES INTEGER)

distortosSetConfiguration(BOOLEAN
		distortos_Memory_00_Flash_prefetch
		ON
		HELP "Enable flash prefetch option in FLASH->ACR register."
		OUTPUT_NAME CONFIG_CHIP_STM32F7_FLASH_PREFETCH_ENABLE)

distortosSetConfiguration(BOOLEAN
		distortos_Memory_01_Flash_ART_accelerator
		ON
		HELP "Enable flash ART accelerator in FLASH->ACR register."
		OUTPUT_NAME CONFIG_CHIP_STM32F7_FLASH_ART_ACCELERATOR_ENABLE)

target_include_directories(distortos PUBLIC
		${CMAKE_CURRENT_LIST_DIR}/../include
		${CMAKE_CURRENT_LIST_DIR}/include
		${CMAKE_CURRENT_LIST_DIR}/external/CMSIS-STM32F7)

target_sources(distortos PRIVATE
		${CMAKE_CURRENT_LIST_DIR}/STM32F7-chipLowLevelInitializer.cpp
		${CMAKE_CURRENT_LIST_DIR}/STM32F7-chipVectors.cpp
		${CMAKE_CURRENT_LIST_DIR}/STM32F7-FLASH.cpp
		${CMAKE_CURRENT_LIST_DIR}/STM32F7-PWR.cpp
		${CMAKE_CURRENT_LIST_DIR}/STM32F7-RCC.cpp)

doxygen(INPUT ${CMAKE_CURRENT_LIST_DIR}
		INCLUDE_PATH ${CMAKE_CURRENT_LIST_DIR}/include ${CMAKE_CURRENT_LIST_DIR}/external/CMSIS-STM32F7
		EXCLUDE ${CMAKE_CURRENT_LIST_DIR}/external)
