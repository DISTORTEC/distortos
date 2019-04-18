/**
 * \file
 * \brief Definitions of low-level UART drivers for USARTv2 in ST,NUCLEO-F091RC (ST,STM32F091RC chip)
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

#ifdef DISTORTOS_CHIP_USART1_ENABLE

ChipUartLowLevel usart1 {ChipUartLowLevel::usart1Parameters};

#endif	// def DISTORTOS_CHIP_USART1_ENABLE

#ifdef DISTORTOS_CHIP_USART2_ENABLE

ChipUartLowLevel usart2 {ChipUartLowLevel::usart2Parameters};

#endif	// def DISTORTOS_CHIP_USART2_ENABLE

#ifdef DISTORTOS_CHIP_USART3_ENABLE

ChipUartLowLevel usart3 {ChipUartLowLevel::usart3Parameters};

#endif	// def DISTORTOS_CHIP_USART3_ENABLE

#ifdef DISTORTOS_CHIP_USART4_ENABLE

ChipUartLowLevel usart4 {ChipUartLowLevel::usart4Parameters};

#endif	// def DISTORTOS_CHIP_USART4_ENABLE

#ifdef DISTORTOS_CHIP_USART5_ENABLE

ChipUartLowLevel usart5 {ChipUartLowLevel::usart5Parameters};

#endif	// def DISTORTOS_CHIP_USART5_ENABLE

#ifdef DISTORTOS_CHIP_USART6_ENABLE

ChipUartLowLevel usart6 {ChipUartLowLevel::usart6Parameters};

#endif	// def DISTORTOS_CHIP_USART6_ENABLE

#ifdef DISTORTOS_CHIP_USART7_ENABLE

ChipUartLowLevel usart7 {ChipUartLowLevel::usart7Parameters};

#endif	// def DISTORTOS_CHIP_USART7_ENABLE

#ifdef DISTORTOS_CHIP_USART8_ENABLE

ChipUartLowLevel usart8 {ChipUartLowLevel::usart8Parameters};

#endif	// def DISTORTOS_CHIP_USART8_ENABLE

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

#ifdef DISTORTOS_CHIP_USART1_ENABLE

/**
 * \brief USART1 interrupt handler
 */

extern "C" void USART1_IRQHandler()
{
	usart1.interruptHandler();
}

#endif	// def DISTORTOS_CHIP_USART1_ENABLE

#ifdef DISTORTOS_CHIP_USART2_ENABLE

/**
 * \brief USART2 interrupt handler
 */

extern "C" void USART2_IRQHandler()
{
	usart2.interruptHandler();
}

#endif	// def DISTORTOS_CHIP_USART2_ENABLE

#ifdef DISTORTOS_CHIP_USART3_ENABLE

/**
 * \brief USART3 interrupt handler
 */

extern "C" void USART3_IRQHandler()
{
	usart3.interruptHandler();
}

#endif	// def DISTORTOS_CHIP_USART3_ENABLE

#ifdef DISTORTOS_CHIP_USART4_ENABLE

/**
 * \brief USART4 interrupt handler
 */

extern "C" void USART4_IRQHandler()
{
	usart4.interruptHandler();
}

#endif	// def DISTORTOS_CHIP_USART4_ENABLE

#ifdef DISTORTOS_CHIP_USART5_ENABLE

/**
 * \brief USART5 interrupt handler
 */

extern "C" void USART5_IRQHandler()
{
	usart5.interruptHandler();
}

#endif	// def DISTORTOS_CHIP_USART5_ENABLE

#ifdef DISTORTOS_CHIP_USART6_ENABLE

/**
 * \brief USART6 interrupt handler
 */

extern "C" void USART6_IRQHandler()
{
	usart6.interruptHandler();
}

#endif	// def DISTORTOS_CHIP_USART6_ENABLE

#ifdef DISTORTOS_CHIP_USART7_ENABLE

/**
 * \brief USART7 interrupt handler
 */

extern "C" void USART7_IRQHandler()
{
	usart7.interruptHandler();
}

#endif	// def DISTORTOS_CHIP_USART7_ENABLE

#ifdef DISTORTOS_CHIP_USART8_ENABLE

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
