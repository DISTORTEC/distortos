/**
 * \file
 * \brief Declaration of buttons for ST,NUCLEO-F091RC
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

#ifndef SOURCE_BOARD_ST_NUCLEO_F091RC_INCLUDE_DISTORTOS_BOARD_BUTTONS_HPP_
#define SOURCE_BOARD_ST_NUCLEO_F091RC_INCLUDE_DISTORTOS_BOARD_BUTTONS_HPP_

#include <cstddef>

/// total number of buttons on the board
#define DISTORTOS_BOARD_TOTAL_BUTTONS		(1)

namespace distortos
{

namespace chip
{

class ChipInputPin;

}	// namespace chip

namespace board
{

/// total number of buttons on the board
constexpr size_t totalButtons {DISTORTOS_BOARD_TOTAL_BUTTONS};

/*---------------------------------------------------------------------------------------------------------------------+
| indexes of buttons
+---------------------------------------------------------------------------------------------------------------------*/

enum
{
		/// index of b1 button (user)
		b1ButtonIndex,
};

/*---------------------------------------------------------------------------------------------------------------------+
| alternative (label-based) indexes of buttons
+---------------------------------------------------------------------------------------------------------------------*/

/// alternative index of b1 button (user)
constexpr size_t userButtonIndex {b1ButtonIndex};

/*---------------------------------------------------------------------------------------------------------------------+
| indexed access to buttons objects
+---------------------------------------------------------------------------------------------------------------------*/

/// array with all buttons objects
extern const chip::ChipInputPin buttons[totalButtons];

}	// namespace board

}	// namespace distortos

#endif	// SOURCE_BOARD_ST_NUCLEO_F091RC_INCLUDE_DISTORTOS_BOARD_BUTTONS_HPP_
