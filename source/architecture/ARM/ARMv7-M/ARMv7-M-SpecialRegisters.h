/**
 * \file
 * \brief Header with definition of bits in special registers in ARMv7-M core (Cortex-M3 / Cortex-M4).
 *
 * \author Copyright (C) 2014 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2014-07-21
 */

#ifndef ARMV7_M_SPECIALREGISTERS_H_
#define ARMV7_M_SPECIALREGISTERS_H_

/*---------------------------------------------------------------------------------------------------------------------+
| PRIMASK - Register to mask out configurable exceptions
+---------------------------------------------------------------------------------------------------------------------*/

#define PRIMASK_MASK_EXCEPTIONS_bit			0

#define PRIMASK_MASK_EXCEPTIONS				(1 << PRIMASK_MASK_EXCEPTIONS_bit)

/*---------------------------------------------------------------------------------------------------------------------+
| FAULTMASK - Register to raise priority to the HardFault level
+---------------------------------------------------------------------------------------------------------------------*/

#define FAULTMASK_MASK_EXCEPTIONS_bit		0

#define FAULTMASK_MASK_EXCEPTIONS			(1 << FAULTMASK_MASK_EXCEPTIONS_bit)

/*---------------------------------------------------------------------------------------------------------------------+
| CONTROL - The special-purpose control register
+---------------------------------------------------------------------------------------------------------------------*/

#define CONTROL_THREAD_UNPRIVILEGED_bit		0
#define CONTROL_ALTERNATE_STACK_bit			1

#define CONTROL_THREAD_UNPRIVILEGED			(1 << CONTROL_THREAD_UNPRIVILEGED_bit)
#define CONTROL_ALTERNATE_STACK				(1 << CONTROL_ALTERNATE_STACK_bit)

#endif	// ARMV7_M_SPECIALREGISTERS_H_
