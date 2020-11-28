/**
 * \file
 * \brief Definitions of low-level UART drivers for USARTv2 in ST,STM32F0DISCOVERY (ST,STM32F051R8 chip)
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
