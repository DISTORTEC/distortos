#
# file: CMakeLists.txt
#
# author: Copyright (C) 2018-2019 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
#

add_executable(AligningBlockDevice-unit-test
		AligningBlockDevice-unit-test.cpp
		${DISTORTOS_PATH}/source/devices/memory/AligningBlockDevice.cpp
		${MAIN_CPP})

target_compile_definitions(AligningBlockDevice-unit-test PUBLIC
		DISTORTOS_BLOCKDEVICE_BUFFER_ALIGNMENT=128
		DISTORTOS_UNIT_TEST)

add_custom_target(run-AligningBlockDevice-unit-test
		COMMAND AligningBlockDevice-unit-test
		COMMENT AligningBlockDevice-unit-test
		USES_TERMINAL)
add_dependencies(run run-AligningBlockDevice-unit-test)
