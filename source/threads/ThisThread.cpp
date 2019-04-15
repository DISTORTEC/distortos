/**
 * \file
 * \brief ThisThread namespace implementation
 *
 * \author Copyright (C) 2014-2019 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/ThisThread.hpp"

#include "distortos/internal/scheduler/getScheduler.hpp"
#include "distortos/internal/scheduler/RunnableThread.hpp"
#include "distortos/internal/scheduler/Scheduler.hpp"
#include "distortos/internal/scheduler/threadExiter.hpp"

#include "distortos/internal/CHECK_FUNCTION_CONTEXT.hpp"

#include "distortos/ThreadIdentifier.hpp"

#include <cerrno>

namespace distortos
{

namespace ThisThread
{

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

#ifdef DISTORTOS_THREAD_DETACH_ENABLE

int detach()
{
	return ThisThread::get().detach();
}

#endif	// def DISTORTOS_THREAD_DETACH_ENABLE

void exit()
{
	CHECK_FUNCTION_CONTEXT();

	internal::threadExiter(internal::getScheduler().getCurrentThreadControlBlock().getOwner());
}

Thread& get()
{
	CHECK_FUNCTION_CONTEXT();

	return internal::getScheduler().getCurrentThreadControlBlock().getOwner();
}

uint8_t getEffectivePriority()
{
	CHECK_FUNCTION_CONTEXT();

	return internal::getScheduler().getCurrentThreadControlBlock().getEffectivePriority();
}

ThreadIdentifier getIdentifier()
{
	CHECK_FUNCTION_CONTEXT();

	const auto& threadControlBlock = internal::getScheduler().getCurrentThreadControlBlock();
	return {threadControlBlock, threadControlBlock.getSequenceNumber()};
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

}	// namespace ThisThread

}	// namespace distortos
