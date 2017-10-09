/**
 * \file
 * \brief ThisThread namespace implementation
 *
 * \author Copyright (C) 2014-2017 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/ThisThread.hpp"

#include "distortos/internal/scheduler/getScheduler.hpp"
#include "distortos/internal/scheduler/Scheduler.hpp"

#include "distortos/internal/CHECK_FUNCTION_CONTEXT.hpp"

#include "distortos/ThreadCommon.hpp"

#include <cerrno>

namespace distortos
{

namespace ThisThread
{

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

#ifdef CONFIG_THREAD_DETACH_ENABLE

int detach()
{
	return ThisThread::get().detach();
}

#endif	// def CONFIG_THREAD_DETACH_ENABLE

Thread& get()
{
	CHECK_FUNCTION_CONTEXT();

	auto & thcom = internal::getScheduler().getCurrentThreadControlBlock().getOwner();
#ifdef CONFIG_THREAD_DETACH_ENABLE
	return thcom.getThreadInterface();
#else
	return thcom;
#endif
}

uint8_t getEffectivePriority()
{
	CHECK_FUNCTION_CONTEXT();

	return internal::getScheduler().getCurrentThreadControlBlock().getEffectivePriority();
}

uint8_t getPriority()
{
	CHECK_FUNCTION_CONTEXT();

	return internal::getScheduler().getCurrentThreadControlBlock().getPriority();
}

SchedulingPolicy getSchedulingPolicy()
{
	CHECK_FUNCTION_CONTEXT();

	return internal::getScheduler().getCurrentThreadControlBlock().getSchedulingPolicy();
}

size_t getStackHighWaterMark()
{
	return get().getStackHighWaterMark();
}

size_t getStackSize()
{
	return get().getStackSize();
}

void setPriority(const uint8_t priority, const bool alwaysBehind)
{
	CHECK_FUNCTION_CONTEXT();

	internal::getScheduler().getCurrentThreadControlBlock().setPriority(priority, alwaysBehind);
}

void setSchedulingPolicy(const SchedulingPolicy schedulingPolicy)
{
	CHECK_FUNCTION_CONTEXT();

	internal::getScheduler().getCurrentThreadControlBlock().setSchedulingPolicy(schedulingPolicy);
}

int sleepFor(const TickClock::duration duration)
{
	return sleepUntil(TickClock::now() + duration + TickClock::duration{1});
}

int sleepUntil(const TickClock::time_point timePoint)
{
	auto& scheduler = internal::getScheduler();
	internal::ThreadList sleepingList;
	const auto ret = scheduler.blockUntil(sleepingList, ThreadState::sleeping, timePoint);
	return ret == ETIMEDOUT ? 0 : ret;
}

void yield()
{
	CHECK_FUNCTION_CONTEXT();

	internal::getScheduler().yield();
}

#ifdef CONFIG_THREAD_EXIT_ENABLE
void	exit()
{
	internal::getScheduler().getCurrentThreadControlBlock().getOwner().exit();
}
#endif

}	// namespace ThisThread

}	// namespace distortos
