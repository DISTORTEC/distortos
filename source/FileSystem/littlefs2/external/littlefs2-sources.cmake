#
# file: littlefs2-sources.cmake
#
# author: Copyright (C) 2018-2019 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
#

add_library(littlefs2 STATIC
		${CMAKE_CURRENT_LIST_DIR}/littlefs2/lfs2_util.c
		${CMAKE_CURRENT_LIST_DIR}/littlefs2/lfs2.c)
target_compile_features(littlefs2 PUBLIC
		c_std_99)
target_compile_definitions(littlefs2 PUBLIC
		LFS2_NO_DEBUG
		LFS2_NO_ERROR
		LFS2_NO_WARN)
target_include_directories(littlefs2 INTERFACE
		${CMAKE_CURRENT_LIST_DIR}/littlefs2)
