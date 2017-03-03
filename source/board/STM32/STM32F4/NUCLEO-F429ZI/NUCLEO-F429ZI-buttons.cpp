/**
 * \file
 * \brief Definition of buttons for NUCLEO-F429ZI
 *
 * \author Copyright (C) 2016 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/board/buttons.hpp"

#ifdef CONFIG_BOARD_BUTTONS_ENABLE

#include "distortos/chip/ChipInputPin.hpp"

namespace distortos
{

namespace board
{

/*---------------------------------------------------------------------------------------------------------------------+
| global objects
+---------------------------------------------------------------------------------------------------------------------*/

const chip::ChipInputPin buttons[totalButtons]
{
		chip::ChipInputPin{chip::Pin::pc13, chip::PinPull::none, false},
};

}	// namespace board

}	// namespace distortos

#endif	// def CONFIG_BOARD_BUTTONS_ENABLE
