/**
 * \file
 * \brief forceContextSwitch() declaration
 *
 * \author Copyright (C) 2014-2016 Kamil Szczygiel http://www.distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_INTERNAL_SCHEDULER_FORCECONTEXTSWITCH_HPP_
#define INCLUDE_DISTORTOS_INTERNAL_SCHEDULER_FORCECONTEXTSWITCH_HPP_

namespace distortos
{

namespace internal
{

/**
 * \brief Forces unconditional context switch.
 *
 * Requests unconditional context switch and temporarily disables any interrupt masking.
 */

void forceContextSwitch();

}	// namespace internal

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_INTERNAL_SCHEDULER_FORCECONTEXTSWITCH_HPP_
