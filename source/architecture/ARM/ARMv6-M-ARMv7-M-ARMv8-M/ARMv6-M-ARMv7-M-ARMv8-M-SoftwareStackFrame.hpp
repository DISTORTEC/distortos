/**
 * \file
 * \brief SoftwareStackFrame struct header
 *
 * \author Copyright (C) 2015-2024 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef SOURCE_ARCHITECTURE_ARM_ARMV6_M_ARMV7_M_ARMV8_M_ARMV6_M_ARMV7_M_ARMV8_M_SOFTWARESTACKFRAME_HPP_
#define SOURCE_ARCHITECTURE_ARM_ARMV6_M_ARMV7_M_ARMV8_M_ARMV6_M_ARMV7_M_ARMV8_M_SOFTWARESTACKFRAME_HPP_

#include "distortos/chip/CMSIS-proxy.h"

namespace distortos
{

namespace architecture
{

/// stack frame for ARMv6-M and ARMv7-M created by software during context switch
struct SoftwareStackFrame
{
#if __FPU_PRESENT == 1 && __FPU_USED == 1

	/// default exception return value
	constexpr static uintptr_t defaultExceptionReturn {0xfffffffd};

#endif	// __FPU_PRESENT == 1 && __FPU_USED == 1

#if defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7EM__)

	/// r4 register
	void* r4;
	/// r5 register
	void* r5;
	/// r6 register
	void* r6;
	/// r7 register
	void* r7;

#endif	// defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7EM__)

	/// r8 register
	void* r8;
	/// r9 register
	void* r9;
	/// r10 register
	void* r10;
	/// r11 register
	void* r11;

#if !defined(__ARM_ARCH_7M__) && !defined(__ARM_ARCH_7EM__)

	/// r4 register
	void* r4;
	/// r5 register
	void* r5;
	/// r6 register
	void* r6;
	/// r7 register
	void* r7;

#endif	// !defined(__ARM_ARCH_7M__) && !defined(__ARM_ARCH_7EM__)

#if __FPU_PRESENT == 1 && __FPU_USED == 1

	/// reserved space (double-word alignment)
	void* reserved;
	/// lr register with exception return value
	void* exceptionReturn;

#endif	// __FPU_PRESENT == 1 && __FPU_USED == 1
};

}	// namespace architecture

}	// namespace distortos

#endif	// SOURCE_ARCHITECTURE_ARM_ARMV6_M_ARMV7_M_ARMV8_M_ARMV6_M_ARMV7_M_ARMV8_M_SOFTWARESTACKFRAME_HPP_
