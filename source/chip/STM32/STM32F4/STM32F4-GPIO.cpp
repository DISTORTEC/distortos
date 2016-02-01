/**
 * \file
 * \brief Implementation of GPIO-related functions for STM32F4
 *
 * \author Copyright (C) 2016 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/chip/STM32F4-GPIO.hpp"

#include "distortos/chip/STM32F4-GPIO-bits.h"

#include "distortos/architecture/InterruptMaskingLock.hpp"

namespace distortos
{

namespace chip
{

/*---------------------------------------------------------------------------------------------------------------------+
| local functions
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief Configures pin's alternate function.
 *
 * \param [in] port is a reference to GPIO port
 * \param [in] pinNumber is the pin number, [0; 15]
 * \param [in] alternateFunction is the desired alternate function of pin
 */

void configureAlternateFunction(GPIO_TypeDef& port, const uint8_t pinNumber,
		const PinAlternateFunction alternateFunction)
{
	auto afr = port.AFR[pinNumber / 8];
	afr &= ~(GPIO_AFRx_mask << ((pinNumber % 8) * 4));
	afr |= static_cast<uint32_t>(alternateFunction) << ((pinNumber % 8) * 4);
	port.AFR[pinNumber / 8] = afr;
}

/**
 * \brief Configures pin's mode.
 *
 * \param [in] port is a reference to GPIO port
 * \param [in] pinNumber is the pin number, [0; 15]
 * \param [in] mode is the desired mode of pin
 */

void configureMode(GPIO_TypeDef& port, const uint8_t pinNumber, const PinMode mode)
{
	auto moder = port.MODER;
	moder &= ~(GPIO_MODER_mask << (pinNumber * 2));
	moder |= static_cast<uint32_t>(mode) << (pinNumber * 2);
	port.MODER = moder;
}

/**
 * \brief Configures pin's output speed.
 *
 * \param [in] port is a reference to GPIO port
 * \param [in] pinNumber is the pin number, [0; 15]
 * \param [in] outputSpeed is the desired output speed of pin
 */

void configureOutputSpeed(GPIO_TypeDef& port, const uint8_t pinNumber, const PinOutputSpeed outputSpeed)
{
	auto ospeedr = port.OSPEEDR;
	ospeedr &= ~(GPIO_OSPEEDR_mask << (pinNumber * 2));
	ospeedr |= static_cast<uint32_t>(outputSpeed) << (pinNumber * 2);
	port.OSPEEDR = ospeedr;
}

/**
 * \brief Configures pin's output type.
 *
 * \param [in] port is a reference to GPIO port
 * \param [in] pinNumber is the pin number, [0; 15]
 * \param [in] openDrain is the desired output type of pin: push-pull (false) or open-drain (true)
 */

void configureOutputType(GPIO_TypeDef& port, const uint8_t pinNumber, const bool openDrain)
{
	auto otyper = port.OTYPER;
	otyper &= ~(GPIO_OTYPER_mask << pinNumber);
	otyper |= static_cast<uint32_t>(openDrain) << pinNumber;
	port.OTYPER = otyper;
}

/**
 * \brief Configures pin's pull-up/pull-down configuration.
 *
 * \param [in] port is a reference to GPIO port
 * \param [in] pinNumber is the pin number, [0; 15]
 * \param [in] pull is the desired pull-up/pull-down configuration of pin
 */

void configurePull(GPIO_TypeDef& port, const uint8_t pinNumber, const PinPull pull)
{
	auto pupdr = port.PUPDR;
	pupdr &= ~(GPIO_PUPDR_mask << (pinNumber * 2));
	pupdr |= static_cast<uint32_t>(pull) << (pinNumber * 2);
	port.PUPDR = pupdr;
}

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

void configurePin(const Pin pin, const PinMode mode, const bool openDrain, const PinOutputSpeed outputSpeed,
		const PinPull pull, const PinAlternateFunction alternateFunction, const bool initialState)
{
	const auto decodedPin = decodePin(pin);
	auto& port = *decodedPin.first;
	const auto pinNumber = decodedPin.second;

	port.BSRR = 1 << (pinNumber + (initialState == false ? 16 : 0));

	architecture::InterruptMaskingLock interruptMaskingLock;

	configureMode(port, pinNumber, mode);
	configureOutputType(port, pinNumber, openDrain);
	configureOutputSpeed(port, pinNumber, outputSpeed);
	configurePull(port, pinNumber, pull);
	configureAlternateFunction(port, pinNumber, alternateFunction);
}

}	// namespace chip

}	// namespace distortos
