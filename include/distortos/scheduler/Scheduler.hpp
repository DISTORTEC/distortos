/**
 * \file
 * \brief Scheduler class header
 *
 * \author Copyright (C) 2014-2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-02-16
 */

#ifndef INCLUDE_DISTORTOS_SCHEDULER_SCHEDULER_HPP_
#define INCLUDE_DISTORTOS_SCHEDULER_SCHEDULER_HPP_

#include "distortos/scheduler/ThreadControlBlockList.hpp"
#include "distortos/scheduler/SoftwareTimerControlBlockSupervisor.hpp"

namespace distortos
{

/// scheduler namespace has symbols related to scheduling
namespace scheduler
{

class MainThread;

/// Scheduler class is a system's scheduler
class Scheduler
{
public:

	/**
	 * \brief Scheduler's constructor
	 */

	Scheduler();

	/**
	 * \brief Adds new ThreadControlBlock to scheduler.
	 *
	 * ThreadControlBlock's state is changed to "runnable".
	 *
	 * \param [in] threadControlBlock is a reference to added ThreadControlBlock object
	 */

	void add(ThreadControlBlock& threadControlBlock);

	/**
	 * \brief Blocks current thread, transferring it to provided container.
	 *
	 * \param [in] container is a reference to destination container to which the thread will be transferred
	 * \param [in] unblockFunctor is a pointer to ThreadControlBlock::UnblockFunctor which will be executed in
	 * ThreadControlBlock::unblockHook(), default - nullptr (no functor will be executed)
	 *
	 * \return 0 on success, error code otherwise:
	 * - ETIMEDOUT - thread was unblocked with ThreadControlBlock::UnblockReason::Timeout;
	 */

	int block(ThreadControlBlockList& container, const ThreadControlBlock::UnblockFunctor* unblockFunctor = {});

	/**
	 * \brief Blocks thread, transferring it to provided container.
	 *
	 * The thread must be on "runnable" list - trying to block thread in other state is an error.
	 *
	 * \param [in] container is a reference to destination container to which the thread will be transferred
	 * \param [in] iterator is the iterator to the thread that will be blocked
	 * \param [in] unblockFunctor is a pointer to ThreadControlBlock::UnblockFunctor which will be executed in
	 * ThreadControlBlock::unblockHook(), default - nullptr (no functor will be executed)
	 *
	 * \return 0 on success, error code otherwise:
	 * - EINVAL - provided thread is not on "runnable" list;
	 * - ETIMEDOUT - thread was unblocked with ThreadControlBlock::UnblockReason::Timeout (possible only when blocking
	 * current thread);
	 */

	int block(ThreadControlBlockList& container, ThreadControlBlockListIterator iterator,
			const ThreadControlBlock::UnblockFunctor* unblockFunctor = {});

	/**
	 * \brief Blocks current thread with timeout, transferring it to provided container.
	 *
	 * \param [in] container is a reference to destination container to which the thread will be transferred
	 * \param [in] timePoint is the time point at which the thread will be unblocked (if not already unblocked)
	 * \param [in] unblockFunctor is a pointer to ThreadControlBlock::UnblockFunctor which will be executed in
	 * ThreadControlBlock::unblockHook(), default - nullptr (no functor will be executed)
	 *
	 * \return 0 on success, error code otherwise:
	 * - ETIMEDOUT - thread was unblocked because timePoint was reached;
	 */

	int blockUntil(ThreadControlBlockList& container, TickClock::time_point timePoint,
			const ThreadControlBlock::UnblockFunctor* unblockFunctor = {});

	/**
	 * \return number of context switches
	 */

	uint64_t getContextSwitchCount() const;

	/**
	 * \return reference to currently active ThreadControlBlock
	 */

	ThreadControlBlock& getCurrentThreadControlBlock() const
	{
		return *currentThreadControlBlock_;
	}

	/**
	 * \return reference to internal MutexControlBlockListAllocator::Pool object
	 */

	MutexControlBlockListAllocator::Pool& getMutexControlBlockListAllocatorPool()
	{
		return mutexControlBlockListAllocatorPool_;
	}

	/**
	 * \return const reference to internal MutexControlBlockListAllocator:Pool object
	 */

	const MutexControlBlockListAllocator::Pool& getMutexControlBlockListAllocatorPool() const
	{
		return mutexControlBlockListAllocatorPool_;
	}

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
	 * \return const reference to internal ThreadControlBlockListAllocator object
	 */

	const ThreadControlBlockListAllocator& getThreadControlBlockListAllocator() const
	{
		return threadControlBlockListAllocator_;
	}

	/**
	 * \return current value of tick count
	 */

	uint64_t getTickCount() const;

	/**
	 * \brief Scheduler's initialization
	 *
	 * \attention This must be called after constructor, before enabling any scheduling. Priority of main thread must
	 * be higher than priority of idle thread
	 *
	 * \param [in] mainThread is a reference to main thread
	 */

	void initialize(MainThread& mainThread);

	/**
	 * \brief Requests context switch if it is needed.
	 *
	 * \attention This function must be called with interrupt masking enabled.
	 */

	void maybeRequestContextSwitch() const;

	/**
	 * \brief Removes current thread from Scheduler's control.
	 *
	 * Thread's state is changed to "terminated" and it's terminationHook() is called.
	 *
	 * \note This function can be used only after thread's function returns an all cleanup is done.
	 *
	 * \return 0 on success, error code otherwise:
	 * - EINVAL - provided thread is not on "runnable" list and cannot be removed/terminated;
	 */

	int remove();

	/**
	 * \brief Resumes suspended thread.
	 *
	 * The thread must be on the "suspended" list - trying to resume thread that is not suspended is an error.
	 *
	 * \param [in] iterator is the iterator to the thread that will be resumed
	 *
	 * \return 0 on success, error code otherwise:
	 * - EINVAL - provided thread is not on "suspended" list;
	 */

	int resume(ThreadControlBlockListIterator iterator);

	/**
	 * \brief Suspends current thread.
	 *
	 * \return 0 on success, error code otherwise
	 */

	int suspend();

	/**
	 * \brief Suspends thread.
	 *
	 * The thread must be on "runnable" list - trying to suspend thread in other state is an error.
	 *
	 * \param [in] iterator is the iterator to the thread that will be suspended
	 *
	 * \return 0 on success, error code otherwise:
	 * - EINVAL - provided thread is not on "runnable" list;
	 */

	int suspend(ThreadControlBlockListIterator iterator);

	/**
	 * \brief Called by architecture-specific code to do final context switch.
	 *
	 * Current task is suspended and the next available task is started.
	 *
	 * \param [in] stackPointer is the current value of current thread's stack pointer
	 *
	 * \return new thread's stack pointer
	 */

	void* switchContext(void* stackPointer);

	/**
	 * \brief Handler of "tick" interrupt.
	 *
	 * \note this must not be called by user code
	 *
	 * \return true if context switch is required, false otherwise
	 */

	bool tickInterruptHandler();

	/**
	 * \brief Unblocks provided thread, transferring it from it's current container to "runnable" container.
	 *
	 * Current container of the thread is obtained with ThreadControlBlock::getList().
	 *
	 * \param [in] iterator is the iterator which points to unblocked thread
	 */

	void unblock(ThreadControlBlockListIterator iterator);

	/**
	 * \brief Yields time slot of the scheduler to next thread.
	 */

	void yield();

private:

	/**
	 * \brief Adds new ThreadControlBlock to scheduler.
	 *
	 * Internal version - without interrupt masking and call to Scheduler::maybeRequestContextSwitch()
	 *
	 * \param [in] threadControlBlock is a reference to added ThreadControlBlock object
	 */

	void addInternal(ThreadControlBlock& threadControlBlock);

	/**
	 * \brief Blocks thread, transferring it to provided container.
	 *
	 * Internal version - without interrupt masking and forced context switch.
	 *
	 * \param [in] container is a reference to destination container to which the thread will be transferred
	 * \param [in] iterator is the iterator to the thread that will be blocked
	 * \param [in] unblockFunctor is a pointer to ThreadControlBlock::UnblockFunctor which will be executed in
	 * ThreadControlBlock::unblockHook()
	 *
	 * \return 0 on success, error code otherwise:
	 * - EINVAL - provided thread is not on "runnable" list;
	 */

	int blockInternal(ThreadControlBlockList& container, ThreadControlBlockListIterator iterator,
			const ThreadControlBlock::UnblockFunctor* unblockFunctor);

	/**
	 * \brief Tests whether context switch is required or not.
	 *
	 * Context switch is required in following situations:
	 * - current thread is no longer on "runnable" list,
	 * - current thread is no longer on the beginning of the "runnable" list (because higher-priority thread is
	 * available or current thread was "rotated" due to round-robin scheduling policy).
	 *
	 * \return true if context switch is required
	 */

	bool isContextSwitchRequired() const;

	/**
	 * \brief Unblocks provided thread, transferring it from it's current container to "runnable" container.
	 *
	 * Current container of the thread is obtained with ThreadControlBlock::getList(). Round-robin quantum of thread is
	 * reset.
	 *
	 * \note Internal version - without interrupt masking and yield()
	 *
	 * \param [in] iterator is the iterator which points to unblocked thread
	 * \param [in] unblockReason is the reason of unblocking of the thread, default -
	 * ThreadControlBlock::UnblockReason::UnblockRequest
	 */

	void unblockInternal(ThreadControlBlockListIterator iterator,
			ThreadControlBlock::UnblockReason unblockReason = ThreadControlBlock::UnblockReason::UnblockRequest);

	/// iterator to the currently active ThreadControlBlock
	ThreadControlBlockListIterator currentThreadControlBlock_;

	/// pool instance used by MutexControlBlockListAllocator
	MutexControlBlockListAllocator::Pool mutexControlBlockListAllocatorPool_;

	/// pool instance used by threadControlBlockListAllocator_
	ThreadControlBlockListAllocator::Pool threadControlBlockListAllocatorPool_;

	/// PoolAllocator<> of ThreadControlBlockList
	ThreadControlBlockListAllocator threadControlBlockListAllocator_;

	/// list of ThreadControlBlock elements in "runnable" state, sorted by priority in descending order
	ThreadControlBlockList runnableList_;

	/// list of ThreadControlBlock elements in "suspended" state, sorted by priority in descending order
	ThreadControlBlockList suspendedList_;

	/// internal SoftwareTimerControlBlockSupervisor object
	SoftwareTimerControlBlockSupervisor softwareTimerControlBlockSupervisor_;

	/// number of context switches
	uint64_t contextSwitchCount_;

	/// tick count
	uint64_t tickCount_;
};

}	// namespace scheduler

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_SCHEDULER_SCHEDULER_HPP_
