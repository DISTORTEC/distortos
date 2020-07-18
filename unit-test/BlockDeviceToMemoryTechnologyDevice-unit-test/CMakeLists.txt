#
# file: CMakeLists.txt
#
# author: Copyright (C) 2018-2019 Kamil Szczygiel https://distortec.com https://freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
#

add_executable(BlockDeviceToMemoryTechnologyDevice-unit-test
		BlockDeviceToMemoryTechnologyDevice-unit-test.cpp
		${DISTORTOS_PATH}/source/devices/memory/BlockDeviceToMemoryTechnologyDevice.cpp
		${MAIN_CPP})

target_compile_definitions(BlockDeviceToMemoryTechnologyDevice-unit-test PUBLIC
		DISTORTOS_UNIT_TEST)

add_custom_target(run-BlockDeviceToMemoryTechnologyDevice-unit-test
		COMMAND BlockDeviceToMemoryTechnologyDevice-unit-test
		COMMENT BlockDeviceToMemoryTechnologyDevice-unit-test
		USES_TERMINAL)
add_dependencies(run run-BlockDeviceToMemoryTechnologyDevice-unit-test)
