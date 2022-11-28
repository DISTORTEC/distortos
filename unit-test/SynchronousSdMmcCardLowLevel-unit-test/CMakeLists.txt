#
# file: CMakeLists.txt
#
# author: Copyright (C) 2018-2022 Kamil Szczygiel https://distortec.com https://freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
#

add_executable(SynchronousSdMmcCardLowLevel-unit-test
		SynchronousSdMmcCardLowLevel-unit-test.cpp
		${DISTORTOS_PATH}/source/devices/memory/SynchronousSdMmcCardLowLevel.cpp
		$<TARGET_OBJECTS:main.cpp-object-library>)

target_compile_definitions(SynchronousSdMmcCardLowLevel-unit-test PUBLIC
		DISTORTOS_UNIT_TEST_SEMAPHOREMOCK_USE_WRAPPER)
target_include_directories(SynchronousSdMmcCardLowLevel-unit-test BEFORE PUBLIC
		${INCLUDE_MOCKS}/distortosConfiguration.h
		${INCLUDE_MOCKS}/Semaphore.hpp)

add_custom_target(run-SynchronousSdMmcCardLowLevel-unit-test
		COMMAND SynchronousSdMmcCardLowLevel-unit-test
		COMMENT SynchronousSdMmcCardLowLevel-unit-test
		USES_TERMINAL)
add_dependencies(run run-SynchronousSdMmcCardLowLevel-unit-test)
