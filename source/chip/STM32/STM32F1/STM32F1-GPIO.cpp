/**
 * \file
 * \brief Implementation of GPIO-related functions for STM32F1
 *
 * \author Copyright (C) 2016 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/chip/STM32F1-GPIO.hpp"

#include "distortos/architecture/InterruptMaskingLock.hpp"

namespace distortos
{

namespace chip
{

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

void configurePin(const Pin pin, const PinConfiguration configuration, const bool initialState)
{
	const auto decodedPin = decodePin(pin);
	auto& port = *decodedPin.first;
	const auto pinNumber = decodedPin.second;
	auto& cr = pinNumber < 8 ? port.CRL : port.CRH;
	const auto invertedMask = ~((GPIO_CRL_CNF0 | GPIO_CRL_MODE0) << ((pinNumber * 4) % 32));
	const auto configurationValue = static_cast<uint32_t>(configuration) << ((pinNumber * 4) % 32);

	(initialState == false ? port.BRR : port.BSRR) = 1 << pinNumber;

	architecture::InterruptMaskingLock interruptMaskingLock;

	cr = (cr & invertedMask) | configurationValue;
}

}	// namespace chip

}	// namespace distortos
