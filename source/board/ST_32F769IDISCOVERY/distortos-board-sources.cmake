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

if(CONFIG_BOARD_SOURCE_BOARD_ST_32F769IDISCOVERY_ST_32F769IDISCOVERY)

	target_sources(distortos PRIVATE
			${CMAKE_CURRENT_LIST_DIR}/ST_32F769IDISCOVERY-leds.cpp
			${CMAKE_CURRENT_LIST_DIR}/ST_32F769IDISCOVERY-buttons.cpp)

endif()
