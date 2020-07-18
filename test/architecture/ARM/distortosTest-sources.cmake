#
# file: distortosTest-sources.cmake
#
# author: Copyright (C) 2018-2019 Kamil Szczygiel https://distortec.com https://freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
#

if(DISTORTOS_ARCHITECTURE_ARM)

	include(${CMAKE_CURRENT_LIST_DIR}/ARMv6-M/distortosTest-sources.cmake)
	include(${CMAKE_CURRENT_LIST_DIR}/ARMv7-M/distortosTest-sources.cmake)

endif()
