/**
 * \file
 * \brief Definition of LEDs for NUCLEO-F103RB
 *
 * \author Copyright (C) 2016 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/board/leds.hpp"

#ifdef CONFIG_BOARD_LEDS_ENABLE

#include "distortos/chip/ChipOutputPin.hpp"

namespace distortos
{

namespace board
{

/*---------------------------------------------------------------------------------------------------------------------+
| global objects
+---------------------------------------------------------------------------------------------------------------------*/

chip::ChipOutputPin leds[totalLeds]
{
		chip::ChipOutputPin{chip::Pin::pa5, false, chip::PinOutputSpeed::_2Mhz, false, false},
};

}	// namespace board

}	// namespace distortos

#endif	// def CONFIG_BOARD_LEDS_ENABLE
