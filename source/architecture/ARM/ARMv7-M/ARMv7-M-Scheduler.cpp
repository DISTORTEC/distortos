/**
 * \file
 * \brief Architecture-specific Scheduler class implementation
 *
 * \author Copyright (C) 2014 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2014-07-24
 */

#include "distortos/scheduler/Scheduler.hpp"

#include "distortos/chip/CMSIS-proxy.h"

namespace distortos
{

namespace scheduler
{

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

void Scheduler::yield() const
{
	SCB->ICSR = SCB_ICSR_PENDSVSET_Msk;
}

}	// namespace scheduler

}	// namespace distortos
