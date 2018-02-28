#
# file: distortos-sources.cmake
#
# author: Copyright (C) 2018 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
#

if(CONFIG_CHIP_STM32L0)

	target_sources(distortos PRIVATE
			${CMAKE_CURRENT_LIST_DIR}/STM32L0-chipLowLevelInitializer.cpp
			${CMAKE_CURRENT_LIST_DIR}/STM32L0-chipVectors.cpp
			${CMAKE_CURRENT_LIST_DIR}/STM32L0-FLASH.cpp
			${CMAKE_CURRENT_LIST_DIR}/STM32L0-PWR.cpp
			${CMAKE_CURRENT_LIST_DIR}/STM32L0-RCC.cpp)

	doxygen(INPUT ${CMAKE_CURRENT_LIST_DIR}
			INCLUDE_PATH ${CMAKE_CURRENT_LIST_DIR}/include ${CMAKE_CURRENT_LIST_DIR}/external/CMSIS-STM32L0
			EXCLUDE ${CMAKE_CURRENT_LIST_DIR}/external)

endif()
