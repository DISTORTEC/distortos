/**
 * \file
 * \brief SVC_Handler() for ARMv6-M and ARMv7-M
 *
 * \author Copyright (C) 2015-2016 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

.text
.balign 2
.syntax unified
.thumb
.thumb_func
.global SVC_Handler

/*---------------------------------------------------------------------------------------------------------------------+
| ARMv6-M and ARMv7-M SVC (supervisor call) code
+---------------------------------------------------------------------------------------------------------------------*/

SVC_Handler:

	mrs		r0, psp				// get thread stack pointer
	push	{r0, lr}

#ifdef __ARM_ARCH_6M__

	ldr		r1, [r0, #0x10]		// load address of function from the stack
	mov		r12, r1
	ldmia	r0!, {r3}			// load last function argument from the stack
	ldmia	r0, {r0-r2}			// load remaining function arguments from the stack
	blx		r12					// execute the function

	pop		{r1, r2}
	str		r0, [r1]			// save returned value back on thread's stack

	bx		r2

#else	// !def __ARM_ARCH_6M__

	ldr		r12, [r0], #4		// load address of function from the stack
	ldmia	r0, {r0-r3}			// load function arguments from the stack
	blx		r12					// execute the function

	pop		{r1, lr}
	str		r0, [r1]			// save returned value back on thread's stack

	bx		lr

#endif	// !def __ARM_ARCH_6M__
