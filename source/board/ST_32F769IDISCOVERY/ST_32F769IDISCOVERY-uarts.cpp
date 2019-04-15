/**
 * \file
 * \brief Definitions of low-level UART drivers for USARTv2 in ST,32F769IDISCOVERY (ST,STM32F769NI chip)
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

#ifdef DISTORTOS_CHIP_STM32_USARTV2_USART3_ENABLE

ChipUartLowLevel usart3 {ChipUartLowLevel::usart3Parameters};

#endif	// def DISTORTOS_CHIP_STM32_USARTV2_USART3_ENABLE

#ifdef DISTORTOS_CHIP_STM32_USARTV2_UART4_ENABLE

ChipUartLowLevel uart4 {ChipUartLowLevel::uart4Parameters};

#endif	// def DISTORTOS_CHIP_STM32_USARTV2_UART4_ENABLE

#ifdef DISTORTOS_CHIP_STM32_USARTV2_UART5_ENABLE

ChipUartLowLevel uart5 {ChipUartLowLevel::uart5Parameters};

#endif	// def DISTORTOS_CHIP_STM32_USARTV2_UART5_ENABLE

#ifdef DISTORTOS_CHIP_STM32_USARTV2_USART6_ENABLE

ChipUartLowLevel usart6 {ChipUartLowLevel::usart6Parameters};

#endif	// def DISTORTOS_CHIP_STM32_USARTV2_USART6_ENABLE

#ifdef DISTORTOS_CHIP_STM32_USARTV2_UART7_ENABLE

ChipUartLowLevel uart7 {ChipUartLowLevel::uart7Parameters};

#endif	// def DISTORTOS_CHIP_STM32_USARTV2_UART7_ENABLE

#ifdef DISTORTOS_CHIP_STM32_USARTV2_UART8_ENABLE

ChipUartLowLevel uart8 {ChipUartLowLevel::uart8Parameters};

#endif	// def DISTORTOS_CHIP_STM32_USARTV2_UART8_ENABLE

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

#ifdef DISTORTOS_CHIP_STM32_USARTV2_USART3_ENABLE

/**
 * \brief USART3 interrupt handler
 */

extern "C" void USART3_IRQHandler()
{
	usart3.interruptHandler();
}

#endif	// def DISTORTOS_CHIP_STM32_USARTV2_USART3_ENABLE

#ifdef DISTORTOS_CHIP_STM32_USARTV2_UART4_ENABLE

/**
 * \brief UART4 interrupt handler
 */

extern "C" void UART4_IRQHandler()
{
	uart4.interruptHandler();
}

#endif	// def DISTORTOS_CHIP_STM32_USARTV2_UART4_ENABLE

#ifdef DISTORTOS_CHIP_STM32_USARTV2_UART5_ENABLE

/**
 * \brief UART5 interrupt handler
 */

extern "C" void UART5_IRQHandler()
{
	uart5.interruptHandler();
}

#endif	// def DISTORTOS_CHIP_STM32_USARTV2_UART5_ENABLE

#ifdef DISTORTOS_CHIP_STM32_USARTV2_USART6_ENABLE

/**
 * \brief USART6 interrupt handler
 */

extern "C" void USART6_IRQHandler()
{
	usart6.interruptHandler();
}

#endif	// def DISTORTOS_CHIP_STM32_USARTV2_USART6_ENABLE

#ifdef DISTORTOS_CHIP_STM32_USARTV2_UART7_ENABLE

/**
 * \brief UART7 interrupt handler
 */

extern "C" void UART7_IRQHandler()
{
	uart7.interruptHandler();
}

#endif	// def DISTORTOS_CHIP_STM32_USARTV2_UART7_ENABLE

#ifdef DISTORTOS_CHIP_STM32_USARTV2_UART8_ENABLE

/**
 * \brief UART8 interrupt handler
 */

extern "C" void UART8_IRQHandler()
{
	uart8.interruptHandler();
}

#endif	// def DISTORTOS_CHIP_STM32_USARTV2_UART8_ENABLE

}	// namespace chip

}	// namespace distortos
