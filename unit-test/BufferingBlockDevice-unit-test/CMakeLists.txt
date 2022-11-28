#
# file: CMakeLists.txt
#
# author: Copyright (C) 2018-2022 Kamil Szczygiel https://distortec.com https://freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
#

add_executable(BufferingBlockDevice-unit-test
		BufferingBlockDevice-unit-test.cpp
		${DISTORTOS_PATH}/source/devices/memory/BufferingBlockDevice.cpp
		$<TARGET_OBJECTS:main.cpp-object-library>)

target_compile_definitions(BufferingBlockDevice-unit-test PUBLIC
		DISTORTOS_BLOCKDEVICE_BUFFER_ALIGNMENT=8)

add_custom_target(run-BufferingBlockDevice-unit-test
		COMMAND BufferingBlockDevice-unit-test
		COMMENT BufferingBlockDevice-unit-test
		USES_TERMINAL)
add_dependencies(run run-BufferingBlockDevice-unit-test)
