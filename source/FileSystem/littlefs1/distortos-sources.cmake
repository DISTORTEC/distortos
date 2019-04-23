#
# file: distortos-sources.cmake
#
# author: Copyright (C) 2018 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
#

target_sources(distortos PRIVATE
		${CMAKE_CURRENT_LIST_DIR}/LittlefsDirectory.cpp
		${CMAKE_CURRENT_LIST_DIR}/littlefsErrorToErrorCode.cpp
		${CMAKE_CURRENT_LIST_DIR}/LittlefsFile.cpp
		${CMAKE_CURRENT_LIST_DIR}/LittlefsFileSystem.cpp)

doxygen(INCLUDE_PATH ${CMAKE_CURRENT_LIST_DIR}/external/littlefs
		EXCLUDE ${CMAKE_CURRENT_LIST_DIR}/external)

include(${CMAKE_CURRENT_LIST_DIR}/external/littlefs-sources.cmake)
