#
# file: CMakeLists.txt
#
# author: Copyright (C) 2018-2019 Kamil Szczygiel https://distortec.com https://freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
#

add_executable(AddressRange-unit-test
		AddressRange-unit-test.cpp
		${MAIN_CPP})

target_include_directories(AddressRange-unit-test PUBLIC
		${DISTORTOS_PATH}/source/devices/memory)

add_custom_target(run-AddressRange-unit-test
		COMMAND AddressRange-unit-test
		COMMENT AddressRange-unit-test
		USES_TERMINAL)
add_dependencies(run run-AddressRange-unit-test)
