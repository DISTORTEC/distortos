/**
 * \file
 * \brief supervisorCall() implementation for ARMv6-M, ARMv7-M and ARMv8-M
 *
 * \author Copyright (C) 2015-2024 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "ARMv6-M-ARMv7-M-ARMv8-M-supervisorCall.hpp"

namespace distortos
{

namespace architecture
{

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

__attribute__ ((naked))
int supervisorCall(int (& function)(int, int, int, int), const int argument1, const int argument2, const int argument3,
		const int argument4)
{
	asm volatile
	(
#ifdef __ARM_ARCH_6M__
			"	mov		r12, r0			\n"	// copy first argument from r0 to r12
			"	ldr		r0, [sp]		\n"	// load the last argument from stack to r0
			"	svc		0				\n"
#else	// !def __ARM_ARCH_6M__
			"	ldr		r12, [sp]		\n"	// load the last argument from stack to r12
			"	svc		0				\n"
#endif	// !def __ARM_ARCH_6M__
			"							\n"
			"	bx		lr				\n"	// return
	);

	__builtin_unreachable();

	// suppress warnings
	(void)function;
	(void)argument1;
	(void)argument2;
	(void)argument3;
	(void)argument4;
}

}	// namespace architecture

}	// namespace distortos
