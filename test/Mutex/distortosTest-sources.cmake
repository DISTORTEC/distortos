#
# file: distortosTest-sources.cmake
#
# author: Copyright (C) 2018 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
#

target_sources(distortosTest PRIVATE
		${CMAKE_CURRENT_LIST_DIR}/MutexErrorCheckingOperationsTestCase.cpp
		${CMAKE_CURRENT_LIST_DIR}/MutexOperationsTestCase.cpp
		${CMAKE_CURRENT_LIST_DIR}/MutexPriorityInheritanceOperationsTestCase.cpp
		${CMAKE_CURRENT_LIST_DIR}/MutexPriorityProtectOperationsTestCase.cpp
		${CMAKE_CURRENT_LIST_DIR}/MutexPriorityProtocolTestCase.cpp
		${CMAKE_CURRENT_LIST_DIR}/MutexPriorityTestCase.cpp
		${CMAKE_CURRENT_LIST_DIR}/MutexRecursiveOperationsTestCase.cpp
		${CMAKE_CURRENT_LIST_DIR}/mutexTestCases.cpp
		${CMAKE_CURRENT_LIST_DIR}/mutexTestTryLockWhenLocked.cpp
		${CMAKE_CURRENT_LIST_DIR}/mutexTestUnlockFromWrongThread.cpp)
