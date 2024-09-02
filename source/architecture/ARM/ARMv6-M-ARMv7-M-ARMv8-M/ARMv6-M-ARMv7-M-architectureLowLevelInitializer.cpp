/**
 * \file
 * \brief Low-level architecture initializer for ARMv6-M and ARMv7-M
 *
 * \author Copyright (C) 2015-2019 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "distortos/chip/CMSIS-proxy.h"

#include "distortos/BIND_LOW_LEVEL_INITIALIZER.h"

namespace distortos
{

namespace architecture
{

namespace
{

/*---------------------------------------------------------------------------------------------------------------------+
| local functions
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief Low-level architecture initializer for ARMv6-M and ARMv7-M
 *
 * This function is called before constructors for global and static objects via BIND_LOW_LEVEL_INITIALIZER().
 */

void architectureLowLevelInitializer()
{
#ifdef DISTORTOS_ARCHITECTURE_ARM_CORTEX_M3_R1P1
	SCB->CCR |= SCB_CCR_STKALIGN_Msk;
#endif	// def DISTORTOS_ARCHITECTURE_ARM_CORTEX_M3_R1P1
#if __FPU_PRESENT == 1 && __FPU_USED == 1
	SCB->CPACR |= 3 << 10 * 2 | 3 << 11 * 2;	// full access to CP10 and CP11
#endif	// __FPU_PRESENT == 1 && __FPU_USED == 1
}

BIND_LOW_LEVEL_INITIALIZER(30, architectureLowLevelInitializer);

}	// namespace

}	// namespace architecture

}	// namespace distortos
