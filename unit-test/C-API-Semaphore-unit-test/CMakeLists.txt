#
# file: CMakeLists.txt
#
# author: Copyright (C) 2017 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
#

add_executable(C-API-Semaphore-unit-test-0
		C-API-Semaphore-unit-test-0.cpp
		${DISTORTOS_PATH}/source/C-API/C-API-Semaphore.cpp
		${MAIN_CPP})

target_compile_definitions(C-API-Semaphore-unit-test-0 PUBLIC DISTORTOS_UNIT_TEST)
target_include_directories(C-API-Semaphore-unit-test-0 BEFORE PUBLIC ${INCLUDE_MOCKS}/distortosConfiguration.h)
target_include_directories(C-API-Semaphore-unit-test-0 BEFORE PUBLIC ${INCLUDE_MOCKS}/fromCApi.hpp)
target_include_directories(C-API-Semaphore-unit-test-0 BEFORE PUBLIC ${INCLUDE_MOCKS}/Semaphore.hpp)
