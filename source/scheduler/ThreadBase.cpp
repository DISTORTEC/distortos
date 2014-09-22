/**
 * \file
 * \brief ThreadBase class implementation
 *
 * \author Copyright (C) 2014 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2014-08-27
 */

#include "distortos/scheduler/ThreadBase.hpp"

#include "distortos/scheduler/schedulerInstance.hpp"
#include "distortos/scheduler/Scheduler.hpp"

#include <cerrno>

namespace distortos
{

namespace scheduler
{

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

ThreadBase::ThreadBase(void* const buffer, const size_t size, const uint8_t priority) :
		ThreadControlBlock{buffer, size, priority},
		joinSemaphore_{0}
{

}

int ThreadBase::join()
{
	if (this == &schedulerInstance.getCurrentThreadControlBlock())
		return EDEADLK;

	int ret;
	while ((ret = joinSemaphore_.wait()) == EINTR);
	return ret;
}

int ThreadBase::start()
{
	if (getState() != ThreadControlBlock::State::New)
		return EINVAL;

	schedulerInstance.add(*this);
	return 0;
}

/*---------------------------------------------------------------------------------------------------------------------+
| private functions
+---------------------------------------------------------------------------------------------------------------------*/

void ThreadBase::terminationHook_()
{
	joinSemaphore_.post();
}

}	// namespace scheduler

}	// namespace distortos
