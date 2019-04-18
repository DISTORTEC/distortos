#
# file: cmake/70-STM32-DMAv1.cmake
#
# author: Copyright (C) 2018-2019 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
#
# Automatically generated file - do not edit!
#

distortosSetConfiguration(BOOLEAN
		distortos_Peripherals_DMA1
		OFF
		DEPENDENTS ${STM32_DMA_V1_DMA1_DEPENDENTS}
		HELP "Enable DMA1 low-level driver."
		OUTPUT_NAME DISTORTOS_CHIP_DMA1_ENABLE)

if(distortos_Peripherals_DMA1)

	distortosSetConfiguration(BOOLEAN
			distortos_Peripherals_DMA1_channel_1
			OFF
			DEPENDENTS ${STM32_DMA_V1_DMA1_CHANNEL1_DEPENDENTS}
			HELP "Enable DMA1 channel 1 low-level driver."
			OUTPUT_NAME DISTORTOS_CHIP_DMA1_CHANNEL1_ENABLE)

	list(LENGTH STM32_DMA_V1_DMA1_CHANNEL1_DEPENDENTS length)
	if(length GREATER 1)
		string(REPLACE ";" ", " dependents "${STM32_DMA_V1_DMA1_CHANNEL1_DEPENDENTS}")
		message(STATUS "Warning: DMA1 channel 1 has ${length} dependents: ${dependents}. "
				"DMA channel cannot be used simultaneously by multiple drivers.")
	endif()

	if(distortos_Peripherals_DMA1_channel_1)
		set(ARCHITECTURE_NVIC_DMA1_CH1_ENABLE ON)
	endif(distortos_Peripherals_DMA1_channel_1)

	distortosSetConfiguration(BOOLEAN
			distortos_Peripherals_DMA1_channel_2
			OFF
			DEPENDENTS ${STM32_DMA_V1_DMA1_CHANNEL2_DEPENDENTS}
			HELP "Enable DMA1 channel 2 low-level driver."
			OUTPUT_NAME DISTORTOS_CHIP_DMA1_CHANNEL2_ENABLE)

	list(LENGTH STM32_DMA_V1_DMA1_CHANNEL2_DEPENDENTS length)
	if(length GREATER 1)
		string(REPLACE ";" ", " dependents "${STM32_DMA_V1_DMA1_CHANNEL2_DEPENDENTS}")
		message(STATUS "Warning: DMA1 channel 2 has ${length} dependents: ${dependents}. "
				"DMA channel cannot be used simultaneously by multiple drivers.")
	endif()

	if(distortos_Peripherals_DMA1_channel_2)
		set(ARCHITECTURE_NVIC_DMA1_CH2_ENABLE ON)
	endif(distortos_Peripherals_DMA1_channel_2)

	distortosSetConfiguration(BOOLEAN
			distortos_Peripherals_DMA1_channel_3
			OFF
			DEPENDENTS ${STM32_DMA_V1_DMA1_CHANNEL3_DEPENDENTS}
			HELP "Enable DMA1 channel 3 low-level driver."
			OUTPUT_NAME DISTORTOS_CHIP_DMA1_CHANNEL3_ENABLE)

	list(LENGTH STM32_DMA_V1_DMA1_CHANNEL3_DEPENDENTS length)
	if(length GREATER 1)
		string(REPLACE ";" ", " dependents "${STM32_DMA_V1_DMA1_CHANNEL3_DEPENDENTS}")
		message(STATUS "Warning: DMA1 channel 3 has ${length} dependents: ${dependents}. "
				"DMA channel cannot be used simultaneously by multiple drivers.")
	endif()

	if(distortos_Peripherals_DMA1_channel_3)
		set(ARCHITECTURE_NVIC_DMA1_CH3_ENABLE ON)
	endif(distortos_Peripherals_DMA1_channel_3)

	distortosSetConfiguration(BOOLEAN
			distortos_Peripherals_DMA1_channel_4
			OFF
			DEPENDENTS ${STM32_DMA_V1_DMA1_CHANNEL4_DEPENDENTS}
			HELP "Enable DMA1 channel 4 low-level driver."
			OUTPUT_NAME DISTORTOS_CHIP_DMA1_CHANNEL4_ENABLE)

	list(LENGTH STM32_DMA_V1_DMA1_CHANNEL4_DEPENDENTS length)
	if(length GREATER 1)
		string(REPLACE ";" ", " dependents "${STM32_DMA_V1_DMA1_CHANNEL4_DEPENDENTS}")
		message(STATUS "Warning: DMA1 channel 4 has ${length} dependents: ${dependents}. "
				"DMA channel cannot be used simultaneously by multiple drivers.")
	endif()

	if(distortos_Peripherals_DMA1_channel_4)
		set(ARCHITECTURE_NVIC_DMA1_CH4_ENABLE ON)
	endif(distortos_Peripherals_DMA1_channel_4)

	distortosSetConfiguration(BOOLEAN
			distortos_Peripherals_DMA1_channel_5
			OFF
			DEPENDENTS ${STM32_DMA_V1_DMA1_CHANNEL5_DEPENDENTS}
			HELP "Enable DMA1 channel 5 low-level driver."
			OUTPUT_NAME DISTORTOS_CHIP_DMA1_CHANNEL5_ENABLE)

	list(LENGTH STM32_DMA_V1_DMA1_CHANNEL5_DEPENDENTS length)
	if(length GREATER 1)
		string(REPLACE ";" ", " dependents "${STM32_DMA_V1_DMA1_CHANNEL5_DEPENDENTS}")
		message(STATUS "Warning: DMA1 channel 5 has ${length} dependents: ${dependents}. "
				"DMA channel cannot be used simultaneously by multiple drivers.")
	endif()

	if(distortos_Peripherals_DMA1_channel_5)
		set(ARCHITECTURE_NVIC_DMA1_CH5_ENABLE ON)
	endif(distortos_Peripherals_DMA1_channel_5)

	distortosSetConfiguration(BOOLEAN
			distortos_Peripherals_DMA1_channel_6
			OFF
			DEPENDENTS ${STM32_DMA_V1_DMA1_CHANNEL6_DEPENDENTS}
			HELP "Enable DMA1 channel 6 low-level driver."
			OUTPUT_NAME DISTORTOS_CHIP_DMA1_CHANNEL6_ENABLE)

	list(LENGTH STM32_DMA_V1_DMA1_CHANNEL6_DEPENDENTS length)
	if(length GREATER 1)
		string(REPLACE ";" ", " dependents "${STM32_DMA_V1_DMA1_CHANNEL6_DEPENDENTS}")
		message(STATUS "Warning: DMA1 channel 6 has ${length} dependents: ${dependents}. "
				"DMA channel cannot be used simultaneously by multiple drivers.")
	endif()

	if(distortos_Peripherals_DMA1_channel_6)
		set(ARCHITECTURE_NVIC_DMA1_CH6_ENABLE ON)
	endif(distortos_Peripherals_DMA1_channel_6)

	distortosSetConfiguration(BOOLEAN
			distortos_Peripherals_DMA1_channel_7
			OFF
			DEPENDENTS ${STM32_DMA_V1_DMA1_CHANNEL7_DEPENDENTS}
			HELP "Enable DMA1 channel 7 low-level driver."
			OUTPUT_NAME DISTORTOS_CHIP_DMA1_CHANNEL7_ENABLE)

	list(LENGTH STM32_DMA_V1_DMA1_CHANNEL7_DEPENDENTS length)
	if(length GREATER 1)
		string(REPLACE ";" ", " dependents "${STM32_DMA_V1_DMA1_CHANNEL7_DEPENDENTS}")
		message(STATUS "Warning: DMA1 channel 7 has ${length} dependents: ${dependents}. "
				"DMA channel cannot be used simultaneously by multiple drivers.")
	endif()

	if(distortos_Peripherals_DMA1_channel_7)
		set(ARCHITECTURE_NVIC_DMA1_CH7_ENABLE ON)
	endif(distortos_Peripherals_DMA1_channel_7)

endif(distortos_Peripherals_DMA1)

distortosSetConfiguration(BOOLEAN
		distortos_Peripherals_DMA2
		OFF
		DEPENDENTS ${STM32_DMA_V1_DMA2_DEPENDENTS}
		HELP "Enable DMA2 low-level driver."
		OUTPUT_NAME DISTORTOS_CHIP_DMA2_ENABLE)

if(distortos_Peripherals_DMA2)

	distortosSetConfiguration(BOOLEAN
			distortos_Peripherals_DMA2_channel_1
			OFF
			DEPENDENTS ${STM32_DMA_V1_DMA2_CHANNEL1_DEPENDENTS}
			HELP "Enable DMA2 channel 1 low-level driver."
			OUTPUT_NAME DISTORTOS_CHIP_DMA2_CHANNEL1_ENABLE)

	list(LENGTH STM32_DMA_V1_DMA2_CHANNEL1_DEPENDENTS length)
	if(length GREATER 1)
		string(REPLACE ";" ", " dependents "${STM32_DMA_V1_DMA2_CHANNEL1_DEPENDENTS}")
		message(STATUS "Warning: DMA2 channel 1 has ${length} dependents: ${dependents}. "
				"DMA channel cannot be used simultaneously by multiple drivers.")
	endif()

	if(distortos_Peripherals_DMA2_channel_1)
		set(ARCHITECTURE_NVIC_DMA2_CH1_ENABLE ON)
	endif(distortos_Peripherals_DMA2_channel_1)

	distortosSetConfiguration(BOOLEAN
			distortos_Peripherals_DMA2_channel_2
			OFF
			DEPENDENTS ${STM32_DMA_V1_DMA2_CHANNEL2_DEPENDENTS}
			HELP "Enable DMA2 channel 2 low-level driver."
			OUTPUT_NAME DISTORTOS_CHIP_DMA2_CHANNEL2_ENABLE)

	list(LENGTH STM32_DMA_V1_DMA2_CHANNEL2_DEPENDENTS length)
	if(length GREATER 1)
		string(REPLACE ";" ", " dependents "${STM32_DMA_V1_DMA2_CHANNEL2_DEPENDENTS}")
		message(STATUS "Warning: DMA2 channel 2 has ${length} dependents: ${dependents}. "
				"DMA channel cannot be used simultaneously by multiple drivers.")
	endif()

	if(distortos_Peripherals_DMA2_channel_2)
		set(ARCHITECTURE_NVIC_DMA2_CH2_ENABLE ON)
	endif(distortos_Peripherals_DMA2_channel_2)

	distortosSetConfiguration(BOOLEAN
			distortos_Peripherals_DMA2_channel_3
			OFF
			DEPENDENTS ${STM32_DMA_V1_DMA2_CHANNEL3_DEPENDENTS}
			HELP "Enable DMA2 channel 3 low-level driver."
			OUTPUT_NAME DISTORTOS_CHIP_DMA2_CHANNEL3_ENABLE)

	list(LENGTH STM32_DMA_V1_DMA2_CHANNEL3_DEPENDENTS length)
	if(length GREATER 1)
		string(REPLACE ";" ", " dependents "${STM32_DMA_V1_DMA2_CHANNEL3_DEPENDENTS}")
		message(STATUS "Warning: DMA2 channel 3 has ${length} dependents: ${dependents}. "
				"DMA channel cannot be used simultaneously by multiple drivers.")
	endif()

	if(distortos_Peripherals_DMA2_channel_3)
		set(ARCHITECTURE_NVIC_DMA2_CH3_ENABLE ON)
	endif(distortos_Peripherals_DMA2_channel_3)

	distortosSetConfiguration(BOOLEAN
			distortos_Peripherals_DMA2_channel_4
			OFF
			DEPENDENTS ${STM32_DMA_V1_DMA2_CHANNEL4_DEPENDENTS}
			HELP "Enable DMA2 channel 4 low-level driver."
			OUTPUT_NAME DISTORTOS_CHIP_DMA2_CHANNEL4_ENABLE)

	list(LENGTH STM32_DMA_V1_DMA2_CHANNEL4_DEPENDENTS length)
	if(length GREATER 1)
		string(REPLACE ";" ", " dependents "${STM32_DMA_V1_DMA2_CHANNEL4_DEPENDENTS}")
		message(STATUS "Warning: DMA2 channel 4 has ${length} dependents: ${dependents}. "
				"DMA channel cannot be used simultaneously by multiple drivers.")
	endif()

	if(distortos_Peripherals_DMA2_channel_4)
		set(ARCHITECTURE_NVIC_DMA2_CH4_ENABLE ON)
	endif(distortos_Peripherals_DMA2_channel_4)

	distortosSetConfiguration(BOOLEAN
			distortos_Peripherals_DMA2_channel_5
			OFF
			DEPENDENTS ${STM32_DMA_V1_DMA2_CHANNEL5_DEPENDENTS}
			HELP "Enable DMA2 channel 5 low-level driver."
			OUTPUT_NAME DISTORTOS_CHIP_DMA2_CHANNEL5_ENABLE)

	list(LENGTH STM32_DMA_V1_DMA2_CHANNEL5_DEPENDENTS length)
	if(length GREATER 1)
		string(REPLACE ";" ", " dependents "${STM32_DMA_V1_DMA2_CHANNEL5_DEPENDENTS}")
		message(STATUS "Warning: DMA2 channel 5 has ${length} dependents: ${dependents}. "
				"DMA channel cannot be used simultaneously by multiple drivers.")
	endif()

	if(distortos_Peripherals_DMA2_channel_5)
		set(ARCHITECTURE_NVIC_DMA2_CH5_ENABLE ON)
	endif(distortos_Peripherals_DMA2_channel_5)

endif(distortos_Peripherals_DMA2)

include("${CMAKE_CURRENT_SOURCE_DIR}/source/chip/STM32/peripherals/DMAv1/distortos-sources.cmake")
