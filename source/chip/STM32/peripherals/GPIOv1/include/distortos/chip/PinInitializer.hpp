/**
 * \file
 * \brief PinInitializer class header for GPIOv1 in STM32
 *
 * \author Copyright (C) 2020 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef SOURCE_CHIP_STM32_PERIPHERALS_GPIOV1_INCLUDE_DISTORTOS_CHIP_PININITIALIZER_HPP_
#define SOURCE_CHIP_STM32_PERIPHERALS_GPIOV1_INCLUDE_DISTORTOS_CHIP_PININITIALIZER_HPP_

#include "distortos/chip/STM32-GPIOv1.hpp"

namespace distortos
{

namespace chip
{

/**
 * \brief PinInitializer class is a functor used to initialize single pin of GPIOv1 in STM32.
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
	 * \param [in] configuration is the desired configuration of pin
	 * \param [in] initialState is the initial state of pin
	 */

	constexpr PinInitializer(const Pin pin, const PinConfiguration configuration, const bool initialState) :
			pin_{pin},
			configuration_{configuration},
			initialState_{initialState}
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
		configurePin(pin_, configuration_, initialState_);
	}

private:

	/// identifier of pin
	Pin pin_;

	/// desired configuration of pin
	PinConfiguration configuration_;

	/// initial state of pin
	bool initialState_;
};

/**
 * \brief Makes PinInitializer for alternate function pin.
 *
 * \relates PinInitializer
 *
 * \param [in] pin is the identifier of pin
 * \param [in] openDrain is the desired output type of pin: push-pull (false) or open-drain (true)
 * \param [in] outputSpeed is the desired output speed of pin
 * \param [in] pull is the desired pull-up/pull-down configuration of pin
 */

constexpr inline static PinInitializer makeAlternateFunctionPinInitializer(const Pin pin, const bool openDrain,
		const PinOutputSpeed outputSpeed, const PinPull pull)
{
	return {pin,
			outputSpeed == PinOutputSpeed::_2Mhz ?
					openDrain == false ? PinConfiguration::pushPull2MhzAlternateFunction :
							PinConfiguration::openDrain2MhzAlternateFunction :
			outputSpeed == PinOutputSpeed::_10Mhz ?
					openDrain == false ? PinConfiguration::pushPull10MhzAlternateFunction :
							PinConfiguration::openDrain10MhzAlternateFunction :
			/* outputSpeed == PinOutputSpeed::_50Mhz ? */
					openDrain == false ? PinConfiguration::pushPull50MhzAlternateFunction :
							PinConfiguration::openDrain50MhzAlternateFunction,
			pull == PinPull::up};
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
	return {pin, PinConfiguration::analogInput, false};
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
	return {pin, pull == PinPull::none ? PinConfiguration::floatingInput : PinConfiguration::inputWithPullUpDown,
			pull == PinPull::up};
}

/**
 * \brief Makes PinInitializer for output pin.
 *
 * \relates PinInitializer
 *
 * \param [in] pin is the identifier of pin
 * \param [in] openDrain is the desired output type of pin: push-pull (false) or open-drain (true)
 * \param [in] outputSpeed is the desired output speed of pin
 * \param [in] initialState is the initial state of pin
 */

constexpr inline static PinInitializer makeOutputPinInitializer(const Pin pin, const bool openDrain,
		const PinOutputSpeed outputSpeed, const bool initialState)
{
	return {pin,
			outputSpeed == PinOutputSpeed::_2Mhz ?
					openDrain == false ?
							PinConfiguration::pushPull2MhzOutput : PinConfiguration::openDrain2MhzOutput :
			outputSpeed == PinOutputSpeed::_10Mhz ?
					openDrain == false ?
							PinConfiguration::pushPull10MhzOutput : PinConfiguration::openDrain10MhzOutput :
			/* outputSpeed == PinOutputSpeed::_50Mhz ? */
					openDrain == false ?
							PinConfiguration::pushPull50MhzOutput : PinConfiguration::openDrain50MhzOutput,
			initialState};
}

}	// namespace chip

}	// namespace distortos

#endif	// SOURCE_CHIP_STM32_PERIPHERALS_GPIOV1_INCLUDE_DISTORTOS_CHIP_PININITIALIZER_HPP_
