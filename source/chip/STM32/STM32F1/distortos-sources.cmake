#
# file: distortos-sources.cmake
#
# author: Copyright (C) 2018-2019 Kamil Szczygiel http://www.distortec.com https://freddiechopin.info
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

		if(distortos_Clocks_01_HSE AND DISTORTOS_CHIP MATCHES "STM32F10[57]")

			distortosSetConfiguration(BOOLEAN
					distortos_Clocks_02_PREDIV2_PLL2_PLL3
					OFF
					HELP "Enable PREDIV2, PLL2 and PLL3 configuration."
					OUTPUT_NAME DISTORTOS_CHIP_RCC_PREDIV2_PLL2_PLL3_ENABLE)

			if(distortos_Clocks_02_PREDIV2_PLL2_PLL3)

				distortosSetConfiguration(INTEGER
						distortos_Clocks_03_PREDIV2
						2
						MIN 1
						MAX 16
						HELP "PREDIV2 value for PLL2 and PLL3.

						It is used to divide HSE clock before it is fed to PLL2 and PLL3. PLL2 and PLL3 input frequency
						(PLL23in) must be in the range [3 MHz; 5 MHz].

						PLL23in = HSE / PREDIV2"
						OUTPUT_NAME DISTORTOS_CHIP_RCC_PREDIV2)

				distortosSetConfiguration(BOOLEAN
						distortos_Clocks_04_PLL2
						OFF
						HELP "Enable PLL2."
						OUTPUT_NAME DISTORTOS_CHIP_RCC_PLL2_ENABLE)

				if(distortos_Clocks_04_PLL2)

					distortosSetConfiguration(STRING
							distortos_Clocks_05_PLL2MUL
							8
							9
							10 DEFAULT
							11
							12
							13
							14
							16
							20
							HELP "PLL2MUL value for PLL2.

							It is used to multiply PLL2 and PLL3 input frequency (PLL23in). Resulting PLL2 output
							frequency (PLL2out) must be in the range [40 MHz; 74 MHz].

							PLL2out = PLL23in * PLL2MUL"
							OUTPUT_NAME DISTORTOS_CHIP_RCC_PLL2MUL
							OUTPUT_TYPES INTEGER)

				endif(distortos_Clocks_04_PLL2)

				distortosSetConfiguration(BOOLEAN
						distortos_Clocks_06_PLL3
						OFF
						HELP "Enable PLL3."
						OUTPUT_NAME DISTORTOS_CHIP_RCC_PLL3_ENABLE)

				if(distortos_Clocks_06_PLL3)

					distortosSetConfiguration(STRING
							distortos_Clocks_07_PLL3MUL
							8
							9
							10 DEFAULT
							11
							12
							13
							14
							16
							20
							HELP "PLL3MUL value for PLL3.

							It is used to multiply PLL2 and PLL3 input frequency (PLL23in). Resulting PLL3 output
							frequency (PLL3out) must be in the range [40 MHz; 74 MHz] and PLL3 VCO output frequency
							(PLL3VCOout) must be in the range [80 MHz; 148 MHz].

							PLL3out = PLL23in * PLL3MUL
							PLL3VCOout = 2 * PLL3out"
							OUTPUT_NAME DISTORTOS_CHIP_RCC_PLL3MUL
							OUTPUT_TYPES INTEGER)

				endif(distortos_Clocks_06_PLL3)

			endif(distortos_Clocks_02_PREDIV2_PLL2_PLL3)

		endif(distortos_Clocks_01_HSE AND DISTORTOS_CHIP MATCHES "STM32F10[57]")

	endif(DISTORTOS_CHIP_RCC_HSE_FREQUENCY)

	distortosSetConfiguration(BOOLEAN
			distortos_Clocks_08_PLL
			ON
			HELP "Enable PLL."
			OUTPUT_NAME DISTORTOS_CHIP_RCC_PLL_ENABLE)

	if(distortos_Clocks_08_PLL)

		if(distortos_Clocks_01_HSE)
			set(pllClockSourcePrediv1 PREDIV1)
		endif()

		distortosSetConfiguration(STRING
				distortos_Clocks_09_Clock_source_of_PLL
				HSIDIV2
				${pllClockSourcePrediv1}
				HELP "Select clock source of main PLL.

				PLL input frequency (PLLin) must be in the range:
				- [1 MHz; 24 MHz] for STM32F100 chips,
				- [3 MHz; 12 MHz] for STM32F105 and STM32F107 chips,
				- [1 MHz; 25 MHz] for other chips."
				OUTPUT_NAME DISTORTOS_CHIP_RCC_PLLSRC
				OUTPUT_TYPES BOOLEAN)

		if(distortos_Clocks_09_Clock_source_of_PLL STREQUAL PREDIV1)

			if(DISTORTOS_CHIP MATCHES "STM32F10[57]")

				if(distortos_Clocks_04_PLL2)
					set(prediv1ClockSourcePll2 PLL2)
				endif()

				distortosSetConfiguration(STRING
						distortos_Clocks_10_Clock_source_of_PREDIV1
						HSE
						${prediv1ClockSourcePll2}
						HELP "Select clock source of PREDIV1."
						OUTPUT_NAME DISTORTOS_CHIP_RCC_PREDIV1SRC
						OUTPUT_TYPES BOOLEAN)

			endif(DISTORTOS_CHIP MATCHES "STM32F10[57]")

			if(DISTORTOS_CHIP MATCHES "STM32F10[057]")
				set(max 16)
			else()
				set(max 2)
			endif()

			distortosSetConfiguration(INTEGER
					distortos_Clocks_11_PREDIV1
					1
					MIN 1
					MAX ${max}
					HELP "PREDIV1 value for main PLL.

					It is used to divide PREDIV1 input frequency (PREDIV1in) before it is fed to main PLL clock source
					selector.

					PREDIV1out = PREDIV1in / PREDIV1"
					OUTPUT_NAME DISTORTOS_CHIP_RCC_PREDIV1)

		endif(distortos_Clocks_09_Clock_source_of_PLL STREQUAL PREDIV1)

		if(DISTORTOS_CHIP MATCHES "STM32F10[57]")
			unset(pllmul2)
			unset(pllmul3)
			set(pllmul6_5 6.5)
			unset(pllmul10)
			unset(pllmul11)
			unset(pllmul12)
			unset(pllmul13)
			unset(pllmul14)
			unset(pllmul15)
			unset(pllmul16)
		else()
			set(pllmul2 2)
			set(pllmul3 3)
			unset(pllmul6_5)
			set(pllmul10 10)
			set(pllmul11 11)
			set(pllmul12 12)
			set(pllmul13 13)
			set(pllmul14 14)
			set(pllmul15 15)
			set(pllmul16 16)
		endif()
		if(DISTORTOS_CHIP MATCHES "STM32F100")
			set(default6 DEFAULT)
			unset(default9)
			unset(default12)
			unset(default16)
		elseif(DISTORTOS_CHIP MATCHES "STM32F10[157]")
			unset(default6)
			set(default9 DEFAULT)
			unset(default12)
			unset(default16)
		elseif(DISTORTOS_CHIP MATCHES "STM32F102")
			unset(default6)
			unset(default9)
			set(default12 DEFAULT)
			unset(default16)
		else()
			unset(default6)
			unset(default9)
			unset(default12)
			set(default16 DEFAULT)
		endif()

		distortosSetConfiguration(STRING
				distortos_Clocks_12_PLLMUL
				${pllmul2}
				${pllmul3}
				4
				5
				6 ${default6}
				${pllmul6_5}
				7
				8
				9 ${default9}
				${pllmul10}
				${pllmul11}
				${pllmul12} ${default12}
				${pllmul13}
				${pllmul14}
				${pllmul15}
				${pllmul16} ${default16}
				HELP "PLLMUL value for main PLL.

				It is used to multiply main PLL input frequency (PLLin). Resulting PLL output frequency (PLLout) must be
				in the range:
				- [16 MHz; 24 MHz] for STM32F100 chips,
				- [16 MHz; 36 MHz] for STM32F101 chips,
				- [16 MHz; 48 MHz] for STM32F102 chips,
				- [18 MHz; 72 MHz] for STM32F105 and STM32F107 chips,
				- [16 MHz; 72 MHz] for other chips.

				PLLout = PLLin * PLLMUL"
				NO_OUTPUT)

		if(distortos_Clocks_12_PLLMUL STREQUAL "6.5")
			set(numerator 13)
			set(denominator 2)
			set(pllmul6_5 ON)
		else()
			set(numerator ${distortos_Clocks_12_PLLMUL})
			set(denominator 1)
			set(pllmul6_5 OFF)
		endif()

		distortosSetFixedConfiguration(INTEGER
				DISTORTOS_CHIP_RCC_PLLMUL_NUMERATOR
				${numerator})
		distortosSetFixedConfiguration(INTEGER
				DISTORTOS_CHIP_RCC_PLLMUL_DENOMINATOR
				${denominator})
		distortosSetFixedConfiguration(BOOLEAN
				DISTORTOS_CHIP_RCC_PLLMUL6_5
				${pllmul6_5})

	endif(distortos_Clocks_08_PLL)

	if(distortos_Clocks_01_HSE)
		set(systemClockSourceHse HSE)
	endif()
	if(distortos_Clocks_08_PLL)
		set(systemClockSourcePll PLL DEFAULT)
	endif()

	distortosSetConfiguration(STRING
			distortos_Clocks_13_System_clock_source
			HSI
			${systemClockSourceHse}
			${systemClockSourcePll}
			HELP "Select system clock source."
			OUTPUT_NAME DISTORTOS_CHIP_RCC_SYSCLK
			OUTPUT_TYPES BOOLEAN)
else(distortos_Clocks_00_Standard_configuration_of_clocks)

	if(DISTORTOS_CHIP MATCHES "STM32F100")
		set(max 24000000)
	elseif(DISTORTOS_CHIP MATCHES "STM32F101")
		set(max 36000000)
	elseif(DISTORTOS_CHIP MATCHES "STM32F102")
		set(max 48000000)
	else()
		set(max 72000000)
	endif()

	distortosSetConfiguration(INTEGER
			distortos_Clocks_01_SYSCLK_frequency
			8000000
			MIN 1
			MAX ${max}
			HELP "Frequency of SYSCLK, Hz.

			RCC must be configured by user to achieve that frequency."
			OUTPUT_NAME DISTORTOS_CHIP_RCC_SYSCLK_FREQUENCY)

endif(distortos_Clocks_00_Standard_configuration_of_clocks)

distortosSetConfiguration(STRING
		distortos_Clocks_14_HPRE
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

if(DISTORTOS_CHIP MATCHES "STM32F10[01]")
	unset(default2)
else()
	set(default2 DEFAULT)
endif()

distortosSetConfiguration(STRING
		distortos_Clocks_15_PPRE1
		1
		2 ${default2}
		4
		8
		16
		HELP "APB1 (low speed) clock division factor.

		APB1 clock must not exceed 36 MHz.

		APB1clk = AHBclk / APB1divider"
		OUTPUT_NAME DISTORTOS_CHIP_RCC_PPRE1
		OUTPUT_TYPES INTEGER)

distortosSetConfiguration(STRING
		distortos_Clocks_16_PPRE2
		1
		2
		4
		8
		16
		HELP "APB2 (high speed) clock division factor.

		APB2 clock must not exceed 72 MHz.

		APB2clk = AHBclk / APB2divider"
		OUTPUT_NAME DISTORTOS_CHIP_RCC_PPRE2
		OUTPUT_TYPES INTEGER)

if(NOT DISTORTOS_CHIP MATCHES "STM32F100")

	if(distortos_Clocks_14_HPRE STREQUAL 1)
		unset(dependents)
	else()
		set(dependents "HPRE != 1")
	endif()

	distortosSetConfiguration(BOOLEAN
			distortos_Memory_00_Flash_prefetch
			ON
			DEPENDENTS ${dependents}
			HELP "Enable flash prefetch option in FLASH->ACR register. Has to be enabled when HPRE != 1."
			OUTPUT_NAME DISTORTOS_CHIP_FLASH_PREFETCH_ENABLE)

endif(NOT DISTORTOS_CHIP MATCHES "STM32F100")

if(distortos_Clocks_14_HPRE STREQUAL 1)
	unset(offDependents)
else()
	set(offDependents "HPRE != 1")
endif()

distortosSetConfiguration(BOOLEAN
		distortos_Memory_01_Flash_half_cycle_access
		OFF
		OFF_DEPENDENTS ${offDependents}
		HELP "Enable flash half cycle access option in FLASH->ACR register. Has to be disabled when HPRE != 1."
		OUTPUT_NAME DISTORTOS_CHIP_FLASH_HALF_CYCLE_ACCESS_ENABLE)

target_include_directories(distortos PUBLIC
		${CMAKE_CURRENT_LIST_DIR}/../include
		${CMAKE_CURRENT_LIST_DIR}/include
		${CMAKE_CURRENT_LIST_DIR}/external/CMSIS-STM32F1)

target_sources(distortos PRIVATE
		${CMAKE_CURRENT_LIST_DIR}/STM32F1-chipLowLevelInitializer.cpp
		${CMAKE_CURRENT_LIST_DIR}/STM32F1-FLASH.cpp
		${CMAKE_CURRENT_LIST_DIR}/STM32F1-RCC.cpp)

doxygen(INPUT ${CMAKE_CURRENT_LIST_DIR} ${CMAKE_CURRENT_LIST_DIR}/../include
		INCLUDE_PATH ${CMAKE_CURRENT_LIST_DIR}/include ${CMAKE_CURRENT_LIST_DIR}/external/CMSIS-STM32F1
		EXCLUDE ${CMAKE_CURRENT_LIST_DIR}/external)
