#
# file: distortos-sources.cmake
#
# author: Copyright (C) 2018 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
#

if(CONFIG_CHIP_STM32)

	doxygen(INPUT ${CMAKE_CURRENT_LIST_DIR}/include INCLUDE_PATH ${CMAKE_CURRENT_LIST_DIR}/include)

	include(${CMAKE_CURRENT_LIST_DIR}/peripherals/distortos-sources.cmake)
	include(${CMAKE_CURRENT_LIST_DIR}/STM32F0/distortos-sources.cmake)
	include(${CMAKE_CURRENT_LIST_DIR}/STM32F1/distortos-sources.cmake)
	include(${CMAKE_CURRENT_LIST_DIR}/STM32F4/distortos-sources.cmake)
	include(${CMAKE_CURRENT_LIST_DIR}/STM32F7/distortos-sources.cmake)
	include(${CMAKE_CURRENT_LIST_DIR}/STM32L0/distortos-sources.cmake)
	include(${CMAKE_CURRENT_LIST_DIR}/STM32L4/distortos-sources.cmake)

endif()
