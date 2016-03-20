/**
 * \file
 * \brief configureSysTick() implementation for ARMv6-M and ARMv7-M
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/architecture/ARMv6-M-ARMv7-M-configureSysTick.hpp"

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
