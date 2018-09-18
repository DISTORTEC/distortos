#
# file: distortos-board-sources.cmake
#
# author: Copyright (C) 2018 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
#
# Automatically generated file - do not edit!
#

distortosSetConfiguration(STRING
		CONFIG_BOARD
		"ST,32F746GDISCOVERY"
		INTERNAL)

target_include_directories(distortos PUBLIC
		${CMAKE_CURRENT_LIST_DIR}/include)

target_sources(distortos PRIVATE
		${CMAKE_CURRENT_LIST_DIR}/ST_32F746GDISCOVERY-buttons.cpp
		${CMAKE_CURRENT_LIST_DIR}/ST_32F746GDISCOVERY-leds.cpp
		${CMAKE_CURRENT_LIST_DIR}/ST_32F746GDISCOVERY-spis.cpp
		${CMAKE_CURRENT_LIST_DIR}/ST_32F746GDISCOVERY-uarts.cpp
		${CMAKE_CURRENT_LIST_DIR}/ST_32F746GDISCOVERY-vectorTable.cpp)

doxygen(INPUT ${CMAKE_CURRENT_LIST_DIR} INCLUDE_PATH ${CMAKE_CURRENT_LIST_DIR}/include)

include(${CMAKE_CURRENT_LIST_DIR}/cmake/10-buttons.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/cmake/10-leds.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/cmake/70-STM32-GPIOv2.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/cmake/70-STM32-SPIv2.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/cmake/70-STM32-USARTv2.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/cmake/80-STM32.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/cmake/90-ARMv6-M-ARMv7-M.cmake)

include(${CMAKE_CURRENT_LIST_DIR}/distortos-board-sources.extension.cmake OPTIONAL)

set(DISTORTOS_BOARD_VERSION 1)
