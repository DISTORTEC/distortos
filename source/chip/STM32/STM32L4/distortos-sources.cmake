#
# file: distortos-sources.cmake
#
# author: Copyright (C) 2018 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
#

if(CONFIG_CHIP_STM32L4)

	target_sources(distortos PRIVATE
			${CMAKE_CURRENT_LIST_DIR}/STM32L4-3-4-5-6-chipVectors.cpp.cpp
			${CMAKE_CURRENT_LIST_DIR}/STM32L4-71-chipVectors.cpp
			${CMAKE_CURRENT_LIST_DIR}/STM32L4-75-76-8-9-A-chipVectors.cpp
			${CMAKE_CURRENT_LIST_DIR}/STM32L4-chipLowLevelInitializer.cpp
			${CMAKE_CURRENT_LIST_DIR}/STM32L4-FLASH.cpp
			${CMAKE_CURRENT_LIST_DIR}/STM32L4-PWR.cpp
			${CMAKE_CURRENT_LIST_DIR}/STM32L4-RCC.cpp)

endif()
