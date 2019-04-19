#
# file: cmake/60-STM32-SDMMCv1.cmake
#
# author: Copyright (C) 2019 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
#
# Automatically generated file - do not edit!
#

distortosSetConfiguration(BOOLEAN
		distortos_Peripherals_SDMMC1
		OFF
		HELP "Enable SDMMC1 low-level driver."
		OUTPUT_NAME DISTORTOS_CHIP_SDMMC1_ENABLE)

if(distortos_Peripherals_SDMMC1)

	distortosSetConfiguration(STRING
			distortos_Peripherals_SDMMC1_00_DMA
			"DMA2 channel 3 (request 4)"
			"DMA2 channel 6 (request 4)"
			HELP "Select DMA channel used by SDMMC1 low-level driver."
			NO_OUTPUT)

	string(REGEX MATCH
			"DMA([0-9]+) channel ([0-9]+) \\(request ([0-9]+)\\)"
			dummy
			"${distortos_Peripherals_SDMMC1_00_DMA}")
	distortosSetFixedConfiguration(INTEGER
			DISTORTOS_CHIP_SDMMC1_DMA
			"${CMAKE_MATCH_1}")
	distortosSetFixedConfiguration(INTEGER
			DISTORTOS_CHIP_SDMMC1_DMA_CHANNEL
			"${CMAKE_MATCH_2}")
	distortosSetFixedConfiguration(INTEGER
			DISTORTOS_CHIP_SDMMC1_DMA_REQUEST
			"${CMAKE_MATCH_3}")
	list(APPEND DISTORTOS_CHIP_DMA${CMAKE_MATCH_1}_DEPENDENTS "SDMMC1")
	list(APPEND DISTORTOS_CHIP_DMA${CMAKE_MATCH_1}_CHANNEL${CMAKE_MATCH_2}_DEPENDENTS "SDMMC1")

	set(ARCHITECTURE_NVIC_SDMMC1_ENABLE ON)

endif(distortos_Peripherals_SDMMC1)

distortosSetConfiguration(BOOLEAN
		distortos_Peripherals_SDMMC2
		OFF
		HELP "Enable SDMMC2 low-level driver."
		OUTPUT_NAME DISTORTOS_CHIP_SDMMC2_ENABLE)

if(distortos_Peripherals_SDMMC2)

	distortosSetConfiguration(STRING
			distortos_Peripherals_SDMMC2_00_DMA
			"DMA2 channel 0 (request 11)"
			"DMA2 channel 5 (request 11)"
			HELP "Select DMA channel used by SDMMC2 low-level driver."
			NO_OUTPUT)

	string(REGEX MATCH
			"DMA([0-9]+) channel ([0-9]+) \\(request ([0-9]+)\\)"
			dummy
			"${distortos_Peripherals_SDMMC2_00_DMA}")
	distortosSetFixedConfiguration(INTEGER
			DISTORTOS_CHIP_SDMMC2_DMA
			"${CMAKE_MATCH_1}")
	distortosSetFixedConfiguration(INTEGER
			DISTORTOS_CHIP_SDMMC2_DMA_CHANNEL
			"${CMAKE_MATCH_2}")
	distortosSetFixedConfiguration(INTEGER
			DISTORTOS_CHIP_SDMMC2_DMA_REQUEST
			"${CMAKE_MATCH_3}")
	list(APPEND DISTORTOS_CHIP_DMA${CMAKE_MATCH_1}_DEPENDENTS "SDMMC2")
	list(APPEND DISTORTOS_CHIP_DMA${CMAKE_MATCH_1}_CHANNEL${CMAKE_MATCH_2}_DEPENDENTS "SDMMC2")

	set(ARCHITECTURE_NVIC_SDMMC2_ENABLE ON)

endif(distortos_Peripherals_SDMMC2)

include("${CMAKE_CURRENT_SOURCE_DIR}/source/chip/STM32/peripherals/SDMMCv1/distortos-sources.cmake")
