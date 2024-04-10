#
# file: cmake/10-leds.cmake
#
# author: Copyright (C) 2018-2019 Kamil Szczygiel https://distortec.com https://freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
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
			distortos_leds_Ld4
			ON
			HELP "Enable Ld4 (Green, User)"
			OUTPUT_NAME DISTORTOS_BOARD_LEDS_LD4_ENABLE)

	if(distortos_leds_Ld4)
		list(APPEND DISTORTOS_CHIP_GPIOA_DEPENDENTS "leds Ld4")
	endif()

endif(distortos_leds)
