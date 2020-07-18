/**
 * \file
 * \brief Scheduler class header
 *
 * \author Copyright (C) 2014-2018 Kamil Szczygiel http://www.distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_INTERNAL_SCHEDULER_SCHEDULER_HPP_
#define INCLUDE_DISTORTOS_INTERNAL_SCHEDULER_SCHEDULER_HPP_

#include "distortos/internal/scheduler/ThreadControlBlock.hpp"
#include "distortos/internal/scheduler/ThreadList.hpp"
#include "distortos/internal/scheduler/SoftwareTimerSupervisor.hpp"

namespace distortos
{

namespace internal
{

/// Scheduler class is a system's scheduler
class Scheduler
{
public:

	/**
	 * \brief Scheduler's constructor
	 */

	constexpr Scheduler() :
			currentThreadControlBlock_{},
			runnableList_{},
			suspendedList_{},
			softwareTimerSupervisor_{},
			contextSwitchCount_{},
			tickCount_{}
	{

	}

	/**
	 * \brief Adds new ThreadControlBlock to scheduler.
	 *
	 * ThreadControlBlock's state is changed to "runnable".
	 *
	 * \param [in] threadControlBlock is a reference to added ThreadControlBlock object
	 *
	 * \return 0 on success, error code otherwise:
	 * - EINVAL - thread is already started;
	 * - error codes returned by Scheduler::addInternal();
	 * - error codes returned by Stack::initialize();
	 */

	int add(ThreadControlBlock& threadControlBlock);

	/**
	 * \brief Blocks current thread, transferring it to provided container.
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \param [in] container is a reference to destination container to which the thread will be transferred
	 * \param [in] state is the new state of thread that will be blocked
	 * \param [in] unblockFunctor is a pointer to UnblockFunctor which will be executed in
	 * ThreadControlBlock::unblockHook(), default - nullptr (no functor will be executed)
	 *
	 * \return 0 on success, error code otherwise:
	 * - EINTR - thread was unblocked with UnblockReason::signal;
	 * - ETIMEDOUT - thread was unblocked with UnblockReason::timeout;
	 */

	int block(ThreadList& container, ThreadState state, const UnblockFunctor* unblockFunctor = {});

	/**
	 * \brief Blocks thread, transferring it to provided container.
	 *
	 * The thread must be on "runnable" list - trying to block thread in other state is an error.
	 *
	 * \param [in] container is a reference to destination container to which the thread will be transferred
	 * \param [in] iterator is the iterator to the thread that will be blocked
	 * \param [in] state is the new state of thread that will be blocked
	 * \param [in] unblockFunctor is a pointer to UnblockFunctor which will be executed in
	 * ThreadControlBlock::unblockHook(), default - nullptr (no functor will be executed)
	 *
	 * \return 0 on success, error code otherwise:
	 * - EINTR - thread was unblocked with UnblockReason::signal (possible only when blocking current thread);
	 * - EINVAL - provided thread is not on "runnable" list;
	 * - ETIMEDOUT - thread was unblocked with UnblockReason::timeout (possible only when blocking current thread);
	 */

	int block(ThreadList& container, ThreadList::iterator iterator, ThreadState state,
			const UnblockFunctor* unblockFunctor = {});

	/**
	 * \brief Blocks current thread with timeout, transferring it to provided container.
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \param [in] container is a reference to destination container to which the thread will be transferred
	 * \param [in] state is the new state of thread that will be blocked
	 * \param [in] timePoint is the time point at which the thread will be unblocked (if not already unblocked)
	 * \param [in] unblockFunctor is a pointer to UnblockFunctor which will be executed in
	 * ThreadControlBlock::unblockHook(), default - nullptr (no functor will be executed)
	 *
	 * \return 0 on success, error code otherwise:
	 * - EINTR - thread was unblocked with UnblockReason::signal;
	 * - ETIMEDOUT - thread was unblocked because timePoint was reached;
	 */

	int blockUntil(ThreadList& container, ThreadState state, TickClock::time_point timePoint,
			const UnblockFunctor* unblockFunctor = {});

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
	 * \return reference to internal SoftwareTimerSupervisor object
	 */

	SoftwareTimerSupervisor& getSoftwareTimerSupervisor()
	{
		return softwareTimerSupervisor_;
	}

	/**
	 * \return const reference to internal SoftwareTimerSupervisor object
	 */

	const SoftwareTimerSupervisor& getSoftwareTimerSupervisor() const
	{
		return softwareTimerSupervisor_;
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
	 * \param [in] mainThreadControlBlock is a reference to ThreadControlBlock object of main() thread
	 *
	 * \return 0 on success, error code otherwise:
	 * - error codes returned by Scheduler::addInternal();
	 */

	int initialize(ThreadControlBlock& mainThreadControlBlock);

	/**
	 * \brief Requests context switch if it is needed.
	 *
	 * \attention This function must be called with interrupt masking enabled.
	 */

	void maybeRequestContextSwitch() const;

	/**
	 * \brief Removes current thread from Scheduler's control.
	 *
	 * Thread's state is changed to "terminated".
	 *
	 * \note This function must be called with masked interrupts.
	 *
	 * \note This function can be used only after thread's function returns an all cleanup is done.
	 *
	 * \warning This function must not be called from interrupt context!
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

	int resume(ThreadList::iterator iterator);

	/**
	 * \brief Suspends current thread.
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \return 0 on success, error code otherwise:
	 * - EINTR - thread was unblocked with UnblockReason::signal;
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
	 * - EINTR - thread was unblocked with UnblockReason::signal;
	 * - EINVAL - provided thread is not on "runnable" list;
	 */

	int suspend(ThreadList::iterator iterator);

	/**
	 * \brief Called by architecture-specific code to do final context switch.
	 *
	 * Current task is suspended and the next available task is started. This function also checks "stack guard" when
	 * this functionality is enabled - if the check fails, FATAL_ERROR() is called.
	 *
	 * \param [in] stackPointer is the current value of current thread's stack pointer
	 *
	 * \return new thread's stack pointer
	 */

	void* switchContext(void* stackPointer);

	/**
	 * \brief Handler of "tick" interrupt.
	 *
	 * This function also checks "stack guard" when this functionality is enabled - if the check fails, FATAL_ERROR() is
	 * called.
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
	 * \param [in] unblockReason is the reason of unblocking of the thread, default - UnblockReason::unblockRequest
	 */

	void unblock(ThreadList::iterator iterator, UnblockReason unblockReason = UnblockReason::unblockRequest);

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
	 *
	 * \return 0 on success, error code otherwise:
	 * - error codes returned by ThreadControlBlock::addHook();
	 */

	int addInternal(ThreadControlBlock& threadControlBlock);

	/**
	 * \brief Blocks thread, transferring it to provided container.
	 *
	 * Internal version - without interrupt masking and forced context switch.
	 *
	 * \param [in] container is a reference to destination container to which the thread will be transferred
	 * \param [in] iterator is the iterator to the thread that will be blocked
	 * \param [in] state is the new state of thread that will be blocked
	 * \param [in] unblockFunctor is a pointer to UnblockFunctor which will be executed in
	 * ThreadControlBlock::unblockHook()
	 *
	 * \return 0 on success, error code otherwise:
	 * - EINVAL - provided thread is not on "runnable" list;
	 */

	int blockInternal(ThreadList& container, ThreadList::iterator iterator, ThreadState state,
			const UnblockFunctor* unblockFunctor);

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
	 * \param [in] unblockReason is the reason of unblocking of the thread
	 */

	void unblockInternal(ThreadList::iterator iterator, UnblockReason unblockReason);

	/// iterator to the currently active ThreadControlBlock
	ThreadList::iterator currentThreadControlBlock_;

	/// list of ThreadControlBlock elements in "runnable" state, sorted by priority in descending order
	ThreadList runnableList_;

	/// list of ThreadControlBlock elements in "suspended" state, sorted by priority in descending order
	ThreadList suspendedList_;

	/// internal SoftwareTimerSupervisor object
	SoftwareTimerSupervisor softwareTimerSupervisor_;

	/// number of context switches
	uint64_t contextSwitchCount_;

	/// tick count
	uint64_t tickCount_;
};

}	// namespace internal

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_INTERNAL_SCHEDULER_SCHEDULER_HPP_
