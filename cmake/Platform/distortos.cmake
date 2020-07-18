#
# file: distortos.cmake
#
# author: Copyright (C) 2018 Kamil Szczygiel https://distortec.com https://freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
#

if(CMAKE_PLATFORM_DISTORTOS_CMAKE_)
	return()
endif()
set(CMAKE_PLATFORM_DISTORTOS_CMAKE_ 1)

set(CMAKE_EXECUTABLE_SUFFIX .elf)
set(UNIX True)
