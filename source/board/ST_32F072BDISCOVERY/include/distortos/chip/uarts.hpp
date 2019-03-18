/**
 * \file
 * \brief Declarations of low-level UART drivers for USARTv2 in ST,32F072BDISCOVERY (ST,STM32F072RB chip)
 *
 * \author Copyright (C) 2016-2018 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \warning
 * Automatically generated file - do not edit!
 */

#ifndef SOURCE_BOARD_ST_32F072BDISCOVERY_INCLUDE_DISTORTOS_CHIP_UARTS_HPP_
#define SOURCE_BOARD_ST_32F072BDISCOVERY_INCLUDE_DISTORTOS_CHIP_UARTS_HPP_

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

#ifdef CONFIG_CHIP_STM32_USARTV2_USART4_ENABLE

/// UART low-level driver for USART4
extern ChipUartLowLevel usart4;

#endif	// def CONFIG_CHIP_STM32_USARTV2_USART4_ENABLE

}	// namespace chip

}	// namespace distortos

#endif	// SOURCE_BOARD_ST_32F072BDISCOVERY_INCLUDE_DISTORTOS_CHIP_UARTS_HPP_
