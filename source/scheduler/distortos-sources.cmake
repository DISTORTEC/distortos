#
# file: distortos-sources.cmake
#
# author: Copyright (C) 2018 Kamil Szczygiel http://www.distortec.com https://freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
#

target_sources(distortos PRIVATE
		${CMAKE_CURRENT_LIST_DIR}/DynamicSoftwareTimer.cpp
		${CMAKE_CURRENT_LIST_DIR}/forceContextSwitch.cpp
		${CMAKE_CURRENT_LIST_DIR}/getScheduler.cpp
		${CMAKE_CURRENT_LIST_DIR}/IdleThread.cpp
		${CMAKE_CURRENT_LIST_DIR}/MainThread.cpp
		${CMAKE_CURRENT_LIST_DIR}/RoundRobinQuantum.cpp
		${CMAKE_CURRENT_LIST_DIR}/Scheduler.cpp
		${CMAKE_CURRENT_LIST_DIR}/SoftwareTimerCommon.cpp
		${CMAKE_CURRENT_LIST_DIR}/SoftwareTimerControlBlock.cpp
		${CMAKE_CURRENT_LIST_DIR}/SoftwareTimer.cpp
		${CMAKE_CURRENT_LIST_DIR}/SoftwareTimerSupervisor.cpp
		${CMAKE_CURRENT_LIST_DIR}/Stack.cpp
		${CMAKE_CURRENT_LIST_DIR}/statistics.cpp
		${CMAKE_CURRENT_LIST_DIR}/ThreadControlBlock.cpp
		${CMAKE_CURRENT_LIST_DIR}/ThreadGroupControlBlock.cpp)
