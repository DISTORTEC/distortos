/**
 * \file
 * \brief InputPin class implementation for GPIOv1 in STM32
 *
 * \author Copyright (C) 2016-2020 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/chip/InputPin.hpp"

namespace distortos
{

namespace chip
{

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

InputPin::InputPin(const Pin pin, const PinPull pull, const bool inverted) :
		pin_{pin},
		inverted_{inverted}
{
	configureInputPin(pin_, pull);
}

bool InputPin::get() const
{
	const auto decodedPin = decodePin(pin_);
	auto& port = *decodedPin.first;
	const auto pinNumber = decodedPin.second;
	return static_cast<bool>(port.IDR & (1 << pinNumber)) != inverted_;
}

}	// namespace chip

}	// namespace distortos
