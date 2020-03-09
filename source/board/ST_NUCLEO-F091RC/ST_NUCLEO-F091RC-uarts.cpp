/**
 * \file
 * \brief Definitions of low-level UART drivers for USARTv2 in ST,NUCLEO-F091RC (ST,STM32F091RC chip)
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
		makeAlternateFunctionPinInitializer(Pin::pa3,
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

#ifdef DISTORTOS_CHIP_USART4_ENABLE

/*---------------------------------------------------------------------------------------------------------------------+
| USART4
+---------------------------------------------------------------------------------------------------------------------*/

ChipUartLowLevel usart4 {ChipUartLowLevel::usart4Parameters};

/**
 * \brief USART4 interrupt handler
 */

extern "C" void USART4_IRQHandler()
{
	usart4.interruptHandler();
}

#endif	// def DISTORTOS_CHIP_USART4_ENABLE

#ifdef DISTORTOS_CHIP_USART5_ENABLE

/*---------------------------------------------------------------------------------------------------------------------+
| USART5
+---------------------------------------------------------------------------------------------------------------------*/

ChipUartLowLevel usart5 {ChipUartLowLevel::usart5Parameters};

/**
 * \brief USART5 interrupt handler
 */

extern "C" void USART5_IRQHandler()
{
	usart5.interruptHandler();
}

#endif	// def DISTORTOS_CHIP_USART5_ENABLE

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

#ifdef DISTORTOS_CHIP_USART7_ENABLE

/*---------------------------------------------------------------------------------------------------------------------+
| USART7
+---------------------------------------------------------------------------------------------------------------------*/

ChipUartLowLevel usart7 {ChipUartLowLevel::usart7Parameters};

/**
 * \brief USART7 interrupt handler
 */

extern "C" void USART7_IRQHandler()
{
	usart7.interruptHandler();
}

#endif	// def DISTORTOS_CHIP_USART7_ENABLE

#ifdef DISTORTOS_CHIP_USART8_ENABLE

/*---------------------------------------------------------------------------------------------------------------------+
| USART8
+---------------------------------------------------------------------------------------------------------------------*/

ChipUartLowLevel usart8 {ChipUartLowLevel::usart8Parameters};

/**
 * \brief USART8 interrupt handler
 */

extern "C" void USART8_IRQHandler()
{
	usart8.interruptHandler();
}

#endif	// def DISTORTOS_CHIP_USART8_ENABLE

}	// namespace chip

}	// namespace distortos
