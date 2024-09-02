/**
 * \file
 * \brief enableInterruptMasking() implementation for ARMv6-M and ARMv7-M
 *
 * \author Copyright (C) 2014-2019 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "distortos/architecture/enableInterruptMasking.hpp"

#include "distortos/chip/CMSIS-proxy.h"

namespace distortos
{

namespace architecture
{

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

InterruptMask enableInterruptMasking()
{
#if DISTORTOS_ARCHITECTURE_KERNEL_BASEPRI != 0

	const auto interruptMask = __get_BASEPRI();
	constexpr auto basepriValue = DISTORTOS_ARCHITECTURE_KERNEL_BASEPRI << (8 - __NVIC_PRIO_BITS);
	static_assert(basepriValue > 0 && basepriValue <= UINT8_MAX,
			"Invalid DISTORTOS_ARCHITECTURE_KERNEL_BASEPRI value!");

#ifdef DISTORTOS_ARCHITECTURE_ARM_CORTEX_M7_R0P1
	__disable_irq();	// ARM Cortex-M7 r0p1 bug ID 837070
#endif	// def DISTORTOS_ARCHITECTURE_ARM_CORTEX_M7_R0P1

	__set_BASEPRI(basepriValue);

#ifdef DISTORTOS_ARCHITECTURE_ARM_CORTEX_M7_R0P1
	__enable_irq();	// ARM Cortex-M7 r0p1 bug ID 837070
#endif	// def DISTORTOS_ARCHITECTURE_ARM_CORTEX_M7_R0P1

	return interruptMask;

#else	// DISTORTOS_ARCHITECTURE_KERNEL_BASEPRI == 0

	const auto interruptMask = __get_PRIMASK();
	__disable_irq();
	return interruptMask;

#endif	// DISTORTOS_ARCHITECTURE_KERNEL_BASEPRI == 0
}

}	// namespace architecture

}	// namespace distortos
