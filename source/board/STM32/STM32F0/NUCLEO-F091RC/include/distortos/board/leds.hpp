/**
 * \file
 * \brief Declaration of LEDs for NUCLEO-F091RC
 *
 * \author Copyright (C) 2016 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef SOURCE_BOARD_STM32_STM32F0_NUCLEO_F091RC_INCLUDE_DISTORTOS_BOARD_LEDS_HPP_
#define SOURCE_BOARD_STM32_STM32F0_NUCLEO_F091RC_INCLUDE_DISTORTOS_BOARD_LEDS_HPP_

#include "distortos/chip/STM32F0-GPIO.hpp"

#include <array>

namespace distortos
{

#ifdef CONFIG_BOARD_LEDS_ENABLE

namespace chip
{

class ChipOutputPin;

}	// namespace chip

#endif	// def CONFIG_BOARD_LEDS_ENABLE

namespace board
{

/// total number of LEDs on the board
constexpr size_t totalLeds {CONFIG_BOARD_TOTAL_LEDS};

/*---------------------------------------------------------------------------------------------------------------------+
| LED indexes
+---------------------------------------------------------------------------------------------------------------------*/

/// index of LD2 LED (green)
constexpr size_t ld2LedIndex {0};

/*---------------------------------------------------------------------------------------------------------------------+
| alternative (color-based) LED indexes
+---------------------------------------------------------------------------------------------------------------------*/

/// alternative index of LD2 LED (green)
constexpr size_t greenLedIndex {ld2LedIndex};

/*---------------------------------------------------------------------------------------------------------------------+
| indexed access to pin identifiers
+---------------------------------------------------------------------------------------------------------------------*/

/// array with pin identifiers of all LEDs
constexpr std::array<chip::Pin, totalLeds> ledPins
{
		chip::Pin::pa5,
};

#ifdef CONFIG_BOARD_LEDS_ENABLE

/*---------------------------------------------------------------------------------------------------------------------+
| indexed access to LED objects
+---------------------------------------------------------------------------------------------------------------------*/

/// array with all LED objects
extern chip::ChipOutputPin leds[totalLeds];

#endif	// def CONFIG_BOARD_LEDS_ENABLE

}	// namespace board

}	// namespace distortos

#endif	// SOURCE_BOARD_STM32_STM32F0_NUCLEO_F091RC_INCLUDE_DISTORTOS_BOARD_LEDS_HPP_
