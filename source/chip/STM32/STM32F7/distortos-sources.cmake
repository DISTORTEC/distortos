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
			distortos_Clocks_04_PLLs
			ON
			HELP "Enable PLLs."
			OUTPUT_NAME CONFIG_CHIP_STM32F7_RCC_PLLS_ENABLE)

	if(distortos_Clocks_04_PLLs)

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

				It is used to divide PLL input clock (PLLin) before it is fed to VCO of each PLL. VCO input frequency
				(VCOin) must be in the range [1 MHz; 2 MHz], 2 MHz is recommended to limit PLL jitter.

				VCOin = PLLin / PLLM"
				OUTPUT_NAME CONFIG_CHIP_STM32F7_RCC_PLLM)

		distortosSetConfiguration(BOOLEAN
				distortos_Clocks_07_PLL
				ON
				HELP "Enable main PLL."
				OUTPUT_NAME CONFIG_CHIP_STM32F7_RCC_PLL_ENABLE)

		if(distortos_Clocks_07_PLL)

			distortosSetConfiguration(INTEGER
					distortos_Clocks_08_PLLN
					336
					MIN 50
					MAX 432
					HELP "PLLN value for main PLL.

					It is used to multiply VCO input frequency (VCOin). Resulting VCO output frequency (VCOout) must be
					in the range [100 MHz; 432 MHz].

					VCOout = VCOin * PLLN = PLLin / PLLM * PLLN"
					OUTPUT_NAME CONFIG_CHIP_STM32F7_RCC_PLLN)

			distortosSetConfiguration(STRING
					distortos_Clocks_09_PLLP
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
					distortos_Clocks_10_PLLQ
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
						distortos_Clocks_11_PLLR
						7
						MIN 2
						MAX 7
						HELP "PLLR value for main PLL.

						It is used to divide VCO output frequency (VCOout) to produce clock for DSI (PLLRout).

						PLLRout = VCOout / PLLR = PLLin / PLLM * PLLN / PLLR"
						OUTPUT_NAME CONFIG_CHIP_STM32F7_RCC_PLLR)

			endif(CONFIG_CHIP MATCHES "STM32F7[67]")

		endif(distortos_Clocks_07_PLL)

		distortosSetConfiguration(BOOLEAN
				distortos_Clocks_12_PLLI2S
				OFF
				HELP "Enable PLLI2S."
				OUTPUT_NAME CONFIG_CHIP_STM32F7_RCC_PLLI2S_ENABLE)

		if(distortos_Clocks_12_PLLI2S)

			distortosSetConfiguration(INTEGER
					distortos_Clocks_13_PLLI2SN
					336
					MIN 50
					MAX 432
					HELP "PLLI2SN value for PLLI2S.

					It is used to multiply VCO input frequency (VCOin). Resulting PLLI2S VCO output frequency
					(PLLI2SVCOout) must be in the range [100 MHz; 432 MHz].

					PLLI2SVCOout = VCOin * PLLI2SN = PLLin / PLLM * PLLI2SN"
					OUTPUT_NAME CONFIG_CHIP_STM32F7_RCC_PLLI2SN)

			if(NOT CONFIG_CHIP MATCHES "STM32F7[23]")

				distortosSetConfiguration(STRING
						distortos_Clocks_14_PLLI2SP
						2
						4
						6
						8
						HELP "PLLI2SP value for PLLI2S.

						It is used to divide PLLI2S VCO output frequency (PLLI2SVCOout) to produce clock for SPDIFRX
						(PLLI2Sout). PLLI2S output frequency must not exceed 216 MHz.

						PLLI2Sout = PLLI2SVCOout / PLLI2SP = PLLin / PLLM * PLLI2SN / PLLI2SP"
						OUTPUT_NAME CONFIG_CHIP_STM32F7_RCC_PLLI2SP
						OUTPUT_TYPES INTEGER)

			endif(NOT CONFIG_CHIP MATCHES "STM32F7[23]")

			distortosSetConfiguration(INTEGER
					distortos_Clocks_15_PLLI2SQ
					2
					MIN 2
					MAX 15
					HELP "PLLI2SQ value for PLLI2S.

					It is used to divide PLLI2S VCO output frequency (PLLI2SVCOout) to produce clock for SAIs
					(PLLI2SQout). PLLI2S \"Q\" output frequency must not exceed 216 MHz.

					PLLI2SQout = PLLI2SVCOout / PLLI2SQ = PLLin / PLLM * PLLI2SN / PLLI2SQ"
					OUTPUT_NAME CONFIG_CHIP_STM32F7_RCC_PLLI2SQ)

			distortosSetConfiguration(INTEGER
					distortos_Clocks_16_PLLI2SR
					2
					MIN 2
					MAX 7
					HELP "PLLI2SR value for PLLI2S.

					It is used to divide PLLI2S VCO output frequency (PLLI2SVCOout) to produce clock for I2S
					(PLLI2SRout). PLLI2S \"R\" output frequency must not exceed 192 MHz.

					PLLI2SRout = PLLI2SVCOout / PLLI2SR = PLLin / PLLM * PLLI2SN / PLLI2SR"
					OUTPUT_NAME CONFIG_CHIP_STM32F7_RCC_PLLI2SR)

		endif(distortos_Clocks_12_PLLI2S)

		distortosSetConfiguration(BOOLEAN
				distortos_Clocks_17_PLLSAI
				OFF
				HELP "Enable PLLSAI."
				OUTPUT_NAME CONFIG_CHIP_STM32F7_RCC_PLLSAI_ENABLE)

		if(distortos_Clocks_17_PLLSAI)

			distortosSetConfiguration(INTEGER
					distortos_Clocks_18_PLLSAIN
					288
					MIN 50
					MAX 432
					HELP "PLLSAIN value for PLLSAI.

					It is used to multiply VCO input frequency (VCOin). Resulting PLLSAI VCO output frequency
					(PLLSAIVCOout) must be in the range [100 MHz; 432 MHz].

					PLLSAIVCOout = VCOin * PLLSAIN = PLLin / PLLM * PLLSAIN"
					OUTPUT_NAME CONFIG_CHIP_STM32F7_RCC_PLLSAIN)

			distortosSetConfiguration(STRING
					distortos_Clocks_19_PLLSAIP
					2
					4
					6 DEFAULT
					8
					HELP "PLLSAIP value for PLLSAI.

					It is used to divide PLLSAI VCO output frequency (PLLSAIVCOout) to produce clock for USB, RNG
					and SDMMC (PLLSAIout). PLLSAI output frequency must not exceed 48 MHz.

					PLLSAIout = PLLSAIVCOout / PLLSAIP = PLLin / PLLM * PLLSAIN / PLLSAIP"
					OUTPUT_NAME CONFIG_CHIP_STM32F7_RCC_PLLSAIP
					OUTPUT_TYPES INTEGER)

			distortosSetConfiguration(INTEGER
					distortos_Clocks_20_PLLSAIQ
					2
					MIN 2
					MAX 15
					HELP "PLLSAIQ value for PLLSAI.

					It is used to divide PLLSAI VCO output frequency (PLLSAIVCOout) to produce clock for SAI
					(PLLSAIQout). PLLSAI \"Q\" output frequency must not exceed 216 MHz.

					PLLSAIQout = PLLSAIVCOout / PLLSAIQ = PLLin / PLLM * PLLSAIN / PLLSAIQ"
					OUTPUT_NAME CONFIG_CHIP_STM32F7_RCC_PLLSAIQ)

			if(NOT CONFIG_CHIP MATCHES "STM32F7[23]")

				distortosSetConfiguration(INTEGER
						distortos_Clocks_21_PLLSAIR
						2
						MIN 2
						MAX 7
						HELP "PLLSAIR value for PLLSAI.

						It is used to divide PLLSAI VCO output frequency (PLLSAIVCOout) to produce clock for LCD
						(PLLSAIRout). PLLSAI \"R\" output frequency must not exceed 216 MHz.

						PLLSAIRout = PLLSAIVCOout / PLLSAIR = PLLin / PLLM * PLLSAIN / PLLSAIR"
						OUTPUT_NAME CONFIG_CHIP_STM32F7_RCC_PLLSAIR)

			endif(NOT CONFIG_CHIP MATCHES "STM32F7[23]")

		endif(distortos_Clocks_17_PLLSAI)

		if(distortos_Clocks_07_PLL OR distortos_Clocks_17_PLLSAI)

			if(distortos_Clocks_07_PLL)
				set(pll48ClockSourcePllq PLLQ)
			endif()
			if(distortos_Clocks_17_PLLSAI)
				set(pll48ClockSourcePllsaip PLLSAIP)
			endif()

			distortosSetConfiguration(STRING
					distortos_Clocks_22_PLL48_clock_source
					${pll48ClockSourcePllq}
					${pll48ClockSourcePllsaip}
					HELP "Select PLL48 clock source."
					OUTPUT_NAME CONFIG_CHIP_STM32F7_RCC_PLL48CLK
					OUTPUT_TYPES BOOLEAN)

		endif(distortos_Clocks_07_PLL OR distortos_Clocks_17_PLLSAI)

	endif(distortos_Clocks_04_PLLs)

	if(distortos_Clocks_03_HSE)
		set(systemClockSourceHse HSE)
	endif()
	if(distortos_Clocks_07_PLL)
		set(systemClockSourcePll PLL DEFAULT)
	endif()

	distortosSetConfiguration(STRING
			distortos_Clocks_23_System_clock_source
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

	distortosSetConfiguration(INTEGER
			distortos_Clocks_02_PLL48CLK_frequency
			0
			MIN 0
			MAX 48000000
			HELP "Frequency of PLL48CLK, Hz.

			RCC must be configured by user to achieve that frequency. PLL48CLK frequency must not exceed 48 MHz. Set to
			0 if PLL48CLK is not available."
			OUTPUT_NAME CONFIG_CHIP_STM32F7_RCC_PLL48CLK_FREQUENCY)

endif(distortos_Clocks_00_Standard_configuration_of_clocks)

distortosSetConfiguration(STRING
		distortos_Clocks_24_HPRE
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
		distortos_Clocks_25_PPRE1
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
		distortos_Clocks_26_PPRE2
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
		${CMAKE_CURRENT_LIST_DIR}/STM32F7-FLASH.cpp
		${CMAKE_CURRENT_LIST_DIR}/STM32F7-PWR.cpp
		${CMAKE_CURRENT_LIST_DIR}/STM32F7-RCC.cpp)

doxygen(INPUT ${CMAKE_CURRENT_LIST_DIR} ${CMAKE_CURRENT_LIST_DIR}/../include
		INCLUDE_PATH ${CMAKE_CURRENT_LIST_DIR}/include ${CMAKE_CURRENT_LIST_DIR}/external/CMSIS-STM32F7
		EXCLUDE ${CMAKE_CURRENT_LIST_DIR}/external)
