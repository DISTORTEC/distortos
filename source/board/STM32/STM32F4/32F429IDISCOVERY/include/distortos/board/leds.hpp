/**
 * \file
 * \brief Declaration of LEDs for 32F429IDISCOVERY
 *
 * \author Copyright (C) 2014-2017 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 * \author Copyright (C) 2017 Cezary Gapinski cezary.gapinski@gmail.com
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \warning
 * Automatically generated file - do not edit!
 *
 * \date 2017-03-15 17:37:29
 */

#ifndef SOURCE_BOARD_STM32_STM32F4_32F429IDISCOVERY_INCLUDE_DISTORTOS_BOARD_LEDS_HPP_
#define SOURCE_BOARD_STM32_STM32F4_32F429IDISCOVERY_INCLUDE_DISTORTOS_BOARD_LEDS_HPP_

#include "distortos/distortosConfiguration.h"

#include <cstddef>

/// indicates whether LD3 LED (green) is enabled (1) or not (0)
#ifdef CONFIG_CHIP_STM32_GPIOV2_GPIOG_ENABLE
#define DISTORTOS_BOARD_LD3_LED_ENABLE		1
#else	// !def CONFIG_CHIP_STM32_GPIOV2_GPIOG_ENABLE
#define DISTORTOS_BOARD_LD3_LED_ENABLE		0
#endif	// !def CONFIG_CHIP_STM32_GPIOV2_GPIOG_ENABLE

/// indicates whether LD4 LED (red) is enabled (1) or not (0)
#ifdef CONFIG_CHIP_STM32_GPIOV2_GPIOG_ENABLE
#define DISTORTOS_BOARD_LD4_LED_ENABLE		1
#else	// !def CONFIG_CHIP_STM32_GPIOV2_GPIOG_ENABLE
#define DISTORTOS_BOARD_LD4_LED_ENABLE		0
#endif	// !def CONFIG_CHIP_STM32_GPIOV2_GPIOG_ENABLE

/// total number of leds on the board
#define DISTORTOS_BOARD_TOTAL_LEDS		( \
		DISTORTOS_BOARD_LD3_LED_ENABLE + \
		DISTORTOS_BOARD_LD4_LED_ENABLE + \
		0)

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
constexpr size_t totalLeds {DISTORTOS_BOARD_TOTAL_LEDS};

/*---------------------------------------------------------------------------------------------------------------------+
| LED indexes
+---------------------------------------------------------------------------------------------------------------------*/

enum
{
#if DISTORTOS_BOARD_LD3_LED_ENABLE == 1
		/// index of LD3 LED (green)
		ld3LedIndex,
#endif	// DISTORTOS_BOARD_LD3_LED_ENABLE == 1
#if DISTORTOS_BOARD_LD4_LED_ENABLE == 1
		/// index of LD4 LED (red)
		ld4LedIndex,
#endif	// DISTORTOS_BOARD_LD4_LED_ENABLE == 1
};

#ifdef CONFIG_BOARD_LEDS_ENABLE

/*---------------------------------------------------------------------------------------------------------------------+
| alternative (color-based) LED indexes
+---------------------------------------------------------------------------------------------------------------------*/

#if DISTORTOS_BOARD_LD3_LED_ENABLE == 1
/// alternative index of LD3 LED (green)
constexpr size_t greenLedIndex {ld3LedIndex};
#endif	// DISTORTOS_BOARD_LD3_LED_ENABLE == 1

#if DISTORTOS_BOARD_LD4_LED_ENABLE == 1
/// alternative index of LD4 LED (red)
constexpr size_t redLedIndex {ld4LedIndex};
#endif	// DISTORTOS_BOARD_LD4_LED_ENABLE == 1

/*---------------------------------------------------------------------------------------------------------------------+
| indexed access to LED objects
+---------------------------------------------------------------------------------------------------------------------*/

/// array with all LED objects
extern chip::ChipOutputPin leds[totalLeds];

#endif	// def CONFIG_BOARD_LEDS_ENABLE

}	// namespace board

}	// namespace distortos

#endif	// SOURCE_BOARD_STM32_STM32F4_32F429IDISCOVERY_INCLUDE_DISTORTOS_BOARD_LEDS_HPP_
