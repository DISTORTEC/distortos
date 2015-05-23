/**
 * \file
 * \brief Header with definition of bits in CONTROL register in ARMv7-M core (Cortex-M3 / Cortex-M4).
 *
 * \author Copyright (C) 2014-2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-05-23
 */

#ifndef SOURCE_ARCHITECTURE_ARM_ARMV7_M_ARMV7_M_CONTROL_H_
#define SOURCE_ARCHITECTURE_ARM_ARMV7_M_ARMV7_M_CONTROL_H_

/*---------------------------------------------------------------------------------------------------------------------+
| CONTROL - The special-purpose control register
+---------------------------------------------------------------------------------------------------------------------*/

#define CONTROL_THREAD_UNPRIVILEGED_bit		0
#define CONTROL_ALTERNATE_STACK_bit			1

#define CONTROL_THREAD_UNPRIVILEGED			(1 << CONTROL_THREAD_UNPRIVILEGED_bit)
#define CONTROL_ALTERNATE_STACK				(1 << CONTROL_ALTERNATE_STACK_bit)

#endif	// SOURCE_ARCHITECTURE_ARM_ARMV7_M_ARMV7_M_CONTROL_H_
