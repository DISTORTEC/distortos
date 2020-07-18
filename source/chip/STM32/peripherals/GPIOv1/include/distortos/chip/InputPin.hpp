/**
 * \file
 * \brief InputPin class header for GPIOv1 in STM32
 *
 * \author Copyright (C) 2016-2020 Kamil Szczygiel http://www.distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef SOURCE_CHIP_STM32_PERIPHERALS_GPIOV1_INCLUDE_DISTORTOS_CHIP_INPUTPIN_HPP_
#define SOURCE_CHIP_STM32_PERIPHERALS_GPIOV1_INCLUDE_DISTORTOS_CHIP_INPUTPIN_HPP_

#include "distortos/chip/STM32-GPIOv1.hpp"

#include "distortos/devices/io/InputPin.hpp"

namespace distortos
{

namespace chip
{

/**
 * \brief InputPin class is a single input pin of GPIOv1 in STM32.
 *
 * \ingroup devices
 */

class InputPin : public devices::InputPin
{
public:

	/**
	 * \brief InputPin's constructor
	 *
	 * \param [in] pin is the identifier of pin
	 * \param [in] pull is the desired pull-up/pull-down configuration of pin, default - PinPull::none
	 * \param [in] inverted selects whether the pin is inverted (true) - get() returns true when GPIO state is low and
	 * false when GPIO state is high - or not (false), default - false, not inverted
	 */

	explicit InputPin(Pin pin, PinPull pull = {}, bool inverted = {});

	/**
	 * \return current state of pin
	 */

	bool get() const override;

protected:

	/**
	 * \return true if inverted mode is enabled, false otherwise
	 */

	bool getInvertedMode() const
	{
		return inverted_;
	}

	/**
	 * \brief Enables or disables inverted mode.
	 *
	 * \param [in] inverted selects whether the pin is inverted (true) or not (false)
	 */

	void setInvertedMode(const bool inverted)
	{
		inverted_ = inverted;
	}

private:

	/// identifier of pin
	Pin pin_;

	/// selects whether the pin is inverted (true) or not (false)
	bool inverted_;
};

}	// namespace chip

}	// namespace distortos

#endif	// SOURCE_CHIP_STM32_PERIPHERALS_GPIOV1_INCLUDE_DISTORTOS_CHIP_INPUTPIN_HPP_
