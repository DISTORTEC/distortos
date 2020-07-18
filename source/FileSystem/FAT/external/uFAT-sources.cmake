#
# file: uFAT-sources.cmake
#
# author: Copyright (C) 2018-2019 Kamil Szczygiel https://distortec.com https://freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
#

add_library(uFAT STATIC
		${CMAKE_CURRENT_LIST_DIR}/uFAT/ufat.c
		${CMAKE_CURRENT_LIST_DIR}/uFAT/ufat_dir.c
		${CMAKE_CURRENT_LIST_DIR}/uFAT/ufat_ent.c
		${CMAKE_CURRENT_LIST_DIR}/uFAT/ufat_file.c
		${CMAKE_CURRENT_LIST_DIR}/uFAT/ufat_mkfs.c)
target_compile_features(uFAT PUBLIC
		c_std_99)
target_include_directories(uFAT INTERFACE
		${CMAKE_CURRENT_LIST_DIR}/uFAT)
