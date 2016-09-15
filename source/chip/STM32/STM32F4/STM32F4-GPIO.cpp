/**
 * \file
 * \brief Implementation of GPIO-related functions for STM32F4
 *
 * \author Copyright (C) 2016 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/chip/STM32-GPIOv2.hpp"

#include "distortos/chip/STM32-GPIOv2-bits.h"

#include "distortos/architecture/InterruptMaskingLock.hpp"

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
	const auto moderInvertedMask = ~(GPIO_MODER_mask << (pinNumber * 2));
	const auto shiftedMode = static_cast<uint32_t>(mode) << (pinNumber * 2);
	const auto otyperInvertedMask = ~(GPIO_OTYPER_mask << pinNumber);
	const auto shiftedOpenDrain = static_cast<uint32_t>(openDrain) << pinNumber;
	const auto ospeedrInvertedMask = ~(GPIO_OSPEEDR_mask << (pinNumber * 2));
	const auto shiftedOutputSpeed = static_cast<uint32_t>(outputSpeed) << (pinNumber * 2);
	const auto pupdrInvertedMask = ~(GPIO_PUPDR_mask << (pinNumber * 2));
	const auto shiftedPull = static_cast<uint32_t>(pull) << (pinNumber * 2);
	const auto afrInvertedMask = ~(GPIO_AFRx_mask << ((pinNumber * 4) % 32));
	const auto shiftedAlternateFunction = static_cast<uint32_t>(alternateFunction) << ((pinNumber * 4) % 32);

	port.BSRR = 1 << (pinNumber + (initialState == false ? 16 : 0));

	architecture::InterruptMaskingLock interruptMaskingLock;

	port.MODER = (port.MODER & moderInvertedMask) | shiftedMode;
	port.OTYPER = (port.OTYPER & otyperInvertedMask) | shiftedOpenDrain;
	port.OSPEEDR = (port.OSPEEDR & ospeedrInvertedMask) | shiftedOutputSpeed;
	port.PUPDR = (port.PUPDR & pupdrInvertedMask) | shiftedPull;
	afr = (afr & afrInvertedMask) | shiftedAlternateFunction;
}

}	// namespace chip

}	// namespace distortos
