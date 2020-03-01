/**
 * \file
 * \brief OutputPin class header for GPIOv1 in STM32
 *
 * \author Copyright (C) 2016-2020 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef SOURCE_CHIP_STM32_PERIPHERALS_GPIOV1_INCLUDE_DISTORTOS_CHIP_OUTPUTPIN_HPP_
#define SOURCE_CHIP_STM32_PERIPHERALS_GPIOV1_INCLUDE_DISTORTOS_CHIP_OUTPUTPIN_HPP_

#include "distortos/chip/STM32-GPIOv1.hpp"

#include "distortos/devices/io/OutputPin.hpp"

namespace distortos
{

namespace chip
{

/**
 * \brief OutputPin class is a single output pin of GPIOv1 in STM32.
 *
 * \ingroup devices
 */

class OutputPin : public devices::OutputPin
{
public:

	/**
	 * \brief OutputPin's constructor
	 *
	 * \param [in] pin is the identifier of pin
	 * \param [in] openDrain is the desired output type of pin: push-pull (false) or open-drain (true),
	 * default - push-pull (false)
	 * \param [in] outputSpeed is the desired output speed of pin, default - PinOutputSpeed::_2Mhz
	 * \param [in] initialState is the initial state of pin, default - false
	 * \param [in] inverted selects whether the pin is inverted (true) - get() returns true when GPIO state is low and
	 * false when GPIO state is high, set() sets GPIO state to low when argument is true and to high when argument is
	 * false - or not (false), default - false, not inverted
	 */

	explicit OutputPin(Pin pin, bool openDrain = {}, PinOutputSpeed outputSpeed = {}, bool initialState = {},
			bool inverted = {});

	/**
	 * \return current state of pin
	 */

	bool get() const override;

	/**
	 * \brief Sets state of pin.
	 *
	 * \param [in] state is the new state of pin
	 */

	void set(bool state) override;

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

#endif	// SOURCE_CHIP_STM32_PERIPHERALS_GPIOV1_INCLUDE_DISTORTOS_CHIP_OUTPUTPIN_HPP_
