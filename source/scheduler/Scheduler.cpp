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
 * \date 2014-07-31
 */

#include "distortos/scheduler/Scheduler.hpp"

#include "distortos/scheduler/ThreadControlBlock.hpp"

#include "distortos/architecture.hpp"
#include "distortos/architecture/InterruptMaskingLock.hpp"

#include "distortos/distortosConfiguration.h"

#include <algorithm>

namespace distortos
{

namespace scheduler
{

namespace
{

/**
 * \brief Finds insert position after elements of the same priority.
 *
 * \param [in] list is a reference to searched list of elements
 * \param [in] priority is the priority which is searched
 *
 * \return iterator to insert position - element after the last element with given priority
 */

Scheduler::ThreadControlBlockList::iterator findInsertPosition_(Scheduler::ThreadControlBlockList &list,
		const uint8_t priority)
{
	return std::find_if(list.begin(), list.end(),
			[priority](const std::remove_reference<decltype(list)>::type::value_type &tcb) -> bool
			{
				return tcb.get().getPriority() < priority;
			}
	);
}

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

Scheduler::Scheduler() :
		tickCount_{0}
{

}

void Scheduler::add(ThreadControlBlock &thread_control_block)
{
	const auto priority = thread_control_block.getPriority();
	const auto insert_position = findInsertPosition_(threadControlBlockList_, priority);
	threadControlBlockList_.emplace(insert_position, thread_control_block);
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

void Scheduler::start()
{
	currentThreadControlBlock_ = threadControlBlockList_.begin();

	architecture::startScheduling();
}

void * Scheduler::switchContext(void *stack_pointer)
{
	getCurrentThreadControlBlock().getStack().setStackPointer(stack_pointer);

	// move current thread to the end of same-priority group to implement round-robin scheduling
	const auto priority = getCurrentThreadControlBlock().getPriority();
	const auto insert_position = findInsertPosition_(threadControlBlockList_, priority);
	threadControlBlockList_.splice(insert_position, threadControlBlockList_, currentThreadControlBlock_);
	currentThreadControlBlock_ = threadControlBlockList_.begin();

	return getCurrentThreadControlBlock().getStack().getStackPointer();
}

bool Scheduler::tickInterruptHandler()
{
	architecture::InterruptMaskingLock lock;

	++tickCount_;

	return true;
}

}	// namespace scheduler

}	// namespace distortos
