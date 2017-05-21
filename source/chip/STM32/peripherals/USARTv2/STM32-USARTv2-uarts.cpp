/**
 * \file
 * \brief Definitions of low-level UART drivers for USARTv2 in STM32
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

#ifdef CONFIG_CHIP_STM32_USARTV2_UART4_ENABLE

ChipUartLowLevel uart4 {ChipUartLowLevel::uart4Parameters};

#endif	// def CONFIG_CHIP_STM32_USARTV2_UART4_ENABLE

#ifdef CONFIG_CHIP_STM32_USARTV2_USART4_ENABLE

ChipUartLowLevel usart4 {ChipUartLowLevel::usart4Parameters};

#endif	// def CONFIG_CHIP_STM32_USARTV2_USART4_ENABLE

#ifdef CONFIG_CHIP_STM32_USARTV2_UART5_ENABLE

ChipUartLowLevel uart5 {ChipUartLowLevel::uart5Parameters};

#endif	// def CONFIG_CHIP_STM32_USARTV2_UART5_ENABLE

#ifdef CONFIG_CHIP_STM32_USARTV2_USART5_ENABLE

ChipUartLowLevel usart5 {ChipUartLowLevel::usart5Parameters};

#endif	// def CONFIG_CHIP_STM32_USARTV2_USART5_ENABLE

#ifdef CONFIG_CHIP_STM32_USARTV2_USART6_ENABLE

ChipUartLowLevel usart6 {ChipUartLowLevel::usart6Parameters};

#endif	// def CONFIG_CHIP_STM32_USARTV2_USART6_ENABLE

#ifdef CONFIG_CHIP_STM32_USARTV2_UART7_ENABLE

ChipUartLowLevel uart7 {ChipUartLowLevel::uart7Parameters};

#endif	// def CONFIG_CHIP_STM32_USARTV2_UART7_ENABLE

#ifdef CONFIG_CHIP_STM32_USARTV2_USART7_ENABLE

ChipUartLowLevel usart7 {ChipUartLowLevel::usart7Parameters};

#endif	// def CONFIG_CHIP_STM32_USARTV2_USART7_ENABLE

#ifdef CONFIG_CHIP_STM32_USARTV2_UART8_ENABLE

ChipUartLowLevel uart8 {ChipUartLowLevel::uart8Parameters};

#endif	// def CONFIG_CHIP_STM32_USARTV2_UART8_ENABLE

#ifdef CONFIG_CHIP_STM32_USARTV2_USART8_ENABLE

ChipUartLowLevel usart8 {ChipUartLowLevel::usart8Parameters};

#endif	// def CONFIG_CHIP_STM32_USARTV2_USART8_ENABLE

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

#if defined(CONFIG_CHIP_STM32_USARTV2_USART3_USART8_COMBINED_INTERRUPT)

#if defined(CONFIG_CHIP_STM32_USARTV2_USART3_ENABLE) || defined(CONFIG_CHIP_STM32_USARTV2_USART4_ENABLE) || \
		defined(CONFIG_CHIP_STM32_USARTV2_USART5_ENABLE) || defined(CONFIG_CHIP_STM32_USARTV2_USART6_ENABLE) || \
		defined(CONFIG_CHIP_STM32_USARTV2_USART7_ENABLE) || defined(CONFIG_CHIP_STM32_USARTV2_USART8_ENABLE)

/**
 * \brief USART3 - USART8 interrupt handler
 */

extern "C" void USART3_8_IRQHandler()
{
#ifdef CONFIG_CHIP_STM32_USARTV2_USART3_ENABLE
	usart3.interruptHandler();
#endif	// def CONFIG_CHIP_STM32_USARTV2_USART3_ENABLE
#ifdef CONFIG_CHIP_STM32_USARTV2_USART4_ENABLE
	usart4.interruptHandler();
#endif	// def CONFIG_CHIP_STM32_USARTV2_USART4_ENABLE
#ifdef CONFIG_CHIP_STM32_USARTV2_USART5_ENABLE
	usart5.interruptHandler();
#endif	// def CONFIG_CHIP_STM32_USARTV2_USART5_ENABLE
#ifdef CONFIG_CHIP_STM32_USARTV2_USART6_ENABLE
	usart6.interruptHandler();
#endif	// def CONFIG_CHIP_STM32_USARTV2_USART6_ENABLE
#ifdef CONFIG_CHIP_STM32_USARTV2_USART7_ENABLE
	usart7.interruptHandler();
#endif	// def CONFIG_CHIP_STM32_USARTV2_USART7_ENABLE
#ifdef CONFIG_CHIP_STM32_USARTV2_USART8_ENABLE
	usart8.interruptHandler();
#endif	// def CONFIG_CHIP_STM32_USARTV2_USART8_ENABLE
}

#endif	// defined(CONFIG_CHIP_STM32_USARTV2_USART3_ENABLE) || defined(CONFIG_CHIP_STM32_USARTV2_USART4_ENABLE) ||
		// defined(CONFIG_CHIP_STM32_USARTV2_USART5_ENABLE) || defined(CONFIG_CHIP_STM32_USARTV2_USART6_ENABLE) ||
		// defined(CONFIG_CHIP_STM32_USARTV2_USART7_ENABLE) || defined(CONFIG_CHIP_STM32_USARTV2_USART8_ENABLE)

#elif defined(CONFIG_CHIP_STM32_USARTV2_USART3_USART6_COMBINED_INTERRUPT)

#if defined(CONFIG_CHIP_STM32_USARTV2_USART3_ENABLE) || defined(CONFIG_CHIP_STM32_USARTV2_USART4_ENABLE) || \
		defined(CONFIG_CHIP_STM32_USARTV2_USART5_ENABLE) || defined(CONFIG_CHIP_STM32_USARTV2_USART6_ENABLE)

/**
 * \brief USART3 - USART6 interrupt handler
 */

extern "C" void USART3_6_IRQHandler()
{
#ifdef CONFIG_CHIP_STM32_USARTV2_USART3_ENABLE
	usart3.interruptHandler();
#endif	// def CONFIG_CHIP_STM32_USARTV2_USART3_ENABLE
#ifdef CONFIG_CHIP_STM32_USARTV2_USART4_ENABLE
	usart4.interruptHandler();
#endif	// def CONFIG_CHIP_STM32_USARTV2_USART4_ENABLE
#ifdef CONFIG_CHIP_STM32_USARTV2_USART5_ENABLE
	usart5.interruptHandler();
#endif	// def CONFIG_CHIP_STM32_USARTV2_USART5_ENABLE
#ifdef CONFIG_CHIP_STM32_USARTV2_USART6_ENABLE
	usart6.interruptHandler();
#endif	// def CONFIG_CHIP_STM32_USARTV2_USART6_ENABLE
}

#endif	// defined(CONFIG_CHIP_STM32_USARTV2_USART3_ENABLE) || defined(CONFIG_CHIP_STM32_USARTV2_USART4_ENABLE) ||
		// defined(CONFIG_CHIP_STM32_USARTV2_USART5_ENABLE) || defined(CONFIG_CHIP_STM32_USARTV2_USART6_ENABLE)

#else	// !defined(CONFIG_CHIP_STM32_USARTV2_USART3_USART8_COMBINED_INTERRUPT) &&
		// !defined(CONFIG_CHIP_STM32_USARTV2_USART3_USART6_COMBINED_INTERRUPT)

#ifdef CONFIG_CHIP_STM32_USARTV2_USART3_ENABLE

/**
 * \brief USART3 interrupt handler
 */

extern "C" void USART3_IRQHandler()
{
	usart3.interruptHandler();
}

#endif	// def CONFIG_CHIP_STM32_USARTV2_USART3_ENABLE

#if defined(CONFIG_CHIP_STM32_USARTV2_USART4_USART5_COMBINED_INTERRUPT)

#if defined(CONFIG_CHIP_STM32_USARTV2_USART4_ENABLE) || defined(CONFIG_CHIP_STM32_USARTV2_USART5_ENABLE)

/**
 * \brief USART4 - USART5 interrupt handler
 */

extern "C" void USART4_5_IRQHandler()
{
#ifdef CONFIG_CHIP_STM32_USARTV2_USART4_ENABLE
	usart4.interruptHandler();
#endif	// def CONFIG_CHIP_STM32_USARTV2_USART4_ENABLE
#ifdef CONFIG_CHIP_STM32_USARTV2_USART5_ENABLE
	usart5.interruptHandler();
#endif	// def CONFIG_CHIP_STM32_USARTV2_USART5_ENABLE
}

#endif	// defined(CONFIG_CHIP_STM32_USARTV2_USART4_ENABLE) || defined(CONFIG_CHIP_STM32_USARTV2_USART5_ENABLE)

#else	// !defined(CONFIG_CHIP_STM32_USARTV2_USART4_USART5_COMBINED_INTERRUPT)

#ifdef CONFIG_CHIP_STM32_USARTV2_USART4_ENABLE

/**
 * \brief USART4 interrupt handler
 */

extern "C" void USART4_IRQHandler()
{
	usart4.interruptHandler();
}

#endif	// def CONFIG_CHIP_STM32_USARTV2_USART4_ENABLE

#ifdef CONFIG_CHIP_STM32_USARTV2_USART5_ENABLE

/**
 * \brief USART5 interrupt handler
 */

extern "C" void USART5_IRQHandler()
{
	usart5.interruptHandler();
}

#endif	// def CONFIG_CHIP_STM32_USARTV2_USART5_ENABLE

#endif	// !defined(CONFIG_CHIP_STM32_USARTV2_USART4_USART5_COMBINED_INTERRUPT)

#ifdef CONFIG_CHIP_STM32_USARTV2_UART4_ENABLE

/**
 * \brief UART4 interrupt handler
 */

extern "C" void UART4_IRQHandler()
{
	uart4.interruptHandler();
}

#endif	// def CONFIG_CHIP_STM32_USARTV2_UART4_ENABLE

#ifdef CONFIG_CHIP_STM32_USARTV2_UART5_ENABLE

/**
 * \brief UART5 interrupt handler
 */

extern "C" void UART5_IRQHandler()
{
	uart5.interruptHandler();
}

#endif	// def CONFIG_CHIP_STM32_USARTV2_UART5_ENABLE

#ifdef CONFIG_CHIP_STM32_USARTV2_USART6_ENABLE

/**
 * \brief USART6 interrupt handler
 */

extern "C" void USART6_IRQHandler()
{
	usart6.interruptHandler();
}

#endif	// def CONFIG_CHIP_STM32_USARTV2_USART6_ENABLE

#ifdef CONFIG_CHIP_STM32_USARTV2_UART7_ENABLE

/**
 * \brief UART7 interrupt handler
 */

extern "C" void UART7_IRQHandler()
{
	uart7.interruptHandler();
}

#endif	// def CONFIG_CHIP_STM32_USARTV2_UART7_ENABLE

#ifdef CONFIG_CHIP_STM32_USARTV2_USART7_ENABLE

/**
 * \brief USART7 interrupt handler
 */

extern "C" void USART7_IRQHandler()
{
	usart7.interruptHandler();
}

#endif	// def CONFIG_CHIP_STM32_USARTV2_USART7_ENABLE

#ifdef CONFIG_CHIP_STM32_USARTV2_UART8_ENABLE

/**
 * \brief UART8 interrupt handler
 */

extern "C" void UART8_IRQHandler()
{
	uart8.interruptHandler();
}

#endif	// def CONFIG_CHIP_STM32_USARTV2_UART8_ENABLE

#ifdef CONFIG_CHIP_STM32_USARTV2_USART8_ENABLE

/**
 * \brief USART8 interrupt handler
 */

extern "C" void USART8_IRQHandler()
{
	usart8.interruptHandler();
}

#endif	// def CONFIG_CHIP_STM32_USARTV2_USART8_ENABLE

#endif	// !defined(CONFIG_CHIP_STM32_USARTV2_USART3_USART8_COMBINED_INTERRUPT) &&
		// !defined(CONFIG_CHIP_STM32_USARTV2_USART3_USART6_COMBINED_INTERRUPT)

}	// namespace chip

}	// namespace distortos
