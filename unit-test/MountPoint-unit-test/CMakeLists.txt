#
# file: CMakeLists.txt
#
# author: Copyright (C) 2018-2021 Kamil Szczygiel https://distortec.com https://freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
#

add_executable(MountPoint-unit-test
		MountPoint-unit-test.cpp
		${DISTORTOS_PATH}/source/FileSystem/MountPoint.cpp
		${MAIN_CPP})

target_compile_definitions(MountPoint-unit-test PUBLIC
		DISTORTOS_UNIT_TEST_MUTEXMOCK_USE_WRAPPER
		__machine_fsblkcnt_t_defined
		__machine_fsfilcnt_t_defined)
target_include_directories(MountPoint-unit-test BEFORE PUBLIC
		${INCLUDE_MOCKS}/distortosConfiguration.h
		${INCLUDE_MOCKS}/Mutex.hpp)

add_custom_target(run-MountPoint-unit-test
		COMMAND MountPoint-unit-test
		COMMENT MountPoint-unit-test
		USES_TERMINAL)
add_dependencies(run run-MountPoint-unit-test)
