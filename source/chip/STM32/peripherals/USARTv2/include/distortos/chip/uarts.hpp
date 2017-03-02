/**
 * \file
 * \brief Declarations of low-level UART drivers for USARTv2 in STM32
 *
 * \author Copyright (C) 2016-2017 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef SOURCE_CHIP_STM32_PERIPHERALS_USARTV2_INCLUDE_DISTORTOS_CHIP_UARTS_HPP_
#define SOURCE_CHIP_STM32_PERIPHERALS_USARTV2_INCLUDE_DISTORTOS_CHIP_UARTS_HPP_

#include "distortos/distortosConfiguration.h"

namespace distortos
{

namespace chip
{

class ChipUartLowLevel;

/*---------------------------------------------------------------------------------------------------------------------+
| global objects
+---------------------------------------------------------------------------------------------------------------------*/

#ifdef CONFIG_CHIP_STM32_USARTV2_USART1_ENABLE

/// UART low-level driver for USART1
extern ChipUartLowLevel usart1;

#endif	// def CONFIG_CHIP_STM32_USARTV2_USART1_ENABLE

#ifdef CONFIG_CHIP_STM32_USARTV2_USART2_ENABLE

/// UART low-level driver for USART2
extern ChipUartLowLevel usart2;

#endif	// def CONFIG_CHIP_STM32_USARTV2_USART2_ENABLE

#ifdef CONFIG_CHIP_STM32_USARTV2_USART3_ENABLE

/// UART low-level driver for USART3
extern ChipUartLowLevel usart3;

#endif	// def CONFIG_CHIP_STM32_USARTV2_USART3_ENABLE

#ifdef CONFIG_CHIP_STM32_USARTV2_UART4_ENABLE

/// UART low-level driver for UART4
extern ChipUartLowLevel uart4;

#endif	// def CONFIG_CHIP_STM32_USARTV2_UART4_ENABLE

#ifdef CONFIG_CHIP_STM32_USARTV2_USART4_ENABLE

/// UART low-level driver for USART4
extern ChipUartLowLevel usart4;

#endif	// def CONFIG_CHIP_STM32_USARTV2_USART4_ENABLE

#ifdef CONFIG_CHIP_STM32_USARTV2_UART5_ENABLE

/// UART low-level driver for UART5
extern ChipUartLowLevel uart5;

#endif	// def CONFIG_CHIP_STM32_USARTV2_UART5_ENABLE

#ifdef CONFIG_CHIP_STM32_USARTV2_USART5_ENABLE

/// UART low-level driver for USART5
extern ChipUartLowLevel usart5;

#endif	// def CONFIG_CHIP_STM32_USARTV2_USART5_ENABLE

#ifdef CONFIG_CHIP_STM32_USARTV2_USART6_ENABLE

/// UART low-level driver for USART6
extern ChipUartLowLevel usart6;

#endif	// def CONFIG_CHIP_STM32_USARTV2_USART6_ENABLE

#ifdef CONFIG_CHIP_STM32_USARTV2_UART7_ENABLE

/// UART low-level driver for UART7
extern ChipUartLowLevel uart7;

#endif	// def CONFIG_CHIP_STM32_USARTV2_UART7_ENABLE

#ifdef CONFIG_CHIP_STM32_USARTV2_USART7_ENABLE

/// UART low-level driver for USART7
extern ChipUartLowLevel usart7;

#endif	// def CONFIG_CHIP_STM32_USARTV2_USART7_ENABLE

#ifdef CONFIG_CHIP_STM32_USARTV2_UART8_ENABLE

/// UART low-level driver for UART8
extern ChipUartLowLevel uart8;

#endif	// def CONFIG_CHIP_STM32_USARTV2_UART8_ENABLE

#ifdef CONFIG_CHIP_STM32_USARTV2_USART8_ENABLE

/// UART low-level driver for USART8
extern ChipUartLowLevel usart8;

#endif	// def CONFIG_CHIP_STM32_USARTV2_USART8_ENABLE

}	// namespace chip

}	// namespace distortos

#endif	// SOURCE_CHIP_STM32_PERIPHERALS_USARTV2_INCLUDE_DISTORTOS_CHIP_UARTS_HPP_
