/**
 * \file
 * \brief Thread class implementation
 *
 * \author Copyright (C) 2014-2016 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2016-01-02
 */

#include "distortos/Thread.hpp"

#include "distortos/internal/scheduler/forceContextSwitch.hpp"
#include "distortos/internal/scheduler/getScheduler.hpp"
#include "distortos/internal/scheduler/Scheduler.hpp"

#include "distortos/architecture/InterruptMaskingLock.hpp"

namespace distortos
{

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

Thread::~Thread()
{

}

/*---------------------------------------------------------------------------------------------------------------------+
| protected static functions
+---------------------------------------------------------------------------------------------------------------------*/

void Thread::threadRunner(Thread& thread, void (& run)(Thread&))
{
	run(thread);

	{
		architecture::InterruptMaskingLock interruptMaskingLock;

		internal::getScheduler().remove(&Thread::terminationHook);
	}

	internal::forceContextSwitch();

	while (1);
}

}	// namespace distortos
