#
# file: distortos-sources.cmake
#
# author: Copyright (C) 2024 Kamil Szczygiel https://distortec.com https://freddiechopin.info
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

	distortosSetConfiguration(INTEGER
			distortos_Clocks_01_Voltage_scaling_range
			1
			MIN 1
			MAX 2
			HELP "Select voltage scaling range."
			OUTPUT_NAME DISTORTOS_CHIP_PWR_VOLTAGE_SCALING_RANGE)

	if(DISTORTOS_CHIP_RCC_HSE_FREQUENCY)

		if(DISTORTOS_CHIP_RCC_HSE_CLOCK_BYPASS)
			set(help "Enable HSE external user clock, ${DISTORTOS_CHIP_RCC_HSE_FREQUENCY} Hz.")
		else()
			set(help "Enable HSE crystal/ceramic resonator, ${DISTORTOS_CHIP_RCC_HSE_FREQUENCY} Hz.")
		endif()

		distortosSetConfiguration(BOOLEAN
				distortos_Clocks_02_HSE
				OFF
				HELP ${help}
				OUTPUT_NAME DISTORTOS_CHIP_RCC_HSE_ENABLE)

	endif(DISTORTOS_CHIP_RCC_HSE_FREQUENCY)

	distortosSetConfiguration(BOOLEAN
			distortos_Clocks_03_HSI16
			ON
			HELP "Enable HSI16."
			OUTPUT_NAME DISTORTOS_CHIP_RCC_HSI16_ENABLE)

	if(distortos_Clocks_03_HSI16)

		distortosSetConfiguration(STRING
				distortos_Clocks_04_HSIDIV
				1
				2
				4
				8
				16
				32
				64
				128
				HELP "HSIDIV value for HSI16.

				It is used to divide HSI16 frequency to produce HSISYS clock.

				HSISYS = HSI16 / HSIDIV"
				OUTPUT_NAME DISTORTOS_CHIP_RCC_HSIDIV
				OUTPUT_TYPES INTEGER)

	endif(distortos_Clocks_03_HSI16)

	if(DISTORTOS_CHIP_RCC_LSE_FREQUENCY)

		if(DISTORTOS_CHIP_RCC_LSE_CLOCK_BYPASS)
			set(help "Enable LSE external user clock, ${DISTORTOS_CHIP_RCC_LSE_FREQUENCY} Hz.")
		else()
			set(help "Enable LSE crystal/ceramic resonator, ${DISTORTOS_CHIP_RCC_LSE_FREQUENCY} Hz.")
		endif()

		distortosSetConfiguration(BOOLEAN
				distortos_Clocks_05_LSE
				OFF
				HELP ${help}
				OUTPUT_NAME DISTORTOS_CHIP_RCC_LSE_ENABLE)

		if(distortos_Clocks_05_LSE AND NOT DISTORTOS_CHIP_RCC_LSE_CLOCK_BYPASS)

			distortosSetConfiguration(STRING
					distortos_Clocks_06_LSE_drive_capability
					low
					mediumLow
					mediumHigh
					high
					HELP "Select LSE crystal/ceramic resonator driving strength."
					OUTPUT_NAME DISTORTOS_CHIP_RCC_LSE_DRIVE_CAPABILITY
					OUTPUT_TYPES BOOLEAN)

		endif(distortos_Clocks_05_LSE AND NOT DISTORTOS_CHIP_RCC_LSE_CLOCK_BYPASS)

	endif(DISTORTOS_CHIP_RCC_LSE_FREQUENCY)

	if(distortos_Clocks_02_HSE OR distortos_Clocks_03_HSI16)

		distortosSetConfiguration(BOOLEAN
				distortos_Clocks_07_PLL
				ON
				HELP "Enable PLL."
				OUTPUT_NAME DISTORTOS_CHIP_RCC_PLL_ENABLE)

		if(distortos_Clocks_07_PLL)

			if(distortos_Clocks_02_HSE)
				set(pllClockSourceHse HSE)
			endif()
			if(distortos_Clocks_03_HSI16)
				set(pllClockSourceHsi16 HSI16)
			endif()

			distortosSetConfiguration(STRING
					distortos_Clocks_08_Clock_source_of_PLL
					${pllClockSourceHsi16}
					${pllClockSourceHse}
					HELP "Select clock source of PLL."
					OUTPUT_NAME DISTORTOS_CHIP_RCC_PLLSRC
					OUTPUT_TYPES BOOLEAN)

			distortosSetConfiguration(INTEGER
					distortos_Clocks_09_PLLM
					2
					MIN 1
					MAX 8
					HELP "PLLM value for PLLs.

					It is used to divide PLL input clock (PLLin) before it is fed to VCO. VCO input frequency (VCOin)
					must be in the range [2.66 MHz; 16 MHz].

					VCOin = PLLin / PLLM"
					OUTPUT_NAME DISTORTOS_CHIP_RCC_PLLM)

			distortosSetConfiguration(INTEGER
					distortos_Clocks_10_PLLN
					16
					MIN 8
					MAX 86
					HELP "PLLN value for main PLL.

					It is used to multiply VCO input frequency (VCOin). Resulting VCO output frequency (VCOout) must be
					in the range [96 MHz; 344 MHz] in voltage scaling range 1 or [96 MHz; 128 MHz] in voltage scaling
					range 2.

					VCOout = VCOin * PLLN = PLLin / PLLM * PLLN"
					OUTPUT_NAME DISTORTOS_CHIP_RCC_PLLN)

			distortosSetConfiguration(INTEGER
					distortos_Clocks_11_PLLP
					2
					MIN 2
					MAX 32
					HELP "PLLP value for main PLL.

					It is used to divide VCO output frequency (VCOout) to produce clock for ADC and I2S (PLLPout).
					PLL \"P\" output frequency (PLLPout) must be in the range [3.09 MHz; 122 MHz] in voltage scaling
					range 1 or [3.09 MHz; 40 MHz] in voltage scaling range 2.

					PLLPout = VCOout / PLLP = PLLin / PLLM * PLLN / PLLP"
					OUTPUT_NAME DISTORTOS_CHIP_RCC_PLLP)

			if(DISTORTOS_CHIP MATCHES "STM32G0[357]0")
				set(hasPllq 0)
			else()
				set(hasPllq 1)
			endif()

			if(hasPllq)
				distortosSetConfiguration(INTEGER
						distortos_Clocks_12_PLLQ
						2
						MIN 2
						MAX 8
						HELP "PLLQ value for main PLL.

						It is used to divide VCO output frequency (VCOout) to produce clock for FDCAN, RNG, timers and
						USB (PLLQout). PLL \"Q\" output frequency (PLLQout) must be in the range [12 MHz; 128 MHz] in
						voltage scaling range 1 or [12 MHz; 33 MHz] in voltage scaling range 2.

						PLLQout = VCOout / PLLQ = PLLin / PLLM * PLLN / PLLQ"
						OUTPUT_NAME DISTORTOS_CHIP_RCC_PLLQ)
			endif(hasPllq)

			distortosSetConfiguration(INTEGER
					distortos_Clocks_13_PLLR
					2
					MIN 2
					MAX 8
					HELP "PLLR value for main PLL.

					It is used to divide VCO output frequency (VCOout) to produce system clock (PLLRout). PLL \"R\"
					output frequency (PLLRout) must be in the range [12 MHz; 64 MHz] in voltage scaling range 1 or
					[12 MHz; 16 MHz] in voltage scaling range 2.

					PLLRout = VCOout / PLLR = PLLin / PLLM * PLLN / PLLR"
					OUTPUT_NAME DISTORTOS_CHIP_RCC_PLLR)

			distortosSetConfiguration(BOOLEAN
					distortos_Clocks_14_PLLP_output
					OFF
					HELP "Enable PLL's \"P\" output."
					OUTPUT_NAME DISTORTOS_CHIP_RCC_PLLP_ENABLE)

			if(hasPllq)

				distortosSetConfiguration(BOOLEAN
						distortos_Clocks_15_PLLQ_output
						OFF
						HELP "Enable PLL's \"Q\" output."
						OUTPUT_NAME DISTORTOS_CHIP_RCC_PLLQ_ENABLE)

			endif(hasPllq)

			distortosSetConfiguration(BOOLEAN
					distortos_Clocks_16_PLLR_output
					ON
					HELP "Enable PLL's \"R\" output."
					OUTPUT_NAME DISTORTOS_CHIP_RCC_PLLR_ENABLE)

		endif(distortos_Clocks_07_PLL)

	endif(distortos_Clocks_02_HSE OR distortos_Clocks_03_HSI16)

	if(distortos_Clocks_02_HSE)
		set(systemClockSourceHse HSE)
	endif()
	if(distortos_Clocks_03_HSI16)
		set(systemClockSourceHsisys HSISYS)
	endif()
	if(distortos_Clocks_05_LSE)
		set(systemClockSourceLse LSE)
	endif()
	if(distortos_Clocks_16_PLLR_output)
		set(systemClockSourcePllr PLLR DEFAULT)
	endif()

	distortosSetConfiguration(STRING
			distortos_Clocks_17_System_clock_source
			${systemClockSourceHse}
			${systemClockSourceHsisys}
			${systemClockSourceLse}
			LSI
			${systemClockSourcePllr}
			HELP "Select system clock source."
			OUTPUT_NAME DISTORTOS_CHIP_RCC_SYSCLK
			OUTPUT_TYPES BOOLEAN)

else(distortos_Clocks_00_Standard_configuration_of_clocks)

	distortosSetConfiguration(INTEGER
			distortos_Clocks_01_SYSCLK_frequency
			16000000
			MIN 1
			MAX 64000000
			HELP "Frequency of SYSCLK, Hz.

			RCC must be configured by user to achieve that frequency. SYSCLK frequency must not exceed:
			- 64 MHz in voltage scale range 1,
			- 16 MHz in voltage scale range 2."
			OUTPUT_NAME DISTORTOS_CHIP_RCC_SYSCLK_FREQUENCY)

endif(distortos_Clocks_00_Standard_configuration_of_clocks)

distortosSetConfiguration(STRING
		distortos_Clocks_18_HPRE
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
		distortos_Clocks_19_PPRE
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

distortosSetConfiguration(BOOLEAN
		distortos_Memory_01_Flash_instruction_cache
		ON
		HELP "Enable flash instruction cache option in FLASH->ACR register."
		OUTPUT_NAME DISTORTOS_CHIP_FLASH_INSTRUCTION_CACHE_ENABLE)

target_include_directories(distortos PUBLIC
		${CMAKE_CURRENT_LIST_DIR}/../include
		${CMAKE_CURRENT_LIST_DIR}/include
		${CMAKE_CURRENT_LIST_DIR}/external/CMSIS-STM32G0)

target_sources(distortos PRIVATE
		${CMAKE_CURRENT_LIST_DIR}/STM32G0-chipLowLevelInitializer.cpp
		${CMAKE_CURRENT_LIST_DIR}/STM32G0-FLASH.cpp
		${CMAKE_CURRENT_LIST_DIR}/STM32G0-PWR.cpp
		${CMAKE_CURRENT_LIST_DIR}/STM32G0-RCC.cpp)

doxygen(INPUT ${CMAKE_CURRENT_LIST_DIR} ${CMAKE_CURRENT_LIST_DIR}/../include
		INCLUDE_PATH ${CMAKE_CURRENT_LIST_DIR}/include ${CMAKE_CURRENT_LIST_DIR}/external/CMSIS-STM32G0
		EXCLUDE ${CMAKE_CURRENT_LIST_DIR}/external)
