#
# file: cmake/60-STM32-GPIOv2.cmake
#
# author: Copyright (C) 2018-2019 Kamil Szczygiel https://distortec.com https://freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
#
# Automatically generated file - do not edit!
#

distortosSetFixedConfiguration(BOOLEAN
		DISTORTOS_CHIP_GPIO_HAS_4_AF_BITS
		ON)

distortosSetFixedConfiguration(BOOLEAN
		DISTORTOS_CHIP_GPIO_HAS_HIGH_SPEED
		ON)

distortosSetConfiguration(BOOLEAN
		distortos_Peripherals_GPIOA
		OFF
		DEPENDENTS ${DISTORTOS_CHIP_GPIOA_DEPENDENTS}
		HELP "Enable GPIOA."
		OUTPUT_NAME DISTORTOS_CHIP_GPIOA_ENABLE)

distortosSetConfiguration(BOOLEAN
		distortos_Peripherals_GPIOB
		OFF
		DEPENDENTS ${DISTORTOS_CHIP_GPIOB_DEPENDENTS}
		HELP "Enable GPIOB."
		OUTPUT_NAME DISTORTOS_CHIP_GPIOB_ENABLE)

distortosSetConfiguration(BOOLEAN
		distortos_Peripherals_GPIOC
		OFF
		DEPENDENTS ${DISTORTOS_CHIP_GPIOC_DEPENDENTS}
		HELP "Enable GPIOC."
		OUTPUT_NAME DISTORTOS_CHIP_GPIOC_ENABLE)

distortosSetConfiguration(BOOLEAN
		distortos_Peripherals_GPIOD
		OFF
		DEPENDENTS ${DISTORTOS_CHIP_GPIOD_DEPENDENTS}
		HELP "Enable GPIOD."
		OUTPUT_NAME DISTORTOS_CHIP_GPIOD_ENABLE)

distortosSetConfiguration(BOOLEAN
		distortos_Peripherals_GPIOE
		OFF
		DEPENDENTS ${DISTORTOS_CHIP_GPIOE_DEPENDENTS}
		HELP "Enable GPIOE."
		OUTPUT_NAME DISTORTOS_CHIP_GPIOE_ENABLE)

distortosSetConfiguration(BOOLEAN
		distortos_Peripherals_GPIOF
		OFF
		DEPENDENTS ${DISTORTOS_CHIP_GPIOF_DEPENDENTS}
		HELP "Enable GPIOF."
		OUTPUT_NAME DISTORTOS_CHIP_GPIOF_ENABLE)

distortosSetConfiguration(BOOLEAN
		distortos_Peripherals_GPIOG
		OFF
		DEPENDENTS ${DISTORTOS_CHIP_GPIOG_DEPENDENTS}
		HELP "Enable GPIOG."
		OUTPUT_NAME DISTORTOS_CHIP_GPIOG_ENABLE)

distortosSetConfiguration(BOOLEAN
		distortos_Peripherals_GPIOH
		OFF
		DEPENDENTS ${DISTORTOS_CHIP_GPIOH_DEPENDENTS}
		HELP "Enable GPIOH."
		OUTPUT_NAME DISTORTOS_CHIP_GPIOH_ENABLE)

include("${CMAKE_CURRENT_SOURCE_DIR}/source/chip/STM32/peripherals/GPIOv2/distortos-sources.cmake")
