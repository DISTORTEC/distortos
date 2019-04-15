/**
 * \file
 * \brief Header for GPIOv2 functions for STM32
 *
 * \author Copyright (C) 2016-2019 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef SOURCE_CHIP_STM32_PERIPHERALS_GPIOV2_INCLUDE_DISTORTOS_CHIP_STM32_GPIOV2_HPP_
#define SOURCE_CHIP_STM32_PERIPHERALS_GPIOV2_INCLUDE_DISTORTOS_CHIP_STM32_GPIOV2_HPP_

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

#ifdef GPIOH
static_assert((GPIOH_BASE & 15) == 0, "Cannot combine pin number with GPIOH address!");
#endif	// def GPIOH

#ifdef GPIOI
static_assert((GPIOI_BASE & 15) == 0, "Cannot combine pin number with GPIOI address!");
#endif	// def GPIOI

#ifdef GPIOJ
static_assert((GPIOJ_BASE & 15) == 0, "Cannot combine pin number with GPIOJ address!");
#endif	// def GPIOJ

#ifdef GPIOK
static_assert((GPIOK_BASE & 15) == 0, "Cannot combine pin number with GPIOK address!");
#endif	// def GPIOK

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

#ifdef GPIOH

		/// PH0 pin
		ph0 = GPIOH_BASE | 0,
		/// PH1 pin
		ph1 = GPIOH_BASE | 1,
		/// PH2 pin
		ph2 = GPIOH_BASE | 2,
		/// PH3 pin
		ph3 = GPIOH_BASE | 3,
		/// PH4 pin
		ph4 = GPIOH_BASE | 4,
		/// PH5 pin
		ph5 = GPIOH_BASE | 5,
		/// PH6 pin
		ph6 = GPIOH_BASE | 6,
		/// PH7 pin
		ph7 = GPIOH_BASE | 7,
		/// PH8 pin
		ph8 = GPIOH_BASE | 8,
		/// PH9 pin
		ph9 = GPIOH_BASE | 9,
		/// PH10 pin
		ph10 = GPIOH_BASE | 10,
		/// PH11 pin
		ph11 = GPIOH_BASE | 11,
		/// PH12 pin
		ph12 = GPIOH_BASE | 12,
		/// PH13 pin
		ph13 = GPIOH_BASE | 13,
		/// PH14 pin
		ph14 = GPIOH_BASE | 14,
		/// PH15 pin
		ph15 = GPIOH_BASE | 15,

#endif	// def GPIOH

#ifdef GPIOI

		/// PI0 pin
		pi0 = GPIOI_BASE | 0,
		/// PI1 pin
		pi1 = GPIOI_BASE | 1,
		/// PI2 pin
		pi2 = GPIOI_BASE | 2,
		/// PI3 pin
		pi3 = GPIOI_BASE | 3,
		/// PI4 pin
		pi4 = GPIOI_BASE | 4,
		/// PI5 pin
		pi5 = GPIOI_BASE | 5,
		/// PI6 pin
		pi6 = GPIOI_BASE | 6,
		/// PI7 pin
		pi7 = GPIOI_BASE | 7,
		/// PI8 pin
		pi8 = GPIOI_BASE | 8,
		/// PI9 pin
		pi9 = GPIOI_BASE | 9,
		/// PI10 pin
		pi10 = GPIOI_BASE | 10,
		/// PI11 pin
		pi11 = GPIOI_BASE | 11,
		/// PI12 pin
		pi12 = GPIOI_BASE | 12,
		/// PI13 pin
		pi13 = GPIOI_BASE | 13,
		/// PI14 pin
		pi14 = GPIOI_BASE | 14,
		/// PI15 pin
		pi15 = GPIOI_BASE | 15,

#endif	// def GPIOI

#ifdef GPIOJ

		/// PJ0 pin
		pj0 = GPIOJ_BASE | 0,
		/// PJ1 pin
		pj1 = GPIOJ_BASE | 1,
		/// PJ2 pin
		pj2 = GPIOJ_BASE | 2,
		/// PJ3 pin
		pj3 = GPIOJ_BASE | 3,
		/// PJ4 pin
		pj4 = GPIOJ_BASE | 4,
		/// PJ5 pin
		pj5 = GPIOJ_BASE | 5,
		/// PJ6 pin
		pj6 = GPIOJ_BASE | 6,
		/// PJ7 pin
		pj7 = GPIOJ_BASE | 7,
		/// PJ8 pin
		pj8 = GPIOJ_BASE | 8,
		/// PJ9 pin
		pj9 = GPIOJ_BASE | 9,
		/// PJ10 pin
		pj10 = GPIOJ_BASE | 10,
		/// PJ11 pin
		pj11 = GPIOJ_BASE | 11,
		/// PJ12 pin
		pj12 = GPIOJ_BASE | 12,
		/// PJ13 pin
		pj13 = GPIOJ_BASE | 13,
		/// PJ14 pin
		pj14 = GPIOJ_BASE | 14,
		/// PJ15 pin
		pj15 = GPIOJ_BASE | 15,

#endif	// def GPIOJ

#ifdef GPIOK

		/// PK0 pin
		pk0 = GPIOK_BASE | 0,
		/// PK1 pin
		pk1 = GPIOK_BASE | 1,
		/// PK2 pin
		pk2 = GPIOK_BASE | 2,
		/// PK3 pin
		pk3 = GPIOK_BASE | 3,
		/// PK4 pin
		pk4 = GPIOK_BASE | 4,
		/// PK5 pin
		pk5 = GPIOK_BASE | 5,
		/// PK6 pin
		pk6 = GPIOK_BASE | 6,
		/// PK7 pin
		pk7 = GPIOK_BASE | 7,
		/// PK8 pin
		pk8 = GPIOK_BASE | 8,
		/// PK9 pin
		pk9 = GPIOK_BASE | 9,
		/// PK10 pin
		pk10 = GPIOK_BASE | 10,
		/// PK11 pin
		pk11 = GPIOK_BASE | 11,
		/// PK12 pin
		pk12 = GPIOK_BASE | 12,
		/// PK13 pin
		pk13 = GPIOK_BASE | 13,
		/// PK14 pin
		pk14 = GPIOK_BASE | 14,
		/// PK15 pin
		pk15 = GPIOK_BASE | 15,

#endif	// def GPIOK
};

/// all possible alternate functions of pin
enum class PinAlternateFunction : uint8_t
{
		/// alternate function 0
		af0,
		/// alternate function 1
		af1,
		/// alternate function 2
		af2,
		/// alternate function 3
		af3,
		/// alternate function 4
		af4,
		/// alternate function 5
		af5,
		/// alternate function 6
		af6,
		/// alternate function 7
		af7,
#ifdef DISTORTOS_CHIP_STM32_GPIOV2_HAS_4_AF_BITS
		/// alternate function 8
		af8,
		/// alternate function 9
		af9,
		/// alternate function 10
		af10,
		/// alternate function 11
		af11,
		/// alternate function 12
		af12,
		/// alternate function 13
		af13,
		/// alternate function 14
		af14,
		/// alternate function 15
		af15,
#endif	// def DISTORTOS_CHIP_STM32_GPIOV2_HAS_4_AF_BITS
};

/// all possible modes of pin
enum class PinMode : uint8_t
{
		/// input
		input,
		/// output
		output,
		/// alternate function
		alternateFunction,
		/// analog
		analog,
};

/// all possible output speeds of pin
enum class PinOutputSpeed : uint8_t
{
		/// low speed
		low,
		/// medium speed
		medium,
#ifdef DISTORTOS_CHIP_STM32_GPIOV2_HAS_HIGH_SPEED
		/// high speed
		high,
#endif	// def DISTORTOS_CHIP_STM32_GPIOV2_HAS_HIGH_SPEED
		/// very high speed
		veryHigh = 3,
};

/// all possible pull-up/pull-down configurations of pin
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
 * \param [in] mode is the desired mode of pin
 * \param [in] openDrain is the desired output type of pin: push-pull (false) or open-drain (true)
 * \param [in] outputSpeed is the desired output speed of pin
 * \param [in] pull is the desired pull-up/pull-down configuration of pin
 * \param [in] alternateFunction is the desired alternate function of pin
 * \param [in] initialState is the initial state of pin, default - false
 */

void configurePin(Pin pin, PinMode mode, bool openDrain, PinOutputSpeed outputSpeed, PinPull pull,
		PinAlternateFunction alternateFunction, bool initialState = {});

/**
 * \brief Configures pin for "alternate function" mode.
 *
 * \param [in] pin is the identifier of pin
 * \param [in] alternateFunction is the desired alternate function of pin
 * \param [in] openDrain is the desired output type of pin: push-pull (false) or open-drain (true), default - false
 * \param [in] outputSpeed is the desired output speed of pin, default - PinOutputSpeed::low
 * \param [in] pull is the desired pull-up/pull-down configuration of pin, default - PinPull::none
 */

inline void configureAlternateFunctionPin(const Pin pin, const PinAlternateFunction alternateFunction,
		const bool openDrain = {}, const PinOutputSpeed outputSpeed = {}, const PinPull pull = {})
{
	configurePin(pin, PinMode::alternateFunction, openDrain, outputSpeed, pull, alternateFunction, {});
}

/**
 * \brief Configures pin for "analog" mode.
 *
 * \param [in] pin is the identifier of pin
 */

inline void configureAnalogPin(const Pin pin)
{
	configurePin(pin, PinMode::analog, {}, {}, {}, {}, {});
}

/**
 * \brief Configures pin for "input" mode.
 *
 * \param [in] pin is the identifier of pin
 * \param [in] pull is the desired pull-up/pull-down configuration of pin, default - PinPull::none
 */

inline void configureInputPin(const Pin pin, const PinPull pull = {})
{
	configurePin(pin, PinMode::input, {}, {}, pull, {}, {});
}

/**
 * \brief Configures pin for "output" mode.
 *
 * \param [in] pin is the identifier of pin
 * \param [in] openDrain is the desired output type of pin: push-pull (false) or open-drain (true), default - false
 * \param [in] outputSpeed is the desired output speed of pin, default - PinOutputSpeed::low
 * \param [in] pull is the desired pull-up/pull-down configuration of pin, default - PinPull::none
 * \param [in] initialState is the initial state of pin, default - false
 */

inline void configureOutputPin(const Pin pin, const bool openDrain = {}, const PinOutputSpeed outputSpeed = {},
		const PinPull pull = {}, const bool initialState = {})
{
	configurePin(pin, PinMode::output, openDrain, outputSpeed, pull, {}, initialState);
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

#endif	// SOURCE_CHIP_STM32_PERIPHERALS_GPIOV2_INCLUDE_DISTORTOS_CHIP_STM32_GPIOV2_HPP_
