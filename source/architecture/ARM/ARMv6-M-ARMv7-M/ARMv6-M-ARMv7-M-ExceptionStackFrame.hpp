/**
 * \file
 * \brief ExceptionStackFrame struct header
 *
 * \author Copyright (C) 2015-2016 Kamil Szczygiel http://www.distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef SOURCE_ARCHITECTURE_ARM_ARMV6_M_ARMV7_M_ARMV6_M_ARMV7_M_EXCEPTIONSTACKFRAME_HPP_
#define SOURCE_ARCHITECTURE_ARM_ARMV6_M_ARMV7_M_ARMV6_M_ARMV7_M_EXCEPTIONSTACKFRAME_HPP_

#include <cstdint>

namespace distortos
{

namespace architecture
{

/// stack frame for ARMv6-M and ARMv7-M created by hardware on exception entry
struct ExceptionStackFrame
{
	/// default value of xPSR register
	constexpr static uintptr_t defaultXpsr {0x01000000};

	/// r0 register
	void* r0;
	/// r1 register
	void* r1;
	/// r2 register
	void* r2;
	/// r3 register
	void* r3;
	/// r12 register
	void* r12;
	/// lr register
	void* lr;
	/// pc register
	void* pc;
	/// xpsr register
	void* xpsr;
};

}	// namespace architecture

}	// namespace distortos

#endif	// SOURCE_ARCHITECTURE_ARM_ARMV6_M_ARMV7_M_ARMV6_M_ARMV7_M_EXCEPTIONSTACKFRAME_HPP_
