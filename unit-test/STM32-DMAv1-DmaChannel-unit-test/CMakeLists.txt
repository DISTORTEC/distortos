#
# file: CMakeLists.txt
#
# author: Copyright (C) 2018 Kamil Szczygiel https://distortec.com https://freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
#

function(addStm32Dmav1DmaChannelUnitTest channelRequestBits)

	set(target STM32-DMAv1-DmaChannel-unit-test-${channelRequestBits}bits)

	add_executable(${target}
			STM32-DMAv1-DmaChannel-unit-test.cpp
			${DISTORTOS_PATH}/source/chip/STM32/peripherals/DMAv1/STM32-DMAv1-DmaChannel.cpp
			${MAIN_CPP})

	target_compile_definitions(${target} PUBLIC
			DISTORTOS_CHIP_DMA_CHANNEL_REQUEST_BITS=${channelRequestBits})
	target_include_directories(${target} BEFORE PUBLIC
			${INCLUDE_MOCKS}/chip/STM32-DMAv1-DmaChannelPeripheral.hpp
			${INCLUDE_MOCKS}/chip/STM32-DMAv1-DmaPeripheral.hpp
			${INCLUDE_MOCKS}/InterruptMaskingLock.hpp)
	target_include_directories(${target} PUBLIC
			${DISTORTOS_PATH}/source/chip/STM32/peripherals/DMAv1/include
			${DISTORTOS_PATH}/source/chip/STM32/include)

	add_custom_target(run-${target}
			COMMAND ${target}
			COMMENT ${target}
			USES_TERMINAL)
	add_dependencies(run run-${target})

endfunction()

addStm32Dmav1DmaChannelUnitTest(0)
addStm32Dmav1DmaChannelUnitTest(4)
