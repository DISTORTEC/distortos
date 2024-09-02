/**
 * \file
 * \brief InterruptMask type alias
 *
 * \author Copyright (C) 2014-2024 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef SOURCE_ARCHITECTURE_ARM_ARMV6_M_ARMV7_M_ARMV8_M_INCLUDE_DISTORTOS_ARCHITECTURE_INTERRUPTMASK_HPP_
#define SOURCE_ARCHITECTURE_ARM_ARMV6_M_ARMV7_M_ARMV8_M_INCLUDE_DISTORTOS_ARCHITECTURE_INTERRUPTMASK_HPP_

#include <cstdint>

namespace distortos
{

namespace architecture
{

/// interrupt mask
using InterruptMask = uint32_t;

}	// namespace architecture

}	// namespace distortos

#endif	// SOURCE_ARCHITECTURE_ARM_ARMV6_M_ARMV7_M_ARMV8_M_INCLUDE_DISTORTOS_ARCHITECTURE_INTERRUPTMASK_HPP_
