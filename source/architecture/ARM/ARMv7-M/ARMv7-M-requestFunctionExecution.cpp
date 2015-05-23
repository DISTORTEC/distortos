/**
 * \file
 * \brief requestFunctionExecution() implementation for ARMv7-M (Cortex-M3 / Cortex-M4)
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-05-23
 */

#include "distortos/architecture/requestFunctionExecution.hpp"

#include "ExceptionFpuStackFrame.hpp"
#include "supervisorCall.hpp"

#include "distortos/scheduler/Scheduler.hpp"
#include "distortos/scheduler/getScheduler.hpp"

#include <cstring>

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
 * \brief Removes stack frame inserted by requestFunctionExecution() in case of interrupt -> current thread request.
 *
 * \param [in] savedStackPointer is the stack pointer value before new stack frame was created
 * \param [in] fpuContextActive (only when FPU is enabled) tells whether thread has active FPU context (true) or not
 * (false)
 */

#if __FPU_PRESENT == 1 && __FPU_USED == 1
void removeStackFrame(const void* const savedStackPointer, const bool fpuContextActive)
#else
void removeStackFrame(const void* const savedStackPointer)
#endif	// __FPU_PRESENT == 1 && __FPU_USED == 1
{
#if __FPU_PRESENT == 1 && __FPU_USED == 1

	if (fpuContextActive == true)
		asm volatile ("vmov s0, s0");	// force stacking of FPU context

#endif	// __FPU_PRESENT == 1 && __FPU_USED == 1

	__set_PSP(reinterpret_cast<uint32_t>(savedStackPointer));
}

/**
 * \brief Trampoline used to execute function on new stack frame.
 *
 * After the function returns stack frame is removed and old stack pointer value is restored.
 *
 * \param [in] function is a reference to function that will be executed
 * \param [in] savedStackPointer is the stack pointer value before new stack frame was created
 */

__attribute__ ((naked))
void functionTrampoline(void (& function)(), const void* const savedStackPointer)
{
	using SupervisorCall = int(int (&)(int, int, int, int), int, int, int, int);	// type of supervisorCall()

	asm volatile
	(
#if __FPU_PRESENT == 1 && __FPU_USED == 1
			"	mrs		r2, CONTROL					\n"		// save current value of CONTROL register
#endif	// __FPU_PRESENT == 1 && __FPU_USED == 1
			// push savedStackPointer and value of CONTROL register to stack
			"	push	{r1, r2}					\n"
			"	blx		%[function]					\n"		// execute function
			// restore savedStackPointer (2nd supervisorCall() argument) and value of CONTROL register, don't update SP
			"	ldm		sp, {r1, r2}				\n"
			"	mov		r3, #0						\n"		// 4th supervisorCall() argument - 0
			"	str		r3, [sp]					\n"		// 5th supervisorCall() argument - 0
#if __FPU_PRESENT == 1 && __FPU_USED == 1
			// restore previous value of CONTROL register, possibly deactivating FPU context
			"	msr		CONTROL, r2					\n"
			// 3rd supervisorCall() argument - extracted CONTROL.FPCA, 1 if FPU context is active, 0 otherwise
			"	ubfx	r2, r2, #2, #1				\n"
#else
			"	mov		r2, r3						\n"		// 3rd supervisorCall() argument - 0
#endif	// __FPU_PRESENT == 1 && __FPU_USED == 1
			"	ldr		r0, =%[removeStackFrame]	\n"		// 1st supervisorCall() argument - removeStackFrame
			"	b		%[supervisorCall]			\n"		// jump to supervisorCall(), this does not return

			::	[function] "r"	(function),
				[removeStackFrame] "i" (removeStackFrame),
				[savedStackPointer] "r" (savedStackPointer),
				[supervisorCall] "i" (static_cast<SupervisorCall&>(supervisorCall))
	);

	__builtin_unreachable();
}

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

void requestFunctionExecution(const scheduler::ThreadControlBlock& threadControlBlock, void (& function)())
{
	const auto& currentThreadControlBlock = scheduler::getScheduler().getCurrentThreadControlBlock();
	if (&threadControlBlock == &currentThreadControlBlock)	// request to current thread?
	{
		const auto inInterrupt = __get_IPSR() != 0;
		if (inInterrupt == false)	// current thread is sending the request to itself?
			function();				// execute function right away
		else						// interrupt is sending the request to current thread?
		{
			const auto stackPointer = __get_PSP();
			// it's not possible to know whether the thread has active FPU context, so the only option is to assume it
			// does; ExceptionFpuStackFrame is equal to ExceptionStackFrame in case of compilation with disabled FPU, so
			// no memory is wasted in that case
			const auto exceptionFpuStackFrame = reinterpret_cast<ExceptionFpuStackFrame*>(stackPointer) - 1;

#if __FPU_PRESENT == 1 && __FPU_USED == 1

			memset(exceptionFpuStackFrame, 0, sizeof(*exceptionFpuStackFrame));
			exceptionFpuStackFrame->fpscr = reinterpret_cast<void*>(FPU->FPDSCR);

#endif	// __FPU_PRESENT == 1 && __FPU_USED == 1

			exceptionFpuStackFrame->exceptionStackFrame.r0 = reinterpret_cast<void*>(&function);
			exceptionFpuStackFrame->exceptionStackFrame.r1 = reinterpret_cast<void*>(stackPointer);
			exceptionFpuStackFrame->exceptionStackFrame.r2 = reinterpret_cast<void*>(0x22222222);
			exceptionFpuStackFrame->exceptionStackFrame.r3 = reinterpret_cast<void*>(0x33333333);
			exceptionFpuStackFrame->exceptionStackFrame.r12 = reinterpret_cast<void*>(0xcccccccc);
			exceptionFpuStackFrame->exceptionStackFrame.lr = nullptr;
			exceptionFpuStackFrame->exceptionStackFrame.pc = reinterpret_cast<void*>(&functionTrampoline);
			exceptionFpuStackFrame->exceptionStackFrame.xpsr = ExceptionStackFrame::defaultXpsr;

			__set_PSP(reinterpret_cast<uint32_t>(exceptionFpuStackFrame));
		}
	}
}

}	// namespace architecture

}	// namespace distortos
