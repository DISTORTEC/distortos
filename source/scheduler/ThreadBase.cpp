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
 * \date 2014-08-14
 */

#include "distortos/scheduler/ThreadBase.hpp"

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
		ThreadControlBlock{buffer, size, priority}
{

}

int ThreadBase::start(Scheduler& scheduler)
{
	if (getState() != ThreadControlBlock::State::New)
		return EINVAL;

	scheduler.add(*this);
	return 0;
}

}	// namespace scheduler

}	// namespace distortos
