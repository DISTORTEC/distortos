/**
 * \file
 * \brief Definition of buttons for ST,NUCLEO-F103RB
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
		chip::ChipInputPin{chip::Pin::pc13, chip::PinPull::none, true},
};

}	// namespace board

}	// namespace distortos
