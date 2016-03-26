/**
 * \file
 * \brief ChipOutputPin class header for STM32F1
 *
 * \author Copyright (C) 2016 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef SOURCE_CHIP_STM32_STM32F1_INCLUDE_DISTORTOS_CHIP_CHIPOUTPUTPIN_HPP_
#define SOURCE_CHIP_STM32_STM32F1_INCLUDE_DISTORTOS_CHIP_CHIPOUTPUTPIN_HPP_

#include "distortos/chip/STM32F1-GPIO.hpp"

#include "distortos/devices/io/OutputPin.hpp"

namespace distortos
{

namespace chip
{

/**
 * ChipOutputPin class is a single output pin of STM32F1
 *
 * \ingroup devices
 */

class ChipOutputPin : public devices::OutputPin
{
public:

	/**
	 * \brief ChipOutputPin's constructor
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

	explicit ChipOutputPin(Pin pin, bool openDrain = false, PinOutputSpeed outputSpeed = PinOutputSpeed::_2Mhz,
			bool initialState = false, bool inverted = false);

	/**
	 * \return current state of pin
	 */

	virtual bool get() const override;

	/**
	 * \brief Sets state of pin.
	 *
	 * \param [in] state is the new state of pin
	 */

	virtual void set(bool state) override;

private:

	/// identifier of pin
	Pin pin_;

	/// selects whether the pin is inverted (true) or not (false)
	bool inverted_;
};

}	// namespace chip

}	// namespace distortos

#endif	// SOURCE_CHIP_STM32_STM32F1_INCLUDE_DISTORTOS_CHIP_CHIPOUTPUTPIN_HPP_
