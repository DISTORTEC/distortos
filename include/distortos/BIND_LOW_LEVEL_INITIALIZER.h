/**
 * \file
 * \brief BIND_LOW_LEVEL_INITIALIZER() macro
 *
 * \author Copyright (C) 2018-2019 Kamil Szczygiel http://www.distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_BIND_LOW_LEVEL_INITIALIZER_H_
#define INCLUDE_DISTORTOS_BIND_LOW_LEVEL_INITIALIZER_H_

#include "distortos/internal/BIND_LOW_LEVEL_INITIALIZER_IMPLEMENTATION.h"

/**
 * \brief Binds function as low-level initializer with specified priority.
 *
 * Low-level initializers are executed after .bss and .data sections have been initialized, but before constructors for
 * global and static objects. They are automatically executed in ascending order of \a priority. When there is a group
 * of multiple low-level initializers with the same \a priority, the execution order within that group is unspecified.
 *
 * Values of \a priority used internally by distortos:
 * - 10 - main() thread and scheduler low-level initialization,
 * - 20 - idle thread low-level initialization,
 * - 30 - architecture low-level initialization,
 * - 40 - chip low-level initialization,
 * - 50 - peripherals low-level initialization,
 * - 60 - board low-level initialization,
 * - 70 - start of scheduling.
 *
 * \param [in] priority is the priority of the low-level initializer, [0; 99]
 * \param [in] function is the low-level initializer function
 */

#define BIND_LOW_LEVEL_INITIALIZER(priority, function) \
		BIND_LOW_LEVEL_INITIALIZER_IMPLEMENTATION(priority, function, ".low_level_initializers.")

#endif	/* INCLUDE_DISTORTOS_BIND_LOW_LEVEL_INITIALIZER_H_ */
