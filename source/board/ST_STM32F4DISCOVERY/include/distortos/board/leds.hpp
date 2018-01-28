/**
 * \file
 * \brief Declaration of leds for ST,STM32F4DISCOVERY
 *
 * \author Copyright (C) 2014-2018 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \warning
 * Automatically generated file - do not edit!
 */

#ifndef SOURCE_BOARD_ST_STM32F4DISCOVERY_INCLUDE_DISTORTOS_BOARD_LEDS_HPP_
#define SOURCE_BOARD_ST_STM32F4DISCOVERY_INCLUDE_DISTORTOS_BOARD_LEDS_HPP_

#include "distortos/distortosConfiguration.h"

#include <cstddef>

#ifdef CONFIG_BOARD_LEDS_LD3_ENABLE
#define DISTORTOS_BOARD_LEDS_LD3_ENABLED	1
#else	// !def CONFIG_BOARD_LEDS_LD3_ENABLE
#define DISTORTOS_BOARD_LEDS_LD3_ENABLED	0
#endif	// !def CONFIG_BOARD_LEDS_LD3_ENABLE

#ifdef CONFIG_BOARD_LEDS_LD4_ENABLE
#define DISTORTOS_BOARD_LEDS_LD4_ENABLED	1
#else	// !def CONFIG_BOARD_LEDS_LD4_ENABLE
#define DISTORTOS_BOARD_LEDS_LD4_ENABLED	0
#endif	// !def CONFIG_BOARD_LEDS_LD4_ENABLE

#ifdef CONFIG_BOARD_LEDS_LD5_ENABLE
#define DISTORTOS_BOARD_LEDS_LD5_ENABLED	1
#else	// !def CONFIG_BOARD_LEDS_LD5_ENABLE
#define DISTORTOS_BOARD_LEDS_LD5_ENABLED	0
#endif	// !def CONFIG_BOARD_LEDS_LD5_ENABLE

#ifdef CONFIG_BOARD_LEDS_LD6_ENABLE
#define DISTORTOS_BOARD_LEDS_LD6_ENABLED	1
#else	// !def CONFIG_BOARD_LEDS_LD6_ENABLE
#define DISTORTOS_BOARD_LEDS_LD6_ENABLED	0
#endif	// !def CONFIG_BOARD_LEDS_LD6_ENABLE

/// total number of leds on the board
#define DISTORTOS_BOARD_TOTAL_LEDS	(DISTORTOS_BOARD_LEDS_LD3_ENABLED + \
		DISTORTOS_BOARD_LEDS_LD4_ENABLED + \
		DISTORTOS_BOARD_LEDS_LD5_ENABLED + \
		DISTORTOS_BOARD_LEDS_LD6_ENABLED)

#if defined(CONFIG_BOARD_LEDS_ENABLE) && DISTORTOS_BOARD_TOTAL_LEDS != 0

namespace distortos
{

namespace chip
{

class ChipOutputPin;

}	// namespace chip

namespace board
{

/// total number of leds on the board
constexpr size_t totalLeds {DISTORTOS_BOARD_TOTAL_LEDS};

/*---------------------------------------------------------------------------------------------------------------------+
| indexes of leds
+---------------------------------------------------------------------------------------------------------------------*/

enum
{
#ifdef CONFIG_BOARD_LEDS_LD3_ENABLE
		/// index of ld3 LED (orange)
		ld3LedIndex,
#endif	// def CONFIG_BOARD_LEDS_LD3_ENABLE
#ifdef CONFIG_BOARD_LEDS_LD4_ENABLE
		/// index of ld4 LED (green)
		ld4LedIndex,
#endif	// def CONFIG_BOARD_LEDS_LD4_ENABLE
#ifdef CONFIG_BOARD_LEDS_LD5_ENABLE
		/// index of ld5 LED (red)
		ld5LedIndex,
#endif	// def CONFIG_BOARD_LEDS_LD5_ENABLE
#ifdef CONFIG_BOARD_LEDS_LD6_ENABLE
		/// index of ld6 LED (blue)
		ld6LedIndex,
#endif	// def CONFIG_BOARD_LEDS_LD6_ENABLE
};

/*---------------------------------------------------------------------------------------------------------------------+
| alternative indexes of leds
+---------------------------------------------------------------------------------------------------------------------*/

#ifdef CONFIG_BOARD_LEDS_LD3_ENABLE

/// alternative index of ld3 LED (orange)
constexpr size_t orangeLedIndex {ld3LedIndex};

#endif	// def CONFIG_BOARD_LEDS_LD3_ENABLE

#ifdef CONFIG_BOARD_LEDS_LD4_ENABLE

/// alternative index of ld4 LED (green)
constexpr size_t greenLedIndex {ld4LedIndex};

#endif	// def CONFIG_BOARD_LEDS_LD4_ENABLE

#ifdef CONFIG_BOARD_LEDS_LD5_ENABLE

/// alternative index of ld5 LED (red)
constexpr size_t redLedIndex {ld5LedIndex};

#endif	// def CONFIG_BOARD_LEDS_LD5_ENABLE

#ifdef CONFIG_BOARD_LEDS_LD6_ENABLE

/// alternative index of ld6 LED (blue)
constexpr size_t blueLedIndex {ld6LedIndex};

#endif	// def CONFIG_BOARD_LEDS_LD6_ENABLE

/*---------------------------------------------------------------------------------------------------------------------+
| indexed access to leds objects
+---------------------------------------------------------------------------------------------------------------------*/

/// array with all leds objects
extern chip::ChipOutputPin leds[totalLeds];

}	// namespace board

}	// namespace distortos

#endif	// defined(CONFIG_BOARD_LEDS_ENABLE) && DISTORTOS_BOARD_TOTAL_LEDS != 0

#endif	// SOURCE_BOARD_ST_STM32F4DISCOVERY_INCLUDE_DISTORTOS_BOARD_LEDS_HPP_
