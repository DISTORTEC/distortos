#
# file: CMakeLists.txt
#
# author: Copyright (C) 2018-2022 Kamil Szczygiel https://distortec.com https://freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
#

add_executable(STM32-SDMMCv1-SdMmcCardLowLevel-unit-test
		STM32-SDMMCv1-SdMmcCardLowLevel-unit-test.cpp
		${DISTORTOS_PATH}/source/chip/STM32/peripherals/SDMMCv1/STM32-SDMMCv1-SdMmcCardLowLevel.cpp
		$<TARGET_OBJECTS:main.cpp-object-library>)

target_compile_definitions(STM32-SDMMCv1-SdMmcCardLowLevel-unit-test PUBLIC
		DISTORTOS_CHIP_STM32_DMAV2)
target_include_directories(STM32-SDMMCv1-SdMmcCardLowLevel-unit-test BEFORE PUBLIC
		${INCLUDE_MOCKS}/chip/STM32-DMAv1-DMAv2-DmaChannel.hpp
		${INCLUDE_MOCKS}/chip/STM32-SDMMCv1-SdmmcPeripheral.hpp)
target_include_directories(STM32-SDMMCv1-SdMmcCardLowLevel-unit-test PUBLIC
		${DISTORTOS_PATH}/source/chip/STM32/peripherals/SDMMCv1/include
		${DISTORTOS_PATH}/source/chip/STM32/include)

add_custom_target(run-STM32-SDMMCv1-SdMmcCardLowLevel-unit-test
		COMMAND STM32-SDMMCv1-SdMmcCardLowLevel-unit-test
		COMMENT STM32-SDMMCv1-SdMmcCardLowLevel-unit-test
		USES_TERMINAL)
add_dependencies(run run-STM32-SDMMCv1-SdMmcCardLowLevel-unit-test)
