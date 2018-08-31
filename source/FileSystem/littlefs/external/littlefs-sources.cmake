#
# file: littlefs-sources.cmake
#
# author: Copyright (C) 2018 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
#

add_library(littlefs STATIC
		${CMAKE_CURRENT_LIST_DIR}/littlefs/lfs_util.c
		${CMAKE_CURRENT_LIST_DIR}/littlefs/lfs.c)
set_target_properties(littlefs PROPERTIES
		C_STANDARD 99
		C_STANDARD_REQUIRED ON)
target_compile_definitions(littlefs PUBLIC
		LFS_NO_DEBUG
		LFS_NO_WARN
		LFS_NO_ERROR)
target_include_directories(littlefs INTERFACE
		${CMAKE_CURRENT_LIST_DIR}/littlefs)
