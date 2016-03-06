/**
 * \file
 * \brief board::lowLevelInitialization() implementation for NUCLEO-F103RB
 *
 * \author Copyright (C) 2016 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/board/lowLevelInitialization.hpp"

#include "distortos/chip/CMSIS-proxy.h"

namespace distortos
{

namespace board
{

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

void lowLevelInitialization()
{
	RCC->APB2ENR |=
#ifdef CONFIG_BOARD_LEDS_ENABLE
			RCC_APB2ENR_IOPAEN |
#endif	// def CONFIG_BOARD_LEDS_ENABLE
			0;
}

}	// namespace board

}	// namespace distortos
