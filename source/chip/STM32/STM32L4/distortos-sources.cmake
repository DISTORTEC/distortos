#
# file: distortos-sources.cmake
#
# author: Copyright (C) 2018-2019 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
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
		OUTPUT_NAME CONFIG_CHIP_STM32L4_STANDARD_CLOCK_CONFIGURATION_ENABLE)

if(distortos_Clocks_00_Standard_configuration_of_clocks)

	distortosSetConfiguration(INTEGER
			distortos_Clocks_01_Voltage_scaling_range
			1
			MIN 1
			MAX 2
			HELP "Select voltage scaling range."
			OUTPUT_NAME CONFIG_CHIP_STM32L4_PWR_VOLTAGE_SCALING_RANGE)

	if(CONFIG_CHIP_STM32L4_RCC_HSE_FREQUENCY)

		if(CONFIG_CHIP_STM32L4_RCC_HSE_CLOCK_BYPASS)
			set(help "Enable HSE external user clock, ${CONFIG_CHIP_STM32L4_RCC_HSE_FREQUENCY} Hz.")
		else()
			set(help "Enable HSE crystal/ceramic resonator, ${CONFIG_CHIP_STM32L4_RCC_HSE_FREQUENCY} Hz.")
		endif()

		distortosSetConfiguration(BOOLEAN
				distortos_Clocks_02_HSE
				OFF
				HELP ${help}
				OUTPUT_NAME CONFIG_CHIP_STM32L4_RCC_HSE_ENABLE)

	endif(CONFIG_CHIP_STM32L4_RCC_HSE_FREQUENCY)

	distortosSetConfiguration(BOOLEAN
			distortos_Clocks_03_HSI16
			ON
			HELP "Enable HSI16."
			OUTPUT_NAME CONFIG_CHIP_STM32L4_RCC_HSI16_ENABLE)

	distortosSetConfiguration(BOOLEAN
			distortos_Clocks_04_MSI
			OFF
			HELP "Enable MSI."
			OUTPUT_NAME CONFIG_CHIP_STM32L4_RCC_MSI_ENABLE)

	if(distortos_Clocks_04_MSI)

		distortosSetConfiguration(INTEGER
				distortos_Clocks_05_MSI_range
				8
				MIN 0
				MAX 11
				HELP "Select MSI clock range:
				- range 0 around 100 kHz,
				- range 1 around 200 kHz,
				- range 2 around 400 kHz,
				- range 3 around 800 kHz,
				- range 4 around 1 MHz,
				- range 5 around 2 MHz,
				- range 6 around 4 MHz,
				- range 7 around 8 MHz,
				- range 8 around 16 MHz,
				- range 9 around 24 MHz,
				- range 10 around 32 MHz,
				- range 11 around 48 MHz."
				OUTPUT_NAME CONFIG_CHIP_STM32L4_RCC_MSI_RANGE)

	endif(distortos_Clocks_04_MSI)

	if(distortos_Clocks_02_HSE OR distortos_Clocks_03_HSI16 OR distortos_Clocks_04_MSI)

		distortosSetConfiguration(BOOLEAN
				distortos_Clocks_06_PLL
				ON
				HELP "Enable PLL."
				OUTPUT_NAME CONFIG_CHIP_STM32L4_RCC_PLL_ENABLE)

		if(distortos_Clocks_06_PLL)

			if(distortos_Clocks_02_HSE)
				set(pllClockSourceHse HSE)
			endif()
			if(distortos_Clocks_03_HSI16)
				set(pllClockSourceHsi16 HSI16)
			endif()
			if(distortos_Clocks_04_MSI)
				set(pllClockSourceMsi MSI)
			endif()

			distortosSetConfiguration(STRING
					distortos_Clocks_07_Clock_source_of_PLLs
					${pllClockSourceHsi16}
					${pllClockSourceHse}
					${pllClockSourceMsi}
					HELP "Select clock source of PLLs."
					OUTPUT_NAME CONFIG_CHIP_STM32L4_RCC_PLLSRC
					OUTPUT_TYPES BOOLEAN)

			distortosSetConfiguration(INTEGER
					distortos_Clocks_08_PLLM
					4
					MIN 1
					MAX 8
					HELP "PLLM value for PLLs.

					It is used to divide PLL input clock (PLLin) before it is fed to VCO. VCO input frequency (VCOin)
					must be in the range [4 MHz; 16 MHz].

					VCOin = PLLin / PLLM"
					OUTPUT_NAME CONFIG_CHIP_STM32L4_RCC_PLLM)

			distortosSetConfiguration(INTEGER
					distortos_Clocks_09_PLLN
					40
					MIN 8
					MAX 86
					HELP "PLLN value for main PLL.

					It is used to multiply VCO input frequency (VCOin). Resulting VCO output frequency (VCOout) must be
					in the range [64 MHz; 344 MHz] in voltage scaling range 1 or [64 MHz; 128 MHz] in voltage scaling
					range 2.

					VCOout = VCOin * PLLN = PLLin / PLLM * PLLN"
					OUTPUT_NAME CONFIG_CHIP_STM32L4_RCC_PLLN)

			if(CONFIG_CHIP MATCHES "STM32L4[3-6]|STM32L4[9A]6")
				set(pllp2_6 2 3 4 5 6)
				set(pllp8_16 8 9 10 11 12 13 14 15 16)
				set(pllp18_31 18 19 20 21 22 23 24 25 26 27 28 29 30 31)
			else()
				unset(pllp2_6)
				unset(pllp8_16)
				unset(pllp18_31)
			endif()

			distortosSetConfiguration(STRING
					distortos_Clocks_10_PLLP
					${pllp2_6}
					7
					${pllp8_16}
					17
					${pllp18_31}
					HELP "PLLP value for main PLL.

					It is used to divide VCO output frequency (VCOout) to produce clock for SAI1 and SAI2 (PLLPout).
					PLL \"P\" output frequency (PLLPout) must be in the range [2.0645 MHz; 80 MHz] in voltage scaling
					range 1 or [2.0645 MHz; 26 MHz] in voltage scaling range 2.

					PLLPout = VCOout / PLLP = PLLin / PLLM * PLLN / PLLP"
					OUTPUT_NAME CONFIG_CHIP_STM32L4_RCC_PLLP
					OUTPUT_TYPES INTEGER)

			distortosSetConfiguration(STRING
					distortos_Clocks_11_PLLQ
					2
					4
					6
					8
					HELP "PLLQ value for main PLL.

					It is used to divide VCO output frequency (VCOout) to produce clock for RNG, SDMMC and USB
					(PLLQout). PLL \"Q\" output frequency (PLLQout) must be in the range [8 MHz; 80 MHz] in voltage
					scaling range 1 or [8 MHz; 26 MHz] in voltage scaling range 2.

					PLLQout = VCOout / PLLQ = PLLin / PLLM * PLLN / PLLQ"
					OUTPUT_NAME CONFIG_CHIP_STM32L4_RCC_PLLQ
					OUTPUT_TYPES INTEGER)

			distortosSetConfiguration(STRING
					distortos_Clocks_12_PLLR
					2
					4
					6
					8
					HELP "PLLR value for main PLL.

					It is used to divide VCO output frequency (VCOout) to produce system clock (PLLRout). PLL \"R\"
					output frequency (PLLRout) must be in the range [8 MHz; 80 MHz] in voltage scaling range 1 or
					[8 MHz; 26 MHz] in voltage scaling range 2.

					PLLRout = VCOout / PLLR = PLLin / PLLM * PLLN / PLLR"
					OUTPUT_NAME CONFIG_CHIP_STM32L4_RCC_PLLR
					OUTPUT_TYPES INTEGER)


			distortosSetConfiguration(BOOLEAN
					distortos_Clocks_13_PLLP_output
					OFF
					HELP "Enable PLL's \"P\" output."
					OUTPUT_NAME CONFIG_CHIP_STM32L4_RCC_PLLP_ENABLE)

			distortosSetConfiguration(BOOLEAN
					distortos_Clocks_14_PLLQ_output
					OFF
					HELP "Enable PLL's \"Q\" output."
					OUTPUT_NAME CONFIG_CHIP_STM32L4_RCC_PLLQ_ENABLE)

			distortosSetConfiguration(BOOLEAN
					distortos_Clocks_15_PLLR_output
					ON
					HELP "Enable PLL's \"R\" output."
					OUTPUT_NAME CONFIG_CHIP_STM32L4_RCC_PLLR_ENABLE)

		endif(distortos_Clocks_06_PLL)

	endif(distortos_Clocks_02_HSE OR distortos_Clocks_03_HSI16 OR distortos_Clocks_04_MSI)

	if(distortos_Clocks_02_HSE)
		set(systemClockSourceHse HSE)
	endif()
	if(distortos_Clocks_03_HSI16)
		set(systemClockSourceHsi16 HSI16)
	endif()
	if(distortos_Clocks_04_MSI)
		set(systemClockSourceMsi MSI)
	endif()
	if(distortos_Clocks_15_PLLR_output)
		set(systemClockSourcePll PLL DEFAULT)
	endif()

	distortosSetConfiguration(STRING
			distortos_Clocks_16_System_clock_source
			${systemClockSourceHse}
			${systemClockSourceHsi16}
			${systemClockSourceMsi}
			${systemClockSourcePll}
			HELP "Select system clock source."
			OUTPUT_NAME CONFIG_CHIP_STM32L4_RCC_SYSCLK
			OUTPUT_TYPES BOOLEAN)

else(distortos_Clocks_00_Standard_configuration_of_clocks)

	distortosSetConfiguration(INTEGER
			distortos_Clocks_01_SYSCLK_frequency
			4000000
			MIN 1
			MAX 80000000
			HELP "Frequency of SYSCLK, Hz.

			RCC must be configured by user to achieve that frequency. SYSCLK frequency must not exceed:
			- 80 MHz in voltage scale range 1,
			- 26 MHz in voltage scale range 2."
			OUTPUT_NAME CONFIG_CHIP_STM32L4_RCC_SYSCLK_FREQUENCY)

endif(distortos_Clocks_00_Standard_configuration_of_clocks)

distortosSetConfiguration(STRING
		distortos_Clocks_17_HPRE
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
		OUTPUT_NAME CONFIG_CHIP_STM32L4_RCC_HPRE
		OUTPUT_TYPES INTEGER)

distortosSetConfiguration(STRING
		distortos_Clocks_18_PPRE1
		1
		2
		4
		8
		16
		HELP "APB1 (low speed) clock division factor.

		APB1clk = AHBclk / APB1divider"
		OUTPUT_NAME CONFIG_CHIP_STM32L4_RCC_PPRE1
		OUTPUT_TYPES INTEGER)

distortosSetConfiguration(STRING
		distortos_Clocks_19_PPRE2
		1
		2
		4
		8
		16
		HELP "APB2 (high speed) clock division factor.

		APB2clk = AHBclk / APB2divider"
		OUTPUT_NAME CONFIG_CHIP_STM32L4_RCC_PPRE2
		OUTPUT_TYPES INTEGER)

distortosSetConfiguration(BOOLEAN
		distortos_Memory_00_Flash_prefetch
		ON
		HELP "Enable flash prefetch option in FLASH->ACR register."
		OUTPUT_NAME CONFIG_CHIP_STM32L4_FLASH_PREFETCH_ENABLE)

distortosSetConfiguration(BOOLEAN
		distortos_Memory_01_Flash_data_cache
		ON
		HELP "Enable flash data cache option in FLASH->ACR register."
		OUTPUT_NAME CONFIG_CHIP_STM32L4_FLASH_DATA_CACHE_ENABLE)

distortosSetConfiguration(BOOLEAN
		distortos_Memory_02_Flash_instruction_cache
		ON
		HELP "Enable flash instruction cache option in FLASH->ACR register."
		OUTPUT_NAME CONFIG_CHIP_STM32L4_FLASH_INSTRUCTION_CACHE_ENABLE)

target_include_directories(distortos PUBLIC
		${CMAKE_CURRENT_LIST_DIR}/../include
		${CMAKE_CURRENT_LIST_DIR}/include
		${CMAKE_CURRENT_LIST_DIR}/external/CMSIS-STM32L4)

target_sources(distortos PRIVATE
		${CMAKE_CURRENT_LIST_DIR}/STM32L4-chipLowLevelInitializer.cpp
		${CMAKE_CURRENT_LIST_DIR}/STM32L4-FLASH.cpp
		${CMAKE_CURRENT_LIST_DIR}/STM32L4-PWR.cpp
		${CMAKE_CURRENT_LIST_DIR}/STM32L4-RCC.cpp)

doxygen(INPUT ${CMAKE_CURRENT_LIST_DIR} ${CMAKE_CURRENT_LIST_DIR}/../include
		INCLUDE_PATH ${CMAKE_CURRENT_LIST_DIR}/include ${CMAKE_CURRENT_LIST_DIR}/external/CMSIS-STM32L4
		EXCLUDE ${CMAKE_CURRENT_LIST_DIR}/external)
