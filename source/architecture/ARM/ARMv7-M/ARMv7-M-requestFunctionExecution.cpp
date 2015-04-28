/**
 * \file
 * \brief requestFunctionExecution() implementation for ARMv7-M (Cortex-M3 / Cortex-M4)
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-04-28
 */

#include "distortos/architecture/requestFunctionExecution.hpp"

#include "distortos/chip/CMSIS-proxy.h"

#include "distortos/scheduler/Scheduler.hpp"
#include "distortos/scheduler/getScheduler.hpp"

namespace distortos
{

namespace architecture
{

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

void requestFunctionExecution(const scheduler::ThreadControlBlock& threadControlBlock, void (& function)())
{
	const auto& currentThreadControlBlock = scheduler::getScheduler().getCurrentThreadControlBlock();
	if (&threadControlBlock == &currentThreadControlBlock)	// request to current thread?
	{
		const auto inInterrupt = __get_IPSR() != 0;
		if (inInterrupt == false)	// current thread is sending the request to itself?
			function();				// execute function right away
	}
}

}	// namespace architecture

}	// namespace distortos
