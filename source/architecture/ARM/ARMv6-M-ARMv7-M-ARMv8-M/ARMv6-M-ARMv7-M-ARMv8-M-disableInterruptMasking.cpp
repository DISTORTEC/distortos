/**
 * \file
 * \brief disableInterruptMasking() implementation for ARMv6-M and ARMv7-M
 *
 * \author Copyright (C) 2014-2019 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "distortos/architecture/disableInterruptMasking.hpp"

#include "distortos/chip/CMSIS-proxy.h"

namespace distortos
{

namespace architecture
{

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

InterruptMask disableInterruptMasking()
{
#if DISTORTOS_ARCHITECTURE_KERNEL_BASEPRI != 0

	const auto interruptMask = __get_BASEPRI();
	__set_BASEPRI(0);
	return interruptMask;

#else	// DISTORTOS_ARCHITECTURE_KERNEL_BASEPRI == 0

	const auto interruptMask = __get_PRIMASK();
	__enable_irq();
	return interruptMask;

#endif	// DISTORTOS_ARCHITECTURE_KERNEL_BASEPRI == 0
}

}	// namespace architecture

}	// namespace distortos
