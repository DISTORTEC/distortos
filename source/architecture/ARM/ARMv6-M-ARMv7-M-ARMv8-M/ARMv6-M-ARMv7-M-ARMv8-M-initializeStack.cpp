/**
 * \file
 * \brief initializeStack() implementation for ARMv6-M and ARMv7-M
 *
 * \author Copyright (C) 2014-2024 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "distortos/architecture/initializeStack.hpp"

#include "ARMv6-M-ARMv7-M-ARMv8-M-StackFrame.hpp"

#include "distortos/internal/scheduler/threadRunner.hpp"

#include <cerrno>

namespace distortos
{

namespace architecture
{

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

std::pair<int, void*> initializeStack(void* const buffer, const size_t size, internal::RunnableThread& runnableThread)
{
	const auto stackFrame = reinterpret_cast<StackFrame*>(static_cast<uint8_t*>(buffer) + size) - 1;
	if (stackFrame < buffer)
		return {ENOSPC, {}};

	stackFrame->softwareStackFrame.r4 = reinterpret_cast<void*>(0x44444444);
	stackFrame->softwareStackFrame.r5 = reinterpret_cast<void*>(0x55555555);
	stackFrame->softwareStackFrame.r6 = reinterpret_cast<void*>(0x66666666);
	stackFrame->softwareStackFrame.r7 = reinterpret_cast<void*>(0x77777777);
	stackFrame->softwareStackFrame.r8 = reinterpret_cast<void*>(0x88888888);
	stackFrame->softwareStackFrame.r9 = reinterpret_cast<void*>(0x99999999);
	stackFrame->softwareStackFrame.r10 = reinterpret_cast<void*>(0xaaaaaaaa);
	stackFrame->softwareStackFrame.r11 = reinterpret_cast<void*>(0xbbbbbbbb);
#if __FPU_PRESENT == 1 && __FPU_USED == 1
	stackFrame->softwareStackFrame.exceptionReturn =
			reinterpret_cast<void*>(SoftwareStackFrame::defaultExceptionReturn);
#endif	// __FPU_PRESENT == 1 && __FPU_USED == 1

	stackFrame->exceptionStackFrame.r0 = &runnableThread;
	stackFrame->exceptionStackFrame.r1 = reinterpret_cast<void*>(0x11111111);
	stackFrame->exceptionStackFrame.r2 = reinterpret_cast<void*>(0x22222222);
	stackFrame->exceptionStackFrame.r3 = reinterpret_cast<void*>(0x33333333);
	stackFrame->exceptionStackFrame.r12 = reinterpret_cast<void*>(0xcccccccc);
	stackFrame->exceptionStackFrame.lr = nullptr;
	stackFrame->exceptionStackFrame.pc = reinterpret_cast<void*>(&internal::threadRunner);
	stackFrame->exceptionStackFrame.xpsr = reinterpret_cast<void*>(ExceptionStackFrame::defaultXpsr);

	return {{}, stackFrame};
}

}	// namespace architecture

}	// namespace distortos
