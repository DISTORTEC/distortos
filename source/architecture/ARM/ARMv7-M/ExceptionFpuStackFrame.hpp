/**
 * \file
 * \brief ExceptionFpuStackFrame struct header
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-05-27
 */

#ifndef SOURCE_ARCHITECTURE_ARM_ARMV7_M_EXCEPTIONFPUSTACKFRAME_HPP_
#define SOURCE_ARCHITECTURE_ARM_ARMV7_M_EXCEPTIONFPUSTACKFRAME_HPP_

#include "ExceptionStackFrame.hpp"

#include "distortos/chip/CMSIS-proxy.h"

namespace distortos
{

namespace architecture
{

/// stack frame for ARMv7-M (Cortex-M4F) created by hardware on exception entry when FPU context is active
struct ExceptionFpuStackFrame
{
	/// stack frame created by hardware on exception entry
	ExceptionStackFrame exceptionStackFrame;

#if __FPU_PRESENT == 1 && __FPU_USED == 1

	/// S0 register
	void* s0;
	/// S1 register
	void* s1;
	/// S2 register
	void* s2;
	/// S3 register
	void* s3;
	/// S4 register
	void* s4;
	/// S5 register
	void* s5;
	/// S6 register
	void* s6;
	/// S7 register
	void* s7;
	/// S8 register
	void* s8;
	/// S9 register
	void* s9;
	/// S10 register
	void* s10;
	/// S11 register
	void* s11;
	/// S12 register
	void* s12;
	/// S13 register
	void* s13;
	/// S14 register
	void* s14;
	/// S15 register
	void* s15;
	/// FPSCR register
	void* fpscr;
	/// reserved space (double-word alignment)
	void* reserved;

#endif	// __FPU_PRESENT == 1 && __FPU_USED == 1
};

}	// namespace architecture

}	// namespace distortos

#endif	// SOURCE_ARCHITECTURE_ARM_ARMV7_M_EXCEPTIONFPUSTACKFRAME_HPP_
