/**
 * \file
 * \brief ConditionVariable class header
 *
 * \author Copyright (C) 2014 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2014-11-20
 */

#ifndef INCLUDE_DISTORTOS_CONDITIONVARIABLE_HPP_
#define INCLUDE_DISTORTOS_CONDITIONVARIABLE_HPP_

#include "distortos/scheduler/ThreadControlBlockList.hpp"

namespace distortos
{

class Mutex;

/**
 * \brief ConditionVariable is an advanced synchronization primitive
 *
 * Similar to std::condition_variable - http://en.cppreference.com/w/cpp/thread/condition_variable
 * Similar to POSIX pthread_cond_t
 */

class ConditionVariable
{
public:

	/**
	 * \brief ConditionVariable constructor
	 *
	 * Similar to std::condition_variable::condition_variable() -
	 * http://en.cppreference.com/w/cpp/thread/condition_variable/condition_variable
	 * Similar to pthread_cond_init() - http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_cond_init.html
	 */

	ConditionVariable();

	/**
	 * \brief Notifies all waiting threads.
	 *
	 * Similar to std::condition_variable::notify_all() -
	 * http://en.cppreference.com/w/cpp/thread/condition_variable/notify_all
	 * Similar to pthread_cond_broadcast() -
	 * http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_cond_signal.html
	 *
	 * Unblocks all threads waiting on this condition variable. The notifying thread does not need to hold the same
	 * mutex as the one held by the waiting thread(s).
	 */

	void notifyAll();

	/**
	 * \brief Notifies one waiting thread.
	 *
	 * Similar to std::condition_variable::notify_one() -
	 * http://en.cppreference.com/w/cpp/thread/condition_variable/notify_one
	 * Similar to pthread_cond_signal() -
	 * http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_cond_signal.html
	 *
	 * Unblocks one thread waiting on this condition variable. The notifying thread does not need to hold the same
	 * mutex as the one held by the waiting thread(s).
	 */

	void notifyOne();

	/**
	 * \brief Waits for notification.
	 *
	 * Similar to std::condition_variable::wait() - http://en.cppreference.com/w/cpp/thread/condition_variable/wait
	 * Similar to pthread_cond_wait() - http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_cond_wait.html
	 *
	 * Atomically releases supplied mutex and blocks current thread until the condition variable is notified. The thread
	 * will be unblocked when notifyAll() or notifyOne() is executed. When unblocked, regardless of the reason, lock is
	 * reacquired and wait exits.
	 *
	 * \param [in] mutex is a reference to mutex which must be owned by calling thread
	 *
	 * \return zero if the wait was completed successfully, error code otherwise:
	 * - EPERM - the mutex type is ErrorChecking or Recursive, and the current thread does not own the mutex;
	 */

	int wait(Mutex& mutex);

	/**
	 * \brief Waits for predicate to become true.
	 *
	 * Similar to std::condition_variable::wait() - http://en.cppreference.com/w/cpp/thread/condition_variable/wait
	 * Similar to pthread_cond_wait() - http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_cond_wait.html
	 *
	 * Overload for wait() which also checks the predicate. This function will return only if the predicate is true.
	 *
	 * \param Predicate is a type of functor to check the predicate
	 *
	 * \param [in] mutex is a reference to mutex which must be owned by calling thread
	 * \param [in] predicate is the predicate that will be checked
	 *
	 * \return zero if the wait was completed successfully, error code otherwise:
	 * - EPERM - the mutex type is ErrorChecking or Recursive, and the current thread does not own the mutex;
	 */

	template<typename Predicate>
	int wait(Mutex& mutex, Predicate predicate)
	{
		while (predicate() == false)
		{
			const auto ret = wait(mutex);
			if (ret != 0)
				return ret;
		}

		return 0;
	}

private:

	/// ThreadControlBlock objects blocked on this condition variable
	scheduler::ThreadControlBlockList blockedList_;
};

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_CONDITIONVARIABLE_HPP_
