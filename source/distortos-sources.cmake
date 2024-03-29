#
# file: distortos-sources.cmake
#
# author: Copyright (C) 2018-2022 Kamil Szczygiel https://distortec.com https://freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
#

distortosMax(DISTORTOS_SDMMCCARD_BUFFER_ALIGNMENT 1 ${DISTORTOS_SDMMCCARD_BUFFER_ALIGNMENTS})
distortosSetFixedConfiguration(INTEGER
		DISTORTOS_SDMMCCARD_BUFFER_ALIGNMENT
		${DISTORTOS_SDMMCCARD_BUFFER_ALIGNMENT})
list(APPEND DISTORTOS_BLOCKDEVICE_BUFFER_ALIGNMENTS ${DISTORTOS_SDMMCCARD_BUFFER_ALIGNMENT})

distortosMax(DISTORTOS_SPIMASTER_BUFFER_ALIGNMENT 1 ${DISTORTOS_SPIMASTER_BUFFER_ALIGNMENTS})
distortosSetFixedConfiguration(INTEGER
		DISTORTOS_SPIMASTER_BUFFER_ALIGNMENT
		${DISTORTOS_SPIMASTER_BUFFER_ALIGNMENT})
list(APPEND DISTORTOS_BLOCKDEVICE_BUFFER_ALIGNMENTS ${DISTORTOS_SPIMASTER_BUFFER_ALIGNMENT})

distortosMax(DISTORTOS_UART_BUFFER_ALIGNMENT 1 ${DISTORTOS_UART_BUFFER_ALIGNMENTS})
distortosSetFixedConfiguration(INTEGER
		DISTORTOS_UART_BUFFER_ALIGNMENT
		${DISTORTOS_UART_BUFFER_ALIGNMENT})

distortosMax(DISTORTOS_BLOCKDEVICE_BUFFER_ALIGNMENT 1 ${DISTORTOS_BLOCKDEVICE_BUFFER_ALIGNMENTS})
distortosSetFixedConfiguration(INTEGER
		DISTORTOS_BLOCKDEVICE_BUFFER_ALIGNMENT
		${DISTORTOS_BLOCKDEVICE_BUFFER_ALIGNMENT})
list(APPEND DISTORTOS_MEMORYTECHNOLOGYDEVICE_BUFFER_ALIGNMENTS ${DISTORTOS_BLOCKDEVICE_BUFFER_ALIGNMENT})

distortosMax(DISTORTOS_MEMORYTECHNOLOGYDEVICE_BUFFER_ALIGNMENT 1 ${DISTORTOS_MEMORYTECHNOLOGYDEVICE_BUFFER_ALIGNMENTS})
distortosSetFixedConfiguration(INTEGER
		DISTORTOS_MEMORYTECHNOLOGYDEVICE_BUFFER_ALIGNMENT
		${DISTORTOS_MEMORYTECHNOLOGYDEVICE_BUFFER_ALIGNMENT})

target_sources(distortos PRIVATE
		${CMAKE_CURRENT_LIST_DIR}/assertHandler.cpp
		${CMAKE_CURRENT_LIST_DIR}/fatalErrorHandler.cpp)

doxygen(INPUT ${CMAKE_CURRENT_LIST_DIR}/fatalErrorHandler.cpp
		${CMAKE_CURRENT_LIST_DIR}/C-API
		${CMAKE_CURRENT_LIST_DIR}/clocks
		${CMAKE_CURRENT_LIST_DIR}/devices
		${CMAKE_CURRENT_LIST_DIR}/FileSystem
		${CMAKE_CURRENT_LIST_DIR}/gcc
		${CMAKE_CURRENT_LIST_DIR}/memory
		${CMAKE_CURRENT_LIST_DIR}/newlib
		${CMAKE_CURRENT_LIST_DIR}/scheduler
		${CMAKE_CURRENT_LIST_DIR}/synchronization
		${CMAKE_CURRENT_LIST_DIR}/threads)

include(${CMAKE_CURRENT_LIST_DIR}/C-API/distortos-sources.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/clocks/distortos-sources.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/devices/distortos-sources.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/FileSystem/distortos-sources.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/gcc/distortos-sources.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/memory/distortos-sources.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/newlib/distortos-sources.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/scheduler/distortos-sources.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/synchronization/distortos-sources.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/threads/distortos-sources.cmake)
