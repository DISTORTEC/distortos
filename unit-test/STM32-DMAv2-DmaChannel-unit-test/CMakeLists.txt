#
# file: CMakeLists.txt
#
# author: Copyright (C) 2018 Kamil Szczygiel https://distortec.com https://freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
#

add_executable(STM32-DMAv2-DmaChannel-unit-test
		STM32-DMAv2-DmaChannel-unit-test.cpp
		${DISTORTOS_PATH}/source/chip/STM32/peripherals/DMAv2/STM32-DMAv2-DmaChannel.cpp
		${MAIN_CPP})

target_include_directories(STM32-DMAv2-DmaChannel-unit-test BEFORE PUBLIC
		${INCLUDE_MOCKS}/chip/STM32-DMAv2-DmaChannelPeripheral.hpp
		${INCLUDE_MOCKS}/chip/STM32-DMAv2-DmaPeripheral.hpp
		${INCLUDE_MOCKS}/InterruptMaskingLock.hpp)
target_include_directories(STM32-DMAv2-DmaChannel-unit-test PUBLIC
		${DISTORTOS_PATH}/source/chip/STM32/peripherals/DMAv2/include
		${DISTORTOS_PATH}/source/chip/STM32/include)

add_custom_target(run-STM32-DMAv2-DmaChannel-unit-test
		COMMAND STM32-DMAv2-DmaChannel-unit-test
		COMMENT STM32-DMAv2-DmaChannel-unit-test
		USES_TERMINAL)
add_dependencies(run run-STM32-DMAv2-DmaChannel-unit-test)
