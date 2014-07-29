/**
 * \file
 * \brief PendSV_Handler() for ARMv7-M (Cortex-M3 / Cortex-M4)
 *
 * \author Copyright (C) 2014 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2014-07-29
 */

#include "distortos/scheduler/schedulerInstance.hpp"
#include "distortos/scheduler/Scheduler.hpp"
#include "distortos/scheduler/ThreadControlBlock.hpp"

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief Wrapper for getStack().getStackPointer() of current ThreadControlBlock element
 *
 * \return current value of stack pointer for first task
 */

extern "C" void * getStackPointerWrapper()
{
	return distortos::scheduler::schedulerInstance.getCurrentThreadControlBlock().getStack().getStackPointer();
}

/**
 * \brief SVC_Handler() for ARMv7-M (Cortex-M3 / Cortex-M4)
 *
 * Performs the first context switch.
 */

extern "C" __attribute__ ((naked)) void SVC_Handler()
{
	asm volatile
	(
			"	bl		getStackPointerWrapper		\n"	// get context of first thread
			"										\n"
			"	ldmia	r0!, {r4-r11}				\n"	// load context of first thread
			"	msr		PSP, r0						\n"
			"										\n"
			"	mov		lr, #0xfffffffd				\n"	// EXC_RETURN - thread mode, use PSP stack
			"	bx		lr							\n"	// return to first thread
	);

	__builtin_unreachable();
}
