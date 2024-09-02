/**
 * \file
 * \brief SVC_Handler() for ARMv6-M, ARMv7-M and ARMv8-M
 *
 * \author Copyright (C) 2015-2024 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief SVC_Handler() for ARMv6-M, ARMv7-M and ARMv8-M
 *
 * Calls provided function with arguments.
 */

extern "C" __attribute__ ((naked)) void SVC_Handler()
{
	asm volatile
	(
			"	mrs		r0, psp				\n"		// get thread stack pointer
			"	push	{r0, lr}			\n"
			"								\n"
#ifdef __ARM_ARCH_6M__
			"	ldr		r1, [r0, #0x10]		\n"		// load address of function from the stack
			"	mov		r12, r1				\n"
			"	ldmia	r0!, {r3}			\n"		// load last function argument from the stack
			"	ldmia	r0, {r0-r2}			\n"		// load remaining function arguments from the stack
			"	blx		r12					\n"		// execute the function
			"								\n"
			"	pop		{r1, r2}			\n"
			"	str		r0, [r1]			\n"		// save returned value back on thread's stack
			"								\n"
			"	bx		r2					\n"
#else	// !def __ARM_ARCH_6M__
			"	ldr		r12, [r0], #4		\n"		// load address of function from the stack
			"	ldmia	r0, {r0-r3}			\n"		// load function arguments from the stack
			"	blx		r12					\n"		// execute the function
			"								\n"
			"	pop		{r1, lr}			\n"
			"	str		r0, [r1]			\n"		// save returned value back on thread's stack
			"								\n"
			"	bx		lr					\n"
#endif	// !def __ARM_ARCH_6M__
	);

	__builtin_unreachable();
}
