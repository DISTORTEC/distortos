#
# file: distortos-sources.cmake
#
# author: Copyright (C) 2018-2019 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
#

target_sources(distortos PRIVATE
		${CMAKE_CURRENT_LIST_DIR}/BlockDeviceToMemoryTechnologyDevice.cpp
		${CMAKE_CURRENT_LIST_DIR}/BufferingBlockDevice.cpp
		${CMAKE_CURRENT_LIST_DIR}/SdCard.cpp
		${CMAKE_CURRENT_LIST_DIR}/SdCardSpiBased.cpp
		${CMAKE_CURRENT_LIST_DIR}/SpiEeprom.cpp
		${CMAKE_CURRENT_LIST_DIR}/SynchronousSdMmcCardLowLevel.cpp)
