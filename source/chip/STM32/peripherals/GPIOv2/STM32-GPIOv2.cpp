/**
 * \file
 * \brief Implementation of GPIOv2 functions for STM32
 *
 * \author Copyright (C) 2016-2024 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "distortos/chip/STM32-GPIOv2.hpp"

#include "distortos/InterruptMaskingLock.hpp"

#if !defined(GPIO_AFRL_AFSEL0) && defined(GPIO_AFRL_AFRL0)
#define GPIO_AFRL_AFSEL0					GPIO_AFRL_AFRL0
#endif	// !defined(GPIO_AFRL_AFSEL0)
#if !defined(GPIO_MODER_MODE0) && defined(GPIO_MODER_MODER0)
#define GPIO_MODER_MODE0					GPIO_MODER_MODER0
#endif	// !defined(GPIO_MODER_MODE0)
#if !defined(GPIO_OSPEEDR_OSPEED0) && defined(GPIO_OSPEEDR_OSPEEDR0)
#define GPIO_OSPEEDR_OSPEED0				GPIO_OSPEEDR_OSPEEDR0
#endif	// !defined(GPIO_OSPEEDR_OSPEED0)
#if !defined(GPIO_OSPEEDR_OSPEED0) && defined(GPIO_OSPEEDER_OSPEED0)
#define GPIO_OSPEEDR_OSPEED0				GPIO_OSPEEDER_OSPEED0
#endif	// !defined(GPIO_OSPEEDR_OSPEED0)
#if !defined(GPIO_OTYPER_OT0) && defined(GPIO_OTYPER_OT_0)
#define GPIO_OTYPER_OT0						GPIO_OTYPER_OT_0
#endif	// !defined(GPIO_OTYPER_OT0)
#if !defined(GPIO_PUPDR_PUPD0) && defined(GPIO_PUPDR_PUPDR0)
#define GPIO_PUPDR_PUPD0					GPIO_PUPDR_PUPDR0
#endif	// !defined(GPIO_PUPDR_PUPD0)

namespace distortos
{

namespace chip
{

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

void configurePin(const Pin pin, const PinMode mode, const bool openDrain, const PinOutputSpeed outputSpeed,
		const PinPull pull, const PinAlternateFunction alternateFunction, const bool initialState)
{
	const auto decodedPin = decodePin(pin);
	auto& port = *decodedPin.first;
	const auto pinNumber = decodedPin.second;
	auto& afr = port.AFR[pinNumber / 8];
	const auto moderInvertedMask = ~(GPIO_MODER_MODE0 << (pinNumber * 2));
	const auto shiftedMode = static_cast<uint32_t>(mode) << (pinNumber * 2);
	const auto otyperInvertedMask = ~(GPIO_OTYPER_OT0 << pinNumber);
	const auto shiftedOpenDrain = static_cast<uint32_t>(openDrain) << pinNumber;
	const auto ospeedrInvertedMask = ~(GPIO_OSPEEDR_OSPEED0 << (pinNumber * 2));
	const auto shiftedOutputSpeed = static_cast<uint32_t>(outputSpeed) << (pinNumber * 2);
	const auto pupdrInvertedMask = ~(GPIO_PUPDR_PUPD0 << (pinNumber * 2));
	const auto shiftedPull = static_cast<uint32_t>(pull) << (pinNumber * 2);
	const auto afrInvertedMask = ~(GPIO_AFRL_AFSEL0 << ((pinNumber * 4) % 32));
	const auto shiftedAlternateFunction = static_cast<uint32_t>(alternateFunction) << ((pinNumber * 4) % 32);

	port.BSRR = 1 << (pinNumber + (initialState == false ? 16 : 0));

	const InterruptMaskingLock interruptMaskingLock;

	port.MODER = (port.MODER & moderInvertedMask) | shiftedMode;
	port.OTYPER = (port.OTYPER & otyperInvertedMask) | shiftedOpenDrain;
	port.OSPEEDR = (port.OSPEEDR & ospeedrInvertedMask) | shiftedOutputSpeed;
	port.PUPDR = (port.PUPDR & pupdrInvertedMask) | shiftedPull;
	afr = (afr & afrInvertedMask) | shiftedAlternateFunction;
}

}	// namespace chip

}	// namespace distortos
