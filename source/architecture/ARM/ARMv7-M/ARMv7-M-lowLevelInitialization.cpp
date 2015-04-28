/**
 * \file
 * \brief lowLevelInitialization() implementation for ARMv7-M (Cortex-M3 / Cortex-M4)
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-04-28
 */

#include "distortos/architecture/lowLevelInitialization.hpp"

#include "distortos/chip/CMSIS-proxy.h"

namespace distortos
{

namespace architecture
{

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

void lowLevelInitialization()
{
#if __FPU_PRESENT == 1 && __FPU_USED == 1
	SCB->CPACR |= (3 << 10 * 2) | (3 << 11 * 2);	// full access to CP10 and CP11
#endif	// __FPU_PRESENT == 1 && __FPU_USED == 1
}

}	// namespace architecture

}	// namespace distortos

