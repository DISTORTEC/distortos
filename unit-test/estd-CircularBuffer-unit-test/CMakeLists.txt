#
# file: CMakeLists.txt
#
# author: Copyright (C) 2018-2020 Kamil Szczygiel https://distortec.com https://freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
#

add_executable(estd-CircularBuffer-unit-test
		estd-CircularBuffer-unit-test.cpp
		${MAIN_CPP})

add_custom_target(run-estd-CircularBuffer-unit-test
		COMMAND estd-CircularBuffer-unit-test
		COMMENT estd-CircularBuffer-unit-test
		USES_TERMINAL)
add_dependencies(run run-estd-CircularBuffer-unit-test)
