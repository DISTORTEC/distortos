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
 * \date 2015-03-12
 */

#include "distortos/architecture/architecture.hpp"

#include "distortos/distortosConfiguration.h"

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
	__set_BASEPRI(CONFIG_ARCHITECTURE_ARMV7_M_KERNEL_BASEPRI << (8 - __NVIC_PRIO_BITS));
	return interruptMask;
}

}	// namespace architecture

}	// namespace distortos
