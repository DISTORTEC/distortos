#
# file: CMakeLists.txt
#
# author: Copyright (C) 2018-2019 Kamil Szczygiel http://www.distortec.com https://freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
#

add_executable(FatFileSystem-unit-test
		FatFileSystem-unit-test.cpp
		${DISTORTOS_PATH}/source/FileSystem/FAT/FatDirectory.cpp
		${DISTORTOS_PATH}/source/FileSystem/FAT/FatFile.cpp
		${DISTORTOS_PATH}/source/FileSystem/FAT/FatFileSystem.cpp
		${DISTORTOS_PATH}/source/FileSystem/FAT/ufatErrorToErrorCode.cpp
		${MAIN_CPP})

target_compile_definitions(FatFileSystem-unit-test PUBLIC
		DISTORTOS_UNIT_TEST_MUTEXMOCK_USE_WRAPPER
		__machine_fsblkcnt_t_defined
		__machine_fsfilcnt_t_defined)
target_include_directories(FatFileSystem-unit-test BEFORE PUBLIC
		${INCLUDE_MOCKS}/distortosConfiguration.h
		${INCLUDE_MOCKS}/Mutex.hpp)
target_include_directories(FatFileSystem-unit-test PUBLIC
		${DISTORTOS_PATH}/source/FileSystem/FAT/external/uFAT)

add_custom_target(run-FatFileSystem-unit-test
		COMMAND FatFileSystem-unit-test
		COMMENT FatFileSystem-unit-test
		USES_TERMINAL)
add_dependencies(run run-FatFileSystem-unit-test)
