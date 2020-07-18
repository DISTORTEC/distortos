#
# file: Toolchain-ST_NUCLEO-L476RG.cmake
#
# author: Copyright (C) 2018-2019 Kamil Szczygiel https://distortec.com https://freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
#
# Automatically generated file - do not edit!
#

if(SOURCE_BOARD_ST_NUCLEO_L476RG_TOOLCHAIN_ST_NUCLEO_L476RG_CMAKE_)
	return()
endif()
set(SOURCE_BOARD_ST_NUCLEO_L476RG_TOOLCHAIN_ST_NUCLEO_L476RG_CMAKE_ 1)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/../../../cmake")

include(distortos-utilities)

set(CMAKE_SYSTEM_NAME distortos)
set(CMAKE_SYSTEM_VERSION 1)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(TARGET_TRIPLE "arm-none-eabi-")
set(CMAKE_C_COMPILER "${TARGET_TRIPLE}gcc")
set(CMAKE_CXX_COMPILER "${TARGET_TRIPLE}g++")
set(CMAKE_AR "${TARGET_TRIPLE}gcc-ar" CACHE STRING "Name of archiving tool for static libraries.")
set(CMAKE_RANLIB "${TARGET_TRIPLE}gcc-ranlib" CACHE STRING "Name of randomizing tool for static libraries.")
set(CMAKE_SIZE "${TARGET_TRIPLE}size")

set(CMAKE_C_FLAGS
		"-ffunction-sections -fdata-sections -Wall -Wextra -Wshadow -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mthumb"
		CACHE STRING "Flags used by the C compiler during all build types.")
set(CMAKE_CXX_FLAGS
		"-fno-rtti -fno-exceptions -ffunction-sections -fdata-sections -Wall -Wextra -Wshadow -Wno-psabi -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mthumb"
		CACHE STRING "Flags used by the CXX compiler during all build types.")
set(CMAKE_EXE_LINKER_FLAGS
		"-Wl,--gc-sections"
		CACHE STRING "Flags used by the linker during all build types.")

set(CMAKE_C_FLAGS_DEBUG
		"-Og -g -ggdb3"
		CACHE STRING "Flags used by the C compiler during DEBUG builds.")
set(CMAKE_C_FLAGS_MINSIZEREL
		"-Os"
		CACHE STRING "Flags used by the C compiler during MINSIZEREL builds.")
set(CMAKE_C_FLAGS_RELEASE
		"-O2"
		CACHE STRING "Flags used by the C compiler during RELEASE builds.")
set(CMAKE_C_FLAGS_RELWITHDEBINFO
		"-O2 -g -ggdb3"
		CACHE STRING "Flags used by the C compiler during RELWITHDEBINFO builds.")

set(CMAKE_CXX_FLAGS_DEBUG
		"-Og -g -ggdb3"
		CACHE STRING "Flags used by the CXX compiler during DEBUG builds.")
set(CMAKE_CXX_FLAGS_MINSIZEREL
		"-Os"
		CACHE STRING "Flags used by the CXX compiler during MINSIZEREL builds.")
set(CMAKE_CXX_FLAGS_RELEASE
		"-O2"
		CACHE STRING "Flags used by the CXX compiler during RELEASE builds.")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO
		"-O2 -g -ggdb3"
		CACHE STRING "Flags used by the CXX compiler during RELWITHDEBINFO builds.")

set(CMAKE_EXE_LINKER_FLAGS_DEBUG
		""
		CACHE STRING "Flags used by the linker during DEBUG builds.")
set(CMAKE_EXE_LINKER_FLAGS_MINSIZEREL
		""
		CACHE STRING "Flags used by the linker during MINSIZEREL builds.")
set(CMAKE_EXE_LINKER_FLAGS_RELEASE
		""
		CACHE STRING "Flags used by the linker during RELEASE builds.")
set(CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO
		""
		CACHE STRING "Flags used by the linker during RELWITHDEBINFO builds.")

if(DEFINED DISTORTOS_CONFIGURATION_VERSION AND DISTORTOS_CONFIGURATION_VERSION LESS 2)

	message(STATUS "Removing \"-DNDEBUG\" from CMAKE_{C,CXX}_FLAGS_{MINSIZEREL,RELEASE,RELWITHDEBINFO}.")
	message(STATUS "Assertions are now configured with distortos_Checks_..._Asserts option.")
	distortosRemoveFlag(CMAKE_C_FLAGS_MINSIZEREL "-DNDEBUG")
	distortosRemoveFlag(CMAKE_C_FLAGS_RELEASE "-DNDEBUG")
	distortosRemoveFlag(CMAKE_C_FLAGS_RELWITHDEBINFO "-DNDEBUG")
	distortosRemoveFlag(CMAKE_CXX_FLAGS_MINSIZEREL "-DNDEBUG")
	distortosRemoveFlag(CMAKE_CXX_FLAGS_RELEASE "-DNDEBUG")
	distortosRemoveFlag(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-DNDEBUG")

endif()

if(DEFINED DISTORTOS_CONFIGURATION_VERSION AND DISTORTOS_CONFIGURATION_VERSION LESS 3)

	message(STATUS "Removing architecture flags (\"-m...\") from CMAKE_EXE_LINKER_FLAGS.")
	message(STATUS "CMake uses CMAKE_{C,CXX}_FLAGS (which include architecture flags) during linking.")
	distortosRemoveFlag(CMAKE_EXE_LINKER_FLAGS "-mcpu")
	distortosRemoveFlag(CMAKE_EXE_LINKER_FLAGS "-mfpu")
	distortosRemoveFlag(CMAKE_EXE_LINKER_FLAGS "-mthumb")
	distortosRemoveFlag(CMAKE_EXE_LINKER_FLAGS "-mfloat-abi")

endif()

if(DEFINED DISTORTOS_CONFIGURATION_VERSION AND DISTORTOS_CONFIGURATION_VERSION LESS 4)

	message(STATUS "Adding \"-Wno-psabi\" to CMAKE_CXX_FLAGS.")
	distortosRemoveFlag(CMAKE_CXX_FLAGS "-Wno-psabi")
	distortosAddFlag(CMAKE_CXX_FLAGS "-Wno-psabi")

endif()

if(distortos_Build_00_Static_destructors)

	distortosSetConfiguration(BOOLEAN
			distortos_Build_01_Run_time_registration_of_static_destructors
			OFF
			HELP "Register static destructors in run time.

			Register destructors for objects with static storage duration in run time (with __cxa_atexit()). Such
			behaviour is required for fully standards-compliant handling of static destructors, especially when C++
			exceptions are used, but it uses more ROM and RAM (possibly allocated from the heap).

			If this option is disabled, all of these destructors are placed in an array and executed at program exit in
			appropriate order (reversed in respect to constructors)."
			NO_OUTPUT)

endif(distortos_Build_00_Static_destructors)

distortosSetConfiguration(STRING
		distortos_Build_02_Floating_point_ABI
		hard
		soft
		softfp
		HELP "Select floating-point ABI."
		NO_OUTPUT)

distortosRemoveFlag(CMAKE_C_FLAGS "-mfloat-abi")
distortosRemoveFlag(CMAKE_CXX_FLAGS "-mfloat-abi")
distortosAddFlag(CMAKE_C_FLAGS "-mfloat-abi=${distortos_Build_02_Floating_point_ABI}")
distortosAddFlag(CMAKE_CXX_FLAGS "-mfloat-abi=${distortos_Build_02_Floating_point_ABI}")

distortosRemoveFlag(CMAKE_CXX_FLAGS "-f(no-)?use-cxa-atexit")
if(NOT distortos_Build_00_Static_destructors OR NOT distortos_Build_01_Run_time_registration_of_static_destructors)
	distortosAddFlag(CMAKE_CXX_FLAGS "-fno-use-cxa-atexit")
endif()

if(CMAKE_GENERATOR STREQUAL "Ninja")
	add_compile_options(-fdiagnostics-color=always)
endif()

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

set(DISTORTOS_BOARD_PATH "source/board/ST_NUCLEO-L476RG")
