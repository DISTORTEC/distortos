#
# file: CMakeLists.txt
#
# author: Copyright (C) 2018-2019 Kamil Szczygiel http://www.distortec.com https://freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
#

add_executable(estd-RawCircularBuffer-unit-test
		estd-RawCircularBuffer-unit-test.cpp
		${MAIN_CPP})

add_custom_target(run-estd-RawCircularBuffer-unit-test
		COMMAND estd-RawCircularBuffer-unit-test
		COMMENT estd-RawCircularBuffer-unit-test
		USES_TERMINAL)
add_dependencies(run run-estd-RawCircularBuffer-unit-test)
