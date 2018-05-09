#
# file: distortosTest-sources.cmake
#
# author: Copyright (C) 2018 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
#

target_sources(distortosTest PRIVATE
		${CMAKE_CURRENT_LIST_DIR}/abortSignalHandler.cpp
		${CMAKE_CURRENT_LIST_DIR}/SignalCatchingOperationsTestCase.cpp
		${CMAKE_CURRENT_LIST_DIR}/SignalsCatchingTestCase.cpp
		${CMAKE_CURRENT_LIST_DIR}/SignalsGenerationOperationsTestCase.cpp
		${CMAKE_CURRENT_LIST_DIR}/SignalsInterruptionTestCase.cpp
		${CMAKE_CURRENT_LIST_DIR}/SignalsQueuedOperationsTestCase.cpp
		${CMAKE_CURRENT_LIST_DIR}/SignalsTestCaseCommon.cpp
		${CMAKE_CURRENT_LIST_DIR}/signalsTestCases.cpp
		${CMAKE_CURRENT_LIST_DIR}/signalsTestSelfOneSignalPending.cpp
		${CMAKE_CURRENT_LIST_DIR}/SignalsWaitOperationsTestCase.cpp
		${CMAKE_CURRENT_LIST_DIR}/SignalsWaitTestCase.cpp)
