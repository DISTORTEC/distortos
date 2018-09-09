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
		OUTPUT_NAME CONFIG_CHIP_STM32L0_STANDARD_CLOCK_CONFIGURATION_ENABLE)

if(distortos_Clocks_00_Standard_configuration_of_clocks)

	distortosSetConfiguration(INTEGER
			distortos_Clocks_01_Voltage_scale_mode
			1
			MIN 1
			MAX 3
			HELP "Select voltage scale mode."
			OUTPUT_NAME CONFIG_CHIP_STM32L0_PWR_VOLTAGE_SCALE_MODE)

	distortosSetConfiguration(BOOLEAN
			distortos_Clocks_02_MSI
			OFF
			HELP "Enable MSI."
			OUTPUT_NAME CONFIG_CHIP_STM32L0_RCC_MSI_ENABLE)

	if(distortos_Clocks_02_MSI)

		distortosSetConfiguration(INTEGER
				distortos_Clocks_03_MSI_range
				5
				MIN 0
				MAX 6
				HELP "Select MSI clock range:
				- range 0 around 65.536 kHz,
				- range 1 around 131.072 kHz,
				- range 2 around 262.144 kHz,
				- range 3 around 524.288 kHz,
				- range 4 around 1.048 MHz,
				- range 5 around 2.097 MHz,
				- range 6 around 4.194 MHz."
				OUTPUT_NAME CONFIG_CHIP_STM32L0_RCC_MSIRANGE)

	endif(distortos_Clocks_02_MSI)

	if(CONFIG_CHIP_STM32L0_RCC_HSE_FREQUENCY)

		if(CONFIG_CHIP_STM32L0_RCC_HSE_CLOCK_BYPASS)
			set(help "Enable HSE external user clock, ${CONFIG_CHIP_STM32L0_RCC_HSE_FREQUENCY} Hz.")
		else()
			set(help "Enable HSE crystal/ceramic resonator, ${CONFIG_CHIP_STM32L0_RCC_HSE_FREQUENCY} Hz.")
		endif()

		distortosSetConfiguration(BOOLEAN
				distortos_Clocks_04_HSE
				OFF
				HELP ${help}
				OUTPUT_NAME CONFIG_CHIP_STM32L0_RCC_HSE_ENABLE)

	endif(CONFIG_CHIP_STM32L0_RCC_HSE_FREQUENCY)

	distortosSetConfiguration(BOOLEAN
			distortos_Clocks_05_HSI16
			ON
			HELP "Enable HSI16."
			OUTPUT_NAME CONFIG_CHIP_STM32L0_RCC_HSI16_ENABLE)

	if(CONFIG_CHIP MATCHES "STM32L0[5-8][23]")

		distortosSetConfiguration(BOOLEAN
				distortos_Clocks_05_HSI48
				OFF
				HELP "Enable HSI48."
				OUTPUT_NAME CONFIG_CHIP_STM32L0_RCC_HSI48_ENABLE)

	endif(CONFIG_CHIP MATCHES "STM32L0[5-8][23]")

	if(distortos_Clocks_04_HSE OR distortos_Clocks_05_HSI16)

		distortosSetConfiguration(BOOLEAN
				distortos_Clocks_06_PLL
				ON
				HELP "Enable PLL."
				OUTPUT_NAME CONFIG_CHIP_STM32L0_RCC_PLL_ENABLE)

		if(distortos_Clocks_06_PLL)

			if(distortos_Clocks_04_HSE)
				set(pllClockSourceHse HSE)
			endif()
			if(distortos_Clocks_05_HSI16)
				set(pllClockSourceHsi16 HSI16)
			endif()

			distortosSetConfiguration(STRING
					distortos_Clocks_07_Clock_source_of_PLLs
					${pllClockSourceHsi16}
					${pllClockSourceHse}
					HELP "Select clock source of main PLL.

					PLL input frequency (PLLin) must be in [2 MHz; 24 MHz] range."
					OUTPUT_NAME CONFIG_CHIP_STM32L0_RCC_PLLSRC
					OUTPUT_TYPES BOOLEAN)

			distortosSetConfiguration(STRING
					distortos_Clocks_08_PLLMUL
					3
					4
					6 DEFAULT
					8
					12
					16
					24
					32
					48
					HELP "PLLMUL value for main PLL.

					It is used to multiply PLL input frequency (PLLin). Resulting VCO output frequency (VCOout) must not
					exceed:
					- 96 MHz in voltage scale 1 mode,
					- 48 MHz in voltage scale 2 mode,
					- 24 MHz in voltage scale 3 mode.

					VCOout = PLLin * PLLMUL"
					OUTPUT_NAME CONFIG_CHIP_STM32L0_RCC_PLLMUL
					OUTPUT_TYPES INTEGER)

			distortosSetConfiguration(INTEGER
					distortos_Clocks_09_PLLDIV
					3
					MIN 2
					MAX 4
					HELP "PLLDIV value for main PLL.

					It is used to divide VCO output frequency (VCOout). PLL output frequency (PLLout) must be in
					[2 MHz; 32 MHz] range.

					PLLout = VCOout / PLLDIV = PLLin * PLLMUL / PLLDIV"
					OUTPUT_NAME CONFIG_CHIP_STM32L0_RCC_PLLDIV)

		endif(distortos_Clocks_06_PLL)

	endif(distortos_Clocks_04_HSE OR distortos_Clocks_05_HSI16)

	if(distortos_Clocks_02_MSI)
		set(systemClockSourceMsi MSI)
	endif()
	if(distortos_Clocks_05_HSI16)
		set(systemClockSourceHsi16 HSI16)
	endif()
	if(distortos_Clocks_04_HSE)
		set(systemClockSourceHse HSE)
	endif()
	if(distortos_Clocks_06_PLL)
		set(systemClockSourcePll PLL DEFAULT)
	endif()

	distortosSetConfiguration(STRING
			distortos_Clocks_10_System_clock_source
			${systemClockSourceMsi}
			${systemClockSourceHsi16}
			${systemClockSourceHse}
			${systemClockSourcePll}
			HELP "Select system clock source."
			OUTPUT_NAME CONFIG_CHIP_STM32L0_RCC_SYSCLK
			OUTPUT_TYPES BOOLEAN)

else(distortos_Clocks_00_Standard_configuration_of_clocks)

	distortosSetConfiguration(INTEGER
			distortos_Clocks_01_SYSCLK_frequency
			8000000
			MIN 1
			MAX 32000000
			HELP "Frequency of SYSCLK, Hz.

			RCC must be configured by user to achieve that frequency."
			OUTPUT_NAME CONFIG_CHIP_STM32L0_RCC_SYSCLK_FREQUENCY)

endif(distortos_Clocks_00_Standard_configuration_of_clocks)

distortosSetConfiguration(STRING
		distortos_Clocks_11_HPRE
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
		OUTPUT_NAME CONFIG_CHIP_STM32L0_RCC_HPRE
		OUTPUT_TYPES INTEGER)

distortosSetConfiguration(STRING
		distortos_Clocks_12_PPRE1
		1
		2
		4
		8
		16
		HELP "APB1 (low speed) clock division factor.

		APB1clk = AHBclk / APB1divider"
		OUTPUT_NAME CONFIG_CHIP_STM32L0_RCC_PPRE1
		OUTPUT_TYPES INTEGER)

distortosSetConfiguration(STRING
		distortos_Clocks_13_PPRE2
		1
		2
		4
		8
		16
		HELP "APB2 (high speed) clock division factor.

		APB2clk = AHBclk / APB2divider"
		OUTPUT_NAME CONFIG_CHIP_STM32L0_RCC_PPRE2
		OUTPUT_TYPES INTEGER)

distortosSetConfiguration(BOOLEAN
		distortos_Memory_00_Flash_prefetch
		ON
		HELP "Enable flash prefetch option in FLASH->ACR register."
		OUTPUT_NAME CONFIG_CHIP_STM32L0_FLASH_PREFETCH_ENABLE)

target_include_directories(distortos PUBLIC
		${CMAKE_CURRENT_LIST_DIR}/../include
		${CMAKE_CURRENT_LIST_DIR}/include
		${CMAKE_CURRENT_LIST_DIR}/external/CMSIS-STM32L0)

target_sources(distortos PRIVATE
		${CMAKE_CURRENT_LIST_DIR}/STM32L0-chipLowLevelInitializer.cpp
		${CMAKE_CURRENT_LIST_DIR}/STM32L0-FLASH.cpp
		${CMAKE_CURRENT_LIST_DIR}/STM32L0-PWR.cpp
		${CMAKE_CURRENT_LIST_DIR}/STM32L0-RCC.cpp)

doxygen(INPUT ${CMAKE_CURRENT_LIST_DIR}
		INCLUDE_PATH ${CMAKE_CURRENT_LIST_DIR}/include ${CMAKE_CURRENT_LIST_DIR}/external/CMSIS-STM32L0
		EXCLUDE ${CMAKE_CURRENT_LIST_DIR}/external)
