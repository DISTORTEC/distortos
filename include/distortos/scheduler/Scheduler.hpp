/**
 * \file
 * \brief Scheduler class header
 *
 * \author Copyright (C) 2014 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2014-07-31
 */

#ifndef INCLUDE_DISTORTOS_SCHEDULER_SCHEDULER_HPP_
#define INCLUDE_DISTORTOS_SCHEDULER_SCHEDULER_HPP_

#include <list>
#include <functional>
#include <chrono>

namespace distortos
{

/// scheduler namespace has symbols related to scheduling
namespace scheduler
{

class ThreadControlBlock;

/// Scheduler class is a system's scheduler
class Scheduler
{
public:

	/// Scheduler's clock (as in std::chrono)
	struct Clock
	{
		/// duration (as in std::chrono) used by Scheduler's clock
		using duration = std::chrono::nanoseconds;
	};

	/// time point (as in std::chrono) of Scheduler
	using TimePoint = std::chrono::time_point<Clock>;

	/// underlying type of list of ThreadControlBlock elements
	using ThreadControlBlockList = std::list<std::reference_wrapper<ThreadControlBlock>>;

	/**
	 * \brief Scheduler's constructor
	 */

	Scheduler();

	/**
	 * \brief Adds new ThreadControlBlock to scheduler.
	 *
	 * ThreadControlBlock's state is changed to "runnable".
	 *
	 * \param [in] thread_control_block is a reference to added ThreadControlBlock object
	 */

	void add(ThreadControlBlock &thread_control_block);

	/**
	 * \return reference to currently active ThreadControlBlock
	 */

	ThreadControlBlock & getCurrentThreadControlBlock() const { return *currentThreadControlBlock_; }

	/**
	 * \return current value of tick count
	 */

	uint64_t getTickCount() const;

	/**
	 * \return current time point of Scheduler's clock
	 */

	TimePoint getTimePoint() const;

	/**
	 * \brief Makes the calling (current) thread sleep until some tick value is reached.
	 *
	 * Current thread's state is changed to "sleeping".
	 *
	 * \param [in] tick_value is the tick value at which the thread will be woken
	 */

	void sleepUntil(uint64_t tick_value);

	/**
	 * \brief Starts scheduling.
	 *
	 * Initializes scheduler and all required hardware/software to perform context switching and starts the scheduling.
	 */

	__attribute__ ((noreturn))
	void start();

	/**
	 * \brief Called by architecture-specific code to do final context switch.
	 *
	 * Current task is suspended and the next available task is started.
	 *
	 * \param [in] stack_pointer is the current value of current thread's stack pointer
	 *
	 * \return new thread's stack pointer
	 */

	void * switchContext(void *stack_pointer);

	/**
	 * \brief Handler of "tick" interrupt.
	 *
	 * \note this must not be called by user code
	 *
	 * \return true if context switch is required, false otherwise
	 */

	bool tickInterruptHandler();

	/**
	 * \brief Yields time slot of the scheduler to next thread.
	 */

	void yield() const;

private:

	/// iterator to the currently active ThreadControlBlock
	ThreadControlBlockList::iterator currentThreadControlBlock_;

	/// list of ThreadControlBlock elements in "runnable" state
	ThreadControlBlockList runnableList_;

	/// list of ThreadControlBlock elements in "sleeping" state
	ThreadControlBlockList sleepingList_;

	/// tick count
	uint64_t tickCount_;
};

}	// namespace scheduler

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_SCHEDULER_SCHEDULER_HPP_
