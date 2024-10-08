/**
 * \file
 * \brief Definitions of low-level UART drivers for USARTv2 in ST,NUCLEO-U575ZI-Q (ST,STM32U575ZI chip)
 *
 * \author Copyright (C) 2016-2020 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * \warning
 * Automatically generated file - do not edit!
 */

#include "distortos/chip/uarts.hpp"

#include "distortos/chip/ChipUartLowLevel.hpp"
#include "distortos/chip/PinInitializer.hpp"

#include "distortos/BIND_LOW_LEVEL_INITIALIZER.h"

namespace distortos
{

namespace chip
{

#ifdef DISTORTOS_CHIP_LPUART1_ENABLE

/*---------------------------------------------------------------------------------------------------------------------+
| LPUART1
+---------------------------------------------------------------------------------------------------------------------*/

ChipUartLowLevel lpuart1 {ChipUartLowLevel::lpuart1Parameters};

/**
 * \brief LPUART1 interrupt handler
 */

extern "C" void LPUART1_IRQHandler()
{
	lpuart1.interruptHandler();
}

#endif	// def DISTORTOS_CHIP_LPUART1_ENABLE

#ifdef DISTORTOS_CHIP_USART1_ENABLE

/*---------------------------------------------------------------------------------------------------------------------+
| USART1
+---------------------------------------------------------------------------------------------------------------------*/

namespace
{

/// pin initializers for USART1
const PinInitializer usart1PinInitializers[]
{
		// USART1 RX
		makeAlternateFunctionPinInitializer(Pin::pa10,
				PinAlternateFunction::af7,
				false,
				PinOutputSpeed::veryHigh,
				PinPull::up),
		// USART1 TX
		makeAlternateFunctionPinInitializer(Pin::pa9,
				PinAlternateFunction::af7,
				false,
				PinOutputSpeed::veryHigh,
				PinPull::none),
};

/**
 * \brief Low-level chip initializer for USART1
 *
 * This function is called before constructors for global and static objects via BIND_LOW_LEVEL_INITIALIZER().
 */

void usart1LowLevelInitializer()
{
	for (auto& pinInitializer : usart1PinInitializers)
		pinInitializer();
}

BIND_LOW_LEVEL_INITIALIZER(50, usart1LowLevelInitializer);

}	// namespace

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

}	// namespace chip

}	// namespace distortos
