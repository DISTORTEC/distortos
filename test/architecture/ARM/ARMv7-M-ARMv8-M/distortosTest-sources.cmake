#
# file: distortosTest-sources.cmake
#
# author: Copyright (C) 2018-2024 Kamil Szczygiel https://distortec.com https://freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
#

if(DISTORTOS_ARCHITECTURE_ARMV7_M OR DISTORTOS_ARCHITECTURE_ARMV8_M)

	target_sources(distortosTest PRIVATE
			${CMAKE_CURRENT_LIST_DIR}/ARMv7-M-ARMv8-M-architectureTestCases.cpp
			${CMAKE_CURRENT_LIST_DIR}/ARMv7-M-ARMv8-M-checkFpuRegisters.cpp
			${CMAKE_CURRENT_LIST_DIR}/ARMv7-M-ARMv8-M-FpuSignalTestCase.cpp
			${CMAKE_CURRENT_LIST_DIR}/ARMv7-M-ARMv8-M-FpuThreadTestCase.cpp
			${CMAKE_CURRENT_LIST_DIR}/ARMv7-M-ARMv8-M-setFpuRegisters.cpp)

endif()
