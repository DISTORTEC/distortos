/**
 * \file
 * \brief configureSysTick() implementation for ARMv7-M (Cortex-M3 / Cortex-M4)
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-09-12
 */

#include "distortos/architecture/configureSysTick.hpp"

#include "distortos/chip/CMSIS-proxy.h"

namespace distortos
{

namespace architecture
{

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

void configureSysTick(const uint32_t period, const bool divideBy8)
{
	SysTick->LOAD = period - 1;
	SysTick->CTRL = divideBy8 == true ? 0 : SysTick_CTRL_CLKSOURCE_Msk;
}

}	// namespace architecture

}	// namespace distortos
