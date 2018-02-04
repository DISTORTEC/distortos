#
# file: distortos-sources.cmake
#
# author: Copyright (C) 2018 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
#

target_sources(distortos PRIVATE
		${CMAKE_CURRENT_LIST_DIR}/fatalErrorHandler.cpp)

include(${CMAKE_CURRENT_LIST_DIR}/architecture/distortos-sources.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/C-API/distortos-sources.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/chip/distortos-sources.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/clocks/distortos-sources.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/devices/distortos-sources.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/gcc/distortos-sources.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/memory/distortos-sources.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/newlib/distortos-sources.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/scheduler/distortos-sources.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/synchronization/distortos-sources.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/threads/distortos-sources.cmake)
