/**
 * \file
 * \brief Definitions of low-level UART drivers for USARTv1 in STM32
 *
 * \author Copyright (C) 2016-2017 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/chip/uarts.hpp"

#include "distortos/chip/ChipUartLowLevel.hpp"

namespace distortos
{

namespace chip
{

/*---------------------------------------------------------------------------------------------------------------------+
| USART1 global objects and functions
+---------------------------------------------------------------------------------------------------------------------*/

#ifdef CONFIG_CHIP_STM32_USARTV1_USART1_ENABLE

ChipUartLowLevel usart1 {ChipUartLowLevel::usart1Parameters};

/**
 * \brief USART1 interrupt handler
 */

extern "C" void USART1_IRQHandler()
{
	usart1.interruptHandler();
}

#endif	// def CONFIG_CHIP_STM32_USARTV1_USART1_ENABLE

/*---------------------------------------------------------------------------------------------------------------------+
| USART2 global objects and functions
+---------------------------------------------------------------------------------------------------------------------*/

#ifdef CONFIG_CHIP_STM32_USARTV1_USART2_ENABLE

ChipUartLowLevel usart2 {ChipUartLowLevel::usart2Parameters};

/**
 * \brief USART2 interrupt handler
 */

extern "C" void USART2_IRQHandler()
{
	usart2.interruptHandler();
}

#endif	// def CONFIG_CHIP_STM32_USARTV1_USART2_ENABLE

/*---------------------------------------------------------------------------------------------------------------------+
| USART3 global objects and functions
+---------------------------------------------------------------------------------------------------------------------*/

#ifdef CONFIG_CHIP_STM32_USARTV1_USART3_ENABLE

ChipUartLowLevel usart3 {ChipUartLowLevel::usart3Parameters};

/**
 * \brief USART3 interrupt handler
 */

extern "C" void USART3_IRQHandler()
{
	usart3.interruptHandler();
}

#endif	// def CONFIG_CHIP_STM32_USARTV1_USART3_ENABLE

/*---------------------------------------------------------------------------------------------------------------------+
| UART4 global objects and functions
+---------------------------------------------------------------------------------------------------------------------*/

#ifdef CONFIG_CHIP_STM32_USARTV1_UART4_ENABLE

ChipUartLowLevel uart4 {ChipUartLowLevel::uart4Parameters};

/**
 * \brief UART4 interrupt handler
 */

extern "C" void UART4_IRQHandler()
{
	uart4.interruptHandler();
}

#endif	// def CONFIG_CHIP_STM32_USARTV1_UART4_ENABLE

/*---------------------------------------------------------------------------------------------------------------------+
| UART5 global objects and functions
+---------------------------------------------------------------------------------------------------------------------*/

#ifdef CONFIG_CHIP_STM32_USARTV1_UART5_ENABLE

ChipUartLowLevel uart5 {ChipUartLowLevel::uart5Parameters};

/**
 * \brief UART5 interrupt handler
 */

extern "C" void UART5_IRQHandler()
{
	uart5.interruptHandler();
}

#endif	// def CONFIG_CHIP_STM32_USARTV1_UART5_ENABLE

/*---------------------------------------------------------------------------------------------------------------------+
| USART6 global objects and functions
+---------------------------------------------------------------------------------------------------------------------*/

#ifdef CONFIG_CHIP_STM32_USARTV1_USART6_ENABLE

ChipUartLowLevel usart6 {ChipUartLowLevel::usart6Parameters};

/**
 * \brief USART6 interrupt handler
 */

extern "C" void USART6_IRQHandler()
{
	usart6.interruptHandler();
}

#endif	// def CONFIG_CHIP_STM32_USARTV1_USART6_ENABLE

/*---------------------------------------------------------------------------------------------------------------------+
| UART7 global objects and functions
+---------------------------------------------------------------------------------------------------------------------*/

#ifdef CONFIG_CHIP_STM32_USARTV1_UART7_ENABLE

ChipUartLowLevel uart7 {ChipUartLowLevel::uart7Parameters};

/**
 * \brief UART7 interrupt handler
 */

extern "C" void UART7_IRQHandler()
{
	uart7.interruptHandler();
}

#endif	// def CONFIG_CHIP_STM32_USARTV1_UART7_ENABLE

/*---------------------------------------------------------------------------------------------------------------------+
| UART8 global objects and functions
+---------------------------------------------------------------------------------------------------------------------*/

#ifdef CONFIG_CHIP_STM32_USARTV1_UART8_ENABLE

ChipUartLowLevel uart8 {ChipUartLowLevel::uart8Parameters};

/**
 * \brief UART8 interrupt handler
 */

extern "C" void UART8_IRQHandler()
{
	uart8.interruptHandler();
}

#endif	// def CONFIG_CHIP_STM32_USARTV1_UART8_ENABLE

/*---------------------------------------------------------------------------------------------------------------------+
| UART9 global objects and functions
+---------------------------------------------------------------------------------------------------------------------*/

#ifdef CONFIG_CHIP_STM32_USARTV1_UART9_ENABLE

ChipUartLowLevel uart9 {ChipUartLowLevel::uart9Parameters};

/**
 * \brief UART9 interrupt handler
 */

extern "C" void UART9_IRQHandler()
{
	uart9.interruptHandler();
}

#endif	// def CONFIG_CHIP_STM32_USARTV1_UART9_ENABLE

/*---------------------------------------------------------------------------------------------------------------------+
| UART10 global objects and functions
+---------------------------------------------------------------------------------------------------------------------*/

#ifdef CONFIG_CHIP_STM32_USARTV1_UART10_ENABLE

ChipUartLowLevel uart10 {ChipUartLowLevel::uart10Parameters};

/**
 * \brief UART10 interrupt handler
 */

extern "C" void UART10_IRQHandler()
{
	uart10.interruptHandler();
}

#endif	// def CONFIG_CHIP_STM32_USARTV1_UART10_ENABLE

}	// namespace chip

}	// namespace distortos
