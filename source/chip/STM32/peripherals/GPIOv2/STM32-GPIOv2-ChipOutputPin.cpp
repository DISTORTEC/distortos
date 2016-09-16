/**
 * \file
 * \brief ChipOutputPin class implementation for GPIOv2 in STM32
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
		const bool initialState, const bool inverted) :
				pin_{pin},
				inverted_{inverted}
{
	configureOutputPin(pin_, openDrain, outputSpeed, pull, initialState != inverted);
}

bool ChipOutputPin::get() const
{
	const auto decodedPin = decodePin(pin_);
	auto& port = *decodedPin.first;
	const auto pinNumber = decodedPin.second;
	return static_cast<bool>(port.IDR & (1 << pinNumber)) != inverted_;
}

void ChipOutputPin::set(const bool state)
{
	const auto decodedPin = decodePin(pin_);
	auto& port = *decodedPin.first;
	const auto pinNumber = decodedPin.second;
	port.BSRR = 1 << (pinNumber + (state == inverted_ ? 16 : 0));
}

}	// namespace chip

}	// namespace distortos
