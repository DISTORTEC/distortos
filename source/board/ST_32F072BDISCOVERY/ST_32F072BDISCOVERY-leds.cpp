/**
 * \file
 * \brief Definition of leds for ST,32F072BDISCOVERY
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

#include "distortos/board/leds.hpp"

#if defined(CONFIG_BOARD_LEDS_ENABLE) && DISTORTOS_BOARD_LEDS_COUNT != 0

#include "distortos/chip/ChipOutputPin.hpp"

namespace distortos
{

namespace board
{

/*---------------------------------------------------------------------------------------------------------------------+
| global objects
+---------------------------------------------------------------------------------------------------------------------*/

chip::ChipOutputPin leds[ledsCount]
{
#ifdef CONFIG_BOARD_LEDS_LD3_ENABLE
		chip::ChipOutputPin{chip::Pin::pc8, false, chip::PinOutputSpeed::low, chip::PinPull::none, false, false},
#endif	// def CONFIG_BOARD_LEDS_LD3_ENABLE
#ifdef CONFIG_BOARD_LEDS_LD4_ENABLE
		chip::ChipOutputPin{chip::Pin::pc9, false, chip::PinOutputSpeed::low, chip::PinPull::none, false, false},
#endif	// def CONFIG_BOARD_LEDS_LD4_ENABLE
#ifdef CONFIG_BOARD_LEDS_LD5_ENABLE
		chip::ChipOutputPin{chip::Pin::pc6, false, chip::PinOutputSpeed::low, chip::PinPull::none, false, false},
#endif	// def CONFIG_BOARD_LEDS_LD5_ENABLE
#ifdef CONFIG_BOARD_LEDS_LD6_ENABLE
		chip::ChipOutputPin{chip::Pin::pc7, false, chip::PinOutputSpeed::low, chip::PinPull::none, false, false},
#endif	// def CONFIG_BOARD_LEDS_LD6_ENABLE
};

}	// namespace board

}	// namespace distortos

#endif	// defined(CONFIG_BOARD_LEDS_ENABLE) && DISTORTOS_BOARD_LEDS_COUNT != 0
