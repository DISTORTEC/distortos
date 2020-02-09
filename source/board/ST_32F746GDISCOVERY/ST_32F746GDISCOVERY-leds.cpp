/**
 * \file
 * \brief Definition of leds for ST,32F746GDISCOVERY
 *
 * \author Copyright (C) 2014-2020 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \warning
 * Automatically generated file - do not edit!
 */

#include "distortos/board/leds.hpp"

#if defined(DISTORTOS_BOARD_LEDS_ENABLE) && DISTORTOS_BOARD_LEDS_COUNT != 0

#include "distortos/chip/OutputPin.hpp"

namespace distortos
{

namespace board
{

/*---------------------------------------------------------------------------------------------------------------------+
| global objects
+---------------------------------------------------------------------------------------------------------------------*/

chip::OutputPin leds[ledsCount]
{
#ifdef DISTORTOS_BOARD_LEDS_LD1_ENABLE
		chip::OutputPin{chip::Pin::pi1, false, chip::PinOutputSpeed::low, chip::PinPull::none, false, false},
#endif	// def DISTORTOS_BOARD_LEDS_LD1_ENABLE
};

}	// namespace board

}	// namespace distortos

#endif	// defined(DISTORTOS_BOARD_LEDS_ENABLE) && DISTORTOS_BOARD_LEDS_COUNT != 0
