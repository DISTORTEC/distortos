/**
 * \file
 * \brief PinInitializer class header for GPIOv2 in STM32
 *
 * \author Copyright (C) 2020 Kamil Szczygiel http://www.distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef SOURCE_CHIP_STM32_PERIPHERALS_GPIOV2_INCLUDE_DISTORTOS_CHIP_PININITIALIZER_HPP_
#define SOURCE_CHIP_STM32_PERIPHERALS_GPIOV2_INCLUDE_DISTORTOS_CHIP_PININITIALIZER_HPP_

#include "distortos/chip/STM32-GPIOv2.hpp"

namespace distortos
{

namespace chip
{

/**
 * \brief PinInitializer class is a functor used to initialize single pin of GPIOv2 in STM32.
 *
 * \ingroup devices
 */

class PinInitializer
{
public:

	/**
	 * \brief PinInitializer's constructor
	 *
	 * \param [in] pin is the identifier of pin
	 * \param [in] mode is the desired mode of pin
	 * \param [in] openDrain is the desired output type of pin: push-pull (false) or open-drain (true)
	 * \param [in] outputSpeed is the desired output speed of pin
	 * \param [in] pull is the desired pull-up/pull-down configuration of pin
	 * \param [in] alternateFunction is the desired alternate function of pin
	 * \param [in] initialState is the initial state of pin
	 */

	constexpr PinInitializer(const Pin pin, const PinMode mode, const bool openDrain, const PinOutputSpeed outputSpeed,
			const PinPull pull, const PinAlternateFunction alternateFunction, const bool initialState) :
					pin_{pin},
					alternateFunction_{alternateFunction},
					initialState_{initialState},
					mode_{mode},
					openDrain_{openDrain},
					outputSpeed_{outputSpeed},
					pull_{pull}
	{

	}

	/**
	 * \brief PinInitializer's function call operator
	 *
	 * Calls configurePin() using initializer's parameters.
	 *
	 * \pre GPIO is enabled in RCC.
	 */

	void operator()() const
	{
		configurePin(pin_, mode_, openDrain_, outputSpeed_, pull_, alternateFunction_, initialState_);
	}

private:

	/// identifier of pin
	Pin pin_;

	/// desired alternate function of pin
	PinAlternateFunction alternateFunction_;

	/// initial state of pin
	bool initialState_;

	/// desired mode of pin
	PinMode mode_;

	/// desired output type of pin: push-pull (false) or open-drain (true)
	bool openDrain_;

	/// desired output speed of pin
	PinOutputSpeed outputSpeed_;

	/// desired pull-up/pull-down configuration of pin
	PinPull pull_;
};

/**
 * \brief Makes PinInitializer for alternate function pin.
 *
 * \relates PinInitializer
 *
 * \param [in] pin is the identifier of pin
 * \param [in] alternateFunction is the desired alternate function of pin
 * \param [in] openDrain is the desired output type of pin: push-pull (false) or open-drain (true)
 * \param [in] outputSpeed is the desired output speed of pin
 * \param [in] pull is the desired pull-up/pull-down configuration of pin
 */

constexpr inline static PinInitializer makeAlternateFunctionPinInitializer(const Pin pin,
		const PinAlternateFunction alternateFunction, const bool openDrain, const PinOutputSpeed outputSpeed,
		const PinPull pull)
{
	return {pin, PinMode::alternateFunction, openDrain, outputSpeed, pull, alternateFunction, {}};
}

/**
 * \brief Makes PinInitializer for analog pin.
 *
 * \relates PinInitializer
 *
 * \param [in] pin is the identifier of pin
 */

constexpr inline static PinInitializer makeAnalogPinInitializer(const Pin pin)
{
	return {pin, PinMode::analog, {}, {}, {}, {}, {}};
}

/**
 * \brief Makes PinInitializer for input pin.
 *
 * \relates PinInitializer
 *
 * \param [in] pin is the identifier of pin
 * \param [in] pull is the desired pull-up/pull-down configuration of pin
 */

constexpr inline static PinInitializer makeInputPinInitializer(const Pin pin, const PinPull pull)
{
	return {pin, PinMode::input, {}, {}, pull, {}, {}};
}

/**
 * \brief Makes PinInitializer for output pin.
 *
 * \relates PinInitializer
 *
 * \param [in] pin is the identifier of pin
 * \param [in] openDrain is the desired output type of pin: push-pull (false) or open-drain (true)
 * \param [in] outputSpeed is the desired output speed of pin
 * \param [in] pull is the desired pull-up/pull-down configuration of pin
 * \param [in] initialState is the initial state of pin
 */

constexpr inline static PinInitializer makeOutputPinInitializer(const Pin pin, const bool openDrain,
		const PinOutputSpeed outputSpeed, const PinPull pull, const bool initialState)
{
	return {pin, PinMode::output, openDrain, outputSpeed, pull, {}, initialState};
}

}	// namespace chip

}	// namespace distortos

#endif	// SOURCE_CHIP_STM32_PERIPHERALS_GPIOV2_INCLUDE_DISTORTOS_CHIP_PININITIALIZER_HPP_
