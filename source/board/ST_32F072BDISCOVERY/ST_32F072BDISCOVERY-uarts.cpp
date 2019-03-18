/**
 * \file
 * \brief Definitions of low-level UART drivers for USARTv2 in ST,32F072BDISCOVERY (ST,STM32F072RB chip)
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

#include "distortos/chip/uarts.hpp"

#include "distortos/chip/ChipUartLowLevel.hpp"

namespace distortos
{

namespace chip
{

/*---------------------------------------------------------------------------------------------------------------------+
| global objects
+---------------------------------------------------------------------------------------------------------------------*/

#ifdef CONFIG_CHIP_STM32_USARTV2_USART1_ENABLE

ChipUartLowLevel usart1 {ChipUartLowLevel::usart1Parameters};

#endif	// def CONFIG_CHIP_STM32_USARTV2_USART1_ENABLE

#ifdef CONFIG_CHIP_STM32_USARTV2_USART2_ENABLE

ChipUartLowLevel usart2 {ChipUartLowLevel::usart2Parameters};

#endif	// def CONFIG_CHIP_STM32_USARTV2_USART2_ENABLE

#ifdef CONFIG_CHIP_STM32_USARTV2_USART3_ENABLE

ChipUartLowLevel usart3 {ChipUartLowLevel::usart3Parameters};

#endif	// def CONFIG_CHIP_STM32_USARTV2_USART3_ENABLE

#ifdef CONFIG_CHIP_STM32_USARTV2_USART4_ENABLE

ChipUartLowLevel usart4 {ChipUartLowLevel::usart4Parameters};

#endif	// def CONFIG_CHIP_STM32_USARTV2_USART4_ENABLE

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

#ifdef CONFIG_CHIP_STM32_USARTV2_USART1_ENABLE

/**
 * \brief USART1 interrupt handler
 */

extern "C" void USART1_IRQHandler()
{
	usart1.interruptHandler();
}

#endif	// def CONFIG_CHIP_STM32_USARTV2_USART1_ENABLE

#ifdef CONFIG_CHIP_STM32_USARTV2_USART2_ENABLE

/**
 * \brief USART2 interrupt handler
 */

extern "C" void USART2_IRQHandler()
{
	usart2.interruptHandler();
}

#endif	// def CONFIG_CHIP_STM32_USARTV2_USART2_ENABLE

#ifdef CONFIG_CHIP_STM32_USARTV2_USART3_ENABLE

/**
 * \brief USART3 interrupt handler
 */

extern "C" void USART3_IRQHandler()
{
	usart3.interruptHandler();
}

#endif	// def CONFIG_CHIP_STM32_USARTV2_USART3_ENABLE

#ifdef CONFIG_CHIP_STM32_USARTV2_USART4_ENABLE

/**
 * \brief USART4 interrupt handler
 */

extern "C" void USART4_IRQHandler()
{
	usart4.interruptHandler();
}

#endif	// def CONFIG_CHIP_STM32_USARTV2_USART4_ENABLE

}	// namespace chip

}	// namespace distortos
