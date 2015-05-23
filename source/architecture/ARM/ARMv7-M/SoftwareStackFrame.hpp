/**
 * \file
 * \brief SoftwareStackFrame struct header
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-05-23
 */

#ifndef SOURCE_ARCHITECTURE_ARM_ARMV7_M_SOFTWARESTACKFRAME_HPP_
#define SOURCE_ARCHITECTURE_ARM_ARMV7_M_SOFTWARESTACKFRAME_HPP_

#include "distortos/chip/CMSIS-proxy.h"

namespace distortos
{

namespace architecture
{

/// stack frame for ARMv7-M (Cortex-M3 / Cortex-M4) created by software during context switch
struct SoftwareStackFrame
{
#if __FPU_PRESENT == 1 && __FPU_USED == 1

	/// default exception return value
	constexpr static void* defaultExceptionReturn {reinterpret_cast<void*>(0xfffffffd)};

#endif	// __FPU_PRESENT == 1 && __FPU_USED == 1

	/// R4 register
	void* r4;
	/// R5 register
	void* r5;
	/// R6 register
	void* r6;
	/// R7 register
	void* r7;
	/// R8 register
	void* r8;
	/// R9 register
	void* r9;
	/// R10 register
	void* r10;
	/// R11 register
	void* r11;

#if __FPU_PRESENT == 1 && __FPU_USED == 1

	/// LR register with exception return value
	void* exceptionReturn;

#endif	// __FPU_PRESENT == 1 && __FPU_USED == 1
};

}	// namespace architecture

}	// namespace distortos

#endif	// SOURCE_ARCHITECTURE_ARM_ARMV7_M_SOFTWARESTACKFRAME_HPP_
