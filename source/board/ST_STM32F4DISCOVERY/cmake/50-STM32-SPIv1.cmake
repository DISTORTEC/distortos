#
# file: cmake/50-STM32-SPIv1.cmake
#
# author: Copyright (C) 2018-2020 Kamil Szczygiel https://distortec.com https://freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
#
# Automatically generated file - do not edit!
#

distortosSetConfiguration(BOOLEAN
		distortos_Peripherals_SPI1
		OFF
		HELP "Enable SPI1 low-level driver."
		OUTPUT_NAME DISTORTOS_CHIP_SPI1_ENABLE)

if(distortos_Peripherals_SPI1)

	distortosSetConfiguration(BOOLEAN
			distortos_Peripherals_SPI1_00_Use_DMA
			ON
			HELP "Select whether SPI1 low-level driver uses DMA (true) or interrupts (false) for transfers."
			OUTPUT_NAME DISTORTOS_CHIP_SPI1_DMA_BASED)

	if(distortos_Peripherals_SPI1_00_Use_DMA)

		distortosSetConfiguration(STRING
				distortos_Peripherals_SPI1_01_RX_DMA
				"DMA2 channel 0 (request 3)"
				"DMA2 channel 2 (request 3)"
				HELP "Select RX DMA channel used by SPI1 low-level driver."
				NO_OUTPUT)

		string(REGEX MATCH
				"DMA([0-9]+) channel ([0-9]+) \\(request ([0-9]+)\\)"
				dummy
				"${distortos_Peripherals_SPI1_01_RX_DMA}")
		distortosSetFixedConfiguration(INTEGER
				DISTORTOS_CHIP_SPI1_RX_DMA
				"${CMAKE_MATCH_1}")
		distortosSetFixedConfiguration(INTEGER
				DISTORTOS_CHIP_SPI1_RX_DMA_CHANNEL
				"${CMAKE_MATCH_2}")
		distortosSetFixedConfiguration(INTEGER
				DISTORTOS_CHIP_SPI1_RX_DMA_REQUEST
				"${CMAKE_MATCH_3}")
		list(APPEND DISTORTOS_CHIP_DMA${CMAKE_MATCH_1}_DEPENDENTS "SPI1 RX")
		list(APPEND DISTORTOS_CHIP_DMA${CMAKE_MATCH_1}_CHANNEL${CMAKE_MATCH_2}_DEPENDENTS "SPI1 RX")

		distortosSetConfiguration(STRING
				distortos_Peripherals_SPI1_02_TX_DMA
				"DMA2 channel 3 (request 3)"
				"DMA2 channel 5 (request 3)"
				HELP "Select TX DMA channel used by SPI1 low-level driver."
				NO_OUTPUT)

		string(REGEX MATCH
				"DMA([0-9]+) channel ([0-9]+) \\(request ([0-9]+)\\)"
				dummy
				"${distortos_Peripherals_SPI1_02_TX_DMA}")
		distortosSetFixedConfiguration(INTEGER
				DISTORTOS_CHIP_SPI1_TX_DMA
				"${CMAKE_MATCH_1}")
		distortosSetFixedConfiguration(INTEGER
				DISTORTOS_CHIP_SPI1_TX_DMA_CHANNEL
				"${CMAKE_MATCH_2}")
		distortosSetFixedConfiguration(INTEGER
				DISTORTOS_CHIP_SPI1_TX_DMA_REQUEST
				"${CMAKE_MATCH_3}")
		list(APPEND DISTORTOS_CHIP_DMA${CMAKE_MATCH_1}_DEPENDENTS "SPI1 TX")
		list(APPEND DISTORTOS_CHIP_DMA${CMAKE_MATCH_1}_CHANNEL${CMAKE_MATCH_2}_DEPENDENTS "SPI1 TX")

	else(distortos_Peripherals_SPI1_00_Use_DMA)

		set(ARCHITECTURE_NVIC_SPI1_ENABLE ON)

	endif(distortos_Peripherals_SPI1_00_Use_DMA)

	list(APPEND DISTORTOS_CHIP_GPIOA_DEPENDENTS "SPI1 MISO")
	list(APPEND DISTORTOS_CHIP_GPIOA_DEPENDENTS "SPI1 MOSI")
	list(APPEND DISTORTOS_CHIP_GPIOA_DEPENDENTS "SPI1 SCK")

endif(distortos_Peripherals_SPI1)

distortosSetConfiguration(BOOLEAN
		distortos_Peripherals_SPI2
		OFF
		HELP "Enable SPI2 low-level driver."
		OUTPUT_NAME DISTORTOS_CHIP_SPI2_ENABLE)

if(distortos_Peripherals_SPI2)

	distortosSetConfiguration(BOOLEAN
			distortos_Peripherals_SPI2_00_Use_DMA
			ON
			HELP "Select whether SPI2 low-level driver uses DMA (true) or interrupts (false) for transfers."
			OUTPUT_NAME DISTORTOS_CHIP_SPI2_DMA_BASED)

	if(distortos_Peripherals_SPI2_00_Use_DMA)

		distortosSetConfiguration(STRING
				distortos_Peripherals_SPI2_01_RX_DMA
				"DMA1 channel 3 (request 0)"
				HELP "Select RX DMA channel used by SPI2 low-level driver."
				NO_OUTPUT)

		string(REGEX MATCH
				"DMA([0-9]+) channel ([0-9]+) \\(request ([0-9]+)\\)"
				dummy
				"${distortos_Peripherals_SPI2_01_RX_DMA}")
		distortosSetFixedConfiguration(INTEGER
				DISTORTOS_CHIP_SPI2_RX_DMA
				"${CMAKE_MATCH_1}")
		distortosSetFixedConfiguration(INTEGER
				DISTORTOS_CHIP_SPI2_RX_DMA_CHANNEL
				"${CMAKE_MATCH_2}")
		distortosSetFixedConfiguration(INTEGER
				DISTORTOS_CHIP_SPI2_RX_DMA_REQUEST
				"${CMAKE_MATCH_3}")
		list(APPEND DISTORTOS_CHIP_DMA${CMAKE_MATCH_1}_DEPENDENTS "SPI2 RX")
		list(APPEND DISTORTOS_CHIP_DMA${CMAKE_MATCH_1}_CHANNEL${CMAKE_MATCH_2}_DEPENDENTS "SPI2 RX")

		distortosSetConfiguration(STRING
				distortos_Peripherals_SPI2_02_TX_DMA
				"DMA1 channel 4 (request 0)"
				HELP "Select TX DMA channel used by SPI2 low-level driver."
				NO_OUTPUT)

		string(REGEX MATCH
				"DMA([0-9]+) channel ([0-9]+) \\(request ([0-9]+)\\)"
				dummy
				"${distortos_Peripherals_SPI2_02_TX_DMA}")
		distortosSetFixedConfiguration(INTEGER
				DISTORTOS_CHIP_SPI2_TX_DMA
				"${CMAKE_MATCH_1}")
		distortosSetFixedConfiguration(INTEGER
				DISTORTOS_CHIP_SPI2_TX_DMA_CHANNEL
				"${CMAKE_MATCH_2}")
		distortosSetFixedConfiguration(INTEGER
				DISTORTOS_CHIP_SPI2_TX_DMA_REQUEST
				"${CMAKE_MATCH_3}")
		list(APPEND DISTORTOS_CHIP_DMA${CMAKE_MATCH_1}_DEPENDENTS "SPI2 TX")
		list(APPEND DISTORTOS_CHIP_DMA${CMAKE_MATCH_1}_CHANNEL${CMAKE_MATCH_2}_DEPENDENTS "SPI2 TX")

	else(distortos_Peripherals_SPI2_00_Use_DMA)

		set(ARCHITECTURE_NVIC_SPI2_ENABLE ON)

	endif(distortos_Peripherals_SPI2_00_Use_DMA)

endif(distortos_Peripherals_SPI2)

distortosSetConfiguration(BOOLEAN
		distortos_Peripherals_SPI3
		OFF
		HELP "Enable SPI3 low-level driver."
		OUTPUT_NAME DISTORTOS_CHIP_SPI3_ENABLE)

if(distortos_Peripherals_SPI3)

	distortosSetConfiguration(BOOLEAN
			distortos_Peripherals_SPI3_00_Use_DMA
			ON
			HELP "Select whether SPI3 low-level driver uses DMA (true) or interrupts (false) for transfers."
			OUTPUT_NAME DISTORTOS_CHIP_SPI3_DMA_BASED)

	if(distortos_Peripherals_SPI3_00_Use_DMA)

		distortosSetConfiguration(STRING
				distortos_Peripherals_SPI3_01_RX_DMA
				"DMA1 channel 0 (request 0)"
				"DMA1 channel 2 (request 0)"
				HELP "Select RX DMA channel used by SPI3 low-level driver."
				NO_OUTPUT)

		string(REGEX MATCH
				"DMA([0-9]+) channel ([0-9]+) \\(request ([0-9]+)\\)"
				dummy
				"${distortos_Peripherals_SPI3_01_RX_DMA}")
		distortosSetFixedConfiguration(INTEGER
				DISTORTOS_CHIP_SPI3_RX_DMA
				"${CMAKE_MATCH_1}")
		distortosSetFixedConfiguration(INTEGER
				DISTORTOS_CHIP_SPI3_RX_DMA_CHANNEL
				"${CMAKE_MATCH_2}")
		distortosSetFixedConfiguration(INTEGER
				DISTORTOS_CHIP_SPI3_RX_DMA_REQUEST
				"${CMAKE_MATCH_3}")
		list(APPEND DISTORTOS_CHIP_DMA${CMAKE_MATCH_1}_DEPENDENTS "SPI3 RX")
		list(APPEND DISTORTOS_CHIP_DMA${CMAKE_MATCH_1}_CHANNEL${CMAKE_MATCH_2}_DEPENDENTS "SPI3 RX")

		distortosSetConfiguration(STRING
				distortos_Peripherals_SPI3_02_TX_DMA
				"DMA1 channel 5 (request 0)"
				"DMA1 channel 7 (request 0)"
				HELP "Select TX DMA channel used by SPI3 low-level driver."
				NO_OUTPUT)

		string(REGEX MATCH
				"DMA([0-9]+) channel ([0-9]+) \\(request ([0-9]+)\\)"
				dummy
				"${distortos_Peripherals_SPI3_02_TX_DMA}")
		distortosSetFixedConfiguration(INTEGER
				DISTORTOS_CHIP_SPI3_TX_DMA
				"${CMAKE_MATCH_1}")
		distortosSetFixedConfiguration(INTEGER
				DISTORTOS_CHIP_SPI3_TX_DMA_CHANNEL
				"${CMAKE_MATCH_2}")
		distortosSetFixedConfiguration(INTEGER
				DISTORTOS_CHIP_SPI3_TX_DMA_REQUEST
				"${CMAKE_MATCH_3}")
		list(APPEND DISTORTOS_CHIP_DMA${CMAKE_MATCH_1}_DEPENDENTS "SPI3 TX")
		list(APPEND DISTORTOS_CHIP_DMA${CMAKE_MATCH_1}_CHANNEL${CMAKE_MATCH_2}_DEPENDENTS "SPI3 TX")

	else(distortos_Peripherals_SPI3_00_Use_DMA)

		set(ARCHITECTURE_NVIC_SPI3_ENABLE ON)

	endif(distortos_Peripherals_SPI3_00_Use_DMA)

endif(distortos_Peripherals_SPI3)

include("${CMAKE_CURRENT_SOURCE_DIR}/source/chip/STM32/peripherals/SPIv1/distortos-sources.cmake")
