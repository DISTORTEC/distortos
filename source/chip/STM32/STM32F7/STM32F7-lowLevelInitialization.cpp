/**
 * \file
 * \brief chip::lowLevelInitialization() implementation for STM32F7
 *
 * \author Copyright (C) 2017 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/chip/lowLevelInitialization.hpp"

#include "distortos/chip/CMSIS-proxy.h"

namespace distortos
{

namespace chip
{

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

void lowLevelInitialization()
{
	RCC->AHB1ENR |=
#ifdef CONFIG_CHIP_STM32_GPIOV2_GPIOA_ENABLE
			RCC_AHB1ENR_GPIOAEN |
#endif	// def CONFIG_CHIP_STM32_GPIOV2_GPIOA_ENABLE
#ifdef CONFIG_CHIP_STM32_GPIOV2_GPIOB_ENABLE
			RCC_AHB1ENR_GPIOBEN |
#endif	// def CONFIG_CHIP_STM32_GPIOV2_GPIOB_ENABLE
#ifdef CONFIG_CHIP_STM32_GPIOV2_GPIOC_ENABLE
			RCC_AHB1ENR_GPIOCEN |
#endif	// def CONFIG_CHIP_STM32_GPIOV2_GPIOC_ENABLE
#ifdef CONFIG_CHIP_STM32_GPIOV2_GPIOD_ENABLE
			RCC_AHB1ENR_GPIODEN |
#endif	// def CONFIG_CHIP_STM32_GPIOV2_GPIOD_ENABLE
#ifdef CONFIG_CHIP_STM32_GPIOV2_GPIOE_ENABLE
			RCC_AHB1ENR_GPIOEEN |
#endif	// def CONFIG_CHIP_STM32_GPIOV2_GPIOE_ENABLE
#ifdef CONFIG_CHIP_STM32_GPIOV2_GPIOF_ENABLE
			RCC_AHB1ENR_GPIOFEN |
#endif	// def CONFIG_CHIP_STM32_GPIOV2_GPIOF_ENABLE
#ifdef CONFIG_CHIP_STM32_GPIOV2_GPIOG_ENABLE
			RCC_AHB1ENR_GPIOGEN |
#endif	// def CONFIG_CHIP_STM32_GPIOV2_GPIOG_ENABLE
#ifdef CONFIG_CHIP_STM32_GPIOV2_GPIOH_ENABLE
			RCC_AHB1ENR_GPIOHEN |
#endif	// def CONFIG_CHIP_STM32_GPIOV2_GPIOH_ENABLE
#ifdef CONFIG_CHIP_STM32_GPIOV2_GPIOI_ENABLE
			RCC_AHB1ENR_GPIOIEN |
#endif	// def CONFIG_CHIP_STM32_GPIOV2_GPIOI_ENABLE
#ifdef CONFIG_CHIP_STM32_GPIOV2_GPIOJ_ENABLE
			RCC_AHB1ENR_GPIOJEN |
#endif	// def CONFIG_CHIP_STM32_GPIOV2_GPIOJ_ENABLE
#ifdef CONFIG_CHIP_STM32_GPIOV2_GPIOK_ENABLE
			RCC_AHB1ENR_GPIOKEN |
#endif	// def CONFIG_CHIP_STM32_GPIOV2_GPIOK_ENABLE
			0;
}

}	// namespace chip

}	// namespace distortos
