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
		OUTPUT_NAME CONFIG_CHIP_STM32F4_STANDARD_CLOCK_CONFIGURATION_ENABLE)

if(distortos_Clocks_00_Standard_configuration_of_clocks)

	if(CONFIG_CHIP MATCHES "STM32F401")
		set(min 2)
		set(max 3)
	elseif(CONFIG_CHIP MATCHES "STM32F4[01][57]")
		set(min 1)
		set(max 2)
	else()
		set(min 1)
		set(max 3)
	endif()

	distortosSetConfiguration(INTEGER
			distortos_Clocks_01_Voltage_scale_mode
			${min}
			MIN ${min}
			MAX ${max}
			HELP "Select voltage scale mode."
			OUTPUT_NAME CONFIG_CHIP_STM32F4_PWR_VOLTAGE_SCALE_MODE)

	if(distortos_Clocks_01_Voltage_scale_mode EQUAL 1 AND
			CONFIG_CHIP_STM32F4_VDD_MV GREATER_EQUAL 2100 AND
			CONFIG_CHIP MATCHES "STM32F42[79]|STM32F43|STM32F446|STM32F4[67]9")

		distortosSetConfiguration(BOOLEAN
				distortos_Clocks_02_Over_drive_mode
				OFF
				HELP "Enable over-drive mode."
				OUTPUT_NAME CONFIG_CHIP_STM32F4_PWR_OVER_DRIVE_ENABLE)

	endif(distortos_Clocks_01_Voltage_scale_mode EQUAL 1 AND
			CONFIG_CHIP_STM32F4_VDD_MV GREATER_EQUAL 2100 AND
			CONFIG_CHIP MATCHES "STM32F42[79]|STM32F43|STM32F446|STM32F4[67]9")

	if(CONFIG_CHIP_STM32F4_RCC_HSE_FREQUENCY)

		if(CONFIG_CHIP_STM32F4_RCC_HSE_CLOCK_BYPASS)
			set(help "Enable HSE external user clock, ${CONFIG_CHIP_STM32F4_RCC_HSE_FREQUENCY} Hz.")
		else()
			set(help "Enable HSE crystal/ceramic resonator, ${CONFIG_CHIP_STM32F4_RCC_HSE_FREQUENCY} Hz.")
		endif()

		distortosSetConfiguration(BOOLEAN
				distortos_Clocks_03_HSE
				OFF
				HELP ${help}
				OUTPUT_NAME CONFIG_CHIP_STM32F4_RCC_HSE_ENABLE)

	endif(CONFIG_CHIP_STM32F4_RCC_HSE_FREQUENCY)

	distortosSetConfiguration(BOOLEAN
			distortos_Clocks_04_PLL
			ON
			HELP "Enable PLL."
			OUTPUT_NAME CONFIG_CHIP_STM32F4_RCC_PLL_ENABLE)

	if(distortos_Clocks_04_PLL)

		if(distortos_Clocks_03_HSE)
			set(pllClockSourceHse HSE)
		endif()

		distortosSetConfiguration(STRING
				distortos_Clocks_05_Clock_source_of_PLLs
				HSI
				${pllClockSourceHse}
				HELP "Select clock source of main and audio PLLs."
				OUTPUT_NAME CONFIG_CHIP_STM32F4_RCC_PLLSRC
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
				OUTPUT_NAME CONFIG_CHIP_STM32F4_RCC_PLLM)

		if(CONFIG_CHIP MATCHES "STM32F401")
			set(min 192)
		else()
			set(min 50)
		endif()

		distortosSetConfiguration(INTEGER
				distortos_Clocks_07_PLLN
				336
				MIN ${min}
				MAX 432
				HELP "PLLN value for main PLL.

				It is used to multiply VCO input frequency (VCOin). Resulting VCO output frequency (VCOout) must be in
				the range:
				- [192 MHz; 432 MHz] for STM32F401 chips,
				- [100 MHz; 432 MHz] for other chips.

				VCOout = VCOin * PLLN = PLLin / PLLM * PLLN"
				OUTPUT_NAME CONFIG_CHIP_STM32F4_RCC_PLLN)

		if(CONFIG_CHIP MATCHES "STM32F401|STM32F41[0-3]|STM32F423")
			set(default4 DEFAULT)
		else()
			unset(default4)
		endif()

		distortosSetConfiguration(STRING
				distortos_Clocks_08_PLLP
				2
				4 ${default4}
				6
				8
				HELP "PLLP value for main PLL.

				It is used to divide VCO output frequency (VCOout). PLL output frequency (PLLout) must not exceed:
				* for STM32F401 chips:
				- 84 MHz in voltage scale 2 mode,
				- 60 MHz in voltage scale 3 mode,
				* for STM32F405, STM32F407, STM32F415 and STM32F417 chips:
				- 168 MHz in voltage scale 1 mode,
				- 144 MHz in voltage scale 2 mode,
				* for STM32F410, STM32F411, STM32F412, STM32F413 and STM32F423 chips:
				- 100 MHz in voltage scale 1 mode,
				- 84 MHz in voltage scale 2 mode,
				- 64 MHz in voltage scale 3 mode,
				* for other chips:
				- 180 MHz in voltage scale 1 mode with over-drive mode,
				- 168 MHz in voltage scale 1 mode,
				- 144 MHz in voltage scale 2 mode,
				- 120 MHz in voltage scale 3 mode.

				PLLout = VCOout / PLLP = PLLin / PLLM * PLLN / PLLP"
				OUTPUT_NAME CONFIG_CHIP_STM32F4_RCC_PLLP
				OUTPUT_TYPES INTEGER)

		distortosSetConfiguration(INTEGER
				distortos_Clocks_09_PLLQ
				7
				MIN 2
				MAX 15
				HELP "PLLQ value for main PLL.

				It is used to divide VCO output frequency (VCOout) to produce clock for USB OTG FS, SDIO and RNG
				(PLLQout). USB OTG FS requires a 48 MHz clock for correct operation, while SDIO and RNG need a frequency
				lower than or equal to 48 MHz to work correctly.

				PLLQout = VCOout / PLLQ = PLLin / PLLM * PLLN / PLLQ"
				OUTPUT_NAME CONFIG_CHIP_STM32F4_RCC_PLLQ)

		if(CONFIG_CHIP MATCHES "STM32F41[23]|STM32F423|STM32F446|STM32F4[67]9")

			distortosSetConfiguration(INTEGER
					distortos_Clocks_10_PLLR
					7
					MIN 2
					MAX 7
					HELP "PLLR value for main PLL.

					It is used to divide VCO output frequency (VCOout) to produce clock for peripherals like DFSDM, DSI,
					I2S, SAI or SPDIF (PLLRout). This clock can also be used as source of system clock (except on
					STM32F412, STM32F413 or STM32F423).

					PLLRout = VCOout / PLLR = PLLin / PLLM * PLLN / PLLR"
					OUTPUT_NAME CONFIG_CHIP_STM32F4_RCC_PLLR)

		endif(CONFIG_CHIP MATCHES "STM32F41[23]|STM32F423|STM32F446|STM32F4[67]9")

	endif(distortos_Clocks_04_PLL)

	if(distortos_Clocks_03_HSE)
		set(systemClockSourceHse HSE)
	endif()
	if(distortos_Clocks_04_PLL)
		set(systemClockSourcePll PLL DEFAULT)
		if(CONFIG_CHIP MATCHES "STM32F446|STM32F4[67]9")
			set(systemClockSourcePllr PLLR)
		endif()
	endif()

	distortosSetConfiguration(STRING
			distortos_Clocks_11_System_clock_source
			HSI
			${systemClockSourceHse}
			${systemClockSourcePll}
			${systemClockSourcePllr}
			HELP "Select system clock source."
			OUTPUT_NAME CONFIG_CHIP_STM32F4_RCC_SYSCLK
			OUTPUT_TYPES BOOLEAN)

else(distortos_Clocks_00_Standard_configuration_of_clocks)

	if(CONFIG_CHIP MATCHES "STM32F401")
		set(max 84000000)
	elseif(CONFIG_CHIP MATCHES "STM32F4[01][57]")
		set(max 168000000)
	elseif(CONFIG_CHIP MATCHES "STM32F41[0-3]|STM32F423")
		set(max 100000000)
	else()
		set(max 180000000)
	endif()

	distortosSetConfiguration(INTEGER
			distortos_Clocks_01_SYSCLK_frequency
			16000000
			MIN 1
			MAX ${max}
			HELP "Frequency of SYSCLK, Hz.

			RCC must be configured by user to achieve that frequency. SYSCLK frequency must not exceed:
			* for STM32F401 chips:
			- 84 MHz in voltage scale 2 mode,
			- 60 MHz in voltage scale 3 mode,
			* for STM32F405, STM32F407, STM32F415 and STM32F417 chips:
			- 168 MHz in voltage scale 1 mode,
			- 144 MHz in voltage scale 2 mode,
			* for STM32F410, STM32F411, STM32F412, STM32F413 and STM32F423 chips:
			- 100 MHz in voltage scale 1 mode,
			- 84 MHz in voltage scale 2 mode,
			- 64 MHz in voltage scale 3 mode,
			* for other chips:
			- 180 MHz in voltage scale 1 mode with over-drive mode,
			- 168 MHz in voltage scale 1 mode,
			- 144 MHz in voltage scale 2 mode,
			- 120 MHz in voltage scale 3 mode."
			OUTPUT_NAME CONFIG_CHIP_STM32F4_RCC_SYSCLK_FREQUENCY)

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
		OUTPUT_NAME CONFIG_CHIP_STM32F4_RCC_HPRE
		OUTPUT_TYPES INTEGER)

if(CONFIG_CHIP MATCHES "STM32F401|STM32F41[0-3]|STM32F423")
	set(default2 DEFAULT)
	unset(default4)
else()
	unset(default2)
	set(default4 DEFAULT)
endif()

distortosSetConfiguration(STRING
		distortos_Clocks_13_PPRE1
		1
		2 ${default2}
		4 ${default4}
		8
		16
		HELP "APB1 (low speed) clock division factor.

		APB1 clock must not exceed:
		- 42 MHz for STM32F401, STM32F405, STM32F407, STM32F415 and STM32F417 chips,
		- 50 MHz for STM32F410, STM32F411, STM32F412, STM32F413 and STM32F423 chips,
		- 45 MHz (with over-drive mode) or 42 MHz (otherwise) for other chips.

		APB1clk = AHBclk / APB1divider"
		OUTPUT_NAME CONFIG_CHIP_STM32F4_RCC_PPRE1
		OUTPUT_TYPES INTEGER)

if(CONFIG_CHIP MATCHES "STM32F401|STM32F41[0-3]|STM32F423")
	unset(default2)
else()
	set(default2 DEFAULT)
endif()

distortosSetConfiguration(STRING
		distortos_Clocks_14_PPRE2
		1
		2 ${default2}
		4
		8
		16
		HELP "APB2 (high speed) clock division factor.

		APB2 clock must not exceed:
		- 84 MHz for STM32F401, STM32F405, STM32F407, STM32F415 and STM32F417 chips,
		- 100 MHz for STM32F410, STM32F411, STM32F412, STM32F413 and STM32F423 chips,
		- 90 MHz (with over-drive mode) or 84 MHz (otherwise) for other chips.

		APB2clk = AHBclk / APB2divider"
		OUTPUT_NAME CONFIG_CHIP_STM32F4_RCC_PPRE2
		OUTPUT_TYPES INTEGER)

if(CONFIG_CHIP_STM32F4_VDD_MV GREATER_EQUAL 2100)

	distortosSetConfiguration(BOOLEAN
			distortos_Memory_00_Flash_prefetch
			ON
			HELP "Enable flash prefetch option in FLASH->ACR register.

			Early revisions of older STM32F4 devices don't support this feature. Enabling this feature may also increase
			noise of ADC. Check device errata sheet of selected chip to see whether it is affected by any of these
			issues."
			OUTPUT_NAME CONFIG_CHIP_STM32F4_FLASH_PREFETCH_ENABLE)

endif(CONFIG_CHIP_STM32F4_VDD_MV GREATER_EQUAL 2100)

distortosSetConfiguration(BOOLEAN
		distortos_Memory_01_Flash_data_cache
		ON
		HELP "Enable flash data cache option in FLASH->ACR register."
		OUTPUT_NAME CONFIG_CHIP_STM32F4_FLASH_DATA_CACHE_ENABLE)

distortosSetConfiguration(BOOLEAN
		distortos_Memory_02_Flash_instruction_cache
		ON
		HELP "Enable flash instruction cache option in FLASH->ACR register."
		OUTPUT_NAME CONFIG_CHIP_STM32F4_FLASH_INSTRUCTION_CACHE_ENABLE)

target_include_directories(distortos PUBLIC
		${CMAKE_CURRENT_LIST_DIR}/../include
		${CMAKE_CURRENT_LIST_DIR}/include
		${CMAKE_CURRENT_LIST_DIR}/external/CMSIS-STM32F4)

target_sources(distortos PRIVATE
		${CMAKE_CURRENT_LIST_DIR}/STM32F4-chipLowLevelInitializer.cpp
		${CMAKE_CURRENT_LIST_DIR}/STM32F4-FLASH.cpp
		${CMAKE_CURRENT_LIST_DIR}/STM32F4-PWR.cpp
		${CMAKE_CURRENT_LIST_DIR}/STM32F4-RCC.cpp)

doxygen(INPUT ${CMAKE_CURRENT_LIST_DIR}
		INCLUDE_PATH ${CMAKE_CURRENT_LIST_DIR}/include ${CMAKE_CURRENT_LIST_DIR}/external/CMSIS-STM32F4
		EXCLUDE ${CMAKE_CURRENT_LIST_DIR}/external)
