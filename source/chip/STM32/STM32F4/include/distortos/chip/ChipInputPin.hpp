/**
 * \file
 * \brief ChipInputPin class header for STM32F4
 *
 * \author Copyright (C) 2016 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef SOURCE_CHIP_STM32_STM32F4_INCLUDE_DISTORTOS_CHIP_CHIPINPUTPIN_HPP_
#define SOURCE_CHIP_STM32_STM32F4_INCLUDE_DISTORTOS_CHIP_CHIPINPUTPIN_HPP_

#include "distortos/chip/STM32F4-GPIO.hpp"

#include "distortos/devices/io/InputPin.hpp"

namespace distortos
{

namespace chip
{

/**
 * ChipInputPin class is a single input pin of STM32F4
 *
 * \ingroup devices
 */

class ChipInputPin : public devices::InputPin
{
public:

	/**
	 * \brief ChipInputPin's constructor
	 *
	 * \param [in] pin is the identifier of pin
	 * \param [in] pull is the desired pull-up/pull-down configuration of pin, default - PinPull::none
	 * \param [in] inverted selects whether the pin is inverted (true) - get() returns true when GPIO state is low and
	 * false when GPIO state is high - or not (false), default - false, not inverted
	 */

	explicit ChipInputPin(Pin pin, PinPull pull = PinPull::none, bool inverted = false);

	/**
	 * \return current state of pin
	 */

	bool get() const override;

private:

	/// identifier of pin
	Pin pin_;

	/// selects whether the pin is inverted (true) or not (false)
	bool inverted_;
};

}	// namespace chip

}	// namespace distortos

#endif	// SOURCE_CHIP_STM32_STM32F4_INCLUDE_DISTORTOS_CHIP_CHIPINPUTPIN_HPP_
