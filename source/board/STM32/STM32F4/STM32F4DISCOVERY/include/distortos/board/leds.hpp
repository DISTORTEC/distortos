/**
 * \file
 * \brief Declaration of LEDs for STM32F4DISCOVERY
 *
 * \author Copyright (C) 2016-2017 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef SOURCE_BOARD_STM32_STM32F4_STM32F4DISCOVERY_INCLUDE_DISTORTOS_BOARD_LEDS_HPP_
#define SOURCE_BOARD_STM32_STM32F4_STM32F4DISCOVERY_INCLUDE_DISTORTOS_BOARD_LEDS_HPP_

#include "distortos/distortosConfiguration.h"

#include <cstddef>

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

/// index of LD3 LED (orange)
constexpr size_t ld3LedIndex {0};

/// index of LD4 LED (green)
constexpr size_t ld4LedIndex {1};

/// index of LD5 LED (red)
constexpr size_t ld5LedIndex {2};

/// index of LD6 LED (blue)
constexpr size_t ld6LedIndex {3};

/*---------------------------------------------------------------------------------------------------------------------+
| alternative (color-based) LED indexes
+---------------------------------------------------------------------------------------------------------------------*/

/// alternative index of LD3 LED (orange)
constexpr size_t orangeLedIndex {ld3LedIndex};

/// alternative index of LD4 LED (green)
constexpr size_t greenLedIndex {ld4LedIndex};

/// alternative index of LD5 LED (red)
constexpr size_t redLedIndex {ld5LedIndex};

/// alternative index of LD6 LED (blue)
constexpr size_t blueLedIndex {ld6LedIndex};

#ifdef CONFIG_BOARD_LEDS_ENABLE

/*---------------------------------------------------------------------------------------------------------------------+
| indexed access to LED objects
+---------------------------------------------------------------------------------------------------------------------*/

/// array with all LED objects
extern chip::ChipOutputPin leds[totalLeds];

#endif	// def CONFIG_BOARD_LEDS_ENABLE

}	// namespace board

}	// namespace distortos

#endif	// SOURCE_BOARD_STM32_STM32F4_STM32F4DISCOVERY_INCLUDE_DISTORTOS_BOARD_LEDS_HPP_
