/**
 * \file
 * \brief Start of scheduling for ARMv6-M, ARMv7-M and ARMv8-M
 *
 * \author Copyright (C) 2014-2024 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "distortos/chip/clocks.hpp"
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
 * \brief Start of scheduling for ARMv6-M, ARMv7-M and ARMv8-M
 *
 * Initializes all required hardware/software to perform context switching and starts the scheduling. This function is
 * called before constructors for global and static objects via BIND_LOW_LEVEL_INITIALIZER().
 */

void startScheduling()
{
	// SysTick and PendSV - lowest possible priority
	NVIC_SetPriority(SysTick_IRQn, 0xff);
	NVIC_SetPriority(PendSV_IRQn, 0xff);

	// SVCall - high priority
#ifdef DISTORTOS_ARCHITECTURE_KERNEL_BASEPRI
	constexpr uint32_t svcallPriority {DISTORTOS_ARCHITECTURE_KERNEL_BASEPRI};
#else	// !def DISTORTOS_ARCHITECTURE_KERNEL_BASEPRI
	constexpr uint32_t svcallPriority {};
#endif	// !def DISTORTOS_ARCHITECTURE_KERNEL_BASEPRI
	NVIC_SetPriority(SVCall_IRQn, svcallPriority);

	// configure SysTick timer as the tick timer
	constexpr uint32_t period {chip::ahbFrequency / DISTORTOS_TICK_FREQUENCY};
	constexpr uint32_t periodDividedBy8 {period / 8};
	constexpr uint32_t maxSysTickPeriod {1 << 24};
	constexpr bool divideBy8 {period > maxSysTickPeriod};
	// at least one of the periods must be valid
	static_assert(period <= maxSysTickPeriod || periodDividedBy8 <= maxSysTickPeriod, "Invalid SysTick configuration!");
	SysTick->LOAD = (divideBy8 == false ? period : periodDividedBy8) - 1;
	SysTick->VAL = 0;
	SysTick->CTRL = (divideBy8 == true ? 0 : SysTick_CTRL_CLKSOURCE_Msk) | SysTick_CTRL_ENABLE_Msk |
			SysTick_CTRL_TICKINT_Msk;
}

BIND_LOW_LEVEL_INITIALIZER(70, startScheduling);

}	// namespace

}	// namespace architecture

}	// namespace distortos
