#
# file: distortos-sources.cmake
#
# author: Copyright (C) 2018-2024 Kamil Szczygiel https://distortec.com https://freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
#

target_include_directories(distortos PUBLIC
		${CMAKE_CURRENT_LIST_DIR}/include
		${CMAKE_CURRENT_LIST_DIR}/external/CMSIS)

target_sources(distortos PRIVATE
		${CMAKE_CURRENT_LIST_DIR}/ARMv6-M-ARMv7-M-ARMv8-M-architectureLowLevelInitializer.cpp
		${CMAKE_CURRENT_LIST_DIR}/ARMv6-M-ARMv7-M-ARMv8-M-disableInterruptMasking.cpp
		${CMAKE_CURRENT_LIST_DIR}/ARMv6-M-ARMv7-M-ARMv8-M-enableInterruptMasking.cpp
		${CMAKE_CURRENT_LIST_DIR}/ARMv6-M-ARMv7-M-ARMv8-M-getMainStack.cpp
		${CMAKE_CURRENT_LIST_DIR}/ARMv6-M-ARMv7-M-ARMv8-M-initializeStack.cpp
		${CMAKE_CURRENT_LIST_DIR}/ARMv6-M-ARMv7-M-ARMv8-M-isInInterruptContext.cpp
		${CMAKE_CURRENT_LIST_DIR}/ARMv6-M-ARMv7-M-ARMv8-M-PendSV_Handler.cpp
		${CMAKE_CURRENT_LIST_DIR}/ARMv6-M-ARMv7-M-ARMv8-M-requestContextSwitch.cpp
		${CMAKE_CURRENT_LIST_DIR}/ARMv6-M-ARMv7-M-ARMv8-M-requestFunctionExecution.cpp
		${CMAKE_CURRENT_LIST_DIR}/ARMv6-M-ARMv7-M-ARMv8-M-Reset_Handler.cpp
		${CMAKE_CURRENT_LIST_DIR}/ARMv6-M-ARMv7-M-ARMv8-M-restoreInterruptMasking.cpp
		${CMAKE_CURRENT_LIST_DIR}/ARMv6-M-ARMv7-M-ARMv8-M-startScheduling.cpp
		${CMAKE_CURRENT_LIST_DIR}/ARMv6-M-ARMv7-M-ARMv8-M-supervisorCall.cpp
		${CMAKE_CURRENT_LIST_DIR}/ARMv6-M-ARMv7-M-ARMv8-M-SVC_Handler.cpp
		${CMAKE_CURRENT_LIST_DIR}/ARMv6-M-ARMv7-M-ARMv8-M-SysTick_Handler.cpp)

doxygen(INPUT ${CMAKE_CURRENT_LIST_DIR}
		INCLUDE_PATH ${CMAKE_CURRENT_LIST_DIR}/include ${CMAKE_CURRENT_LIST_DIR}/external/CMSIS
		EXCLUDE ${CMAKE_CURRENT_LIST_DIR}/external)
