/**
 * \file
 * \brief startScheduling() implementation for ARMv7-M (Cortex-M3 / Cortex-M4)
 *
 * \author Copyright (C) 2014 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2014-07-24
 */

#include "distortos/architecture.hpp"

#include "distortos/distortosConfiguration.h"

#include "distortos/chip/CMSIS-proxy.h"

namespace distortos
{

namespace architecture
{

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

void startScheduling()
{
	// SysTick and PendSV - lowest possible priority
	NVIC_SetPriority(SysTick_IRQn, 0xff);
	NVIC_SetPriority(PendSV_IRQn, 0xff);

	// configure SysTick timer as the tick timer
	SysTick->LOAD = CONFIG_TICK_CLOCK / CONFIG_TICK_RATE_HZ - 1;
	SysTick->VAL = 0;
	SysTick->CTRL = SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk;

	asm volatile
	(
			"	svc		0		\n"	// request first context switch
	);

	__builtin_unreachable();
}

}	// namespace architecture

}	// namespace distortos
