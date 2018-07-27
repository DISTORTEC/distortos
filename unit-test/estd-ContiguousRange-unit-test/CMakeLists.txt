#
# file: CMakeLists.txt
#
# author: Copyright (C) 2018 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
#

add_executable(estd-ContiguousRange-compile-link-test
		estd-ContiguousRange-compile-link-test.cpp
		${MAIN_CPP})
set_target_properties(estd-ContiguousRange-compile-link-test PROPERTIES
		CXX_STANDARD 17
		CXX_STANDARD_REQUIRED ON)
