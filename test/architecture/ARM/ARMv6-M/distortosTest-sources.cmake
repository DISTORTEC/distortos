#
# file: distortosTest-sources.cmake
#
# author: Copyright (C) 2018-2019 Kamil Szczygiel http://www.distortec.com https://freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
#

if(DISTORTOS_ARCHITECTURE_ARMV6_M)

	target_sources(distortosTest PRIVATE
			${CMAKE_CURRENT_LIST_DIR}/ARMv6-M-architectureTestCases.cpp)

endif()
