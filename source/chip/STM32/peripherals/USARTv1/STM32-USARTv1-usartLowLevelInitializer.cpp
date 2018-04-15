/**
 * \file
 * \brief Low-level peripheral initializer for USARTv1 in STM32
 *
 * \author Copyright (C) 2016-2018 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
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
| local objects
+---------------------------------------------------------------------------------------------------------------------*/

/// priority of USART interrupts
#if defined(CONFIG_ARCHITECTURE_ARMV7_M_KERNEL_BASEPRI)
constexpr uint8_t interruptPriority {CONFIG_ARCHITECTURE_ARMV7_M_KERNEL_BASEPRI};
#else	// !defined(CONFIG_ARCHITECTURE_ARMV7_M_KERNEL_BASEPRI)
constexpr uint8_t interruptPriority {};
#endif	// !defined(CONFIG_ARCHITECTURE_ARMV7_M_KERNEL_BASEPRI)

/*---------------------------------------------------------------------------------------------------------------------+
| local functions
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief Low-level peripheral initializer for USARTv1 in STM32
 *
 * This function is called before constructors for global and static objects via BIND_LOW_LEVEL_INITIALIZER().
 */

void usartLowLevelInitializer()
{
#ifdef CONFIG_CHIP_STM32_USARTV1_USART1_ENABLE
	NVIC_SetPriority(USART1_IRQn, interruptPriority);
	NVIC_EnableIRQ(USART1_IRQn);
#endif	// def CONFIG_CHIP_STM32_USARTV1_USART1_ENABLE
#ifdef CONFIG_CHIP_STM32_USARTV1_USART2_ENABLE
	NVIC_SetPriority(USART2_IRQn, interruptPriority);
	NVIC_EnableIRQ(USART2_IRQn);
#endif	// def CONFIG_CHIP_STM32_USARTV1_USART2_ENABLE
#ifdef CONFIG_CHIP_STM32_USARTV1_USART3_ENABLE
	NVIC_SetPriority(USART3_IRQn, interruptPriority);
	NVIC_EnableIRQ(USART3_IRQn);
#endif	// def CONFIG_CHIP_STM32_USARTV1_USART3_ENABLE
#ifdef CONFIG_CHIP_STM32_USARTV1_UART4_ENABLE
	NVIC_SetPriority(UART4_IRQn, interruptPriority);
	NVIC_EnableIRQ(UART4_IRQn);
#endif	// def CONFIG_CHIP_STM32_USARTV1_UART4_ENABLE
#ifdef CONFIG_CHIP_STM32_USARTV1_UART5_ENABLE
	NVIC_SetPriority(UART5_IRQn, interruptPriority);
	NVIC_EnableIRQ(UART5_IRQn);
#endif	// def CONFIG_CHIP_STM32_USARTV1_UART5_ENABLE
#ifdef CONFIG_CHIP_STM32_USARTV1_USART6_ENABLE
	NVIC_SetPriority(USART6_IRQn, interruptPriority);
	NVIC_EnableIRQ(USART6_IRQn);
#endif	// def CONFIG_CHIP_STM32_USARTV1_USART6_ENABLE
#ifdef CONFIG_CHIP_STM32_USARTV1_UART7_ENABLE
	NVIC_SetPriority(UART7_IRQn, interruptPriority);
	NVIC_EnableIRQ(UART7_IRQn);
#endif	// def CONFIG_CHIP_STM32_USARTV1_UART7_ENABLE
#ifdef CONFIG_CHIP_STM32_USARTV1_UART8_ENABLE
	NVIC_SetPriority(UART8_IRQn, interruptPriority);
	NVIC_EnableIRQ(UART8_IRQn);
#endif	// def CONFIG_CHIP_STM32_USARTV1_UART8_ENABLE
#ifdef CONFIG_CHIP_STM32_USARTV1_UART9_ENABLE
	NVIC_SetPriority(UART9_IRQn, interruptPriority);
	NVIC_EnableIRQ(UART9_IRQn);
#endif	// def CONFIG_CHIP_STM32_USARTV1_UART9_ENABLE
#ifdef CONFIG_CHIP_STM32_USARTV1_UART10_ENABLE
	NVIC_SetPriority(UART10_IRQn, interruptPriority);
	NVIC_EnableIRQ(UART10_IRQn);
#endif	// def CONFIG_CHIP_STM32_USARTV1_UART10_ENABLE
}

BIND_LOW_LEVEL_INITIALIZER(50, usartLowLevelInitializer);

}	// namespace

}	// namespace chip

}	// namespace distortos
