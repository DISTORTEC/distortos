/**
 * \file
 * \brief Declarations of low-level UART drivers for USARTv2 in ST,NUCLEO-F091RC (ST,STM32F091RC chip)
 *
 * \author Copyright (C) 2016-2019 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \warning
 * Automatically generated file - do not edit!
 */

#ifndef SOURCE_BOARD_ST_NUCLEO_F091RC_INCLUDE_DISTORTOS_CHIP_UARTS_HPP_
#define SOURCE_BOARD_ST_NUCLEO_F091RC_INCLUDE_DISTORTOS_CHIP_UARTS_HPP_

#include "distortos/distortosConfiguration.h"

namespace distortos
{

namespace chip
{

class ChipUartLowLevel;

/*---------------------------------------------------------------------------------------------------------------------+
| global objects
+---------------------------------------------------------------------------------------------------------------------*/

#ifdef DISTORTOS_CHIP_STM32_USARTV2_USART1_ENABLE

/// UART low-level driver for USART1
extern ChipUartLowLevel usart1;

#endif	// def DISTORTOS_CHIP_STM32_USARTV2_USART1_ENABLE

#ifdef DISTORTOS_CHIP_STM32_USARTV2_USART2_ENABLE

/// UART low-level driver for USART2
extern ChipUartLowLevel usart2;

#endif	// def DISTORTOS_CHIP_STM32_USARTV2_USART2_ENABLE

#ifdef DISTORTOS_CHIP_STM32_USARTV2_USART3_ENABLE

/// UART low-level driver for USART3
extern ChipUartLowLevel usart3;

#endif	// def DISTORTOS_CHIP_STM32_USARTV2_USART3_ENABLE

#ifdef DISTORTOS_CHIP_STM32_USARTV2_USART4_ENABLE

/// UART low-level driver for USART4
extern ChipUartLowLevel usart4;

#endif	// def DISTORTOS_CHIP_STM32_USARTV2_USART4_ENABLE

#ifdef DISTORTOS_CHIP_STM32_USARTV2_USART5_ENABLE

/// UART low-level driver for USART5
extern ChipUartLowLevel usart5;

#endif	// def DISTORTOS_CHIP_STM32_USARTV2_USART5_ENABLE

#ifdef DISTORTOS_CHIP_STM32_USARTV2_USART6_ENABLE

/// UART low-level driver for USART6
extern ChipUartLowLevel usart6;

#endif	// def DISTORTOS_CHIP_STM32_USARTV2_USART6_ENABLE

#ifdef DISTORTOS_CHIP_STM32_USARTV2_USART7_ENABLE

/// UART low-level driver for USART7
extern ChipUartLowLevel usart7;

#endif	// def DISTORTOS_CHIP_STM32_USARTV2_USART7_ENABLE

#ifdef DISTORTOS_CHIP_STM32_USARTV2_USART8_ENABLE

/// UART low-level driver for USART8
extern ChipUartLowLevel usart8;

#endif	// def DISTORTOS_CHIP_STM32_USARTV2_USART8_ENABLE

}	// namespace chip

}	// namespace distortos

#endif	// SOURCE_BOARD_ST_NUCLEO_F091RC_INCLUDE_DISTORTOS_CHIP_UARTS_HPP_
