/**
 * \file
 * \brief Reset_Handler() for ARMv6-M and ARMv7-M
 *
 * \author Copyright (C) 2014-2017 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/chip/CMSIS-proxy.h"

extern "C"
{

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

// weak definition of lowLevelInitialization0() called at the very beginning of Reset_Handler()
__attribute__ ((weak)) void lowLevelInitialization0()
{

}

/**
 * \brief Reset_Handler() for ARMv6-M and ARMv7-M
 */

__attribute__ ((naked)) void Reset_Handler()
{
	asm volatile
	(
			"	ldr		r0, =%[lowLevelInitialization0]		\n"		// call lowLevelInitialization0() (PSP not set,
			"	blx		r0									\n"		// CONTROL not modified, memory not initialized)
			"												\n"
			"	ldr		r0, =__process_stack_end			\n"		// initialize PSP
			"	msr		psp, r0								\n"
			"												\n"
			"	movs	r0, %[controlSpselMsk]				\n"		// thread mode uses PSP and is privileged
			"	msr		control, r0							\n"
			"	isb											\n"
			"												\n"
			"	ldr		r4, =__data_array_start				\n"		// initialize data_array (including .data)
			"	ldr		r5, =__data_array_end				\n"
			"												\n"
#ifdef __ARM_ARCH_6M__
			"1:	cmp		r4, r5								\n"		// outer loop - addresses from data_array
			"	bhs		4f									\n"
			"	ldmia	r4!, {r1-r3}						\n"		// r1 - start of source, r2 - start of destination,
			"												\n"		// r3 - end of destination
			"	b		3f									\n"
			"2:	ldmia	r1!, {r0}							\n"		// inner loop - section initialization
			"	stmia	r2!, {r0}							\n"
			"3:	cmp		r2, r3								\n"
			"	bne		2b									\n"
			"												\n"
			"	b		1b									\n"		// go back to start
			"												\n"
			"4:												\n"
			"												\n"
#else	// !def __ARM_ARCH_6M__
			"1:	cmp		r4, r5								\n"		// outer loop - addresses from data_array
			"	ite		lo									\n"
			"	ldmialo	r4!, {r1-r3}						\n"		// r1 - start of source, r2 - start of destination,
			"	bhs		3f									\n"		// r3 - end of destination
			"												\n"
			"2:	cmp		r2, r3								\n"		// inner loop - section initialization
			"	ittt	lo									\n"
			"	ldrlo	r0, [r1], #4						\n"
			"	strlo	r0, [r2], #4						\n"
			"	blo		2b									\n"
			"												\n"
			"	b		1b									\n"		// go back to start
			"												\n"
			"3:												\n"
			"												\n"
#endif	// !def __ARM_ARCH_6M__
			"	ldr		r3, =__bss_array_start				\n"		// initialize bss_array (including .bss)
			"	ldr		r4, =__bss_array_end				\n"
			"												\n"
#ifdef __ARM_ARCH_6M__
			"1:	cmp		r3, r4								\n"		// outer loop - addresses from bss_array
			"	bhs		4f									\n"
			"	ldmia	r3!, {r0-r2}						\n"		// r0 - value, r1 - start of destination, r2 - end
			"												\n"		// of destination
			"	b		3f									\n"
			"2:	stmia	r1!, {r0}							\n"		// inner loop - section initialization
			"3:	cmp		r1, r2								\n"
			"	bne		2b									\n"
			"												\n"
			"	b		1b									\n"		// go back to start
			"												\n"
			"4:												\n"
			"												\n"
#else	// !def __ARM_ARCH_6M__
			"1:	cmp		r3, r4								\n"		// outer loop - addresses from bss_array
			"	ite		lo									\n"
			"	ldmialo	r3!, {r0-r2}						\n"		// r0 - value, r1 - start of destination, r2 - end
			"	bhs		3f									\n"		// of destination
			"												\n"
			"2:	cmp		r1, r2								\n"		// inner loop - section initialization
			"	itt		lo									\n"
			"	strlo	r0, [r1], #4						\n"
			"	blo		2b									\n"
			"												\n"
			"	b		1b									\n"		// go back to start
			"												\n"
			"3:												\n"
			"												\n"
#endif	// !def __ARM_ARCH_6M__
			"	ldr		r0, =__libc_init_array				\n"		// call constructors for global and static objects
			"	blx		r0									\n"
			"												\n"
			"	ldr		r0, =main							\n"		// call main()
			"	blx		r0									\n"
			"												\n"
			"	ldr		r0, =__libc_fini_array				\n"		// call destructors for global and static objects
			"	blx		r0									\n"
			"												\n"
			"	b		.									\n"		// on return - loop till the end of the world
			"												\n"
			".ltorg											\n"		// force dumping of literal pool

			::	[controlSpselMsk] "i" (CONTROL_SPSEL_Msk),
				[lowLevelInitialization0] "i" (lowLevelInitialization0)
	);

	__builtin_unreachable();
}

}	// extern "C"
