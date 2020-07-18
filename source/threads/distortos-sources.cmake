#
# file: distortos-sources.cmake
#
# author: Copyright (C) 2018 Kamil Szczygiel https://distortec.com https://freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
#

target_sources(distortos PRIVATE
		${CMAKE_CURRENT_LIST_DIR}/DynamicThreadBase.cpp
		${CMAKE_CURRENT_LIST_DIR}/DynamicThread.cpp
		${CMAKE_CURRENT_LIST_DIR}/ThisThread.cpp
		${CMAKE_CURRENT_LIST_DIR}/ThreadCommon.cpp
		${CMAKE_CURRENT_LIST_DIR}/threadExiter.cpp
		${CMAKE_CURRENT_LIST_DIR}/ThreadIdentifier.cpp
		${CMAKE_CURRENT_LIST_DIR}/threadRunner.cpp
		${CMAKE_CURRENT_LIST_DIR}/UndetachableThread.cpp)
