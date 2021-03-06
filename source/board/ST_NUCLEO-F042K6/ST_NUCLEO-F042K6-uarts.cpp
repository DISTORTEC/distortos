/**
 * \file
 * \brief Definitions of low-level UART drivers for USARTv2 in ST,NUCLEO-F042K6 (ST,STM32F042K6 chip)
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

namespace
{

/// pin initializers for USART2
const PinInitializer usart2PinInitializers[]
{
		// USART2 RX
		makeAlternateFunctionPinInitializer(Pin::pa15,
				PinAlternateFunction::af1,
				false,
				PinOutputSpeed::veryHigh,
				PinPull::up),
		// USART2 TX
		makeAlternateFunctionPinInitializer(Pin::pa2,
				PinAlternateFunction::af1,
				false,
				PinOutputSpeed::veryHigh,
				PinPull::none),
};

/**
 * \brief Low-level chip initializer for USART2
 *
 * This function is called before constructors for global and static objects via BIND_LOW_LEVEL_INITIALIZER().
 */

void usart2LowLevelInitializer()
{
	for (auto& pinInitializer : usart2PinInitializers)
		pinInitializer();
}

BIND_LOW_LEVEL_INITIALIZER(50, usart2LowLevelInitializer);

}	// namespace

ChipUartLowLevel usart2 {ChipUartLowLevel::usart2Parameters};

/**
 * \brief USART2 interrupt handler
 */

extern "C" void USART2_IRQHandler()
{
	usart2.interruptHandler();
}

#endif	// def DISTORTOS_CHIP_USART2_ENABLE

}	// namespace chip

}	// namespace distortos
