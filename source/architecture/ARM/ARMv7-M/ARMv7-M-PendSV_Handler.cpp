/**
 * \file
 * \brief PendSV_Handler() for ARMv7-M (Cortex-M3 / Cortex-M4)
 *
 * \author Copyright (C) 2014-2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-11-27
 */

#include "distortos/internal/scheduler/getScheduler.hpp"
#include "distortos/internal/scheduler/Scheduler.hpp"

#include "distortos/chip/CMSIS-proxy.h"

namespace distortos
{

namespace
{

/*---------------------------------------------------------------------------------------------------------------------+
| local functions
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief Wrapper for void* distortos::scheduler::getScheduler().switchContext(void*)
 *
 * \param [in] stackPointer is the current value of current thread's stack pointer
 *
 * \return new thread's stack pointer
 */

void* schedulerSwitchContextWrapper(void* const stackPointer)
{
	return scheduler::getScheduler().switchContext(stackPointer);
}

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief PendSV_Handler() for ARMv7-M (Cortex-M3 / Cortex-M4)
 *
 * Performs the context switch.
 */

extern "C" __attribute__ ((naked)) void PendSV_Handler()
{
	constexpr auto basepriValue = CONFIG_ARCHITECTURE_ARMV7_M_KERNEL_BASEPRI << (8 - __NVIC_PRIO_BITS);
	static_assert(basepriValue > 0 && basepriValue <= UINT8_MAX,
			"Invalid CONFIG_ARCHITECTURE_ARMV7_M_KERNEL_BASEPRI value!");

	asm volatile
	(
			"	mov			r0, %[basepriValue]				\n"
			"	msr			basepri, r0						\n"	// enable interrupt masking
			"												\n"
			"	mrs			r0, PSP							\n"
#if __FPU_PRESENT == 1 && __FPU_USED == 1
			"	tst			lr, #(1 << 4)					\n"	// was floating-point used by the thread?
			"	it			eq								\n"
			"	vstmdbeq	r0!, {s16-s31}					\n"	// save "floating-point" context of current thread
			// save "regular" context of current thread (r12 is saved just to keep double-word alignment) 
			"	stmdb		r0!, {r4-r12, lr}				\n"	
#else
			"	stmdb		r0!, {r4-r11}					\n"	// save context of current thread
			"	mov			r4, lr							\n"
#endif	// __FPU_PRESENT == 1 && __FPU_USED == 1
			"												\n"
			"	bl			%[schedulerSwitchContext]		\n"	// switch context
			"												\n"
#if __FPU_PRESENT == 1 && __FPU_USED == 1
			"	ldmia		r0!, {r4-r12, lr}				\n"	// load "regular" context of new thread
			"	tst			lr, #(1 << 4)					\n"	// was floating-point used by the thread?
			"	it			eq								\n"
			"	vldmiaeq	r0!, {s16-s31}					\n"	// load "floating-point" context of new thread
#else
			"	mov			lr, r4							\n"
			"	ldmia		r0!, {r4-r11}					\n"	// load context of new thread
#endif	// __FPU_PRESENT == 1 && __FPU_USED == 1
			"	msr			PSP, r0							\n"
			"												\n"
			"	mov			r0, #0							\n"
			"	msr			basepri, r0						\n"	// disable interrupt masking
			"												\n"
			"	bx			lr								\n"	// return to new thread

			::	[schedulerSwitchContext] "i" (schedulerSwitchContextWrapper),
				[basepriValue] "i" (basepriValue)
	);

	__builtin_unreachable();
}

}	// namespace distortos
