/**
 * \file
 * \brief requestContextSwitch() implementation for ARMv6-M and ARMv7-M
 *
 * \author Copyright (C) 2014-2015 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "distortos/architecture/requestContextSwitch.hpp"

#include "distortos/chip/CMSIS-proxy.h"

namespace distortos
{

namespace architecture
{

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

void requestContextSwitch()
{
	SCB->ICSR = SCB_ICSR_PENDSVSET_Msk;
}

}	// namespace architecture

}	// namespace distortos
