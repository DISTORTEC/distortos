/**
 * \file
 * \brief PendSV_Handler() for ARMv6-M and ARMv7-M
 *
 * \author Copyright (C) 2014-2019 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/internal/scheduler/getScheduler.hpp"
#include "distortos/internal/scheduler/Scheduler.hpp"

#include "distortos/chip/CMSIS-proxy.h"

#ifdef DISTORTOS_CHECK_STACK_POINTER_RANGE_CONTEXT_SWITCH_ENABLE

#include "distortos/FATAL_ERROR.h"

#endif	// def DISTORTOS_CHECK_STACK_POINTER_RANGE_CONTEXT_SWITCH_ENABLE

namespace distortos
{

namespace
{

/*---------------------------------------------------------------------------------------------------------------------+
| local functions
+---------------------------------------------------------------------------------------------------------------------*/

#ifdef DISTORTOS_CHECK_STACK_POINTER_RANGE_CONTEXT_SWITCH_ENABLE

/**
 * \brief Wrapper for check of stack pointer range
 *
 * If the check fails, FATAL_ERROR() is called.
 *
 * \param [in] stackPointer is the current thread's stack pointer value, including the frame which will be stacked
 */

void checkStackPointerWrapper(const void* const stackPointer)
{
	if (internal::getScheduler().getCurrentThreadControlBlock().getStack().checkStackPointer(stackPointer) == false)
		FATAL_ERROR("Stack overflow detected!");
}

#endif	// def DISTORTOS_CHECK_STACK_POINTER_RANGE_CONTEXT_SWITCH_ENABLE

/**
 * \brief Wrapper for void* distortos::internal::getScheduler().switchContext(void*)
 *
 * \param [in] stackPointer is the current value of current thread's stack pointer
 *
 * \return new thread's stack pointer
 */

void* schedulerSwitchContextWrapper(void* const stackPointer)
{
	return internal::getScheduler().switchContext(stackPointer);
}

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief PendSV_Handler() for ARMv6-M and ARMv7-M
 *
 * Performs the context switch.
 */

extern "C" __attribute__ ((naked)) void PendSV_Handler()
{
#if DISTORTOS_ARCHITECTURE_ARMV7_M_KERNEL_BASEPRI != 0

	constexpr auto basepriValue = DISTORTOS_ARCHITECTURE_ARMV7_M_KERNEL_BASEPRI << (8 - __NVIC_PRIO_BITS);
	static_assert(basepriValue > 0 && basepriValue <= UINT8_MAX,
			"Invalid DISTORTOS_ARCHITECTURE_ARMV7_M_KERNEL_BASEPRI value!");

#endif	// DISTORTOS_ARCHITECTURE_ARMV7_M_KERNEL_BASEPRI != 0

	asm volatile
	(
#if DISTORTOS_ARCHITECTURE_ARMV7_M_KERNEL_BASEPRI != 0
			"	mov			r0, %[basepriValue]					\n"
			"	msr			basepri, r0							\n"	// enable interrupt masking
#else	// DISTORTOS_ARCHITECTURE_ARMV7_M_KERNEL_BASEPRI == 0
			"	cpsid		i									\n"	// disable interrupts
#endif	// DISTORTOS_ARCHITECTURE_ARMV7_M_KERNEL_BASEPRI == 0
			"													\n"
#ifdef __ARM_ARCH_6M__
			"	mrs			r0, psp								\n"
#ifdef DISTORTOS_CHECK_STACK_POINTER_RANGE_CONTEXT_SWITCH_ENABLE
			"	push		{r0, lr}							\n"
			"	sub			r0, r0, #0x20						\n"
			"	ldr			r1, =%[checkStackPointerWrapper]	\n"
			"	blx			r1									\n"
			"	pop			{r0, r1}							\n"
			"	mov			lr, r1								\n"
#endif	// def DISTORTOS_CHECK_STACK_POINTER_RANGE_CONTEXT_SWITCH_ENABLE
			"	sub			r0, #0x10							\n"
			"	stmia		r0!, {r4-r7}						\n"	// save lower half of current thread's context
			"	mov			r4, r8								\n"
			"	mov			r5, r9								\n"
			"	mov			r6, r10								\n"
			"	mov			r7, r11								\n"
			"	sub			r0, #0x20							\n"
			"	stmia		r0!, {r4-r7}						\n"	// save upper half of current thread's context
			"	sub			r0, #0x10							\n"
			"	mov			r4, lr								\n"
			"													\n"
			"	ldr			r1, =%[schedulerSwitchContext]		\n"
			"	blx			r1									\n"	// switch context
			"													\n"
			"	mov			lr, r4								\n"
			"	ldmia		r0!, {r4-r7}						\n"	// load upper half of new thread's context
			"	mov			r8, r4								\n"
			"	mov			r9, r5								\n"
			"	mov			r10, r6								\n"
			"	mov			r11, r7								\n"
			"	ldmia		r0!, {r4-r7}						\n"	// load lower half of new thread's context
			"	msr			psp, r0								\n"
#else	// !def __ARM_ARCH_6M__
			"	mrs			r0, psp								\n"
#ifdef DISTORTOS_CHECK_STACK_POINTER_RANGE_CONTEXT_SWITCH_ENABLE
			"	push		{r0, lr}							\n"
#if __FPU_PRESENT == 1 && __FPU_USED == 1
			"	tst			lr, #1 << 4							\n"	// was floating-point used by the thread?
			"	it			eq									\n"
			"	subeq		r0, r0, #0x40						\n"	// account for size of "floating-point" context
			"	sub			r0, r0, #0x28						\n"
#else	// __FPU_PRESENT != 1 || __FPU_USED != 1
			"	sub			r0, r0, #0x20						\n"
#endif	// __FPU_PRESENT != 1 || __FPU_USED != 1
			"	bl			%[checkStackPointerWrapper]			\n"
			"	pop			{r0, lr}							\n"
#endif	// def DISTORTOS_CHECK_STACK_POINTER_RANGE_CONTEXT_SWITCH_ENABLE
#if __FPU_PRESENT == 1 && __FPU_USED == 1
			"	tst			lr, #1 << 4							\n"	// was floating-point used by the thread?
			"	it			eq									\n"
			"	vstmdbeq	r0!, {s16-s31}						\n"	// save "floating-point" context of current thread
			// save "regular" context of current thread (r12 is saved just to keep double-word alignment)
			"	stmdb		r0!, {r4-r12, lr}					\n"
#else	// __FPU_PRESENT != 1 || __FPU_USED != 1
			"	stmdb		r0!, {r4-r11}						\n"	// save context of current thread
			"	mov			r4, lr								\n"
#endif	// __FPU_PRESENT != 1 || __FPU_USED != 1
			"													\n"
			"	bl			%[schedulerSwitchContext]			\n"	// switch context
			"													\n"
#if __FPU_PRESENT == 1 && __FPU_USED == 1
			"	ldmia		r0!, {r4-r12, lr}					\n"	// load "regular" context of new thread
			"	tst			lr, #1 << 4							\n"	// was floating-point used by the thread?
			"	it			eq									\n"
			"	vldmiaeq	r0!, {s16-s31}						\n"	// load "floating-point" context of new thread
#else	// __FPU_PRESENT != 1 || __FPU_USED != 1
			"	mov			lr, r4								\n"
			"	ldmia		r0!, {r4-r11}						\n"	// load context of new thread
#endif	// __FPU_PRESENT != 1 || __FPU_USED != 1
			"	msr			psp, r0								\n"
#endif	// !def __ARM_ARCH_6M__
			"													\n"
#if DISTORTOS_ARCHITECTURE_ARMV7_M_KERNEL_BASEPRI != 0
			"	mov			r0, #0								\n"
			"	msr			basepri, r0							\n"	// disable interrupt masking
#else	// DISTORTOS_ARCHITECTURE_ARMV7_M_KERNEL_BASEPRI == 0
			"	cpsie		i									\n"	// enable interrupts
#endif	// DISTORTOS_ARCHITECTURE_ARMV7_M_KERNEL_BASEPRI == 0
			"													\n"
			"	bx			lr									\n"	// return to new thread
			"													\n"
			".ltorg												\n"	// force dumping of literal pool

			::
#if DISTORTOS_ARCHITECTURE_ARMV7_M_KERNEL_BASEPRI != 0
				[basepriValue] "i" (basepriValue),
#endif	// DISTORTOS_ARCHITECTURE_ARMV7_M_KERNEL_BASEPRI != 0
#ifdef DISTORTOS_CHECK_STACK_POINTER_RANGE_CONTEXT_SWITCH_ENABLE
				[checkStackPointerWrapper] "i" (checkStackPointerWrapper),
#endif	// def DISTORTOS_CHECK_STACK_POINTER_RANGE_CONTEXT_SWITCH_ENABLE
				[schedulerSwitchContext] "i" (schedulerSwitchContextWrapper)
	);

	__builtin_unreachable();
}

}	// namespace distortos
