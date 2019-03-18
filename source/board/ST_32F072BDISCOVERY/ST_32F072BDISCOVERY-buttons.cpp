/**
 * \file
 * \brief Definition of buttons for ST,32F072BDISCOVERY
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

#include "distortos/board/buttons.hpp"

#if defined(CONFIG_BOARD_BUTTONS_ENABLE) && DISTORTOS_BOARD_BUTTONS_COUNT != 0

#include "distortos/chip/ChipInputPin.hpp"

namespace distortos
{

namespace board
{

/*---------------------------------------------------------------------------------------------------------------------+
| global objects
+---------------------------------------------------------------------------------------------------------------------*/

const chip::ChipInputPin buttons[buttonsCount]
{
#ifdef CONFIG_BOARD_BUTTONS_B1_ENABLE
		chip::ChipInputPin{chip::Pin::pa0, chip::PinPull::none, false},
#endif	// def CONFIG_BOARD_BUTTONS_B1_ENABLE
};

}	// namespace board

}	// namespace distortos

#endif	// defined(CONFIG_BOARD_BUTTONS_ENABLE) && DISTORTOS_BOARD_BUTTONS_COUNT != 0
