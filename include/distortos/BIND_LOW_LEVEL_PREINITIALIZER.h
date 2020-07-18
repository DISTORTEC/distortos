/**
 * \file
 * \brief BIND_LOW_LEVEL_PREINITIALIZER() macro
 *
 * \author Copyright (C) 2018-2019 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_BIND_LOW_LEVEL_PREINITIALIZER_H_
#define INCLUDE_DISTORTOS_BIND_LOW_LEVEL_PREINITIALIZER_H_

#include "distortos/internal/BIND_LOW_LEVEL_INITIALIZER_IMPLEMENTATION.h"

/**
 * \brief Binds function as low-level preinitializer with specified priority.
 *
 * Low-level preinitializers are executed before .bss and .data sections' initialization, before constructors for global
 * and static objects. They are automatically executed in ascending order of \a priority. When there is a group of
 * multiple low-level preinitializers with the same \a priority, the execution order within that group is unspecified.
 *
 * \param [in] priority is the priority of the low-level preinitializer, [0; 99]
 * \param [in] function is the low-level preinitializer function
 */

#define BIND_LOW_LEVEL_PREINITIALIZER(priority, function) \
		BIND_LOW_LEVEL_INITIALIZER_IMPLEMENTATION(priority, function, ".low_level_preinitializers.")

#endif	/* INCLUDE_DISTORTOS_BIND_LOW_LEVEL_PREINITIALIZER_H_ */
