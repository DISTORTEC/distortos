#
# file: littlefs1-sources.cmake
#
# author: Copyright (C) 2018-2019 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
#

add_library(littlefs1 STATIC
		${CMAKE_CURRENT_LIST_DIR}/littlefs1/lfs_util.c
		${CMAKE_CURRENT_LIST_DIR}/littlefs1/lfs.c)
if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.8)
	target_compile_features(littlefs1 PUBLIC
			c_std_99)
else()
	# equivalent of C99 for earlier versions of CMake
	target_compile_features(littlefs1 PUBLIC
			c_restrict)
endif()
target_compile_definitions(littlefs1 PUBLIC
		LFS_NO_DEBUG
		LFS_NO_ERROR
		LFS_NO_MALLOC
		LFS_NO_WARN)
target_include_directories(littlefs1 INTERFACE
		${CMAKE_CURRENT_LIST_DIR}/littlefs1)
