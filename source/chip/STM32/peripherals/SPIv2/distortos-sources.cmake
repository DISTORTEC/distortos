#
# file: distortos-sources.cmake
#
# author: Copyright (C) 2018 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
#

if(CONFIG_CHIP_STM32_SPIV2)

	target_sources(distortos PRIVATE
			${CMAKE_CURRENT_LIST_DIR}/STM32-SPIv2-ChipSpiMasterLowLevel.cpp
			${CMAKE_CURRENT_LIST_DIR}/STM32-SPIv2-spiLowLevelInitializer.cpp
			${CMAKE_CURRENT_LIST_DIR}/STM32-SPIv2-spis.cpp)

endif()
