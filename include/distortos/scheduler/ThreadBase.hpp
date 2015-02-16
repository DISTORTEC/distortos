/**
 * \file
 * \brief ThreadBase class header
 *
 * \author Copyright (C) 2014-2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-02-16
 */

#ifndef INCLUDE_DISTORTOS_SCHEDULER_THREADBASE_HPP_
#define INCLUDE_DISTORTOS_SCHEDULER_THREADBASE_HPP_

#include "distortos/scheduler/ThreadControlBlock.hpp"

#include "distortos/Semaphore.hpp"

namespace distortos
{

namespace scheduler
{

/// ThreadBase class is a base for threads
class ThreadBase
{
public:

	/**
	 * \brief ThreadBase's constructor.
	 *
	 * \param [in] buffer is a pointer to stack's buffer
	 * \param [in] size is the size of stack's buffer, bytes
	 * \param [in] priority is the thread's priority, 0 - lowest, UINT8_MAX - highest
	 * \param [in] schedulingPolicy is the scheduling policy of the thread
	 */

	ThreadBase(void* buffer, size_t size, uint8_t priority, SchedulingPolicy schedulingPolicy);

	/**
	 * \brief ThreadBase's constructor.
	 *
	 * \param [in] stack is an rvalue reference to architecture::Stack object which will be adopted for this thread
	 * \param [in] priority is the thread's priority, 0 - lowest, UINT8_MAX - highest
	 * \param [in] schedulingPolicy is the scheduling policy of the thread
	 */

	ThreadBase(architecture::Stack&& stack, uint8_t priority, SchedulingPolicy schedulingPolicy);

	/**
	 * \return effective priority of thread
	 */

	uint8_t getEffectivePriority() const
	{
		return threadControlBlock_.getEffectivePriority();
	}

	/**
	 * \return priority of thread
	 */

	uint8_t getPriority() const
	{
		return threadControlBlock_.getPriority();
	}

	/**
	 * \return scheduling policy of the thread
	 */

	SchedulingPolicy getSchedulingPolicy() const
	{
		return threadControlBlock_.getSchedulingPolicy();
	}

	/**
	 * \return current state of thread
	 */

	ThreadControlBlock::State getState() const
	{
		return threadControlBlock_.getState();
	}

	/**
	 * \brief Waits for thread termination.
	 *
	 * Similar to std::thread::join() - http://en.cppreference.com/w/cpp/thread/thread/join
	 * Similar to POSIX pthread_join() - http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_join.html
	 *
	 * Blocks current thread until this thread finishes its execution. The results of multiple simultaneous calls to
	 * join() on the same target thread are undefined.
	 *
	 * \return 0 on success, error code otherwise:
	 * - EDEADLK - deadlock condition was detected,
	 * - EINVAL - this thread is not joinable,
	 * - ...
	 */

	int join();

	/**
	 * \brief Changes priority of thread.
	 *
	 * If the priority really changes, the position in the thread list is adjusted and context switch may be requested.
	 *
	 * \param [in] priority is the new priority of thread
	 * \param [in] alwaysBehind selects the method of ordering when lowering the priority
	 * - false - the thread is moved to the head of the group of threads with the new priority (default),
	 * - true - the thread is moved to the tail of the group of threads with the new priority.
	 */

	void setPriority(const uint8_t priority, const bool alwaysBehind = {})
	{
		threadControlBlock_.setPriority(priority, alwaysBehind);
	}

	/**
	 * param [in] schedulingPolicy is the new scheduling policy of the thread
	 */

	void setSchedulingPolicy(const SchedulingPolicy schedulingPolicy)
	{
		threadControlBlock_.setSchedulingPolicy(schedulingPolicy);
	}

	/**
	 * \brief Starts the thread.
	 *
	 * This operation can be performed on threads in "New" state only.
	 *
	 * \return 0 on success, error code otherwise:
	 * - EINVAL - thread is already started
	 */

	int start();

	ThreadBase(const ThreadBase&) = delete;
	ThreadBase(ThreadBase&&) = default;
	const ThreadBase& operator=(const ThreadBase&) = delete;
	ThreadBase& operator=(ThreadBase&&) = delete;

protected:

	/**
	 * \brief ThreadBase's destructor
	 *
	 * \note Polymorphic objects of ThreadBase type must not be deleted via pointer/reference
	 */

	~ThreadBase()
	{

	}

	/**
	 * \return reference to internal ThreadControlBlock object
	 */

	ThreadControlBlock& getThreadControlBlock()
	{
		return threadControlBlock_;
	}

private:

	/**
	 * \brief Thread runner function - entry point of threads.
	 *
	 * After return from actual thread function, thread is terminated, so this function never returns.
	 *
	 * \param [in] threadBase is a reference to ThreadBase object that is being run
	 */

	static void threadRunner(ThreadBase& threadBase) __attribute__ ((noreturn));

	/**
	 * \brief "Run" function of thread
	 *
	 * This should be overridden by derived classes.
	 */

	virtual void run() = 0;

	/**
	 * \brief Termination hook function of thread
	 *
	 * This function is called after run() completes, from Scheduler::remove().
	 */

	void terminationHook();

	/// internal ThreadControlBlock object
	ThreadControlBlock threadControlBlock_;

	/// semaphore used by join()
	Semaphore joinSemaphore_;
};

}	// namespace scheduler

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_SCHEDULER_THREADBASE_HPP_
