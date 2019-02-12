#
# file: CMakeLists.txt
#
# author: Copyright (C) 2017-2019 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
#

cmake_minimum_required(VERSION 3.1)

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
set(CMAKE_CXX_STANDARD 14)

# standard include folders - valid in all cases
include_directories(
		.
		external/Catch2
		external/Trompeloeil
		${DISTORTOS_PATH}/include)

add_compile_options(-Wall -Wextra -Wshadow)

add_custom_target(run)

add_subdirectory(BlockDeviceToMemoryTechnologyDevice-unit-test)
add_subdirectory(C-API-ConditionVariable-unit-test)
add_subdirectory(C-API-Mutex-unit-test)
add_subdirectory(C-API-Semaphore-unit-test)
add_subdirectory(estd-ContiguousRange-unit-test)
add_subdirectory(STM32-DMAv1-DmaChannel-unit-test)
add_subdirectory(STM32-DMAv2-DmaChannel-unit-test)
add_subdirectory(STM32-SDMMCv1-SdMmcCardLowLevel-unit-test)
add_subdirectory(STM32-SPIv1-unit-test)
add_subdirectory(STM32-SPIv1-SpiMasterLowLevelDmaBased-unit-test)
add_subdirectory(STM32-SPIv1-SpiMasterLowLevelInterruptBased-unit-test)
add_subdirectory(STM32-SPIv2-unit-test)
add_subdirectory(STM32-SPIv2-SpiMasterLowLevelDmaBased-unit-test)
add_subdirectory(STM32-SPIv2-SpiMasterLowLevelInterruptBased-unit-test)
