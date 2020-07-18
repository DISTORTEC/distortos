/**
 * \file
 * \brief checkFpuRegisters() definition for ARMv7-M
 *
 * \author Copyright (C) 2015 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "ARMv7-M-checkFpuRegisters.hpp"

#include "distortos/chip/CMSIS-proxy.h"

namespace distortos
{

namespace test
{

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

bool checkFpuRegisters(const uint32_t value, const uint32_t fpscr)
{
#if __FPU_PRESENT == 1 && __FPU_USED == 1

	bool result {true};
	asm volatile
	(
			"	vmov	r2, s0					\n"	// move test value from FPU register
			"	cmp		r2, %[value]			\n"	// compare with expected value
			"	it		ne						\n"	// no match?
			"	bne		1f						\n"	// branch to error handling
			"	vmov	r2, s1					\n"
			"	cmp		r2, %[value]			\n"
			"	it		ne						\n"
			"	bne		1f						\n"
			"	vmov	r2, s2					\n"
			"	cmp		r2, %[value]			\n"
			"	it		ne						\n"
			"	bne		1f						\n"
			"	vmov	r2, s3					\n"
			"	cmp		r2, %[value]			\n"
			"	it		ne						\n"
			"	bne		1f						\n"
			"	vmov	r2, s4					\n"
			"	cmp		r2, %[value]			\n"
			"	it		ne						\n"
			"	bne		1f						\n"
			"	vmov	r2, s5					\n"
			"	cmp		r2, %[value]			\n"
			"	it		ne						\n"
			"	bne		1f						\n"
			"	vmov	r2, s6					\n"
			"	cmp		r2, %[value]			\n"
			"	it		ne						\n"
			"	bne		1f						\n"
			"	vmov	r2, s7					\n"
			"	cmp		r2, %[value]			\n"
			"	it		ne						\n"
			"	bne		1f						\n"
			"	vmov	r2, s8					\n"
			"	cmp		r2, %[value]			\n"
			"	it		ne						\n"
			"	bne		1f						\n"
			"	vmov	r2, s9					\n"
			"	cmp		r2, %[value]			\n"
			"	it		ne						\n"
			"	bne		1f						\n"
			"	vmov	r2, s10					\n"
			"	cmp		r2, %[value]			\n"
			"	it		ne						\n"
			"	bne		1f						\n"
			"	vmov	r2, s11					\n"
			"	cmp		r2, %[value]			\n"
			"	it		ne						\n"
			"	bne		1f						\n"
			"	vmov	r2, s12					\n"
			"	cmp		r2, %[value]			\n"
			"	it		ne						\n"
			"	bne		1f						\n"
			"	vmov	r2, s13					\n"
			"	cmp		r2, %[value]			\n"
			"	it		ne						\n"
			"	bne		1f						\n"
			"	vmov	r2, s14					\n"
			"	cmp		r2, %[value]			\n"
			"	it		ne						\n"
			"	bne		1f						\n"
			"	vmov	r2, s15					\n"
			"	cmp		r2, %[value]			\n"
			"	it		ne						\n"
			"	bne		1f						\n"
			"	vmov	r2, s16					\n"
			"	cmp		r2, %[value]			\n"
			"	it		ne						\n"
			"	bne		1f						\n"
			"	vmov	r2, s17					\n"
			"	cmp		r2, %[value]			\n"
			"	it		ne						\n"
			"	bne		1f						\n"
			"	vmov	r2, s18					\n"
			"	cmp		r2, %[value]			\n"
			"	it		ne						\n"
			"	bne		1f						\n"
			"	vmov	r2, s19					\n"
			"	cmp		r2, %[value]			\n"
			"	it		ne						\n"
			"	bne		1f						\n"
			"	vmov	r2, s20					\n"
			"	cmp		r2, %[value]			\n"
			"	it		ne						\n"
			"	bne		1f						\n"
			"	vmov	r2, s21					\n"
			"	cmp		r2, %[value]			\n"
			"	it		ne						\n"
			"	bne		1f						\n"
			"	vmov	r2, s22					\n"
			"	cmp		r2, %[value]			\n"
			"	it		ne						\n"
			"	bne		1f						\n"
			"	vmov	r2, s23					\n"
			"	cmp		r2, %[value]			\n"
			"	it		ne						\n"
			"	bne		1f						\n"
			"	vmov	r2, s24					\n"
			"	cmp		r2, %[value]			\n"
			"	it		ne						\n"
			"	bne		1f						\n"
			"	vmov	r2, s25					\n"
			"	cmp		r2, %[value]			\n"
			"	it		ne						\n"
			"	bne		1f						\n"
			"	vmov	r2, s26					\n"
			"	cmp		r2, %[value]			\n"
			"	it		ne						\n"
			"	bne		1f						\n"
			"	vmov	r2, s27					\n"
			"	cmp		r2, %[value]			\n"
			"	it		ne						\n"
			"	bne		1f						\n"
			"	vmov	r2, s28					\n"
			"	cmp		r2, %[value]			\n"
			"	it		ne						\n"
			"	bne		1f						\n"
			"	vmov	r2, s29					\n"
			"	cmp		r2, %[value]			\n"
			"	it		ne						\n"
			"	bne		1f						\n"
			"	vmov	r2, s30					\n"
			"	cmp		r2, %[value]			\n"
			"	it		ne						\n"
			"	bne		1f						\n"
			"	vmov	r2, s31					\n"
			"	cmp		r2, %[value]			\n"
			"	it		ne						\n"
			"	bne		1f						\n"
			"	vmrs	r2, fpscr				\n"	// move test value from fpscr
			"	cmp		r2, %[fpscr]			\n"	// compare with expected fpscr value

			"	it		eq						\n"
			"	beq		2f						\n"	// branch "out" if all compared values were equal

			"1:									\n"	// error handling
			"	mov		%[result], #0			\n"	// result = false;

			"2:									\n"

			:	[result] "+r" (result)
			:	[value] "r" (value),
				[fpscr] "r" (fpscr)
			:	"r2"
	);

	return result;

#else

	// suppress warnings
	(void)value;
	(void)fpscr;

	return true;

#endif	// __FPU_PRESENT == 1 && __FPU_USED == 1
}

}	// namespace test

}	// namespace distortos
