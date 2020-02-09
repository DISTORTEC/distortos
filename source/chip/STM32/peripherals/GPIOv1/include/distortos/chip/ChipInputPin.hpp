/**
 * \file
 * \brief ChipInputPin class header for GPIOv1 in STM32
 *
 * \author Copyright (C) 2016-2020 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef SOURCE_CHIP_STM32_PERIPHERALS_GPIOV1_INCLUDE_DISTORTOS_CHIP_CHIPINPUTPIN_HPP_
#define SOURCE_CHIP_STM32_PERIPHERALS_GPIOV1_INCLUDE_DISTORTOS_CHIP_CHIPINPUTPIN_HPP_

#include "distortos/chip/InputPin.hpp"

namespace distortos
{

namespace chip
{

/**
 * \brief ChipInputPin is an alias for InputPin.
 */

class ChipInputPin : public InputPin
{
public:

	using InputPin::InputPin;
};

}	// namespace chip

}	// namespace distortos

#endif	// SOURCE_CHIP_STM32_PERIPHERALS_GPIOV1_INCLUDE_DISTORTOS_CHIP_CHIPINPUTPIN_HPP_
