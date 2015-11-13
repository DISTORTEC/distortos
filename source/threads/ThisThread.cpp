/**
 * \file
 * \brief ThisThread namespace implementation
 *
 * \author Copyright (C) 2014-2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-11-13
 */

#include "distortos/ThisThread.hpp"

#include "distortos/scheduler/getScheduler.hpp"
#include "distortos/scheduler/Scheduler.hpp"

#include <cerrno>

namespace distortos
{

namespace ThisThread
{

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

Thread& get()
{
	return scheduler::getScheduler().getCurrentThreadControlBlock().getOwner();
}

uint8_t getEffectivePriority()
{
	return scheduler::getScheduler().getCurrentThreadControlBlock().getEffectivePriority();
}

uint8_t getPriority()
{
	return scheduler::getScheduler().getCurrentThreadControlBlock().getPriority();
}

void setPriority(const uint8_t priority, const bool alwaysBehind)
{
	scheduler::getScheduler().getCurrentThreadControlBlock().setPriority(priority, alwaysBehind);
}

int sleepFor(const TickClock::duration duration)
{
	return sleepUntil(TickClock::now() + duration + TickClock::duration{1});
}

int sleepUntil(const TickClock::time_point timePoint)
{
	auto& scheduler = scheduler::getScheduler();
	scheduler::ThreadControlBlockList sleepingList {scheduler.getThreadControlBlockListAllocator(),
			ThreadState::Sleeping};
	const auto ret = scheduler.blockUntil(sleepingList, timePoint);
	return ret == ETIMEDOUT ? 0 : ret;
}

void yield()
{
	scheduler::getScheduler().yield();
}

}	// namespace ThisThread

}	// namespace distortos
