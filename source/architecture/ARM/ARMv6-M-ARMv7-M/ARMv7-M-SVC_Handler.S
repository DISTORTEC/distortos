/**
 * \file
 * \brief SVC_Handler() for ARMv7-M (Cortex-M3 / Cortex-M4)
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
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
| ARMv7-M (Cortex-M3 / Cortex-M4) SVC (supervisor call) code
+---------------------------------------------------------------------------------------------------------------------*/

SVC_Handler:

	mrs		r0, PSP				// get thread stack pointer
	push	{r0, lr}

	ldr		r12, [r0], #4		// load address of function from the stack
	ldmia	r0, {r0-r3}			// load function arguments from the stack
	blx		r12					// execute the function

	pop		{r1, lr}
	str		r0, [r1]			// save returned value back on thread's stack

	bx		lr
