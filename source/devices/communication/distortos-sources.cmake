#
# file: distortos-sources.cmake
#
# author: Copyright (C) 2018 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
#

target_sources(distortos PRIVATE
		${CMAKE_CURRENT_LIST_DIR}/Rs485.cpp
		${CMAKE_CURRENT_LIST_DIR}/SerialPort.cpp
		${CMAKE_CURRENT_LIST_DIR}/SpiDevice.cpp
		${CMAKE_CURRENT_LIST_DIR}/SpiDeviceProxy.cpp
		${CMAKE_CURRENT_LIST_DIR}/SpiDeviceSelectGuard.cpp
		${CMAKE_CURRENT_LIST_DIR}/SpiMaster.cpp
		${CMAKE_CURRENT_LIST_DIR}/SpiMasterProxy.cpp)
