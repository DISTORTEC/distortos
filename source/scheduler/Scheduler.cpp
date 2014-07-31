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
#include "distortos/scheduler/idleThreadControlBlock.hpp"

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
 * \brief Finds insert position in sorted list.
 *
 * Finds the insert position where Compare of value returned by Function called for list element and provided value
 * returns true.
 *
 * \param T is the type of compared value
 * \param Function is the function used to obtain values from elements of list
 * \param Compare is a type of compare object used for comparision, std::less results in descending order, while
 * std::greater - ascending
 *
 * \param [in] list is a reference to searched list of elements
 * \param [in] value is the value which will be compared with elements of the list
 *
 * \return iterator to insert position - element after the last element with given priority
 */

template<typename T, T (ThreadControlBlock::*Function)() const, typename Compare>
Scheduler::ThreadControlBlockList::iterator findInsertPosition_(Scheduler::ThreadControlBlockList &list, const T value)
{
	return std::find_if(list.begin(), list.end(),
			[value](const std::decay<decltype(list)>::type::value_type &tcb) -> bool
			{
				return Compare{}((tcb.get().*Function)(), value);
			}
	);
}

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
	using Type = std::decay<decltype(priority)>::type;
	return findInsertPosition_<Type, &ThreadControlBlock::getPriority, std::less<Type>>(list, priority);
}

/**
 * \brief Finds insert position after elements with smaller "sleep until" tick value.
 *
 * \param [in] list is a reference to searched list of elements
 * \param [in] tick_value is the "sleep until" tick value which is searched
 *
 * \return iterator to insert position - element after the last element with lower "sleep until" tick value
 */

Scheduler::ThreadControlBlockList::iterator findInsertPosition_(Scheduler::ThreadControlBlockList &list,
		const uint64_t tick_value)
{
	using Type = std::decay<decltype(tick_value)>::type;
	return findInsertPosition_<Type, &ThreadControlBlock::getSleepUntil, std::greater<Type>>(list, tick_value);
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
	architecture::InterruptMaskingLock lock;

	const auto priority = thread_control_block.getPriority();
	const auto insert_position = findInsertPosition_(runnableList_, priority);
	runnableList_.emplace(insert_position, thread_control_block);

	thread_control_block.setState(ThreadControlBlock::State::Runnable);
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

	const auto insert_position = findInsertPosition_(sleepingList_, tick_value);
	sleepingList_.splice(insert_position, runnableList_, currentThreadControlBlock_);

	getCurrentThreadControlBlock().setState(ThreadControlBlock::State::Sleeping);

	yield();
}

void Scheduler::start()
{
	add(idleThreadControlBlock);

	currentThreadControlBlock_ = runnableList_.begin();

	architecture::startScheduling();
}

void * Scheduler::switchContext(void *stack_pointer)
{
	architecture::InterruptMaskingLock lock;

	getCurrentThreadControlBlock().getStack().setStackPointer(stack_pointer);

	// do the rotation only if the object is on the "runnable" list
	if (getCurrentThreadControlBlock().getState() == ThreadControlBlock::State::Runnable)
	{
		// move current thread to the end of same-priority group to implement round-robin scheduling
		const auto priority = getCurrentThreadControlBlock().getPriority();
		const auto insert_position = findInsertPosition_(runnableList_, priority);
		runnableList_.splice(insert_position, runnableList_, currentThreadControlBlock_);
	}

	currentThreadControlBlock_ = runnableList_.begin();

	return getCurrentThreadControlBlock().getStack().getStackPointer();
}

bool Scheduler::tickInterruptHandler()
{
	architecture::InterruptMaskingLock lock;

	++tickCount_;

	auto iterator = sleepingList_.begin();

	// wake all threads that reached their timeout
	while (iterator != sleepingList_.end() && iterator->get().getSleepUntil() <= tickCount_)
	{
		const auto priority = iterator->get().getPriority();
		const auto insert_position = findInsertPosition_(runnableList_, priority);
		runnableList_.splice(insert_position, sleepingList_, iterator);
		iterator->get().setState(ThreadControlBlock::State::Runnable);

		++iterator;
	}

	return true;
}

void Scheduler::yield() const
{
	architecture::requestContextSwitch();
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

	const auto next_thread = ++runnableList_.begin();
	const auto next_thread_priority = next_thread->get().getPriority();
	if (getCurrentThreadControlBlock().getPriority() == next_thread_priority)	// next thread has the same priority?
		return true;	// switch context to do round-robin scheduling

	return false;
}

}	// namespace scheduler

}	// namespace distortos
