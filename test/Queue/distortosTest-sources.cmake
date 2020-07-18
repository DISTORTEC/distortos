#
# file: distortosTest-sources.cmake
#
# author: Copyright (C) 2018 Kamil Szczygiel http://www.distortec.com https://freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
#

target_sources(distortosTest PRIVATE
		${CMAKE_CURRENT_LIST_DIR}/FifoQueuePriorityTestCase.cpp
		${CMAKE_CURRENT_LIST_DIR}/MessageQueuePriorityTestCase.cpp
		${CMAKE_CURRENT_LIST_DIR}/QueueOperationsTestCase.cpp
		${CMAKE_CURRENT_LIST_DIR}/queueTestCases.cpp
		${CMAKE_CURRENT_LIST_DIR}/QueueWrappers.cpp)
