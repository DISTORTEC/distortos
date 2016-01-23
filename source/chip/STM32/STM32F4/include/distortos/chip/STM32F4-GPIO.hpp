/**
 * \file
 * \brief Header for GPIO-related functions for STM32F4
 *
 * This file covers devices as described in following places:
 * - RM0368 reference manual (STM32F401xB/C and STM32F401xD/E), Revision 4, 2015-05-04
 * - RM0090 reference manual (STM32F405/415, STM32F407/417, STM32F427/437 and STM32F429/439), Revision 11, 2015-10-20
 * - RM0401 reference manual (STM32F410), Revision 2, 2015-10-26
 * - RM0383 reference manual (STM32F411xC/E), Revision 1, 2014-07-24
 * - RM0390 reference manual (STM32F446xx), Revision 1, 2015-03-17
 * - RM0386 reference manual (STM32F469xx and STM32F479xx), Revision 2, 2015-11-19
 *
 * \author Copyright (C) 2016 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef SOURCE_CHIP_STM32_STM32F4_INCLUDE_DISTORTOS_CHIP_STM32F4_GPIO_HPP_
#define SOURCE_CHIP_STM32_STM32F4_INCLUDE_DISTORTOS_CHIP_STM32F4_GPIO_HPP_

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

#ifdef GPIOH
static_assert((reinterpret_cast<uint32_t>(GPIOH) & 15) == 0, "Cannot combine pin number with GPIOH address!");
#endif	// def GPIOH

#ifdef GPIOI
static_assert((reinterpret_cast<uint32_t>(GPIOI) & 15) == 0, "Cannot combine pin number with GPIOI address!");
#endif	// def GPIOI

#ifdef GPIOJ
static_assert((reinterpret_cast<uint32_t>(GPIOJ) & 15) == 0, "Cannot combine pin number with GPIOJ address!");
#endif	// def GPIOJ

#ifdef GPIOK
static_assert((reinterpret_cast<uint32_t>(GPIOK) & 15) == 0, "Cannot combine pin number with GPIOK address!");
#endif	// def GPIOK

/// identifier of single pin - combined address of GPIO port with pin number
enum class Pin : uint32_t
{
#ifdef GPIOA

		/// PA0 pin
		PA0 = reinterpret_cast<uint32_t>(GPIOA) | 0,
		/// PA1 pin
		PA1 = reinterpret_cast<uint32_t>(GPIOA) | 1,
		/// PA2 pin
		PA2 = reinterpret_cast<uint32_t>(GPIOA) | 2,
		/// PA3 pin
		PA3 = reinterpret_cast<uint32_t>(GPIOA) | 3,
		/// PA4 pin
		PA4 = reinterpret_cast<uint32_t>(GPIOA) | 4,
		/// PA5 pin
		PA5 = reinterpret_cast<uint32_t>(GPIOA) | 5,
		/// PA6 pin
		PA6 = reinterpret_cast<uint32_t>(GPIOA) | 6,
		/// PA7 pin
		PA7 = reinterpret_cast<uint32_t>(GPIOA) | 7,
		/// PA8 pin
		PA8 = reinterpret_cast<uint32_t>(GPIOA) | 8,
		/// PA9 pin
		PA9 = reinterpret_cast<uint32_t>(GPIOA) | 9,
		/// PA10 pin
		PA10 = reinterpret_cast<uint32_t>(GPIOA) | 10,
		/// PA11 pin
		PA11 = reinterpret_cast<uint32_t>(GPIOA) | 11,
		/// PA12 pin
		PA12 = reinterpret_cast<uint32_t>(GPIOA) | 12,
		/// PA13 pin
		PA13 = reinterpret_cast<uint32_t>(GPIOA) | 13,
		/// PA14 pin
		PA14 = reinterpret_cast<uint32_t>(GPIOA) | 14,
		/// PA15 pin
		PA15 = reinterpret_cast<uint32_t>(GPIOA) | 15,

#endif	// def GPIOA

#ifdef GPIOB

		/// PB0 pin
		PB0 = reinterpret_cast<uint32_t>(GPIOB) | 0,
		/// PB1 pin
		PB1 = reinterpret_cast<uint32_t>(GPIOB) | 1,
		/// PB2 pin
		PB2 = reinterpret_cast<uint32_t>(GPIOB) | 2,
		/// PB3 pin
		PB3 = reinterpret_cast<uint32_t>(GPIOB) | 3,
		/// PB4 pin
		PB4 = reinterpret_cast<uint32_t>(GPIOB) | 4,
		/// PB5 pin
		PB5 = reinterpret_cast<uint32_t>(GPIOB) | 5,
		/// PB6 pin
		PB6 = reinterpret_cast<uint32_t>(GPIOB) | 6,
		/// PB7 pin
		PB7 = reinterpret_cast<uint32_t>(GPIOB) | 7,
		/// PB8 pin
		PB8 = reinterpret_cast<uint32_t>(GPIOB) | 8,
		/// PB9 pin
		PB9 = reinterpret_cast<uint32_t>(GPIOB) | 9,
		/// PB10 pin
		PB10 = reinterpret_cast<uint32_t>(GPIOB) | 10,
		/// PB11 pin
		PB11 = reinterpret_cast<uint32_t>(GPIOB) | 11,
		/// PB12 pin
		PB12 = reinterpret_cast<uint32_t>(GPIOB) | 12,
		/// PB13 pin
		PB13 = reinterpret_cast<uint32_t>(GPIOB) | 13,
		/// PB14 pin
		PB14 = reinterpret_cast<uint32_t>(GPIOB) | 14,
		/// PB15 pin
		PB15 = reinterpret_cast<uint32_t>(GPIOB) | 15,

#endif	// def GPIOB

#ifdef GPIOC

		/// PC0 pin
		PC0 = reinterpret_cast<uint32_t>(GPIOC) | 0,
		/// PC1 pin
		PC1 = reinterpret_cast<uint32_t>(GPIOC) | 1,
		/// PC2 pin
		PC2 = reinterpret_cast<uint32_t>(GPIOC) | 2,
		/// PC3 pin
		PC3 = reinterpret_cast<uint32_t>(GPIOC) | 3,
		/// PC4 pin
		PC4 = reinterpret_cast<uint32_t>(GPIOC) | 4,
		/// PC5 pin
		PC5 = reinterpret_cast<uint32_t>(GPIOC) | 5,
		/// PC6 pin
		PC6 = reinterpret_cast<uint32_t>(GPIOC) | 6,
		/// PC7 pin
		PC7 = reinterpret_cast<uint32_t>(GPIOC) | 7,
		/// PC8 pin
		PC8 = reinterpret_cast<uint32_t>(GPIOC) | 8,
		/// PC9 pin
		PC9 = reinterpret_cast<uint32_t>(GPIOC) | 9,
		/// PC10 pin
		PC10 = reinterpret_cast<uint32_t>(GPIOC) | 10,
		/// PC11 pin
		PC11 = reinterpret_cast<uint32_t>(GPIOC) | 11,
		/// PC12 pin
		PC12 = reinterpret_cast<uint32_t>(GPIOC) | 12,
		/// PC13 pin
		PC13 = reinterpret_cast<uint32_t>(GPIOC) | 13,
		/// PC14 pin
		PC14 = reinterpret_cast<uint32_t>(GPIOC) | 14,
		/// PC15 pin
		PC15 = reinterpret_cast<uint32_t>(GPIOC) | 15,

#endif	// def GPIOC

#ifdef GPIOD

		/// PD0 pin
		PD0 = reinterpret_cast<uint32_t>(GPIOD) | 0,
		/// PD1 pin
		PD1 = reinterpret_cast<uint32_t>(GPIOD) | 1,
		/// PD2 pin
		PD2 = reinterpret_cast<uint32_t>(GPIOD) | 2,
		/// PD3 pin
		PD3 = reinterpret_cast<uint32_t>(GPIOD) | 3,
		/// PD4 pin
		PD4 = reinterpret_cast<uint32_t>(GPIOD) | 4,
		/// PD5 pin
		PD5 = reinterpret_cast<uint32_t>(GPIOD) | 5,
		/// PD6 pin
		PD6 = reinterpret_cast<uint32_t>(GPIOD) | 6,
		/// PD7 pin
		PD7 = reinterpret_cast<uint32_t>(GPIOD) | 7,
		/// PD8 pin
		PD8 = reinterpret_cast<uint32_t>(GPIOD) | 8,
		/// PD9 pin
		PD9 = reinterpret_cast<uint32_t>(GPIOD) | 9,
		/// PD10 pin
		PD10 = reinterpret_cast<uint32_t>(GPIOD) | 10,
		/// PD11 pin
		PD11 = reinterpret_cast<uint32_t>(GPIOD) | 11,
		/// PD12 pin
		PD12 = reinterpret_cast<uint32_t>(GPIOD) | 12,
		/// PD13 pin
		PD13 = reinterpret_cast<uint32_t>(GPIOD) | 13,
		/// PD14 pin
		PD14 = reinterpret_cast<uint32_t>(GPIOD) | 14,
		/// PD15 pin
		PD15 = reinterpret_cast<uint32_t>(GPIOD) | 15,

#endif	// def GPIOD

#ifdef GPIOE

		/// PE0 pin
		PE0 = reinterpret_cast<uint32_t>(GPIOE) | 0,
		/// PE1 pin
		PE1 = reinterpret_cast<uint32_t>(GPIOE) | 1,
		/// PE2 pin
		PE2 = reinterpret_cast<uint32_t>(GPIOE) | 2,
		/// PE3 pin
		PE3 = reinterpret_cast<uint32_t>(GPIOE) | 3,
		/// PE4 pin
		PE4 = reinterpret_cast<uint32_t>(GPIOE) | 4,
		/// PE5 pin
		PE5 = reinterpret_cast<uint32_t>(GPIOE) | 5,
		/// PE6 pin
		PE6 = reinterpret_cast<uint32_t>(GPIOE) | 6,
		/// PE7 pin
		PE7 = reinterpret_cast<uint32_t>(GPIOE) | 7,
		/// PE8 pin
		PE8 = reinterpret_cast<uint32_t>(GPIOE) | 8,
		/// PE9 pin
		PE9 = reinterpret_cast<uint32_t>(GPIOE) | 9,
		/// PE10 pin
		PE10 = reinterpret_cast<uint32_t>(GPIOE) | 10,
		/// PE11 pin
		PE11 = reinterpret_cast<uint32_t>(GPIOE) | 11,
		/// PE12 pin
		PE12 = reinterpret_cast<uint32_t>(GPIOE) | 12,
		/// PE13 pin
		PE13 = reinterpret_cast<uint32_t>(GPIOE) | 13,
		/// PE14 pin
		PE14 = reinterpret_cast<uint32_t>(GPIOE) | 14,
		/// PE15 pin
		PE15 = reinterpret_cast<uint32_t>(GPIOE) | 15,

#endif	// def GPIOE

#ifdef GPIOF

		/// PF0 pin
		PF0 = reinterpret_cast<uint32_t>(GPIOF) | 0,
		/// PF1 pin
		PF1 = reinterpret_cast<uint32_t>(GPIOF) | 1,
		/// PF2 pin
		PF2 = reinterpret_cast<uint32_t>(GPIOF) | 2,
		/// PF3 pin
		PF3 = reinterpret_cast<uint32_t>(GPIOF) | 3,
		/// PF4 pin
		PF4 = reinterpret_cast<uint32_t>(GPIOF) | 4,
		/// PF5 pin
		PF5 = reinterpret_cast<uint32_t>(GPIOF) | 5,
		/// PF6 pin
		PF6 = reinterpret_cast<uint32_t>(GPIOF) | 6,
		/// PF7 pin
		PF7 = reinterpret_cast<uint32_t>(GPIOF) | 7,
		/// PF8 pin
		PF8 = reinterpret_cast<uint32_t>(GPIOF) | 8,
		/// PF9 pin
		PF9 = reinterpret_cast<uint32_t>(GPIOF) | 9,
		/// PF10 pin
		PF10 = reinterpret_cast<uint32_t>(GPIOF) | 10,
		/// PF11 pin
		PF11 = reinterpret_cast<uint32_t>(GPIOF) | 11,
		/// PF12 pin
		PF12 = reinterpret_cast<uint32_t>(GPIOF) | 12,
		/// PF13 pin
		PF13 = reinterpret_cast<uint32_t>(GPIOF) | 13,
		/// PF14 pin
		PF14 = reinterpret_cast<uint32_t>(GPIOF) | 14,
		/// PF15 pin
		PF15 = reinterpret_cast<uint32_t>(GPIOF) | 15,

#endif	// def GPIOF

#ifdef GPIOG

		/// PG0 pin
		PG0 = reinterpret_cast<uint32_t>(GPIOG) | 0,
		/// PG1 pin
		PG1 = reinterpret_cast<uint32_t>(GPIOG) | 1,
		/// PG2 pin
		PG2 = reinterpret_cast<uint32_t>(GPIOG) | 2,
		/// PG3 pin
		PG3 = reinterpret_cast<uint32_t>(GPIOG) | 3,
		/// PG4 pin
		PG4 = reinterpret_cast<uint32_t>(GPIOG) | 4,
		/// PG5 pin
		PG5 = reinterpret_cast<uint32_t>(GPIOG) | 5,
		/// PG6 pin
		PG6 = reinterpret_cast<uint32_t>(GPIOG) | 6,
		/// PG7 pin
		PG7 = reinterpret_cast<uint32_t>(GPIOG) | 7,
		/// PG8 pin
		PG8 = reinterpret_cast<uint32_t>(GPIOG) | 8,
		/// PG9 pin
		PG9 = reinterpret_cast<uint32_t>(GPIOG) | 9,
		/// PG10 pin
		PG10 = reinterpret_cast<uint32_t>(GPIOG) | 10,
		/// PG11 pin
		PG11 = reinterpret_cast<uint32_t>(GPIOG) | 11,
		/// PG12 pin
		PG12 = reinterpret_cast<uint32_t>(GPIOG) | 12,
		/// PG13 pin
		PG13 = reinterpret_cast<uint32_t>(GPIOG) | 13,
		/// PG14 pin
		PG14 = reinterpret_cast<uint32_t>(GPIOG) | 14,
		/// PG15 pin
		PG15 = reinterpret_cast<uint32_t>(GPIOG) | 15,

#endif	// def GPIOG

#ifdef GPIOH

		/// PH0 pin
		PH0 = reinterpret_cast<uint32_t>(GPIOH) | 0,
		/// PH1 pin
		PH1 = reinterpret_cast<uint32_t>(GPIOH) | 1,
		/// PH2 pin
		PH2 = reinterpret_cast<uint32_t>(GPIOH) | 2,
		/// PH3 pin
		PH3 = reinterpret_cast<uint32_t>(GPIOH) | 3,
		/// PH4 pin
		PH4 = reinterpret_cast<uint32_t>(GPIOH) | 4,
		/// PH5 pin
		PH5 = reinterpret_cast<uint32_t>(GPIOH) | 5,
		/// PH6 pin
		PH6 = reinterpret_cast<uint32_t>(GPIOH) | 6,
		/// PH7 pin
		PH7 = reinterpret_cast<uint32_t>(GPIOH) | 7,
		/// PH8 pin
		PH8 = reinterpret_cast<uint32_t>(GPIOH) | 8,
		/// PH9 pin
		PH9 = reinterpret_cast<uint32_t>(GPIOH) | 9,
		/// PH10 pin
		PH10 = reinterpret_cast<uint32_t>(GPIOH) | 10,
		/// PH11 pin
		PH11 = reinterpret_cast<uint32_t>(GPIOH) | 11,
		/// PH12 pin
		PH12 = reinterpret_cast<uint32_t>(GPIOH) | 12,
		/// PH13 pin
		PH13 = reinterpret_cast<uint32_t>(GPIOH) | 13,
		/// PH14 pin
		PH14 = reinterpret_cast<uint32_t>(GPIOH) | 14,
		/// PH15 pin
		PH15 = reinterpret_cast<uint32_t>(GPIOH) | 15,

#endif	// def GPIOH

#ifdef GPIOI

		/// PI0 pin
		PI0 = reinterpret_cast<uint32_t>(GPIOI) | 0,
		/// PI1 pin
		PI1 = reinterpret_cast<uint32_t>(GPIOI) | 1,
		/// PI2 pin
		PI2 = reinterpret_cast<uint32_t>(GPIOI) | 2,
		/// PI3 pin
		PI3 = reinterpret_cast<uint32_t>(GPIOI) | 3,
		/// PI4 pin
		PI4 = reinterpret_cast<uint32_t>(GPIOI) | 4,
		/// PI5 pin
		PI5 = reinterpret_cast<uint32_t>(GPIOI) | 5,
		/// PI6 pin
		PI6 = reinterpret_cast<uint32_t>(GPIOI) | 6,
		/// PI7 pin
		PI7 = reinterpret_cast<uint32_t>(GPIOI) | 7,
		/// PI8 pin
		PI8 = reinterpret_cast<uint32_t>(GPIOI) | 8,
		/// PI9 pin
		PI9 = reinterpret_cast<uint32_t>(GPIOI) | 9,
		/// PI10 pin
		PI10 = reinterpret_cast<uint32_t>(GPIOI) | 10,
		/// PI11 pin
		PI11 = reinterpret_cast<uint32_t>(GPIOI) | 11,
		/// PI12 pin
		PI12 = reinterpret_cast<uint32_t>(GPIOI) | 12,
		/// PI13 pin
		PI13 = reinterpret_cast<uint32_t>(GPIOI) | 13,
		/// PI14 pin
		PI14 = reinterpret_cast<uint32_t>(GPIOI) | 14,
		/// PI15 pin
		PI15 = reinterpret_cast<uint32_t>(GPIOI) | 15,

#endif	// def GPIOI

#ifdef GPIOJ

		/// PJ0 pin
		PJ0 = reinterpret_cast<uint32_t>(GPIOJ) | 0,
		/// PJ1 pin
		PJ1 = reinterpret_cast<uint32_t>(GPIOJ) | 1,
		/// PJ2 pin
		PJ2 = reinterpret_cast<uint32_t>(GPIOJ) | 2,
		/// PJ3 pin
		PJ3 = reinterpret_cast<uint32_t>(GPIOJ) | 3,
		/// PJ4 pin
		PJ4 = reinterpret_cast<uint32_t>(GPIOJ) | 4,
		/// PJ5 pin
		PJ5 = reinterpret_cast<uint32_t>(GPIOJ) | 5,
		/// PJ6 pin
		PJ6 = reinterpret_cast<uint32_t>(GPIOJ) | 6,
		/// PJ7 pin
		PJ7 = reinterpret_cast<uint32_t>(GPIOJ) | 7,
		/// PJ8 pin
		PJ8 = reinterpret_cast<uint32_t>(GPIOJ) | 8,
		/// PJ9 pin
		PJ9 = reinterpret_cast<uint32_t>(GPIOJ) | 9,
		/// PJ10 pin
		PJ10 = reinterpret_cast<uint32_t>(GPIOJ) | 10,
		/// PJ11 pin
		PJ11 = reinterpret_cast<uint32_t>(GPIOJ) | 11,
		/// PJ12 pin
		PJ12 = reinterpret_cast<uint32_t>(GPIOJ) | 12,
		/// PJ13 pin
		PJ13 = reinterpret_cast<uint32_t>(GPIOJ) | 13,
		/// PJ14 pin
		PJ14 = reinterpret_cast<uint32_t>(GPIOJ) | 14,
		/// PJ15 pin
		PJ15 = reinterpret_cast<uint32_t>(GPIOJ) | 15,

#endif	// def GPIOJ

#ifdef GPIOK

		/// PK0 pin
		PK0 = reinterpret_cast<uint32_t>(GPIOK) | 0,
		/// PK1 pin
		PK1 = reinterpret_cast<uint32_t>(GPIOK) | 1,
		/// PK2 pin
		PK2 = reinterpret_cast<uint32_t>(GPIOK) | 2,
		/// PK3 pin
		PK3 = reinterpret_cast<uint32_t>(GPIOK) | 3,
		/// PK4 pin
		PK4 = reinterpret_cast<uint32_t>(GPIOK) | 4,
		/// PK5 pin
		PK5 = reinterpret_cast<uint32_t>(GPIOK) | 5,
		/// PK6 pin
		PK6 = reinterpret_cast<uint32_t>(GPIOK) | 6,
		/// PK7 pin
		PK7 = reinterpret_cast<uint32_t>(GPIOK) | 7,
		/// PK8 pin
		PK8 = reinterpret_cast<uint32_t>(GPIOK) | 8,
		/// PK9 pin
		PK9 = reinterpret_cast<uint32_t>(GPIOK) | 9,
		/// PK10 pin
		PK10 = reinterpret_cast<uint32_t>(GPIOK) | 10,
		/// PK11 pin
		PK11 = reinterpret_cast<uint32_t>(GPIOK) | 11,
		/// PK12 pin
		PK12 = reinterpret_cast<uint32_t>(GPIOK) | 12,
		/// PK13 pin
		PK13 = reinterpret_cast<uint32_t>(GPIOK) | 13,
		/// PK14 pin
		PK14 = reinterpret_cast<uint32_t>(GPIOK) | 14,
		/// PK15 pin
		PK15 = reinterpret_cast<uint32_t>(GPIOK) | 15,

#endif	// def GPIOK
};

/// all possible alternate functions of pin
enum class PinAlternateFunction : uint8_t
{
		/// alternate function 0
		AF0,
		/// alternate function 1
		AF1,
		/// alternate function 2
		AF2,
		/// alternate function 3
		AF3,
		/// alternate function 4
		AF4,
		/// alternate function 5
		AF5,
		/// alternate function 6
		AF6,
		/// alternate function 7
		AF7,
		/// alternate function 8
		AF8,
		/// alternate function 9
		AF9,
		/// alternate function 10
		AF10,
		/// alternate function 11
		AF11,
		/// alternate function 12
		AF12,
		/// alternate function 13
		AF13,
		/// alternate function 14
		AF14,
		/// alternate function 15
		AF15,
};

/// all possible modes of pin
enum class PinMode : uint8_t
{
		/// input
		Input,
		/// output
		Output,
		/// alternate function
		AlternateFunction,
		/// analog
		Analog,
};

/// all possible output speeds of pin
enum class PinOutputSpeed : uint8_t
{
		/// low speed
		Low,
		/// medium speed
		Medium,
		/// high speed
		High,
		/// very high speed
		VeryHigh,
};

/// all possible pull-up/pull-down configurations of pin
enum class PinPull : uint8_t
{
		None = 0,				///< no pull-up/pull-down
		Up = 1,					///< pull-up
		Down = 2				///< pull-down
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
 * \param [in] initialState is the initial state of pin
 */

void configurePin(Pin pin, PinMode mode, bool openDrain, PinOutputSpeed outputSpeed, PinPull pull,
		PinAlternateFunction alternateFunction, bool initialState);

/**
 * \brief Configures pin for "alternate function" mode.
 *
 * \param [in] pin is the identifier of pin
 * \param [in] openDrain is the desired output type of pin: push-pull (false) or open-drain (true)
 * \param [in] outputSpeed is the desired output speed of pin
 * \param [in] pull is the desired pull-up/pull-down configuration of pin
 * \param [in] alternateFunction is the desired alternate function of pin
 */

inline void configureAlternateFunctionPin(const Pin pin, const bool openDrain, const PinOutputSpeed outputSpeed,
		const PinPull pull, const PinAlternateFunction alternateFunction)
{
	configurePin(pin, PinMode::AlternateFunction, openDrain, outputSpeed, pull, alternateFunction, {});
}

/**
 * \brief Configures pin for "analog" mode.
 *
 * \param [in] pin is the identifier of pin
 */

inline void configureAnalogPin(const Pin pin)
{
	configurePin(pin, PinMode::Analog, {}, {}, {}, {}, {});
}

/**
 * \brief Configures pin for "input" mode.
 *
 * \param [in] pin is the identifier of pin
 * \param [in] pull is the desired pull-up/pull-down configuration of pin
 */

inline void configureInputPin(const Pin pin, const PinPull pull)
{
	configurePin(pin, PinMode::Input, {}, {}, pull, {}, {});
}

/**
 * \brief Configures pin for "output" mode.
 *
 * \param [in] pin is the identifier of pin
 * \param [in] openDrain is the desired output type of pin: push-pull (false) or open-drain (true)
 * \param [in] outputSpeed is the desired output speed of pin
 * \param [in] pull is the desired pull-up/pull-down configuration of pin
 * \param [in] initialState is the initial state of pin
 */

inline void configureOutputPin(const Pin pin, const bool openDrain, const PinOutputSpeed outputSpeed,
		const PinPull pull, const bool initialState)
{
	configurePin(pin, PinMode::Output, openDrain, outputSpeed, pull, {}, initialState);
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

#endif	// SOURCE_CHIP_STM32_STM32F4_INCLUDE_DISTORTOS_CHIP_STM32F4_GPIO_HPP_
