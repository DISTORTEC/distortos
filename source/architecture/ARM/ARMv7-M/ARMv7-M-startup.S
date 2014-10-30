/**
 * \file
 * \brief ARMv7-M (Cortex-M3 / Cortex-M4) assembly startup code
 *
 * \author Copyright (C) 2014 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2014-10-30
 */

#ifdef __USES_TWO_STACKS

#include "ARMv7-M-SpecialRegisters.h"

#endif	// __USES_TWO_STACKS

/*---------------------------------------------------------------------------------------------------------------------+
| ARMv7-M (Cortex-M3 / Cortex-M4) startup code
+---------------------------------------------------------------------------------------------------------------------*/

.text
.balign 2
.syntax unified
.thumb
.thumb_func
.global Reset_Handler

Reset_Handler:

#ifdef __USES_TWO_STACKS

	// Initialize the process stack pointer
	ldr		r0, =__process_stack_end
	msr		PSP, r0

	// Thread mode uses process stack (PSP) and is privileged
	movs	r0, #CONTROL_ALTERNATE_STACK
	msr		CONTROL, r0
	isb

#endif	// __USES_TWO_STACKS

	// Branch to lowLevelInitialization0() function (.data and .bss are not initialized!)
	ldr		r0, =lowLevelInitialization0
	blx		r0

	// Initialize sections from data_array (including .data)
	ldr		r4, =__data_array_start
	ldr		r5, =__data_array_end

1:	cmp		r4, r5							// outer loop - addresses from data_array
	ittte	lo
	ldrlo	r1, [r4], #4					// start of source address
	ldrlo	r2, [r4], #4					// start of destination address
	ldrlo	r3, [r4], #4					// end of destination address
	bhs		3f

2:	cmp		r2, r3							// inner loop - section initialization
	ittt	lo
	ldrlo	r0, [r1], #4
	strlo	r0, [r2], #4
	blo		2b

	b		1b								// go back to start

3:

	// Zero-init sections from bss_array (including .bss)
	movs	r0, #0
	ldr		r3, =__bss_array_start
	ldr		r4, =__bss_array_end

1:	cmp		r3, r4							// outer loop - addresses from data_array
	itte	lo
	ldrlo	r1, [r3], #4					// start of destination address
	ldrlo	r2, [r3], #4					// end of destination address
	bhs		3f

2:	cmp		r1, r2							// inner loop - section initialization
	itt		lo
	strlo	r0, [r1], #4
	blo		2b

	b		1b								// go back to start

3:

#ifdef __USES_CXX

	// Branch to lowLevelInitialization1() function (global and static constructors not yet executed)
	ldr		r0, =lowLevelInitialization1
	blx		r0

	// Call C++ constructors for global and static objects
	ldr		r0, =__libc_init_array
	blx		r0

#endif	// __USES_CXX

	// Branch to lowLevelInitialization2() function
	ldr		r0, =lowLevelInitialization2
	blx		r0

	// Branch to main() with link
	ldr		r0, =main
	blx		r0

#ifdef __USES_CXX

	// Call C++ destructors for global and static objects
	ldr		r0, =__libc_fini_array
	blx		r0

#endif	// __USES_CXX

	// On return - loop till the end of the world
	b		.

/*---------------------------------------------------------------------------------------------------------------------+
| defaultLowLevelInitialization() - replacement for undefined lowLevelInitialization{0,1,2}(). This function just
| returns.
+---------------------------------------------------------------------------------------------------------------------*/

.text
.balign 2
.syntax unified
.thumb
.thumb_func
.global defaultLowLevelInitialization

defaultLowLevelInitialization:
	bx		lr

/*---------------------------------------------------------------------------------------------------------------------+
| assign undefined lowLevelInitialization0() and/or lowLevelInitialization2() to defaultLowLevelInitialization()
+---------------------------------------------------------------------------------------------------------------------*/

.weak lowLevelInitialization0
.global lowLevelInitialization0
.set lowLevelInitialization0, defaultLowLevelInitialization

#ifdef __USES_CXX

.weak lowLevelInitialization1
.global lowLevelInitialization1
.set lowLevelInitialization1, defaultLowLevelInitialization

#endif	// __USES_CXX

.weak lowLevelInitialization2
.global lowLevelInitialization2
.set lowLevelInitialization2, defaultLowLevelInitialization
