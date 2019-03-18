/**
 * \file
 * \brief Declaration of buttons for ST,32F072BDISCOVERY
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

#ifndef SOURCE_BOARD_ST_32F072BDISCOVERY_INCLUDE_DISTORTOS_BOARD_BUTTONS_HPP_
#define SOURCE_BOARD_ST_32F072BDISCOVERY_INCLUDE_DISTORTOS_BOARD_BUTTONS_HPP_

#include "distortos/distortosConfiguration.h"

#include <cstddef>

#ifdef CONFIG_BOARD_BUTTONS_B1_ENABLE
#define DISTORTOS_BOARD_BUTTONS_B1_ENABLED	1
#else	// !def CONFIG_BOARD_BUTTONS_B1_ENABLE
#define DISTORTOS_BOARD_BUTTONS_B1_ENABLED	0
#endif	// !def CONFIG_BOARD_BUTTONS_B1_ENABLE

/// count of buttons on the board
#define DISTORTOS_BOARD_BUTTONS_COUNT	(DISTORTOS_BOARD_BUTTONS_B1_ENABLED)

#if defined(CONFIG_BOARD_BUTTONS_ENABLE) && DISTORTOS_BOARD_BUTTONS_COUNT != 0

namespace distortos
{

namespace chip
{

class ChipInputPin;

}	// namespace chip

namespace board
{

/// count of buttons on the board
constexpr size_t buttonsCount {DISTORTOS_BOARD_BUTTONS_COUNT};

/*---------------------------------------------------------------------------------------------------------------------+
| indexes of buttons
+---------------------------------------------------------------------------------------------------------------------*/

enum
{
#ifdef CONFIG_BOARD_BUTTONS_B1_ENABLE
		/// index of B1 (User) from buttons
		buttonsB1Index,
#endif	// def CONFIG_BOARD_BUTTONS_B1_ENABLE
};

/*---------------------------------------------------------------------------------------------------------------------+
| alternative indexes of buttons
+---------------------------------------------------------------------------------------------------------------------*/

#ifdef CONFIG_BOARD_BUTTONS_B1_ENABLE

/// alternative index of B1 (User) from buttons
constexpr size_t buttonsUserIndex {buttonsB1Index};

#endif	// def CONFIG_BOARD_BUTTONS_B1_ENABLE

/*---------------------------------------------------------------------------------------------------------------------+
| indexed access to buttons objects
+---------------------------------------------------------------------------------------------------------------------*/

/// array with all buttons objects
extern const chip::ChipInputPin buttons[buttonsCount];

}	// namespace board

}	// namespace distortos

#endif	// defined(CONFIG_BOARD_BUTTONS_ENABLE) && DISTORTOS_BOARD_BUTTONS_COUNT != 0

#endif	// SOURCE_BOARD_ST_32F072BDISCOVERY_INCLUDE_DISTORTOS_BOARD_BUTTONS_HPP_
