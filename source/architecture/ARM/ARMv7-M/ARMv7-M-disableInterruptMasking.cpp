/**
 * \file
 * \brief disableInterruptMasking() implementation for ARMv7-M (Cortex-M3 / Cortex-M4)
 *
 * \author Copyright (C) 2014-2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-04-28
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
	const auto interruptMask = __get_BASEPRI();
	__set_BASEPRI(0);
	return interruptMask;
}

}	// namespace architecture

}	// namespace distortos
