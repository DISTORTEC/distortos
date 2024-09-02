/**
 * \file
 * \brief ExceptionFpuStackFrame struct header
 *
 * \author Copyright (C) 2015-2024 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef SOURCE_ARCHITECTURE_ARM_ARMV6_M_ARMV7_M_ARMV8_M_ARMV7_M_ARMV8_M_EXCEPTIONFPUSTACKFRAME_HPP_
#define SOURCE_ARCHITECTURE_ARM_ARMV6_M_ARMV7_M_ARMV8_M_ARMV7_M_ARMV8_M_EXCEPTIONFPUSTACKFRAME_HPP_

#include "ARMv6-M-ARMv7-M-ARMv8-M-ExceptionStackFrame.hpp"

#include "distortos/chip/CMSIS-proxy.h"

namespace distortos
{

namespace architecture
{

/// stack frame for ARMv7-M created by hardware on exception entry when FPU context is active
struct ExceptionFpuStackFrame
{
	/// stack frame created by hardware on exception entry
	ExceptionStackFrame exceptionStackFrame;

#if __FPU_PRESENT == 1 && __FPU_USED == 1

	/// s0 register
	void* s0;
	/// s1 register
	void* s1;
	/// s2 register
	void* s2;
	/// s3 register
	void* s3;
	/// s4 register
	void* s4;
	/// s5 register
	void* s5;
	/// s6 register
	void* s6;
	/// s7 register
	void* s7;
	/// s8 register
	void* s8;
	/// s9 register
	void* s9;
	/// s10 register
	void* s10;
	/// s11 register
	void* s11;
	/// s12 register
	void* s12;
	/// s13 register
	void* s13;
	/// s14 register
	void* s14;
	/// s15 register
	void* s15;
	/// fpscr register
	void* fpscr;
	/// reserved space (double-word alignment)
	void* reserved;

#endif	// __FPU_PRESENT == 1 && __FPU_USED == 1
};

}	// namespace architecture

}	// namespace distortos

#endif	// SOURCE_ARCHITECTURE_ARM_ARMV6_M_ARMV7_M_ARMV8_M_ARMV7_M_ARMV8_M_EXCEPTIONFPUSTACKFRAME_HPP_
