#
# file: CMakeLists.txt
#
# author: Copyright (C) 2017-2022 Kamil Szczygiel https://distortec.com https://freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
#

add_executable(C-API-Semaphore-compile-link-test
		C-API-Semaphore-compile-link-test.c
		${DISTORTOS_PATH}/source/C-API/C-API-Semaphore.cpp
		${MAIN_CPP})

target_compile_definitions(C-API-Semaphore-compile-link-test PUBLIC
		DISTORTOS_UNIT_TEST_FROMCAPIMOCK_SEMAPHORE)
target_include_directories(C-API-Semaphore-compile-link-test BEFORE PUBLIC
		${INCLUDE_MOCKS}/distortosConfiguration.h
		${INCLUDE_MOCKS}/fromCApi.hpp
		${INCLUDE_MOCKS}/Semaphore.hpp)

add_executable(C-API-Semaphore-unit-test-0
		C-API-Semaphore-unit-test-0.cpp
		${DISTORTOS_PATH}/source/C-API/C-API-Semaphore.cpp
		${MAIN_CPP})

target_compile_definitions(C-API-Semaphore-unit-test-0 PUBLIC
		DISTORTOS_UNIT_TEST_FROMCAPIMOCK_SEMAPHORE)
target_include_directories(C-API-Semaphore-unit-test-0 BEFORE PUBLIC
		${INCLUDE_MOCKS}/distortosConfiguration.h
		${INCLUDE_MOCKS}/fromCApi.hpp
		${INCLUDE_MOCKS}/Semaphore.hpp)

add_custom_target(run-C-API-Semaphore-unit-test-0
		COMMAND C-API-Semaphore-unit-test-0
		COMMENT C-API-Semaphore-unit-test-0
		USES_TERMINAL)
add_dependencies(run run-C-API-Semaphore-unit-test-0)

add_executable(C-API-Semaphore-unit-test-1
		C-API-Semaphore-unit-test-1.cpp
		${DISTORTOS_PATH}/source/C-API/C-API-Semaphore.cpp
		${DISTORTOS_PATH}/source/synchronization/Semaphore.cpp
		${MAIN_CPP})

target_include_directories(C-API-Semaphore-unit-test-1 BEFORE PUBLIC
		${INCLUDE_MOCKS}/internal/scheduler/getScheduler.hpp
		${INCLUDE_MOCKS}/internal/scheduler/Scheduler.hpp
		${INCLUDE_MOCKS}/internal/scheduler/ThreadControlBlock.hpp
		${INCLUDE_MOCKS}/internal/scheduler/ThreadListNode.hpp
		${INCLUDE_MOCKS}/distortosConfiguration.h
		${INCLUDE_MOCKS}/InterruptMaskingLock.hpp
		${INCLUDE_MOCKS}/TickClock.hpp)

add_custom_target(run-C-API-Semaphore-unit-test-1
		COMMAND C-API-Semaphore-unit-test-1
		COMMENT C-API-Semaphore-unit-test-1
		USES_TERMINAL)
add_dependencies(run run-C-API-Semaphore-unit-test-1)
