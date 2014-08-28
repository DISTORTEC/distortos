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
 * \date 2014-08-28
 */

#ifndef INCLUDE_DISTORTOS_SCHEDULER_SCHEDULER_HPP_
#define INCLUDE_DISTORTOS_SCHEDULER_SCHEDULER_HPP_

#include "distortos/scheduler/ThreadControlBlockList.hpp"
#include "distortos/scheduler/SoftwareTimerControlBlockSupervisor.hpp"

#include <chrono>

namespace distortos
{

/// scheduler namespace has symbols related to scheduling
namespace scheduler
{

template<typename Function, typename... Args>
class Thread;
class MainThreadControlBlock;

/// Scheduler class is a system's scheduler
class Scheduler
{
public:

	/// generic iterator for all variants of ThreadControlBlockList
	using Iterator = containers::SortedContainerBase<std::list<ThreadControlBlockListValueType>>::iterator;

	/**
	 * \brief Scheduler's constructor
	 *
	 * \attention Priority of main thread must be higher than priority of idle thread.
	 *
	 * \param [in] mainThreadControlBlock is a reference to main thread
	 * \param [in] idleThread is a reference to idle thread
	 */

	Scheduler(MainThreadControlBlock& mainThreadControlBlock, Thread<void (&)()>& idleThread);

	/**
	 * \brief Adds new ThreadControlBlock to scheduler.
	 *
	 * ThreadControlBlock's state is changed to "runnable".
	 *
	 * \param [in] thread_control_block is a reference to added ThreadControlBlock object
	 */

	void add(ThreadControlBlock& thread_control_block);

	/**
	 * \brief Blocks current thread, transferring it to provided container.
	 *
	 * \param [in] container is a reference to destination container to which the thread will be transferred
	 */

	void block(ThreadControlBlockList& container);

	/**
	 * \brief Blocks thread, transferring it to provided container.
	 *
	 * The thread must be in "runnable" state - trying to block thread in other state is an error.
	 *
	 * \param [in] container is a reference to destination container to which the thread will be transferred
	 * \param [in] iterator is the iterator to the thread that will be blocked
	 *
	 * \return 0 on success, error code otherwise:
	 * - EINVAL - provided thread is not in "runnable" state;
	 */

	int block(ThreadControlBlockList& container, Iterator iterator);

	/**
	 * \return reference to currently active ThreadControlBlock
	 */

	ThreadControlBlock& getCurrentThreadControlBlock() const { return *currentThreadControlBlock_; }

	/**
	 * \return reference to internal SoftwareTimerControlBlockSupervisor object
	 */

	SoftwareTimerControlBlockSupervisor& getSoftwareTimerSupervisor()
	{
		return softwareTimerControlBlockSupervisor_;
	}

	/**
	 * \return const reference to internal SoftwareTimerControlBlockSupervisor object
	 */

	const SoftwareTimerControlBlockSupervisor& getSoftwareTimerSupervisor() const
	{
		return softwareTimerControlBlockSupervisor_;
	}

	/**
	 * \return current value of tick count
	 */

	uint64_t getTickCount() const;

	/**
	 * \brief Removes current thread from Scheduler's control.
	 *
	 * Thread's state is changed to "terminated" and it's terminationHook() is called.
	 *
	 * \note This function can be used only after thread's function returns an all cleanup is done.
	 *
	 * \return 0 on success, error code otherwise:
	 * - EINVAL - provided thread is not in "runnable" state and cannot be removed/terminated;
	 */

	int remove();

	/**
	 * \brief Resumes suspended thread.
	 *
	 * The thread must be in "suspended" state - trying to resume thread that is not suspended is an error.
	 *
	 * \param [in] iterator is the iterator to the thread that will be resumed
	 *
	 * \return 0 on success, error code otherwise:
	 * - EINVAL - provided thread is not in "suspended" state;
	 */

	int resume(Iterator iterator);

	/**
	 * \brief Makes the calling (current) thread sleep for at least given duration.
	 *
	 * Current thread's state is changed to "sleeping".
	 *
	 * \note To fulfill the "at least" requirement, one additional tick is always added to the sleep duration.
	 *
	 * \param [in] duration is the duration after which the thread will be woken
	 */

	void sleepFor(TickClock::duration duration);

	/**
	 * \brief Makes the calling (current) thread sleep until some time point is reached.
	 *
	 * Current thread's state is changed to "sleeping".
	 *
	 * \param [in] time_point is the time point at which the thread will be woken
	 */

	void sleepUntil(TickClock::time_point time_point);

	/**
	 * \brief Suspends current thread.
	 */

	void suspend();

	/**
	 * \brief Suspends thread.
	 *
	 * The thread must be in "runnable" state - trying to suspend thread in other state is an error.
	 *
	 * \param [in] iterator is the iterator to the thread that will be suspended
	 *
	 * \return 0 on success, error code otherwise:
	 * - EINVAL - provided thread is not in "runnable" state;
	 */

	int suspend(Iterator iterator);

	/**
	 * \brief Called by architecture-specific code to do final context switch.
	 *
	 * Current task is suspended and the next available task is started.
	 *
	 * \param [in] stack_pointer is the current value of current thread's stack pointer
	 *
	 * \return new thread's stack pointer
	 */

	void* switchContext(void* stack_pointer);

	/**
	 * \brief Handler of "tick" interrupt.
	 *
	 * \note this must not be called by user code
	 *
	 * \return true if context switch is required, false otherwise
	 */

	bool tickInterruptHandler();

	/**
	 * \brief Unblocks provided thread, transferring it from provided container to "runnable" container.
	 *
	 * \param [in] container is a reference to source container from which the thread will be transferred
	 * \param [in] iterator is the iterator which points to unblocked thread
	 */

	void unblock(ThreadControlBlockList& container, Iterator iterator);

	/**
	 * \brief Yields time slot of the scheduler to next thread.
	 */

	void yield() const;

private:

	/**
	 * \brief Blocks thread, transferring it to provided container.
	 *
	 * Internal version - without interrupt masking and forced context switch.
	 *
	 * \param [in] container is a reference to destination container to which the thread will be transferred
	 * \param [in] iterator is the iterator to the thread that will be blocked
	 *
	 * \return 0 on success, error code otherwise:
	 * - EINVAL - provided thread is not in "runnable" state;
	 */

	int blockInternal_(ThreadControlBlockList& container, Iterator iterator);

	/**
	 * \brief Forces unconditional context switch.
	 *
	 * Temporarily disables any interrupt masking and requests unconditional context switch.
	 */

	void forceContextSwitch_() const;

	/**
	 * \brief Tests whether context switch is required or not.
	 *
	 * Context switch is required in following situations:
	 * - current thread is no longer in "runnable" state,
	 * - there is a higher-priority thread available,
	 * - current thread used its round-robin quantum and same-priority thread is available.
	 *
	 * \return true if context switch is required
	 */

	bool isContextSwitchRequired_() const;

	/**
	 * \brief Requests unconditional context switch.
	 *
	 * Wrapper for architecture::requestContextSwitch()
	 */

	void requestContextSwitch_() const;

	/**
	 * \brief Unblocks provided thread, transferring it from provided container to "runnable" container.
	 *
	 * Internal version - without interrupt masking and yield()
	 *
	 * \param [in] container is a reference to source container from which the thread will be transferred
	 * \param [in] iterator is the iterator which points to unblocked thread
	 */

	void unblockInternal_(ThreadControlBlockList& container, Iterator iterator);

	/// iterator to the currently active ThreadControlBlock
	Iterator currentThreadControlBlock_;

	/// list of ThreadControlBlock elements in "runnable" state, sorted by priority in descending order
	ThreadControlBlockList runnableList_;

	/// list of ThreadControlBlock elements in "suspended" state, sorted by priority in descending order
	ThreadControlBlockList suspendedList_;

	/// internal SoftwareTimerControlBlockSupervisor object
	SoftwareTimerControlBlockSupervisor softwareTimerControlBlockSupervisor_;

	/// tick count
	uint64_t tickCount_;
};

}	// namespace scheduler

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_SCHEDULER_SCHEDULER_HPP_
