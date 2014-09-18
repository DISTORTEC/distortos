/**
 * \file
 * \brief ThreadBase class header
 *
 * \author Copyright (C) 2014 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2014-09-18
 */

#ifndef INCLUDE_DISTORTOS_SCHEDULER_THREADBASE_HPP_
#define INCLUDE_DISTORTOS_SCHEDULER_THREADBASE_HPP_

#include "distortos/scheduler/ThreadControlBlock.hpp"

#include "distortos/scheduler/Semaphore.hpp"

namespace distortos
{

namespace scheduler
{

/// ThreadBase class is a base for threads
class ThreadBase : private ThreadControlBlock
{
public:

	/**
	 * \brief ThreadBase's constructor.
	 *
	 * \param [in] buffer is a pointer to stack's buffer
	 * \param [in] size is the size of stack's buffer, bytes
	 * \param [in] priority is the thread's priority, 0 - lowest, UINT8_MAX - highest
	 */

	ThreadBase(void* buffer, size_t size, uint8_t priority);

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
	 * \brief Starts the thread.
	 *
	 * This operation can be performed on threads in "New" state only.
	 *
	 * \return 0 on success, error code otherwise:
	 * - EINVAL - thread is already started
	 */

	int start();

	using ThreadControlBlock::getPriority;

	using ThreadControlBlock::setPriority;

protected:

	/**
	 * \brief ThreadBase's destructor
	 *
	 * \note Polymorphic objects of ThreadBase type must not be deleted via pointer/reference
	 */

	~ThreadBase()
	{

	}

private:

	/**
	 * \brief Termination hook function of thread
	 *
	 * This function is called after run() completes.
	 */

	virtual void terminationHook_() override;

	/// semaphore used by join()
	Semaphore joinSemaphore_;
};

}	// namespace scheduler

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_SCHEDULER_THREADBASE_HPP_
