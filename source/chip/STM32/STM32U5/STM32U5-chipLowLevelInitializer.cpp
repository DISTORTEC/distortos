/**
 * \file
 * \brief Low-level chip initializer for STM32U5
 *
 * \author Copyright (C) 2024 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "distortos/chip/CMSIS-proxy.h"

#include "distortos/BIND_LOW_LEVEL_INITIALIZER.h"

namespace distortos
{

namespace chip
{

namespace
{

/*---------------------------------------------------------------------------------------------------------------------+
| local functions
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief Low-level chip initializer for STM32U5
 *
 * This function is called before constructors for global and static objects via BIND_LOW_LEVEL_INITIALIZER().
 */

void chipLowLevelInitializer()
{
#ifdef DISTORTOS_CHIP_GPIOG_ENABLE
	RCC->AHB3ENR |= RCC_AHB3ENR_PWREN;
	PWR->SVMCR |= PWR_SVMCR_IO2SV;
#endif	// def DISTORTOS_CHIP_GPIOG_ENABLE

	RCC->AHB2ENR1 |=
#ifdef DISTORTOS_CHIP_GPIOA_ENABLE
			RCC_AHB2ENR1_GPIOAEN |
#endif	// def DISTORTOS_CHIP_GPIOA_ENABLE
#ifdef DISTORTOS_CHIP_GPIOB_ENABLE
			RCC_AHB2ENR1_GPIOBEN |
#endif	// def DISTORTOS_CHIP_GPIOB_ENABLE
#ifdef DISTORTOS_CHIP_GPIOC_ENABLE
			RCC_AHB2ENR1_GPIOCEN |
#endif	// def DISTORTOS_CHIP_GPIOC_ENABLE
#ifdef DISTORTOS_CHIP_GPIOD_ENABLE
			RCC_AHB2ENR1_GPIODEN |
#endif	// def DISTORTOS_CHIP_GPIOD_ENABLE
#ifdef DISTORTOS_CHIP_GPIOE_ENABLE
			RCC_AHB2ENR1_GPIOEEN |
#endif	// def DISTORTOS_CHIP_GPIOE_ENABLE
#ifdef DISTORTOS_CHIP_GPIOF_ENABLE
			RCC_AHB2ENR1_GPIOFEN |
#endif	// def DISTORTOS_CHIP_GPIOF_ENABLE
#ifdef DISTORTOS_CHIP_GPIOG_ENABLE
			RCC_AHB2ENR1_GPIOGEN |
#endif	// def DISTORTOS_CHIP_GPIOG_ENABLE
#ifdef DISTORTOS_CHIP_GPIOH_ENABLE
			RCC_AHB2ENR1_GPIOHEN |
#endif	// def DISTORTOS_CHIP_GPIOH_ENABLE
#ifdef DISTORTOS_CHIP_GPIOI_ENABLE
			RCC_AHB2ENR1_GPIOIEN |
#endif	// def DISTORTOS_CHIP_GPIOI_ENABLE
#ifdef DISTORTOS_CHIP_GPIOJ_ENABLE
			RCC_AHB2ENR1_GPIOJEN |
#endif	// def DISTORTOS_CHIP_GPIOJ_ENABLE
			0;
}

BIND_LOW_LEVEL_INITIALIZER(40, chipLowLevelInitializer);

}	// namespace

}	// namespace chip

}	// namespace distortos
