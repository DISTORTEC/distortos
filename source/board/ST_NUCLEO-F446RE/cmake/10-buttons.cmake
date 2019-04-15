#
# file: cmake/10-buttons.cmake
#
# author: Copyright (C) 2018-2019 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
#
# Automatically generated file - do not edit!
#

distortosSetConfiguration(BOOLEAN
		distortos_buttons
		ON
		HELP "Enable buttons"
		OUTPUT_NAME DISTORTOS_BOARD_BUTTONS_ENABLE)

if(distortos_buttons)

	distortosSetConfiguration(BOOLEAN
			distortos_buttons_B1
			ON
			HELP "Enable B1 (User)"
			OUTPUT_NAME DISTORTOS_BOARD_BUTTONS_B1_ENABLE)

	if(distortos_buttons_B1)
		list(APPEND STM32_GPIO_V2_GPIOC_DEPENDENTS "buttons B1")
	endif()

endif(distortos_buttons)
