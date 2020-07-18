/**
 * \file
 * \brief stackGuardSize constant
 *
 * \author Copyright (C) 2017-2019 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_INTERNAL_SCHEDULER_STACKGUARDSIZE_HPP_
#define INCLUDE_DISTORTOS_INTERNAL_SCHEDULER_STACKGUARDSIZE_HPP_

#include "distortos/distortosConfiguration.h"

#include <cstddef>

namespace distortos
{

namespace internal
{

/// size of "stack guard", bytes
constexpr size_t stackGuardSize {(DISTORTOS_STACK_GUARD_SIZE + DISTORTOS_ARCHITECTURE_STACK_ALIGNMENT - 1) /
		DISTORTOS_ARCHITECTURE_STACK_ALIGNMENT * DISTORTOS_ARCHITECTURE_STACK_ALIGNMENT};

}	// namespace internal

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_INTERNAL_SCHEDULER_STACKGUARDSIZE_HPP_
