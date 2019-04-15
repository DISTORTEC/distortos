#
# file: cmake/10-leds.cmake
#
# author: Copyright (C) 2018-2019 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
#
# Automatically generated file - do not edit!
#

distortosSetConfiguration(BOOLEAN
		distortos_leds
		ON
		HELP "Enable leds"
		OUTPUT_NAME DISTORTOS_BOARD_LEDS_ENABLE)

if(distortos_leds)

	distortosSetConfiguration(BOOLEAN
			distortos_leds_Ld2
			ON
			HELP "Enable Ld2 (Green)"
			OUTPUT_NAME DISTORTOS_BOARD_LEDS_LD2_ENABLE)

	if(distortos_leds_Ld2)
		list(APPEND STM32_GPIO_V2_GPIOA_DEPENDENTS "leds Ld2")
	endif()

endif(distortos_leds)
