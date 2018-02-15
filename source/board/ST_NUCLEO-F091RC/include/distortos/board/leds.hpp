/**
 * \file
 * \brief Declaration of leds for ST,NUCLEO-F091RC
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

#ifndef SOURCE_BOARD_ST_NUCLEO_F091RC_INCLUDE_DISTORTOS_BOARD_LEDS_HPP_
#define SOURCE_BOARD_ST_NUCLEO_F091RC_INCLUDE_DISTORTOS_BOARD_LEDS_HPP_

#include "distortos/distortosConfiguration.h"

#include <cstddef>

#ifdef CONFIG_BOARD_LEDS_LD2_ENABLE
#define DISTORTOS_BOARD_LEDS_LD2_ENABLED	1
#else	// !def CONFIG_BOARD_LEDS_LD2_ENABLE
#define DISTORTOS_BOARD_LEDS_LD2_ENABLED	0
#endif	// !def CONFIG_BOARD_LEDS_LD2_ENABLE

/// total number of leds on the board
#define DISTORTOS_BOARD_TOTAL_LEDS	(DISTORTOS_BOARD_LEDS_LD2_ENABLED)

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
#ifdef CONFIG_BOARD_LEDS_LD2_ENABLE
		/// index of ld2 (green) from leds
		ledsLd2Index,
#endif	// def CONFIG_BOARD_LEDS_LD2_ENABLE
};

/*---------------------------------------------------------------------------------------------------------------------+
| alternative indexes of leds
+---------------------------------------------------------------------------------------------------------------------*/

#ifdef CONFIG_BOARD_LEDS_LD2_ENABLE

/// alternative index of ld2 (green) from leds
constexpr size_t ledsGreenIndex {ledsLd2Index};

#endif	// def CONFIG_BOARD_LEDS_LD2_ENABLE

/*---------------------------------------------------------------------------------------------------------------------+
| indexed access to leds objects
+---------------------------------------------------------------------------------------------------------------------*/

/// array with all leds objects
extern chip::ChipOutputPin leds[totalLeds];

}	// namespace board

}	// namespace distortos

#endif	// defined(CONFIG_BOARD_LEDS_ENABLE) && DISTORTOS_BOARD_TOTAL_LEDS != 0

#endif	// SOURCE_BOARD_ST_NUCLEO_F091RC_INCLUDE_DISTORTOS_BOARD_LEDS_HPP_
