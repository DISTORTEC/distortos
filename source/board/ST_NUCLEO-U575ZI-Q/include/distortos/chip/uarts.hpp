/**
 * \file
 * \brief Declarations of low-level UART drivers for USARTv2 in ST,NUCLEO-U575ZI-Q (ST,STM32U575ZI chip)
 *
 * \author Copyright (C) 2016-2019 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * \warning
 * Automatically generated file - do not edit!
 */

#ifndef SOURCE_BOARD_ST_NUCLEO_U575ZI_Q_INCLUDE_DISTORTOS_CHIP_UARTS_HPP_
#define SOURCE_BOARD_ST_NUCLEO_U575ZI_Q_INCLUDE_DISTORTOS_CHIP_UARTS_HPP_

#include "distortos/distortosConfiguration.h"

namespace distortos
{

namespace chip
{

class ChipUartLowLevel;

/*---------------------------------------------------------------------------------------------------------------------+
| global objects
+---------------------------------------------------------------------------------------------------------------------*/

#ifdef DISTORTOS_CHIP_LPUART1_ENABLE

/// UART low-level driver for LPUART1
extern ChipUartLowLevel lpuart1;

#endif	// def DISTORTOS_CHIP_LPUART1_ENABLE

#ifdef DISTORTOS_CHIP_USART1_ENABLE

/// UART low-level driver for USART1
extern ChipUartLowLevel usart1;

#endif	// def DISTORTOS_CHIP_USART1_ENABLE

#ifdef DISTORTOS_CHIP_USART2_ENABLE

/// UART low-level driver for USART2
extern ChipUartLowLevel usart2;

#endif	// def DISTORTOS_CHIP_USART2_ENABLE

#ifdef DISTORTOS_CHIP_USART3_ENABLE

/// UART low-level driver for USART3
extern ChipUartLowLevel usart3;

#endif	// def DISTORTOS_CHIP_USART3_ENABLE

#ifdef DISTORTOS_CHIP_UART4_ENABLE

/// UART low-level driver for UART4
extern ChipUartLowLevel uart4;

#endif	// def DISTORTOS_CHIP_UART4_ENABLE

#ifdef DISTORTOS_CHIP_UART5_ENABLE

/// UART low-level driver for UART5
extern ChipUartLowLevel uart5;

#endif	// def DISTORTOS_CHIP_UART5_ENABLE

}	// namespace chip

}	// namespace distortos

#endif	// SOURCE_BOARD_ST_NUCLEO_U575ZI_Q_INCLUDE_DISTORTOS_CHIP_UARTS_HPP_
