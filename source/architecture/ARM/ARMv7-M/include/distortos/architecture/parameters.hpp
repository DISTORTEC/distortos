/**
 * \file
 * \brief Architecture-specific parameters
 *
 * \author Copyright (C) 2014 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2014-07-24
 */

#ifndef SOURCE_ARCHITECTURE_ARM_ARMV7_M_INCLUDE_DISTORTOS_ARCHITECTURE_PARAMETERS_HPP_
#define SOURCE_ARCHITECTURE_ARM_ARMV7_M_INCLUDE_DISTORTOS_ARCHITECTURE_PARAMETERS_HPP_

#include <cstddef>

namespace distortos
{

namespace architecture
{

/// alignment of stack, bytes
constexpr size_t stackAlignment {8};

/// divisibility of stack's size
constexpr size_t stackSizeDivisibility {8};

}	// namespace architecture

}	// namespace distortos

#endif	// SOURCE_ARCHITECTURE_ARM_ARMV7_M_INCLUDE_DISTORTOS_ARCHITECTURE_PARAMETERS_HPP_
