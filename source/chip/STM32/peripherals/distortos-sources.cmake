#
# file: distortos-sources.cmake
#
# author: Copyright (C) 2018 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
#

if(CONFIG_CHIP_STM32)

	include(${CMAKE_CURRENT_LIST_DIR}/GPIOv1/distortos-sources.cmake)
	include(${CMAKE_CURRENT_LIST_DIR}/GPIOv2/distortos-sources.cmake)
	include(${CMAKE_CURRENT_LIST_DIR}/SPIv1/distortos-sources.cmake)
	include(${CMAKE_CURRENT_LIST_DIR}/SPIv2/distortos-sources.cmake)
	include(${CMAKE_CURRENT_LIST_DIR}/USARTv1/distortos-sources.cmake)
	include(${CMAKE_CURRENT_LIST_DIR}/USARTv2/distortos-sources.cmake)

endif()
