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
 * \date 2014-10-30
 */

#include "distortos/scheduler/getScheduler.hpp"
#include "distortos/scheduler/Scheduler.hpp"


/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief Wrapper for void* distortos::scheduler::getScheduler().switchContext(void*)
 *
 * \param [in] stackPointer is the current value of current thread's stack pointer
 *
 * \return new thread's stack pointer
 */

extern "C" void* schedulerSwitchContextWrapper(void* const stackPointer)
{
	return distortos::scheduler::getScheduler().switchContext(stackPointer);
}

/**
 * \brief PendSV_Handler() for ARMv7-M (Cortex-M3 / Cortex-M4)
 *
 * Performs the context switch.
 */

extern "C" __attribute__ ((naked)) void PendSV_Handler()
{
	asm volatile
	(
			"	mrs		r0, PSP								\n"	// save context of current thread
			"	stmdb	r0!, {r4-r11}						\n"
			"												\n"
			"	mov		r4, lr								\n"
			"	bl		schedulerSwitchContextWrapper		\n"	// switch context
			"	mov		lr, r4								\n"
			"												\n"
			"	ldmia	r0!, {r4-r11}						\n"	// load context of new thread
			"	msr		PSP, r0								\n"
			"												\n"
			"	bx		lr									\n"	// return to new thread
	);

	__builtin_unreachable();
}
