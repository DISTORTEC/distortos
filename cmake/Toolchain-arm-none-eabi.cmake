#
# file: Toolchain-arm-none-eabi.cmake
#
# author: Copyright (C) 2018 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
#

if(CMAKE_TOOLCHAIN_ARM_NONE_EABI_CMAKE_)
	return()
endif()
set(CMAKE_TOOLCHAIN_ARM_NONE_EABI_CMAKE_ 1)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION 1)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(TARGET_TRIPLE "arm-none-eabi-")
set(CMAKE_C_COMPILER ${TARGET_TRIPLE}gcc)
set(CMAKE_CXX_COMPILER ${TARGET_TRIPLE}g++)
SET(CMAKE_AR ${TARGET_TRIPLE}gcc-ar CACHE STRING "Name of archiving tool for static libraries.")
SET(CMAKE_RANLIB ${TARGET_TRIPLE}gcc-ranlib CACHE STRING "Name of randomizing tool for static libraries.")
set(CMAKE_SIZE ${TARGET_TRIPLE}size)

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR})
