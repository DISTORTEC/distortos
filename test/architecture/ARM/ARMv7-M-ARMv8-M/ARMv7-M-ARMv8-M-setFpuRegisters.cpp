/**
 * \file
 * \brief setFpuRegisters() definition for ARMv7-M
 *
 * \author Copyright (C) 2015-2024 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "ARMv7-M-ARMv8-M-setFpuRegisters.hpp"

#include "distortos/chip/CMSIS-proxy.h"

namespace distortos
{

namespace test
{

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

uint32_t setFpuRegisters(const uint32_t value, const bool full)
{
#if __FPU_PRESENT == 1 && __FPU_USED == 1

	asm volatile
	(
			"	vmov	s0, %[value]	\n"	// move test value to FPU register
			"	vmov	s1, %[value]	\n"
			"	vmov	s2, %[value]	\n"
			"	vmov	s3, %[value]	\n"
			"	vmov	s4, %[value]	\n"
			"	vmov	s5, %[value]	\n"
			"	vmov	s6, %[value]	\n"
			"	vmov	s7, %[value]	\n"
			"	vmov	s8, %[value]	\n"
			"	vmov	s9, %[value]	\n"
			"	vmov	s10, %[value]	\n"
			"	vmov	s11, %[value]	\n"
			"	vmov	s12, %[value]	\n"
			"	vmov	s13, %[value]	\n"
			"	vmov	s14, %[value]	\n"
			"	vmov	s15, %[value]	\n"

			::	[value] "r" (value)
	);

	if (full == true)
	{
		asm volatile
		(
				"	vmov	s16, %[value]	\n"
				"	vmov	s17, %[value]	\n"
				"	vmov	s18, %[value]	\n"
				"	vmov	s19, %[value]	\n"
				"	vmov	s20, %[value]	\n"
				"	vmov	s21, %[value]	\n"
				"	vmov	s22, %[value]	\n"
				"	vmov	s23, %[value]	\n"
				"	vmov	s24, %[value]	\n"
				"	vmov	s25, %[value]	\n"
				"	vmov	s26, %[value]	\n"
				"	vmov	s27, %[value]	\n"
				"	vmov	s28, %[value]	\n"
				"	vmov	s29, %[value]	\n"
				"	vmov	s30, %[value]	\n"
				"	vmov	s31, %[value]	\n"

				::	[value] "r" (value)
		);
	}

	uint32_t fpscr;
	asm volatile
	(
			"	vmsr	fpscr, %[value]	\n"	// move test value to fpscr
			"	vmrs	%[fpscr], fpscr	\n"	// read fpscr to variable (not all fields of fpscr are writable)

			:	[fpscr] "=r" (fpscr)
			:	[value] "r" (value)
	);

	return fpscr;

#else

	(void)full;	// suppress warning

	return value;

#endif	// __FPU_PRESENT == 1 && __FPU_USED == 1
}

}	// namespace test

}	// namespace distortos
