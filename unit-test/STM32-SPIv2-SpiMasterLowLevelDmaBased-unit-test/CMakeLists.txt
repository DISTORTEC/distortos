#
# file: CMakeLists.txt
#
# author: Copyright (C) 2018-2022 Kamil Szczygiel https://distortec.com https://freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
#

add_executable(STM32-SPIv2-SpiMasterLowLevelDmaBased-unit-test
		STM32-SPIv2-SpiMasterLowLevelDmaBased-unit-test.cpp
		${DISTORTOS_PATH}/source/chip/STM32/peripherals/SPIv2/STM32-SPIv2-SpiMasterLowLevelDmaBased.cpp
		$<TARGET_OBJECTS:main.cpp-object-library>)

target_include_directories(STM32-SPIv2-SpiMasterLowLevelDmaBased-unit-test BEFORE PUBLIC
		${INCLUDE_MOCKS}/chip/STM32-DMAv1-DMAv2-DmaChannel.hpp
		${INCLUDE_MOCKS}/chip/STM32-SPIv1-SPIv2.hpp
		${INCLUDE_MOCKS}/chip/STM32-SPIv2-SpiPeripheral.hpp)
target_include_directories(STM32-SPIv2-SpiMasterLowLevelDmaBased-unit-test PUBLIC
		${DISTORTOS_PATH}/source/chip/STM32/peripherals/SPIv2/include
		${DISTORTOS_PATH}/source/chip/STM32/include)

add_custom_target(run-STM32-SPIv2-SpiMasterLowLevelDmaBased-unit-test
		COMMAND STM32-SPIv2-SpiMasterLowLevelDmaBased-unit-test
		COMMENT STM32-SPIv2-SpiMasterLowLevelDmaBased-unit-test
		USES_TERMINAL)
add_dependencies(run run-STM32-SPIv2-SpiMasterLowLevelDmaBased-unit-test)
