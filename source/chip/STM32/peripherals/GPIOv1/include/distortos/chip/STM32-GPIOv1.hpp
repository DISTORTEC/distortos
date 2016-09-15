/**
 * \file
 * \brief Header for GPIOv1 functions for STM32
 *
 * \author Copyright (C) 2016 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef SOURCE_CHIP_STM32_PERIPHERALS_GPIOV1_INCLUDE_DISTORTOS_CHIP_STM32_GPIOV1_HPP_
#define SOURCE_CHIP_STM32_PERIPHERALS_GPIOV1_INCLUDE_DISTORTOS_CHIP_STM32_GPIOV1_HPP_

#include "distortos/chip/CMSIS-proxy.h"

#include <utility>

namespace distortos
{

namespace chip
{

/*---------------------------------------------------------------------------------------------------------------------+
| global types
+---------------------------------------------------------------------------------------------------------------------*/

#ifdef GPIOA
static_assert((reinterpret_cast<uint32_t>(GPIOA) & 15) == 0, "Cannot combine pin number with GPIOA address!");
#endif	// def GPIOA

#ifdef GPIOB
static_assert((reinterpret_cast<uint32_t>(GPIOB) & 15) == 0, "Cannot combine pin number with GPIOB address!");
#endif	// def GPIOB

#ifdef GPIOC
static_assert((reinterpret_cast<uint32_t>(GPIOC) & 15) == 0, "Cannot combine pin number with GPIOC address!");
#endif	// def GPIOC

#ifdef GPIOD
static_assert((reinterpret_cast<uint32_t>(GPIOD) & 15) == 0, "Cannot combine pin number with GPIOD address!");
#endif	// def GPIOD

#ifdef GPIOE
static_assert((reinterpret_cast<uint32_t>(GPIOE) & 15) == 0, "Cannot combine pin number with GPIOE address!");
#endif	// def GPIOE

#ifdef GPIOF
static_assert((reinterpret_cast<uint32_t>(GPIOF) & 15) == 0, "Cannot combine pin number with GPIOF address!");
#endif	// def GPIOF

#ifdef GPIOG
static_assert((reinterpret_cast<uint32_t>(GPIOG) & 15) == 0, "Cannot combine pin number with GPIOG address!");
#endif	// def GPIOG

/// identifier of single pin - combined address of GPIO port with pin number
enum class Pin : uint32_t
{
#ifdef GPIOA

		/// PA0 pin
		pa0 = reinterpret_cast<uint32_t>(GPIOA) | 0,
		/// PA1 pin
		pa1 = reinterpret_cast<uint32_t>(GPIOA) | 1,
		/// PA2 pin
		pa2 = reinterpret_cast<uint32_t>(GPIOA) | 2,
		/// PA3 pin
		pa3 = reinterpret_cast<uint32_t>(GPIOA) | 3,
		/// PA4 pin
		pa4 = reinterpret_cast<uint32_t>(GPIOA) | 4,
		/// PA5 pin
		pa5 = reinterpret_cast<uint32_t>(GPIOA) | 5,
		/// PA6 pin
		pa6 = reinterpret_cast<uint32_t>(GPIOA) | 6,
		/// PA7 pin
		pa7 = reinterpret_cast<uint32_t>(GPIOA) | 7,
		/// PA8 pin
		pa8 = reinterpret_cast<uint32_t>(GPIOA) | 8,
		/// PA9 pin
		pa9 = reinterpret_cast<uint32_t>(GPIOA) | 9,
		/// PA10 pin
		pa10 = reinterpret_cast<uint32_t>(GPIOA) | 10,
		/// PA11 pin
		pa11 = reinterpret_cast<uint32_t>(GPIOA) | 11,
		/// PA12 pin
		pa12 = reinterpret_cast<uint32_t>(GPIOA) | 12,
		/// PA13 pin
		pa13 = reinterpret_cast<uint32_t>(GPIOA) | 13,
		/// PA14 pin
		pa14 = reinterpret_cast<uint32_t>(GPIOA) | 14,
		/// PA15 pin
		pa15 = reinterpret_cast<uint32_t>(GPIOA) | 15,

#endif	// def GPIOA

#ifdef GPIOB

		/// PB0 pin
		pb0 = reinterpret_cast<uint32_t>(GPIOB) | 0,
		/// PB1 pin
		pb1 = reinterpret_cast<uint32_t>(GPIOB) | 1,
		/// PB2 pin
		pb2 = reinterpret_cast<uint32_t>(GPIOB) | 2,
		/// PB3 pin
		pb3 = reinterpret_cast<uint32_t>(GPIOB) | 3,
		/// PB4 pin
		pb4 = reinterpret_cast<uint32_t>(GPIOB) | 4,
		/// PB5 pin
		pb5 = reinterpret_cast<uint32_t>(GPIOB) | 5,
		/// PB6 pin
		pb6 = reinterpret_cast<uint32_t>(GPIOB) | 6,
		/// PB7 pin
		pb7 = reinterpret_cast<uint32_t>(GPIOB) | 7,
		/// PB8 pin
		pb8 = reinterpret_cast<uint32_t>(GPIOB) | 8,
		/// PB9 pin
		pb9 = reinterpret_cast<uint32_t>(GPIOB) | 9,
		/// PB10 pin
		pb10 = reinterpret_cast<uint32_t>(GPIOB) | 10,
		/// PB11 pin
		pb11 = reinterpret_cast<uint32_t>(GPIOB) | 11,
		/// PB12 pin
		pb12 = reinterpret_cast<uint32_t>(GPIOB) | 12,
		/// PB13 pin
		pb13 = reinterpret_cast<uint32_t>(GPIOB) | 13,
		/// PB14 pin
		pb14 = reinterpret_cast<uint32_t>(GPIOB) | 14,
		/// PB15 pin
		pb15 = reinterpret_cast<uint32_t>(GPIOB) | 15,

#endif	// def GPIOB

#ifdef GPIOC

		/// PC0 pin
		pc0 = reinterpret_cast<uint32_t>(GPIOC) | 0,
		/// PC1 pin
		pc1 = reinterpret_cast<uint32_t>(GPIOC) | 1,
		/// PC2 pin
		pc2 = reinterpret_cast<uint32_t>(GPIOC) | 2,
		/// PC3 pin
		pc3 = reinterpret_cast<uint32_t>(GPIOC) | 3,
		/// PC4 pin
		pc4 = reinterpret_cast<uint32_t>(GPIOC) | 4,
		/// PC5 pin
		pc5 = reinterpret_cast<uint32_t>(GPIOC) | 5,
		/// PC6 pin
		pc6 = reinterpret_cast<uint32_t>(GPIOC) | 6,
		/// PC7 pin
		pc7 = reinterpret_cast<uint32_t>(GPIOC) | 7,
		/// PC8 pin
		pc8 = reinterpret_cast<uint32_t>(GPIOC) | 8,
		/// PC9 pin
		pc9 = reinterpret_cast<uint32_t>(GPIOC) | 9,
		/// PC10 pin
		pc10 = reinterpret_cast<uint32_t>(GPIOC) | 10,
		/// PC11 pin
		pc11 = reinterpret_cast<uint32_t>(GPIOC) | 11,
		/// PC12 pin
		pc12 = reinterpret_cast<uint32_t>(GPIOC) | 12,
		/// PC13 pin
		pc13 = reinterpret_cast<uint32_t>(GPIOC) | 13,
		/// PC14 pin
		pc14 = reinterpret_cast<uint32_t>(GPIOC) | 14,
		/// PC15 pin
		pc15 = reinterpret_cast<uint32_t>(GPIOC) | 15,

#endif	// def GPIOC

#ifdef GPIOD

		/// PD0 pin
		pd0 = reinterpret_cast<uint32_t>(GPIOD) | 0,
		/// PD1 pin
		pd1 = reinterpret_cast<uint32_t>(GPIOD) | 1,
		/// PD2 pin
		pd2 = reinterpret_cast<uint32_t>(GPIOD) | 2,
		/// PD3 pin
		pd3 = reinterpret_cast<uint32_t>(GPIOD) | 3,
		/// PD4 pin
		pd4 = reinterpret_cast<uint32_t>(GPIOD) | 4,
		/// PD5 pin
		pd5 = reinterpret_cast<uint32_t>(GPIOD) | 5,
		/// PD6 pin
		pd6 = reinterpret_cast<uint32_t>(GPIOD) | 6,
		/// PD7 pin
		pd7 = reinterpret_cast<uint32_t>(GPIOD) | 7,
		/// PD8 pin
		pd8 = reinterpret_cast<uint32_t>(GPIOD) | 8,
		/// PD9 pin
		pd9 = reinterpret_cast<uint32_t>(GPIOD) | 9,
		/// PD10 pin
		pd10 = reinterpret_cast<uint32_t>(GPIOD) | 10,
		/// PD11 pin
		pd11 = reinterpret_cast<uint32_t>(GPIOD) | 11,
		/// PD12 pin
		pd12 = reinterpret_cast<uint32_t>(GPIOD) | 12,
		/// PD13 pin
		pd13 = reinterpret_cast<uint32_t>(GPIOD) | 13,
		/// PD14 pin
		pd14 = reinterpret_cast<uint32_t>(GPIOD) | 14,
		/// PD15 pin
		pd15 = reinterpret_cast<uint32_t>(GPIOD) | 15,

#endif	// def GPIOD

#ifdef GPIOE

		/// PE0 pin
		pe0 = reinterpret_cast<uint32_t>(GPIOE) | 0,
		/// PE1 pin
		pe1 = reinterpret_cast<uint32_t>(GPIOE) | 1,
		/// PE2 pin
		pe2 = reinterpret_cast<uint32_t>(GPIOE) | 2,
		/// PE3 pin
		pe3 = reinterpret_cast<uint32_t>(GPIOE) | 3,
		/// PE4 pin
		pe4 = reinterpret_cast<uint32_t>(GPIOE) | 4,
		/// PE5 pin
		pe5 = reinterpret_cast<uint32_t>(GPIOE) | 5,
		/// PE6 pin
		pe6 = reinterpret_cast<uint32_t>(GPIOE) | 6,
		/// PE7 pin
		pe7 = reinterpret_cast<uint32_t>(GPIOE) | 7,
		/// PE8 pin
		pe8 = reinterpret_cast<uint32_t>(GPIOE) | 8,
		/// PE9 pin
		pe9 = reinterpret_cast<uint32_t>(GPIOE) | 9,
		/// PE10 pin
		pe10 = reinterpret_cast<uint32_t>(GPIOE) | 10,
		/// PE11 pin
		pe11 = reinterpret_cast<uint32_t>(GPIOE) | 11,
		/// PE12 pin
		pe12 = reinterpret_cast<uint32_t>(GPIOE) | 12,
		/// PE13 pin
		pe13 = reinterpret_cast<uint32_t>(GPIOE) | 13,
		/// PE14 pin
		pe14 = reinterpret_cast<uint32_t>(GPIOE) | 14,
		/// PE15 pin
		pe15 = reinterpret_cast<uint32_t>(GPIOE) | 15,

#endif	// def GPIOE

#ifdef GPIOF

		/// PF0 pin
		pf0 = reinterpret_cast<uint32_t>(GPIOF) | 0,
		/// PF1 pin
		pf1 = reinterpret_cast<uint32_t>(GPIOF) | 1,
		/// PF2 pin
		pf2 = reinterpret_cast<uint32_t>(GPIOF) | 2,
		/// PF3 pin
		pf3 = reinterpret_cast<uint32_t>(GPIOF) | 3,
		/// PF4 pin
		pf4 = reinterpret_cast<uint32_t>(GPIOF) | 4,
		/// PF5 pin
		pf5 = reinterpret_cast<uint32_t>(GPIOF) | 5,
		/// PF6 pin
		pf6 = reinterpret_cast<uint32_t>(GPIOF) | 6,
		/// PF7 pin
		pf7 = reinterpret_cast<uint32_t>(GPIOF) | 7,
		/// PF8 pin
		pf8 = reinterpret_cast<uint32_t>(GPIOF) | 8,
		/// PF9 pin
		pf9 = reinterpret_cast<uint32_t>(GPIOF) | 9,
		/// PF10 pin
		pf10 = reinterpret_cast<uint32_t>(GPIOF) | 10,
		/// PF11 pin
		pf11 = reinterpret_cast<uint32_t>(GPIOF) | 11,
		/// PF12 pin
		pf12 = reinterpret_cast<uint32_t>(GPIOF) | 12,
		/// PF13 pin
		pf13 = reinterpret_cast<uint32_t>(GPIOF) | 13,
		/// PF14 pin
		pf14 = reinterpret_cast<uint32_t>(GPIOF) | 14,
		/// PF15 pin
		pf15 = reinterpret_cast<uint32_t>(GPIOF) | 15,

#endif	// def GPIOF

#ifdef GPIOG

		/// PG0 pin
		pg0 = reinterpret_cast<uint32_t>(GPIOG) | 0,
		/// PG1 pin
		pg1 = reinterpret_cast<uint32_t>(GPIOG) | 1,
		/// PG2 pin
		pg2 = reinterpret_cast<uint32_t>(GPIOG) | 2,
		/// PG3 pin
		pg3 = reinterpret_cast<uint32_t>(GPIOG) | 3,
		/// PG4 pin
		pg4 = reinterpret_cast<uint32_t>(GPIOG) | 4,
		/// PG5 pin
		pg5 = reinterpret_cast<uint32_t>(GPIOG) | 5,
		/// PG6 pin
		pg6 = reinterpret_cast<uint32_t>(GPIOG) | 6,
		/// PG7 pin
		pg7 = reinterpret_cast<uint32_t>(GPIOG) | 7,
		/// PG8 pin
		pg8 = reinterpret_cast<uint32_t>(GPIOG) | 8,
		/// PG9 pin
		pg9 = reinterpret_cast<uint32_t>(GPIOG) | 9,
		/// PG10 pin
		pg10 = reinterpret_cast<uint32_t>(GPIOG) | 10,
		/// PG11 pin
		pg11 = reinterpret_cast<uint32_t>(GPIOG) | 11,
		/// PG12 pin
		pg12 = reinterpret_cast<uint32_t>(GPIOG) | 12,
		/// PG13 pin
		pg13 = reinterpret_cast<uint32_t>(GPIOG) | 13,
		/// PG14 pin
		pg14 = reinterpret_cast<uint32_t>(GPIOG) | 14,
		/// PG15 pin
		pg15 = reinterpret_cast<uint32_t>(GPIOG) | 15,

#endif	// def GPIOG
};

/// all possible configurations of pin
enum class PinConfiguration : uint8_t
{
		/// analog input
		analogInput = (0 << 0) | (0 << 2),
		/// floating input
		floatingInput = (0 << 0) | (1 << 2),
		/// input with pull-up/pull-down
		inputWithPullUpDown = (0 << 0) | (2 << 2),

		/// push-pull output, 10MHz
		pushPull10MhzOutput = (1 << 0) | (0 << 2),
		/// open-drain output, 10MHz
		openDrain10MhzOutput = (1 << 0) | (1 << 2),
		/// push-pull alternate function, 10MHz
		pushPull10MhzAlternateFunction = (1 << 0) | (2 << 2),
		/// open-drain alternate function, 10MHz
		openDrain10MhzAlternateFunction = (1 << 0) | (3 << 2),

		/// push-pull output, 2MHz
		pushPull2MhzOutput = (2 << 0) | (0 << 2),
		/// open-drain output, 2MHz
		openDrain2MhzOutput = (2 << 0) | (1 << 2),
		/// push-pull alternate function, 2MHz
		pushPull2MhzAlternateFunction = (2 << 0) | (2 << 2),
		/// open-drain alternate function, 2MHz
		openDrain2MhzAlternateFunction = (2 << 0) | (3 << 2),

		/// push-pull output, 50MHz
		pushPull50MhzOutput = (3 << 0) | (0 << 2),
		/// open-drain output, 50MHz
		openDrain50MhzOutput = (3 << 0) | (1 << 2),
		/// push-pull alternate function, 50MHz
		pushPull50MhzAlternateFunction = (3 << 0) | (2 << 2),
		/// open-drain alternate function, 50MHz
		openDrain50MhzAlternateFunction = (3 << 0) | (3 << 2),
};

/// all possible output speeds of pin
enum class PinOutputSpeed : uint8_t
{
		/// 2MHz
		_2Mhz,
		/// 10MHz
		_10Mhz,
		/// 50MHz
		_50Mhz,
};

/// all possible pull-up/pull-down configurations of input pin
enum class PinPull : uint8_t
{
		/// no pull-up/pull-down
		none,
		/// pull-up
		up,
		/// pull-down
		down,
};

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief Configures pin.
 *
 * \param [in] pin is the identifier of pin
 * \param [in] configuration is the desired configuration of pin
 * \param [in] initialState is the initial state of pin
 */

void configurePin(Pin pin, PinConfiguration configuration, bool initialState);

/**
 * \brief Configures pin for "input" configuration.
 *
 * \param [in] pin is the identifier of pin
 * \param [in] pull is the desired pull-up/pull-down configuration of pin
 */

inline void configureInputPin(const Pin pin, const PinPull pull)
{
	configurePin(pin, pull == PinPull::none ? PinConfiguration::floatingInput : PinConfiguration::inputWithPullUpDown,
			pull == PinPull::up);
}

/**
 * \brief Configures pin for "output" mode.
 *
 * \param [in] pin is the identifier of pin
 * \param [in] openDrain is the desired output type of pin: push-pull (false) or open-drain (true)
 * \param [in] outputSpeed is the desired output speed of pin
 * \param [in] initialState is the initial state of pin
 */

inline void configureOutputPin(const Pin pin, const bool openDrain, const PinOutputSpeed outputSpeed,
		const bool initialState)
{
	static const PinConfiguration configurations[3][2]
	{
			{PinConfiguration::pushPull2MhzOutput, PinConfiguration::openDrain2MhzOutput},
			{PinConfiguration::pushPull10MhzOutput, PinConfiguration::openDrain10MhzOutput},
			{PinConfiguration::pushPull50MhzOutput, PinConfiguration::openDrain50MhzOutput},
	};
	configurePin(pin, configurations[static_cast<uint8_t>(outputSpeed)][openDrain], initialState);
}

/**
 * \brief Decodes pin identifier into address of associated GPIO port and its pin number
 *
 * \param [in] pin is the identifier of pin
 *
 * \return pair with address of GPIO port and pin number
 */

constexpr std::pair<GPIO_TypeDef*, uint8_t> decodePin(const Pin pin)
{
	return std::make_pair(reinterpret_cast<GPIO_TypeDef*>(static_cast<uint32_t>(pin) & ~15),
			static_cast<uint32_t>(pin) & 15);
}

}	// namespace chip

}	// namespace distortos

#endif	// SOURCE_CHIP_STM32_PERIPHERALS_GPIOV1_INCLUDE_DISTORTOS_CHIP_STM32_GPIOV1_HPP_
