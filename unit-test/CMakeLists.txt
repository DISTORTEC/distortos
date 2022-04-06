#
# file: CMakeLists.txt
#
# author: Copyright (C) 2017-2022 Kamil Szczygiel https://distortec.com https://freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
#

if(CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR)
	message(FATAL_ERROR "In-source builds are not supported. Please use separate directory for build.")
endif()

cmake_minimum_required(VERSION 3.8)
project(distortos-unit-test)

# path to distortos
set(DISTORTOS_PATH ${CMAKE_SOURCE_DIR}/..)

# path to common main.cpp
set(MAIN_CPP ${CMAKE_SOURCE_DIR}/main.cpp)

# include path with mocks
set(INCLUDE_MOCKS ${CMAKE_SOURCE_DIR}/include-mocks)

# use C99 in all cases
set(CMAKE_C_STANDARD 99)

# use C++14 in all cases
set(CMAKE_CXX_STANDARD 17)

# standard include folders - valid in all cases
include_directories(
		.
		external/Catch2
		external/Trompeloeil
		${DISTORTOS_PATH}/include)

add_compile_definitions(DISTORTOS_UNIT_TEST)

add_compile_options(-Wall -Wextra -Wshadow)

if(CMAKE_GENERATOR STREQUAL "Ninja")
	if (CMAKE_C_COMPILER_ID STREQUAL GNU OR CMAKE_CXX_COMPILER_ID STREQUAL GNU)
		add_compile_options(-fdiagnostics-color=always)
	endif()
endif()

add_custom_target(run)

option(COVERAGE "Enable generation of coverage reports with gcovr")

if(COVERAGE)

	add_compile_options(--coverage -fprofile-abs-path)

	if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.13)
		add_link_options(--coverage)
	else()
		link_libraries(--coverage)
	endif()

	add_custom_target(coverage
			COMMAND gcovr --html-details --exclude ${CMAKE_SOURCE_DIR} --output ${CMAKE_BINARY_DIR}/coverage.html
			USES_TERMINAL
			WORKING_DIRECTORY ${DISTORTOS_PATH})
	add_dependencies(coverage run)

endif(COVERAGE)

add_subdirectory(AddressRange-unit-test)
add_subdirectory(BlockDeviceToMemoryTechnologyDevice-unit-test)
add_subdirectory(BufferingBlockDevice-unit-test)
add_subdirectory(C-API-ConditionVariable-unit-test)
add_subdirectory(C-API-Mutex-unit-test)
add_subdirectory(C-API-Semaphore-unit-test)
add_subdirectory(estd-CircularBuffer-unit-test)
add_subdirectory(estd-ContiguousRange-unit-test)
add_subdirectory(estd-RawCircularBuffer-unit-test)
add_subdirectory(FatFileSystem-unit-test)
add_subdirectory(MountPoint-unit-test)
add_subdirectory(SdCard-unit-test)
add_subdirectory(STM32-DMAv1-DmaChannel-unit-test)
add_subdirectory(STM32-DMAv2-DmaChannel-unit-test)
add_subdirectory(STM32-SDMMCv1-SdMmcCardLowLevel-unit-test)
add_subdirectory(STM32-SPIv1-unit-test)
add_subdirectory(STM32-SPIv1-SpiMasterLowLevelDmaBased-unit-test)
add_subdirectory(STM32-SPIv1-SpiMasterLowLevelInterruptBased-unit-test)
add_subdirectory(STM32-SPIv2-unit-test)
add_subdirectory(STM32-SPIv2-SpiMasterLowLevelDmaBased-unit-test)
add_subdirectory(STM32-SPIv2-SpiMasterLowLevelInterruptBased-unit-test)
add_subdirectory(SynchronousSdMmcCardLowLevel-unit-test)

#-----------------------------------------------------------------------------------------------------------------------
# .gitignore for build directory
#-----------------------------------------------------------------------------------------------------------------------

message(STATUS "Generating ${CMAKE_BINARY_DIR}/.gitignore")

file(WRITE ${CMAKE_BINARY_DIR}/.gitignore
		"#\n"
		"# \\file\n"
		"# \\brief .gitignore for build directory\n"
		"#\n"
		"# \\warning\n"
		"# Automatically generated file - do not edit!\n"
		"#\n"
		"\n"
		"# ignore everything\n"
		"*\n")
