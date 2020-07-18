/**
 * \file
 * \brief Reset_Handler() for ARMv6-M and ARMv7-M
 *
 * \author Copyright (C) 2014-2019 Kamil Szczygiel http://www.distortec.com https://freddiechopin.info
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

/**
 * \brief Reset_Handler() for ARMv6-M and ARMv7-M
 */

__attribute__ ((naked)) void Reset_Handler()
{
	asm volatile
	(
	R"(
				ldr		r4, =__low_level_preinitializers_start		// execute low-level preinitializers
				ldr		r5, =__low_level_preinitializers_end

			1:	cmp		r4, r5
				bhs		2f
				ldmia	r4!, {r0}
				blx		r0
				b		1b

			2:

				ldr		r0, =__process_stack_end					// initialize PSP
				msr		psp, r0

				movs	r0, %[controlSpselMsk]						// thread mode uses PSP and is privileged
				msr		control, r0
				isb

				ldr		r4, =__data_initializers_start				// initialize data_initializers (including .data)
				ldr		r5, =__data_initializers_end

	)"
#ifdef __ARM_ARCH_6M__
	R"(
			1:	cmp		r4, r5										// outer loop - addresses from data_initializers
				bhs		4f
				ldmia	r4!, {r1-r3}								// r1 - start of source, r2 - start of destination,
																	// r3 - end of destination
				b		3f
			2:	ldmia	r1!, {r0}									// inner loop - section initialization
				stmia	r2!, {r0}
			3:	cmp		r2, r3
				bne		2b

				b		1b											// go back to start

			4:

	)"
#else	// !def __ARM_ARCH_6M__
	R"(
			1:	cmp		r4, r5										// outer loop - addresses from data_initializers
				ite		lo
				ldmialo	r4!, {r1-r3}								// r1 - start of source, r2 - start of destination,
				bhs		3f											// r3 - end of destination

			2:	cmp		r2, r3										// inner loop - section initialization
				ittt	lo
				ldrlo	r0, [r1], #4
				strlo	r0, [r2], #4
				blo		2b

				b		1b											// go back to start

			3:

	)"
#endif	// !def __ARM_ARCH_6M__
	R"(
				ldr		r3, =__bss_initializers_start				// initialize bss_initializers (including .bss)
				ldr		r4, =__bss_initializers_end

	)"
#ifdef __ARM_ARCH_6M__
	R"(
			1:	cmp		r3, r4										// outer loop - addresses from bss_initializers
				bhs		4f
				ldmia	r3!, {r0-r2}								// r0 - value, r1 - start of destination, r2 - end
																	// of destination
				b		3f
			2:	stmia	r1!, {r0}									// inner loop - section initialization
			3:	cmp		r1, r2
				bne		2b

				b		1b											// go back to start

			4:

	)"
#else	// !def __ARM_ARCH_6M__
	R"(
			1:	cmp		r3, r4										// outer loop - addresses from bss_initializers
				ite		lo
				ldmialo	r3!, {r0-r2}								// r0 - value, r1 - start of destination, r2 - end
				bhs		3f											// of destination

			2:	cmp		r1, r2										// inner loop - section initialization
				itt		lo
				strlo	r0, [r1], #4
				blo		2b

				b		1b											// go back to start

			3:

	)"
#endif	// !def __ARM_ARCH_6M__
	R"(
				ldr		r4, =__low_level_initializers_start			// execute low-level initializers
				ldr		r5, =__low_level_initializers_end

			1:	cmp		r4, r5
				bhs		2f
				ldmia	r4!, {r0}
				blx		r0
				b		1b

			2:
				ldr		r0, =__libc_init_array						// call constructors for global and static objects
				blx		r0

				ldr		r0, =main									// call main()
				blx		r0

	)"
#if DISTORTOS_STATIC_DESTRUCTORS_ENABLE == 1
	R"(
				ldr		r0, =__libc_fini_array						// call destructors for global and static objects
				blx		r0
	)"
#endif	// DISTORTOS_STATIC_DESTRUCTORS_ENABLE == 1
	R"(

				b		.											// on return - loop till the end of the world

			.ltorg													// force dumping of literal pool
	)"

			::	[controlSpselMsk] "i" (CONTROL_SPSEL_Msk)
	);

	__builtin_unreachable();
}

}	// extern "C"
