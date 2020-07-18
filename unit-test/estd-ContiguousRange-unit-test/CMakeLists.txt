#
# file: CMakeLists.txt
#
# author: Copyright (C) 2018-2019 Kamil Szczygiel https://distortec.com https://freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
#

add_executable(estd-ContiguousRange-compile-link-test
		estd-ContiguousRange-compile-link-test.cpp
		${MAIN_CPP})
target_compile_features(estd-ContiguousRange-compile-link-test PUBLIC
		cxx_std_17)
