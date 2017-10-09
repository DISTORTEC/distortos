#
# file: CMakeLists.txt
#
# author: Copyright (C) 2017 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
#

add_executable(C-API-Mutex-compile-link-test
		C-API-Mutex-compile-link-test.c
		${DISTORTOS_PATH}/source/C-API/C-API-Mutex.cpp
		${MAIN_CPP})
		
target_compile_definitions(C-API-Mutex-compile-link-test PUBLIC DISTORTOS_UNIT_TEST)
target_include_directories(C-API-Mutex-compile-link-test BEFORE PUBLIC
		${INCLUDE_MOCKS}/distortosConfiguration.h
		${INCLUDE_MOCKS}/fromCApi.hpp-Mutex
		${INCLUDE_MOCKS}/Mutex.hpp)
