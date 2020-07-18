#
# file: distortos-sources.cmake
#
# author: Copyright (C) 2018-2019 Kamil Szczygiel https://distortec.com https://freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
#

target_sources(distortos PRIVATE
		${CMAKE_CURRENT_LIST_DIR}/Littlefs1Directory.cpp
		${CMAKE_CURRENT_LIST_DIR}/littlefs1ErrorToErrorCode.cpp
		${CMAKE_CURRENT_LIST_DIR}/Littlefs1File.cpp
		${CMAKE_CURRENT_LIST_DIR}/Littlefs1FileSystem.cpp)

doxygen(INCLUDE_PATH ${CMAKE_CURRENT_LIST_DIR}/external/littlefs1
		EXCLUDE ${CMAKE_CURRENT_LIST_DIR}/external)

include(${CMAKE_CURRENT_LIST_DIR}/external/littlefs1-sources.cmake)

target_link_libraries(distortos PUBLIC
		littlefs1)
