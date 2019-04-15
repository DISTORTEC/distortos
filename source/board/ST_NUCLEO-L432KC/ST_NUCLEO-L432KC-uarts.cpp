/**
 * \file
 * \brief Definitions of low-level UART drivers for USARTv2 in ST,NUCLEO-L432KC (ST,STM32L432KC chip)
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

#include "distortos/chip/uarts.hpp"

#include "distortos/chip/ChipUartLowLevel.hpp"

namespace distortos
{

namespace chip
{

/*---------------------------------------------------------------------------------------------------------------------+
| global objects
+---------------------------------------------------------------------------------------------------------------------*/

#ifdef DISTORTOS_CHIP_STM32_USARTV2_USART1_ENABLE

ChipUartLowLevel usart1 {ChipUartLowLevel::usart1Parameters};

#endif	// def DISTORTOS_CHIP_STM32_USARTV2_USART1_ENABLE

#ifdef DISTORTOS_CHIP_STM32_USARTV2_USART2_ENABLE

ChipUartLowLevel usart2 {ChipUartLowLevel::usart2Parameters};

#endif	// def DISTORTOS_CHIP_STM32_USARTV2_USART2_ENABLE

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

#ifdef DISTORTOS_CHIP_STM32_USARTV2_USART1_ENABLE

/**
 * \brief USART1 interrupt handler
 */

extern "C" void USART1_IRQHandler()
{
	usart1.interruptHandler();
}

#endif	// def DISTORTOS_CHIP_STM32_USARTV2_USART1_ENABLE

#ifdef DISTORTOS_CHIP_STM32_USARTV2_USART2_ENABLE

/**
 * \brief USART2 interrupt handler
 */

extern "C" void USART2_IRQHandler()
{
	usart2.interruptHandler();
}

#endif	// def DISTORTOS_CHIP_STM32_USARTV2_USART2_ENABLE

}	// namespace chip

}	// namespace distortos
