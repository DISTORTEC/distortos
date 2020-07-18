#
# file: cmake/50-STM32-SDMMCv1.cmake
#
# author: Copyright (C) 2019-2020 Kamil Szczygiel https://distortec.com https://freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
#
# Automatically generated file - do not edit!
#

distortosSetFixedConfiguration(BOOLEAN
		DISTORTOS_CHIP_SDMMC_SDIO_VARIANT
		ON)

distortosSetConfiguration(BOOLEAN
		distortos_Peripherals_SDIO
		OFF
		HELP "Enable SDIO low-level driver."
		OUTPUT_NAME DISTORTOS_CHIP_SDIO_ENABLE)

if(distortos_Peripherals_SDIO)

	distortosSetConfiguration(STRING
			distortos_Peripherals_SDIO_00_DMA
			"DMA2 channel 3 (request 4)"
			"DMA2 channel 6 (request 4)"
			HELP "Select DMA channel used by SDIO low-level driver."
			NO_OUTPUT)

	string(REGEX MATCH
			"DMA([0-9]+) channel ([0-9]+) \\(request ([0-9]+)\\)"
			dummy
			"${distortos_Peripherals_SDIO_00_DMA}")
	distortosSetFixedConfiguration(INTEGER
			DISTORTOS_CHIP_SDIO_DMA
			"${CMAKE_MATCH_1}")
	distortosSetFixedConfiguration(INTEGER
			DISTORTOS_CHIP_SDIO_DMA_CHANNEL
			"${CMAKE_MATCH_2}")
	distortosSetFixedConfiguration(INTEGER
			DISTORTOS_CHIP_SDIO_DMA_REQUEST
			"${CMAKE_MATCH_3}")
	list(APPEND DISTORTOS_CHIP_DMA${CMAKE_MATCH_1}_DEPENDENTS "SDIO")
	list(APPEND DISTORTOS_CHIP_DMA${CMAKE_MATCH_1}_CHANNEL${CMAKE_MATCH_2}_DEPENDENTS "SDIO")

	set(ARCHITECTURE_NVIC_SDIO_ENABLE ON)

endif(distortos_Peripherals_SDIO)

include("${CMAKE_CURRENT_SOURCE_DIR}/source/chip/STM32/peripherals/SDMMCv1/distortos-sources.cmake")
