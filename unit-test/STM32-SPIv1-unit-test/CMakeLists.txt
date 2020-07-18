#
# file: CMakeLists.txt
#
# author: Copyright (C) 2018-2019 Kamil Szczygiel http://www.distortec.com https://freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
#

add_executable(STM32-SPIv1-unit-test
		STM32-SPIv1-unit-test.cpp
		${DISTORTOS_PATH}/source/chip/STM32/peripherals/SPIv1/STM32-SPIv1.cpp
		${MAIN_CPP})

target_include_directories(STM32-SPIv1-unit-test BEFORE PUBLIC
		${INCLUDE_MOCKS}/chip/STM32-SPIv1-SpiPeripheral.hpp)
target_include_directories(STM32-SPIv1-unit-test PUBLIC
		${DISTORTOS_PATH}/source/chip/STM32/peripherals/SPIv1/include)

add_custom_target(run-STM32-SPIv1-unit-test
		COMMAND STM32-SPIv1-unit-test
		COMMENT STM32-SPIv1-unit-test
		USES_TERMINAL)
add_dependencies(run run-STM32-SPIv1-unit-test)
