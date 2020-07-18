/**
 * \file
 * \brief Declaration of leds for ST,NUCLEO-F446RE
 *
 * \author Copyright (C) 2014-2020 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * \warning
 * Automatically generated file - do not edit!
 */

#ifndef SOURCE_BOARD_ST_NUCLEO_F446RE_INCLUDE_DISTORTOS_BOARD_LEDS_HPP_
#define SOURCE_BOARD_ST_NUCLEO_F446RE_INCLUDE_DISTORTOS_BOARD_LEDS_HPP_

#include "distortos/distortosConfiguration.h"

#include <cstddef>

#ifdef DISTORTOS_BOARD_LEDS_LD2_ENABLE
#define DISTORTOS_BOARD_LEDS_LD2_ENABLED	1
#else	// !def DISTORTOS_BOARD_LEDS_LD2_ENABLE
#define DISTORTOS_BOARD_LEDS_LD2_ENABLED	0
#endif	// !def DISTORTOS_BOARD_LEDS_LD2_ENABLE

/// count of leds on the board
#define DISTORTOS_BOARD_LEDS_COUNT	(DISTORTOS_BOARD_LEDS_LD2_ENABLED)

#if defined(DISTORTOS_BOARD_LEDS_ENABLE) && DISTORTOS_BOARD_LEDS_COUNT != 0

namespace distortos
{

namespace chip
{

class OutputPin;

}	// namespace chip

namespace board
{

/// count of leds on the board
constexpr size_t ledsCount {DISTORTOS_BOARD_LEDS_COUNT};

/*---------------------------------------------------------------------------------------------------------------------+
| indexes of leds
+---------------------------------------------------------------------------------------------------------------------*/

enum
{
#ifdef DISTORTOS_BOARD_LEDS_LD2_ENABLE
		/// index of Ld2 (Green) from leds
		ledsLd2Index,
#endif	// def DISTORTOS_BOARD_LEDS_LD2_ENABLE
};

/*---------------------------------------------------------------------------------------------------------------------+
| alternative indexes of leds
+---------------------------------------------------------------------------------------------------------------------*/

#ifdef DISTORTOS_BOARD_LEDS_LD2_ENABLE

/// alternative index of Ld2 (Green) from leds
constexpr size_t ledsGreenIndex {ledsLd2Index};

#endif	// def DISTORTOS_BOARD_LEDS_LD2_ENABLE

/*---------------------------------------------------------------------------------------------------------------------+
| indexed access to leds objects
+---------------------------------------------------------------------------------------------------------------------*/

/// array with all leds objects
extern chip::OutputPin leds[ledsCount];

}	// namespace board

}	// namespace distortos

#endif	// defined(DISTORTOS_BOARD_LEDS_ENABLE) && DISTORTOS_BOARD_LEDS_COUNT != 0

#endif	// SOURCE_BOARD_ST_NUCLEO_F446RE_INCLUDE_DISTORTOS_BOARD_LEDS_HPP_
