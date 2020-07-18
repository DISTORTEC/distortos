/**
 * \file
 * \brief Header for GPIOv1 functions for STM32
 *
 * \author Copyright (C) 2016-2017 Kamil Szczygiel http://www.distortec.com https://freddiechopin.info
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
static_assert((GPIOA_BASE & 15) == 0, "Cannot combine pin number with GPIOA address!");
#endif	// def GPIOA

#ifdef GPIOB
static_assert((GPIOB_BASE & 15) == 0, "Cannot combine pin number with GPIOB address!");
#endif	// def GPIOB

#ifdef GPIOC
static_assert((GPIOC_BASE & 15) == 0, "Cannot combine pin number with GPIOC address!");
#endif	// def GPIOC

#ifdef GPIOD
static_assert((GPIOD_BASE & 15) == 0, "Cannot combine pin number with GPIOD address!");
#endif	// def GPIOD

#ifdef GPIOE
static_assert((GPIOE_BASE & 15) == 0, "Cannot combine pin number with GPIOE address!");
#endif	// def GPIOE

#ifdef GPIOF
static_assert((GPIOF_BASE & 15) == 0, "Cannot combine pin number with GPIOF address!");
#endif	// def GPIOF

#ifdef GPIOG
static_assert((GPIOG_BASE & 15) == 0, "Cannot combine pin number with GPIOG address!");
#endif	// def GPIOG

/// identifier of single pin - combined address of GPIO port with pin number
enum class Pin : uint32_t
{
#ifdef GPIOA

		/// PA0 pin
		pa0 = GPIOA_BASE | 0,
		/// PA1 pin
		pa1 = GPIOA_BASE | 1,
		/// PA2 pin
		pa2 = GPIOA_BASE | 2,
		/// PA3 pin
		pa3 = GPIOA_BASE | 3,
		/// PA4 pin
		pa4 = GPIOA_BASE | 4,
		/// PA5 pin
		pa5 = GPIOA_BASE | 5,
		/// PA6 pin
		pa6 = GPIOA_BASE | 6,
		/// PA7 pin
		pa7 = GPIOA_BASE | 7,
		/// PA8 pin
		pa8 = GPIOA_BASE | 8,
		/// PA9 pin
		pa9 = GPIOA_BASE | 9,
		/// PA10 pin
		pa10 = GPIOA_BASE | 10,
		/// PA11 pin
		pa11 = GPIOA_BASE | 11,
		/// PA12 pin
		pa12 = GPIOA_BASE | 12,
		/// PA13 pin
		pa13 = GPIOA_BASE | 13,
		/// PA14 pin
		pa14 = GPIOA_BASE | 14,
		/// PA15 pin
		pa15 = GPIOA_BASE | 15,

#endif	// def GPIOA

#ifdef GPIOB

		/// PB0 pin
		pb0 = GPIOB_BASE | 0,
		/// PB1 pin
		pb1 = GPIOB_BASE | 1,
		/// PB2 pin
		pb2 = GPIOB_BASE | 2,
		/// PB3 pin
		pb3 = GPIOB_BASE | 3,
		/// PB4 pin
		pb4 = GPIOB_BASE | 4,
		/// PB5 pin
		pb5 = GPIOB_BASE | 5,
		/// PB6 pin
		pb6 = GPIOB_BASE | 6,
		/// PB7 pin
		pb7 = GPIOB_BASE | 7,
		/// PB8 pin
		pb8 = GPIOB_BASE | 8,
		/// PB9 pin
		pb9 = GPIOB_BASE | 9,
		/// PB10 pin
		pb10 = GPIOB_BASE | 10,
		/// PB11 pin
		pb11 = GPIOB_BASE | 11,
		/// PB12 pin
		pb12 = GPIOB_BASE | 12,
		/// PB13 pin
		pb13 = GPIOB_BASE | 13,
		/// PB14 pin
		pb14 = GPIOB_BASE | 14,
		/// PB15 pin
		pb15 = GPIOB_BASE | 15,

#endif	// def GPIOB

#ifdef GPIOC

		/// PC0 pin
		pc0 = GPIOC_BASE | 0,
		/// PC1 pin
		pc1 = GPIOC_BASE | 1,
		/// PC2 pin
		pc2 = GPIOC_BASE | 2,
		/// PC3 pin
		pc3 = GPIOC_BASE | 3,
		/// PC4 pin
		pc4 = GPIOC_BASE | 4,
		/// PC5 pin
		pc5 = GPIOC_BASE | 5,
		/// PC6 pin
		pc6 = GPIOC_BASE | 6,
		/// PC7 pin
		pc7 = GPIOC_BASE | 7,
		/// PC8 pin
		pc8 = GPIOC_BASE | 8,
		/// PC9 pin
		pc9 = GPIOC_BASE | 9,
		/// PC10 pin
		pc10 = GPIOC_BASE | 10,
		/// PC11 pin
		pc11 = GPIOC_BASE | 11,
		/// PC12 pin
		pc12 = GPIOC_BASE | 12,
		/// PC13 pin
		pc13 = GPIOC_BASE | 13,
		/// PC14 pin
		pc14 = GPIOC_BASE | 14,
		/// PC15 pin
		pc15 = GPIOC_BASE | 15,

#endif	// def GPIOC

#ifdef GPIOD

		/// PD0 pin
		pd0 = GPIOD_BASE | 0,
		/// PD1 pin
		pd1 = GPIOD_BASE | 1,
		/// PD2 pin
		pd2 = GPIOD_BASE | 2,
		/// PD3 pin
		pd3 = GPIOD_BASE | 3,
		/// PD4 pin
		pd4 = GPIOD_BASE | 4,
		/// PD5 pin
		pd5 = GPIOD_BASE | 5,
		/// PD6 pin
		pd6 = GPIOD_BASE | 6,
		/// PD7 pin
		pd7 = GPIOD_BASE | 7,
		/// PD8 pin
		pd8 = GPIOD_BASE | 8,
		/// PD9 pin
		pd9 = GPIOD_BASE | 9,
		/// PD10 pin
		pd10 = GPIOD_BASE | 10,
		/// PD11 pin
		pd11 = GPIOD_BASE | 11,
		/// PD12 pin
		pd12 = GPIOD_BASE | 12,
		/// PD13 pin
		pd13 = GPIOD_BASE | 13,
		/// PD14 pin
		pd14 = GPIOD_BASE | 14,
		/// PD15 pin
		pd15 = GPIOD_BASE | 15,

#endif	// def GPIOD

#ifdef GPIOE

		/// PE0 pin
		pe0 = GPIOE_BASE | 0,
		/// PE1 pin
		pe1 = GPIOE_BASE | 1,
		/// PE2 pin
		pe2 = GPIOE_BASE | 2,
		/// PE3 pin
		pe3 = GPIOE_BASE | 3,
		/// PE4 pin
		pe4 = GPIOE_BASE | 4,
		/// PE5 pin
		pe5 = GPIOE_BASE | 5,
		/// PE6 pin
		pe6 = GPIOE_BASE | 6,
		/// PE7 pin
		pe7 = GPIOE_BASE | 7,
		/// PE8 pin
		pe8 = GPIOE_BASE | 8,
		/// PE9 pin
		pe9 = GPIOE_BASE | 9,
		/// PE10 pin
		pe10 = GPIOE_BASE | 10,
		/// PE11 pin
		pe11 = GPIOE_BASE | 11,
		/// PE12 pin
		pe12 = GPIOE_BASE | 12,
		/// PE13 pin
		pe13 = GPIOE_BASE | 13,
		/// PE14 pin
		pe14 = GPIOE_BASE | 14,
		/// PE15 pin
		pe15 = GPIOE_BASE | 15,

#endif	// def GPIOE

#ifdef GPIOF

		/// PF0 pin
		pf0 = GPIOF_BASE | 0,
		/// PF1 pin
		pf1 = GPIOF_BASE | 1,
		/// PF2 pin
		pf2 = GPIOF_BASE | 2,
		/// PF3 pin
		pf3 = GPIOF_BASE | 3,
		/// PF4 pin
		pf4 = GPIOF_BASE | 4,
		/// PF5 pin
		pf5 = GPIOF_BASE | 5,
		/// PF6 pin
		pf6 = GPIOF_BASE | 6,
		/// PF7 pin
		pf7 = GPIOF_BASE | 7,
		/// PF8 pin
		pf8 = GPIOF_BASE | 8,
		/// PF9 pin
		pf9 = GPIOF_BASE | 9,
		/// PF10 pin
		pf10 = GPIOF_BASE | 10,
		/// PF11 pin
		pf11 = GPIOF_BASE | 11,
		/// PF12 pin
		pf12 = GPIOF_BASE | 12,
		/// PF13 pin
		pf13 = GPIOF_BASE | 13,
		/// PF14 pin
		pf14 = GPIOF_BASE | 14,
		/// PF15 pin
		pf15 = GPIOF_BASE | 15,

#endif	// def GPIOF

#ifdef GPIOG

		/// PG0 pin
		pg0 = GPIOG_BASE | 0,
		/// PG1 pin
		pg1 = GPIOG_BASE | 1,
		/// PG2 pin
		pg2 = GPIOG_BASE | 2,
		/// PG3 pin
		pg3 = GPIOG_BASE | 3,
		/// PG4 pin
		pg4 = GPIOG_BASE | 4,
		/// PG5 pin
		pg5 = GPIOG_BASE | 5,
		/// PG6 pin
		pg6 = GPIOG_BASE | 6,
		/// PG7 pin
		pg7 = GPIOG_BASE | 7,
		/// PG8 pin
		pg8 = GPIOG_BASE | 8,
		/// PG9 pin
		pg9 = GPIOG_BASE | 9,
		/// PG10 pin
		pg10 = GPIOG_BASE | 10,
		/// PG11 pin
		pg11 = GPIOG_BASE | 11,
		/// PG12 pin
		pg12 = GPIOG_BASE | 12,
		/// PG13 pin
		pg13 = GPIOG_BASE | 13,
		/// PG14 pin
		pg14 = GPIOG_BASE | 14,
		/// PG15 pin
		pg15 = GPIOG_BASE | 15,

#endif	// def GPIOG
};

/// all possible configurations of pin
enum class PinConfiguration : uint8_t
{
		/// analog input
		analogInput = 0 << 0 | 0 << 2,
		/// floating input
		floatingInput = 0 << 0 | 1 << 2,
		/// input with pull-up/pull-down
		inputWithPullUpDown = 0 << 0 | 2 << 2,

		/// push-pull output, 10 MHz
		pushPull10MhzOutput = 1 << 0 | 0 << 2,
		/// open-drain output, 10 MHz
		openDrain10MhzOutput = 1 << 0 | 1 << 2,
		/// push-pull alternate function, 10 MHz
		pushPull10MhzAlternateFunction = 1 << 0 | 2 << 2,
		/// open-drain alternate function, 10 MHz
		openDrain10MhzAlternateFunction = 1 << 0 | 3 << 2,

		/// push-pull output, 2 MHz
		pushPull2MhzOutput = 2 << 0 | 0 << 2,
		/// open-drain output, 2 MHz
		openDrain2MhzOutput = 2 << 0 | 1 << 2,
		/// push-pull alternate function, 2 MHz
		pushPull2MhzAlternateFunction = 2 << 0 | 2 << 2,
		/// open-drain alternate function, 2 MHz
		openDrain2MhzAlternateFunction = 2 << 0 | 3 << 2,

		/// push-pull output, 50 MHz
		pushPull50MhzOutput = 3 << 0 | 0 << 2,
		/// open-drain output, 50 MHz
		openDrain50MhzOutput = 3 << 0 | 1 << 2,
		/// push-pull alternate function, 50 MHz
		pushPull50MhzAlternateFunction = 3 << 0 | 2 << 2,
		/// open-drain alternate function, 50 MHz
		openDrain50MhzAlternateFunction = 3 << 0 | 3 << 2,
};

/// all possible output speeds of pin
enum class PinOutputSpeed : uint8_t
{
		/// 2 MHz
		_2Mhz,
		/// 10 MHz
		_10Mhz,
		/// 50 MHz
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
 * \param [in] initialState is the initial state of pin, default - false
 */

void configurePin(Pin pin, PinConfiguration configuration, bool initialState = {});

/**
 * \brief Configures pin for "input" configuration.
 *
 * \param [in] pin is the identifier of pin
 * \param [in] pull is the desired pull-up/pull-down configuration of pin, default - PinPull::none
 */

inline void configureInputPin(const Pin pin, const PinPull pull = {})
{
	configurePin(pin, pull == PinPull::none ? PinConfiguration::floatingInput : PinConfiguration::inputWithPullUpDown,
			pull == PinPull::up);
}

/**
 * \brief Configures pin for "output" mode.
 *
 * \param [in] pin is the identifier of pin
 * \param [in] openDrain is the desired output type of pin: push-pull (false) or open-drain (true), default - false
 * \param [in] outputSpeed is the desired output speed of pin, default - PinOutputSpeed::_2Mhz
 * \param [in] initialState is the initial state of pin, default - false
 */

inline void configureOutputPin(const Pin pin, const bool openDrain = {}, const PinOutputSpeed outputSpeed = {},
		const bool initialState = {})
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
