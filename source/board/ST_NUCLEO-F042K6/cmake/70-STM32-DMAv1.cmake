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

if(NOT distortos_Peripherals_DMA1 AND STM32_DMA_V1_DMA1_DEPENDENTS)
	string(REPLACE ";" ", " dependents "${STM32_DMA_V1_DMA1_DEPENDENTS}")
	message(STATUS "Auto-enabling DMA1. Dependents: ${dependents}")
	set(value ON FORCE)
else()
	set(value OFF)
endif()

distortosSetConfiguration(BOOLEAN
		distortos_Peripherals_DMA1
		${value}
		HELP "Enable DMA1 low-level driver."
		OUTPUT_NAME CONFIG_CHIP_STM32_DMAV1_DMA1_ENABLE)

if(distortos_Peripherals_DMA1)

	string(REPLACE ";" ", " dependents "${STM32_DMA_V1_DMA1_CHANNEL1_DEPENDENTS}")
	if(NOT distortos_Peripherals_DMA1_channel_1 AND
			STM32_DMA_V1_DMA1_CHANNEL1_DEPENDENTS)
		message(STATUS "Auto-enabling DMA1 channel 1. Dependents: ${dependents}")
		set(value ON FORCE)
	else()
		set(value OFF)
	endif()

	list(LENGTH STM32_DMA_V1_DMA1_CHANNEL1_DEPENDENTS length)
	if(length GREATER 1)
		message(STATUS "Warning: DMA1 channel 1 has ${length} dependents: ${dependents}. "
				"DMA channel cannot be used simultaneously by multiple drivers.")
	endif()

	distortosSetConfiguration(BOOLEAN
			distortos_Peripherals_DMA1_channel_1
			${value}
			HELP "Enable DMA1 channel 1 low-level driver."
			OUTPUT_NAME CONFIG_CHIP_STM32_DMAV1_DMA1_CHANNEL1_ENABLE)

	if(distortos_Peripherals_DMA1_channel_1)
		set(ARCHITECTURE_ARMV6_M_ARMV7_M_NVIC_DMA1_CH1_ENABLE ON)
	endif(distortos_Peripherals_DMA1_channel_1)

	string(REPLACE ";" ", " dependents "${STM32_DMA_V1_DMA1_CHANNEL2_DEPENDENTS}")
	if(NOT distortos_Peripherals_DMA1_channel_2 AND
			STM32_DMA_V1_DMA1_CHANNEL2_DEPENDENTS)
		message(STATUS "Auto-enabling DMA1 channel 2. Dependents: ${dependents}")
		set(value ON FORCE)
	else()
		set(value OFF)
	endif()

	list(LENGTH STM32_DMA_V1_DMA1_CHANNEL2_DEPENDENTS length)
	if(length GREATER 1)
		message(STATUS "Warning: DMA1 channel 2 has ${length} dependents: ${dependents}. "
				"DMA channel cannot be used simultaneously by multiple drivers.")
	endif()

	distortosSetConfiguration(BOOLEAN
			distortos_Peripherals_DMA1_channel_2
			${value}
			HELP "Enable DMA1 channel 2 low-level driver."
			OUTPUT_NAME CONFIG_CHIP_STM32_DMAV1_DMA1_CHANNEL2_ENABLE)

	if(distortos_Peripherals_DMA1_channel_2)
		set(ARCHITECTURE_ARMV6_M_ARMV7_M_NVIC_DMA1_CH2_ENABLE ON)
	endif(distortos_Peripherals_DMA1_channel_2)

	string(REPLACE ";" ", " dependents "${STM32_DMA_V1_DMA1_CHANNEL3_DEPENDENTS}")
	if(NOT distortos_Peripherals_DMA1_channel_3 AND
			STM32_DMA_V1_DMA1_CHANNEL3_DEPENDENTS)
		message(STATUS "Auto-enabling DMA1 channel 3. Dependents: ${dependents}")
		set(value ON FORCE)
	else()
		set(value OFF)
	endif()

	list(LENGTH STM32_DMA_V1_DMA1_CHANNEL3_DEPENDENTS length)
	if(length GREATER 1)
		message(STATUS "Warning: DMA1 channel 3 has ${length} dependents: ${dependents}. "
				"DMA channel cannot be used simultaneously by multiple drivers.")
	endif()

	distortosSetConfiguration(BOOLEAN
			distortos_Peripherals_DMA1_channel_3
			${value}
			HELP "Enable DMA1 channel 3 low-level driver."
			OUTPUT_NAME CONFIG_CHIP_STM32_DMAV1_DMA1_CHANNEL3_ENABLE)

	if(distortos_Peripherals_DMA1_channel_3)
		set(ARCHITECTURE_ARMV6_M_ARMV7_M_NVIC_DMA1_CH3_ENABLE ON)
	endif(distortos_Peripherals_DMA1_channel_3)

	string(REPLACE ";" ", " dependents "${STM32_DMA_V1_DMA1_CHANNEL4_DEPENDENTS}")
	if(NOT distortos_Peripherals_DMA1_channel_4 AND
			STM32_DMA_V1_DMA1_CHANNEL4_DEPENDENTS)
		message(STATUS "Auto-enabling DMA1 channel 4. Dependents: ${dependents}")
		set(value ON FORCE)
	else()
		set(value OFF)
	endif()

	list(LENGTH STM32_DMA_V1_DMA1_CHANNEL4_DEPENDENTS length)
	if(length GREATER 1)
		message(STATUS "Warning: DMA1 channel 4 has ${length} dependents: ${dependents}. "
				"DMA channel cannot be used simultaneously by multiple drivers.")
	endif()

	distortosSetConfiguration(BOOLEAN
			distortos_Peripherals_DMA1_channel_4
			${value}
			HELP "Enable DMA1 channel 4 low-level driver."
			OUTPUT_NAME CONFIG_CHIP_STM32_DMAV1_DMA1_CHANNEL4_ENABLE)

	if(distortos_Peripherals_DMA1_channel_4)
		set(ARCHITECTURE_ARMV6_M_ARMV7_M_NVIC_DMA1_CH4_ENABLE ON)
	endif(distortos_Peripherals_DMA1_channel_4)

	string(REPLACE ";" ", " dependents "${STM32_DMA_V1_DMA1_CHANNEL5_DEPENDENTS}")
	if(NOT distortos_Peripherals_DMA1_channel_5 AND
			STM32_DMA_V1_DMA1_CHANNEL5_DEPENDENTS)
		message(STATUS "Auto-enabling DMA1 channel 5. Dependents: ${dependents}")
		set(value ON FORCE)
	else()
		set(value OFF)
	endif()

	list(LENGTH STM32_DMA_V1_DMA1_CHANNEL5_DEPENDENTS length)
	if(length GREATER 1)
		message(STATUS "Warning: DMA1 channel 5 has ${length} dependents: ${dependents}. "
				"DMA channel cannot be used simultaneously by multiple drivers.")
	endif()

	distortosSetConfiguration(BOOLEAN
			distortos_Peripherals_DMA1_channel_5
			${value}
			HELP "Enable DMA1 channel 5 low-level driver."
			OUTPUT_NAME CONFIG_CHIP_STM32_DMAV1_DMA1_CHANNEL5_ENABLE)

	if(distortos_Peripherals_DMA1_channel_5)
		set(ARCHITECTURE_ARMV6_M_ARMV7_M_NVIC_DMA1_CH5_ENABLE ON)
	endif(distortos_Peripherals_DMA1_channel_5)

endif(distortos_Peripherals_DMA1)

include("${CMAKE_CURRENT_SOURCE_DIR}/source/chip/STM32/peripherals/DMAv1/distortos-sources.cmake")
