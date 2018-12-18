#
# file: CMakeLists.txt
#
# author: Copyright (C) 2017-2018 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
#

add_executable(C-API-Mutex-compile-link-test
		C-API-Mutex-compile-link-test.c
		${DISTORTOS_PATH}/source/C-API/C-API-Mutex.cpp
		${MAIN_CPP})

target_compile_definitions(C-API-Mutex-compile-link-test PUBLIC
		DISTORTOS_UNIT_TEST
		DISTORTOS_UNIT_TEST_FROMCAPIMOCK_MUTEX)
target_include_directories(C-API-Mutex-compile-link-test BEFORE PUBLIC
		${INCLUDE_MOCKS}/distortosConfiguration.h
		${INCLUDE_MOCKS}/fromCApi.hpp
		${INCLUDE_MOCKS}/Mutex.hpp)

add_executable(C-API-Mutex-unit-test-0
		C-API-Mutex-unit-test-0.cpp
		${DISTORTOS_PATH}/source/C-API/C-API-Mutex.cpp
		${MAIN_CPP})

target_compile_definitions(C-API-Mutex-unit-test-0 PUBLIC
		DISTORTOS_UNIT_TEST
		DISTORTOS_UNIT_TEST_FROMCAPIMOCK_MUTEX)
target_include_directories(C-API-Mutex-unit-test-0 BEFORE PUBLIC
		${INCLUDE_MOCKS}/distortosConfiguration.h
		${INCLUDE_MOCKS}/fromCApi.hpp
		${INCLUDE_MOCKS}/Mutex.hpp)

add_custom_target(run-C-API-Mutex-unit-test-0
		COMMAND C-API-Mutex-unit-test-0
		COMMENT C-API-Mutex-unit-test-0
		USES_TERMINAL)
add_dependencies(run run-C-API-Mutex-unit-test-0)

add_executable(C-API-Mutex-unit-test-1
		C-API-Mutex-unit-test-1.cpp
		${DISTORTOS_PATH}/source/C-API/C-API-Mutex.cpp
		${DISTORTOS_PATH}/source/synchronization/Mutex.cpp
		${DISTORTOS_PATH}/source/synchronization/MutexControlBlock.cpp
		${MAIN_CPP})

target_include_directories(C-API-Mutex-unit-test-1 BEFORE PUBLIC
		${INCLUDE_MOCKS}/internal/scheduler/getScheduler.hpp
		${INCLUDE_MOCKS}/internal/scheduler/Scheduler.hpp
		${INCLUDE_MOCKS}/internal/scheduler/ThreadControlBlock.hpp
		${INCLUDE_MOCKS}/internal/scheduler/ThreadListNode.hpp
		${INCLUDE_MOCKS}/distortosConfiguration.h
		${INCLUDE_MOCKS}/InterruptMaskingLock.hpp
		${INCLUDE_MOCKS}/TickClock.hpp)

add_custom_target(run-C-API-Mutex-unit-test-1
		COMMAND C-API-Mutex-unit-test-1
		COMMENT C-API-Mutex-unit-test-1
		USES_TERMINAL)
add_dependencies(run run-C-API-Mutex-unit-test-1)
