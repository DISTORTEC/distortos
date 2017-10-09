/**
 * \file
 * \brief threadRunner() definition
 *
 * \author Copyright (C) 2014-2017 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/internal/scheduler/threadRunner.hpp"

#include "distortos/internal/scheduler/forceContextSwitch.hpp"
#include "distortos/internal/scheduler/getScheduler.hpp"
#include "distortos/internal/scheduler/Scheduler.hpp"

#include "distortos/InterruptMaskingLock.hpp"
#ifdef CONFIG_THREAD_EXIT_ENABLE
#include <distortos/ThisThread.hpp>
#endif

namespace distortos
{

namespace internal
{

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

void threadRunner(Thread& thread, void (& run)(Thread&), void (* preTerminationHook)(Thread&),
		void (& terminationHook)(Thread&))
{
	run(thread);
#ifdef CONFIG_THREAD_EXIT_ENABLE
	(void)preTerminationHook;
	(void)terminationHook;
	ThisThread::exit();
#else
	{
		const InterruptMaskingLock interruptMaskingLock;

		if (preTerminationHook != nullptr)
			preTerminationHook(thread);
		internal::getScheduler().remove();
		terminationHook(thread);
	}

	internal::forceContextSwitch();

	while (1);
#endif
}

}	// namespace internal

}	// namespace distortos
