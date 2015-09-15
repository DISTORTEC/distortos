/**
 * \file
 * \brief enableInterruptMasking() implementation for ARMv7-M (Cortex-M3 / Cortex-M4)
 *
 * \author Copyright (C) 2014-2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-09-15
 */

#include "distortos/architecture/enableInterruptMasking.hpp"

#include "distortos/chip/CMSIS-proxy.h"

namespace distortos
{

namespace architecture
{

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

InterruptMask enableInterruptMasking()
{
	const auto interruptMask = __get_BASEPRI();
	constexpr auto basepriValue = CONFIG_ARCHITECTURE_ARMV7_M_KERNEL_BASEPRI << (8 - __NVIC_PRIO_BITS);
	static_assert(basepriValue > 0 && basepriValue <= UINT8_MAX,
			"Invalid CONFIG_ARCHITECTURE_ARMV7_M_KERNEL_BASEPRI value!");
	__set_BASEPRI(basepriValue);
	return interruptMask;
}

}	// namespace architecture

}	// namespace distortos
