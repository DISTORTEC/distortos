/**
 * \file
 * \brief ChipOutputPin class implementation for STM32F4
 *
 * \author Copyright (C) 2016 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/chip/ChipOutputPin.hpp"

namespace distortos
{

namespace chip
{

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

ChipOutputPin::ChipOutputPin(const Pin pin, const bool openDrain, const PinOutputSpeed outputSpeed, const PinPull pull,
		const bool initialState) :
				pin_{pin}
{
	configureOutputPin(pin_, openDrain, outputSpeed, pull, initialState);
}

bool ChipOutputPin::get() const
{
	const auto decodedPin = decodePin(pin_);
	auto& port = *decodedPin.first;
	const auto pinNumber = decodedPin.second;
	return (port.IDR & (1 << pinNumber)) != 0;
}

void ChipOutputPin::set(const bool state)
{
	const auto decodedPin = decodePin(pin_);
	auto& port = *decodedPin.first;
	const auto pinNumber = decodedPin.second;
	port.BSRR = 1 << (pinNumber + (state == false ? 16 : 0));
}

}	// namespace chip

}	// namespace distortos
