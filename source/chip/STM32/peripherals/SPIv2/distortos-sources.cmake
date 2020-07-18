#
# file: distortos-sources.cmake
#
# author: Copyright (C) 2018-2019 Kamil Szczygiel https://distortec.com https://freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
#

list(APPEND DISTORTOS_SPIMASTER_BUFFER_ALIGNMENTS 1)

target_include_directories(distortos PUBLIC
		${CMAKE_CURRENT_LIST_DIR}/include)

target_sources(distortos PRIVATE
		${CMAKE_CURRENT_LIST_DIR}/STM32-SPIv2.cpp
		${CMAKE_CURRENT_LIST_DIR}/STM32-SPIv2-SpiMasterLowLevelDmaBased.cpp
		${CMAKE_CURRENT_LIST_DIR}/STM32-SPIv2-SpiMasterLowLevelInterruptBased.cpp)

doxygen(INPUT ${CMAKE_CURRENT_LIST_DIR} INCLUDE_PATH ${CMAKE_CURRENT_LIST_DIR}/include)
