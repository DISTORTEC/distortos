/**
 * \file
 * \brief chip::usartLowLevelInitialization() definition for USARTv1 in STM32
 *
 * \author Copyright (C) 2016 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "STM32-USARTv1-usartLowLevelInitialization.hpp"

#include "distortos/chip/CMSIS-proxy.h"

namespace distortos
{

namespace chip
{

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

void usartLowLevelInitialization()
{
#ifdef CONFIG_CHIP_STM32_USARTV1_USART1_ENABLE
	NVIC_SetPriority(USART1_IRQn, CONFIG_ARCHITECTURE_ARMV7_M_KERNEL_BASEPRI);
	NVIC_EnableIRQ(USART1_IRQn);
#endif	// def CONFIG_CHIP_STM32_USARTV1_USART1_ENABLE
#ifdef CONFIG_CHIP_STM32_USARTV1_USART2_ENABLE
	NVIC_SetPriority(USART2_IRQn, CONFIG_ARCHITECTURE_ARMV7_M_KERNEL_BASEPRI);
	NVIC_EnableIRQ(USART2_IRQn);
#endif	// def CONFIG_CHIP_STM32_USARTV1_USART2_ENABLE
#ifdef CONFIG_CHIP_STM32_USARTV1_USART3_ENABLE
	NVIC_SetPriority(USART3_IRQn, CONFIG_ARCHITECTURE_ARMV7_M_KERNEL_BASEPRI);
	NVIC_EnableIRQ(USART3_IRQn);
#endif	// def CONFIG_CHIP_STM32_USARTV1_USART3_ENABLE
#ifdef CONFIG_CHIP_STM32_USARTV1_UART4_ENABLE
	NVIC_SetPriority(UART4_IRQn, CONFIG_ARCHITECTURE_ARMV7_M_KERNEL_BASEPRI);
	NVIC_EnableIRQ(UART4_IRQn);
#endif	// def CONFIG_CHIP_STM32_USARTV1_UART4_ENABLE
#ifdef CONFIG_CHIP_STM32_USARTV1_UART5_ENABLE
	NVIC_SetPriority(UART5_IRQn, CONFIG_ARCHITECTURE_ARMV7_M_KERNEL_BASEPRI);
	NVIC_EnableIRQ(UART5_IRQn);
#endif	// def CONFIG_CHIP_STM32_USARTV1_UART5_ENABLE
#ifdef CONFIG_CHIP_STM32_USARTV1_USART6_ENABLE
	NVIC_SetPriority(USART6_IRQn, CONFIG_ARCHITECTURE_ARMV7_M_KERNEL_BASEPRI);
	NVIC_EnableIRQ(USART6_IRQn);
#endif	// def CONFIG_CHIP_STM32_USARTV1_USART6_ENABLE
#ifdef CONFIG_CHIP_STM32_USARTV1_UART7_ENABLE
	NVIC_SetPriority(UART7_IRQn, CONFIG_ARCHITECTURE_ARMV7_M_KERNEL_BASEPRI);
	NVIC_EnableIRQ(UART7_IRQn);
#endif	// def CONFIG_CHIP_STM32_USARTV1_UART7_ENABLE
#ifdef CONFIG_CHIP_STM32_USARTV1_UART8_ENABLE
	NVIC_SetPriority(UART8_IRQn, CONFIG_ARCHITECTURE_ARMV7_M_KERNEL_BASEPRI);
	NVIC_EnableIRQ(UART8_IRQn);
#endif	// def CONFIG_CHIP_STM32_USARTV1_UART8_ENABLE
}

}	// namespace chip

}	// namespace distortos
