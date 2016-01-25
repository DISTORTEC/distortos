/**
 * \file
 * \brief ChipInputPin class implementation for STM32F4
 *
 * \author Copyright (C) 2016 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/chip/ChipInputPin.hpp"

namespace distortos
{

namespace chip
{

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

ChipInputPin::ChipInputPin(const Pin pin, const PinPull pull) :
		pin_{pin}
{
	configureInputPin(pin_, pull);
}

bool ChipInputPin::get() const
{
	const auto decodedPin = decodePin(pin_);
	auto& port = *decodedPin.first;
	const auto pinNumber = decodedPin.second;
	return (port.IDR & (1 << pinNumber)) != 0;
}

}	// namespace chip

}	// namespace distortos
