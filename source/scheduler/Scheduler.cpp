/**
 * \file
 * \brief Scheduler class implementation
 *
 * \author Copyright (C) 2014 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2014-08-05
 */

#include "distortos/scheduler/Scheduler.hpp"

#include "distortos/scheduler/idleThreadControlBlock.hpp"

#include "distortos/architecture/InterruptMaskingLock.hpp"

#include "distortos/distortosConfiguration.h"

namespace distortos
{

namespace scheduler
{

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

Scheduler::Scheduler() :
		currentThreadControlBlock_{},
		runnableList_{ThreadControlBlock::State::Runnable},
		sleepingList_{ThreadControlBlock::State::Sleeping},
		tickCount_{0}
{

}

void Scheduler::add(ThreadControlBlock &thread_control_block)
{
	architecture::InterruptMaskingLock lock;
	runnableList_.sortedEmplace(thread_control_block);
}

uint64_t Scheduler::getTickCount() const
{
	architecture::InterruptMaskingLock lock;
	return tickCount_;
}

Scheduler::TimePoint Scheduler::getTimePoint() const
{
	using TickDuration = std::chrono::duration<TimePoint::duration::rep, std::ratio<1, CONFIG_TICK_RATE_HZ>>;

	const auto tick_count = getTickCount();
	const TickDuration duration {tick_count};
	const TimePoint time_point {duration};
	return time_point;
}

void Scheduler::sleepFor(const uint64_t ticks)
{
	sleepUntil(getTickCount() + ticks + 1);
}

void Scheduler::sleepUntil(const uint64_t tick_value)
{
	architecture::InterruptMaskingLock lock;

	getCurrentThreadControlBlock().setSleepUntil(tick_value);
	sleepingList_.sortedSplice(runnableList_, currentThreadControlBlock_);

	requestContextSwitch_();
}

void Scheduler::start()
{
	add(idleThreadControlBlock);

	currentThreadControlBlock_ = runnableList_.begin();

	getCurrentThreadControlBlock().getRoundRobinQuantum().reset();

	architecture::startScheduling();
}

void * Scheduler::switchContext(void *stack_pointer)
{
	architecture::InterruptMaskingLock lock;

	getCurrentThreadControlBlock().getStack().setStackPointer(stack_pointer);

	// if the object is on the "runnable" list and it used its round-robin quantum - do the rotation
	// move current thread to the end of same-priority group to implement round-robin scheduling
	if (getCurrentThreadControlBlock().getState() == ThreadControlBlock::State::Runnable &&
			getCurrentThreadControlBlock().getRoundRobinQuantum().get() == 0)
		runnableList_.sortedSplice(runnableList_, currentThreadControlBlock_);

	currentThreadControlBlock_ = runnableList_.begin();

	getCurrentThreadControlBlock().getRoundRobinQuantum().reset();

	return getCurrentThreadControlBlock().getStack().getStackPointer();
}

bool Scheduler::tickInterruptHandler()
{
	architecture::InterruptMaskingLock lock;

	++tickCount_;

	getCurrentThreadControlBlock().getRoundRobinQuantum().decrement();

	// wake all threads that reached their timeout
	for (auto iterator = sleepingList_.begin();
			iterator != sleepingList_.end() && iterator->get().getSleepUntil() <= tickCount_;
			iterator = sleepingList_.begin())
		runnableList_.sortedSplice(sleepingList_, iterator);

	return isContextSwitchRequired_();
}

void Scheduler::yield() const
{
	if (isContextSwitchRequired_() == true)
		requestContextSwitch_();
}

/*---------------------------------------------------------------------------------------------------------------------+
| private functions
+---------------------------------------------------------------------------------------------------------------------*/

bool Scheduler::isContextSwitchRequired_() const
{
	if (getCurrentThreadControlBlock().getState() != ThreadControlBlock::State::Runnable)
		return true;

	if (runnableList_.size() == 1)	// single thread available?
		return false;				// no context switch possible

	if (runnableList_.begin() != currentThreadControlBlock_)	// is there a higher-priority thread available?
		return true;

	if (getCurrentThreadControlBlock().getRoundRobinQuantum().get() == 0)
	{
		const auto next_thread = ++runnableList_.begin();
		const auto next_thread_priority = next_thread->get().getPriority();
		// thread with same priority available?
		if (getCurrentThreadControlBlock().getPriority() == next_thread_priority)
			return true;	// switch context to do round-robin scheduling
	}

	return false;
}

void Scheduler::requestContextSwitch_() const
{
	architecture::requestContextSwitch();
}

}	// namespace scheduler

}	// namespace distortos
