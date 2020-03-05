/**
 * \file
 * \brief Definitions of low-level UART drivers for USARTv1 in ST,STM32F4DISCOVERY (ST,STM32F407VG chip)
 *
 * \author Copyright (C) 2016-2020 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
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

#ifdef DISTORTOS_CHIP_USART1_ENABLE

/*---------------------------------------------------------------------------------------------------------------------+
| USART1
+---------------------------------------------------------------------------------------------------------------------*/

ChipUartLowLevel usart1 {ChipUartLowLevel::usart1Parameters};

/**
 * \brief USART1 interrupt handler
 */

extern "C" void USART1_IRQHandler()
{
	usart1.interruptHandler();
}

#endif	// def DISTORTOS_CHIP_USART1_ENABLE

#ifdef DISTORTOS_CHIP_USART2_ENABLE

/*---------------------------------------------------------------------------------------------------------------------+
| USART2
+---------------------------------------------------------------------------------------------------------------------*/

ChipUartLowLevel usart2 {ChipUartLowLevel::usart2Parameters};

/**
 * \brief USART2 interrupt handler
 */

extern "C" void USART2_IRQHandler()
{
	usart2.interruptHandler();
}

#endif	// def DISTORTOS_CHIP_USART2_ENABLE

#ifdef DISTORTOS_CHIP_USART3_ENABLE

/*---------------------------------------------------------------------------------------------------------------------+
| USART3
+---------------------------------------------------------------------------------------------------------------------*/

ChipUartLowLevel usart3 {ChipUartLowLevel::usart3Parameters};

/**
 * \brief USART3 interrupt handler
 */

extern "C" void USART3_IRQHandler()
{
	usart3.interruptHandler();
}

#endif	// def DISTORTOS_CHIP_USART3_ENABLE

#ifdef DISTORTOS_CHIP_UART4_ENABLE

/*---------------------------------------------------------------------------------------------------------------------+
| UART4
+---------------------------------------------------------------------------------------------------------------------*/

ChipUartLowLevel uart4 {ChipUartLowLevel::uart4Parameters};

/**
 * \brief UART4 interrupt handler
 */

extern "C" void UART4_IRQHandler()
{
	uart4.interruptHandler();
}

#endif	// def DISTORTOS_CHIP_UART4_ENABLE

#ifdef DISTORTOS_CHIP_UART5_ENABLE

/*---------------------------------------------------------------------------------------------------------------------+
| UART5
+---------------------------------------------------------------------------------------------------------------------*/

ChipUartLowLevel uart5 {ChipUartLowLevel::uart5Parameters};

/**
 * \brief UART5 interrupt handler
 */

extern "C" void UART5_IRQHandler()
{
	uart5.interruptHandler();
}

#endif	// def DISTORTOS_CHIP_UART5_ENABLE

#ifdef DISTORTOS_CHIP_USART6_ENABLE

/*---------------------------------------------------------------------------------------------------------------------+
| USART6
+---------------------------------------------------------------------------------------------------------------------*/

ChipUartLowLevel usart6 {ChipUartLowLevel::usart6Parameters};

/**
 * \brief USART6 interrupt handler
 */

extern "C" void USART6_IRQHandler()
{
	usart6.interruptHandler();
}

#endif	// def DISTORTOS_CHIP_USART6_ENABLE

}	// namespace chip

}	// namespace distortos
