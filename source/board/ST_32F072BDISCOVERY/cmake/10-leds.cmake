#
# file: cmake/10-leds.cmake
#
# author: Copyright (C) 2018-2019 Kamil Szczygiel http://www.distortec.com https://freddiechopin.info
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
			distortos_leds_Ld3
			ON
			HELP "Enable Ld3 (Orange)"
			OUTPUT_NAME DISTORTOS_BOARD_LEDS_LD3_ENABLE)

	if(distortos_leds_Ld3)
		list(APPEND DISTORTOS_CHIP_GPIOC_DEPENDENTS "leds Ld3")
	endif()

	distortosSetConfiguration(BOOLEAN
			distortos_leds_Ld4
			ON
			HELP "Enable Ld4 (Green)"
			OUTPUT_NAME DISTORTOS_BOARD_LEDS_LD4_ENABLE)

	if(distortos_leds_Ld4)
		list(APPEND DISTORTOS_CHIP_GPIOC_DEPENDENTS "leds Ld4")
	endif()

	distortosSetConfiguration(BOOLEAN
			distortos_leds_Ld5
			ON
			HELP "Enable Ld5 (Red)"
			OUTPUT_NAME DISTORTOS_BOARD_LEDS_LD5_ENABLE)

	if(distortos_leds_Ld5)
		list(APPEND DISTORTOS_CHIP_GPIOC_DEPENDENTS "leds Ld5")
	endif()

	distortosSetConfiguration(BOOLEAN
			distortos_leds_Ld6
			ON
			HELP "Enable Ld6 (Blue)"
			OUTPUT_NAME DISTORTOS_BOARD_LEDS_LD6_ENABLE)

	if(distortos_leds_Ld6)
		list(APPEND DISTORTOS_CHIP_GPIOC_DEPENDENTS "leds Ld6")
	endif()

endif(distortos_leds)
