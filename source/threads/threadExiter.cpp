/**
 * \file
 * \brief threadExiter() definition
 *
 * \author Copyright (C) 2014-2017 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "distortos/internal/scheduler/threadExiter.hpp"

#include "distortos/internal/scheduler/forceContextSwitch.hpp"
#include "distortos/internal/scheduler/getScheduler.hpp"
#include "distortos/internal/scheduler/RunnableThread.hpp"
#include "distortos/internal/scheduler/Scheduler.hpp"

#include "distortos/InterruptMaskingLock.hpp"

namespace distortos
{

namespace internal
{

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

void threadExiter(RunnableThread& runnableThread)
{
	{
		const InterruptMaskingLock interruptMaskingLock;

		runnableThread.exit0Hook();
		internal::getScheduler().remove();
		runnableThread.exit1Hook();
	}

	internal::forceContextSwitch();

	while (1);
}

}	// namespace internal

}	// namespace distortos
