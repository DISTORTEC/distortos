/**
 * \file
 * \brief Definition of buttons for ST,NUCLEO-F429ZI
 *
 * \author Copyright (C) 2014-2020 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \warning
 * Automatically generated file - do not edit!
 */

#include "distortos/board/buttons.hpp"

#if defined(DISTORTOS_BOARD_BUTTONS_ENABLE) && DISTORTOS_BOARD_BUTTONS_COUNT != 0

#include "distortos/chip/InputPin.hpp"

namespace distortos
{

namespace board
{

/*---------------------------------------------------------------------------------------------------------------------+
| global objects
+---------------------------------------------------------------------------------------------------------------------*/

const chip::InputPin buttons[buttonsCount]
{
#ifdef DISTORTOS_BOARD_BUTTONS_B1_ENABLE
		chip::InputPin{chip::Pin::pc13, chip::PinPull::none, false},
#endif	// def DISTORTOS_BOARD_BUTTONS_B1_ENABLE
};

}	// namespace board

}	// namespace distortos

#endif	// defined(DISTORTOS_BOARD_BUTTONS_ENABLE) && DISTORTOS_BOARD_BUTTONS_COUNT != 0
