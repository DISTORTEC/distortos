#
# file: CMakeLists.txt
#
# author: Copyright (C) 2018-2019 Kamil Szczygiel http://www.distortec.com https://freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
#

add_executable(SdCard-unit-test
		SdCard-unit-test.cpp
		${DISTORTOS_PATH}/source/devices/memory/SdCard.cpp
		${MAIN_CPP})

target_compile_definitions(SdCard-unit-test PUBLIC
		DISTORTOS_UNIT_TEST_MUTEXMOCK_USE_WRAPPER)
target_include_directories(SdCard-unit-test BEFORE PUBLIC
		${INCLUDE_MOCKS}/devices/memory/SynchronousSdMmcCardLowLevel.hpp
		${INCLUDE_MOCKS}/distortosConfiguration.h
		${INCLUDE_MOCKS}/Mutex.hpp
		${INCLUDE_MOCKS}/ThisThread.hpp
		${INCLUDE_MOCKS}/TickClock.hpp)

add_custom_target(run-SdCard-unit-test
		COMMAND SdCard-unit-test
		COMMENT SdCard-unit-test
		USES_TERMINAL)
add_dependencies(run run-SdCard-unit-test)
