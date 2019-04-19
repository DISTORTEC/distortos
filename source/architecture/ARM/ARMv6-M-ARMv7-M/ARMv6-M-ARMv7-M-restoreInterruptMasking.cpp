/**
 * \file
 * \brief restoreInterruptMasking() implementation for ARMv6-M and ARMv7-M
 *
 * \author Copyright (C) 2014-2019 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/architecture/restoreInterruptMasking.hpp"

#include "distortos/chip/CMSIS-proxy.h"

namespace distortos
{

namespace architecture
{

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

void restoreInterruptMasking(const InterruptMask interruptMask)
{
#if DISTORTOS_ARCHITECTURE_KERNEL_BASEPRI != 0

#ifdef DISTORTOS_ARCHITECTURE_ARM_CORTEX_M7_R0P1
	__disable_irq();	// ARM Cortex-M7 r0p1 bug ID 837070
#endif	// def DISTORTOS_ARCHITECTURE_ARM_CORTEX_M7_R0P1

	__set_BASEPRI(interruptMask);

#ifdef DISTORTOS_ARCHITECTURE_ARM_CORTEX_M7_R0P1
	__enable_irq();	// ARM Cortex-M7 r0p1 bug ID 837070
#endif	// def DISTORTOS_ARCHITECTURE_ARM_CORTEX_M7_R0P1

#else	// DISTORTOS_ARCHITECTURE_KERNEL_BASEPRI == 0

	__set_PRIMASK(interruptMask);

#endif	// DISTORTOS_ARCHITECTURE_KERNEL_BASEPRI == 0
}

}	// namespace architecture

}	// namespace distortos
