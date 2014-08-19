/**
 * \file
 * \brief Mutex class header
 *
 * \author Copyright (C) 2014 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2014-08-19
 */

#ifndef INCLUDE_DISTORTOS_SCHEDULER_MUTEX_HPP_
#define INCLUDE_DISTORTOS_SCHEDULER_MUTEX_HPP_

#include "distortos/scheduler/ThreadControlBlockList.hpp"

namespace distortos
{

namespace scheduler
{

/**
 * \brief Mutex is the basic synchronization primitive
 *
 * Similar to std::mutex - http://en.cppreference.com/w/cpp/thread/mutex
 * Similar to POSIX pthread_mutex_t, NORMAL, non-robust -
 * http://pubs.opengroup.org/onlinepubs/9699919799/functions/V2_chap02.html#tag_15_09 -> 2.9.3 Thread Mutexes
 */

class Mutex
{
public:

	/**
	 * \brief Mutex constructor
	 *
	 * Similar to std::mutex::mutex() - http://en.cppreference.com/w/cpp/thread/mutex/mutex
	 * Similar to pthread_mutex_init() -
	 * http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_mutex_init.html
	 */

	Mutex();

	/**
	 * \brief Locks the mutex.
	 *
	 * Similar to std::mutex::lock() - http://en.cppreference.com/w/cpp/thread/mutex/lock
	 * Similar to pthread_mutex_lock() -
	 * http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_mutex_lock.html#
	 *
	 * If the mutex is already locked by another thread, the calling thread shall block until the mutex becomes
	 * available. This function shall return with the mutex in the locked state with the calling thread as its owner. If
	 * a thread attempts to relock a mutex that it has already locked, deadlock occurs.
	 */

	void lock();

	/**
	 * \brief Tries to lock the mutex.
	 *
	 * Similar to std::mutex::try_lock() - http://en.cppreference.com/w/cpp/thread/mutex/try_lock
	 * Similar to pthread_mutex_trylock() -
	 * http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_mutex_lock.html#
	 *
	 * This function shall be equivalent to lock(), except that if the mutex is currently locked (by any thread,
	 * including the current thread), the call shall return immediately.
	 *
	 * \return true if the calling thread successfully performed the lock operation, false otherwise
	 */

	bool tryLock();

	/**
	 * \brief Unlocks the mutex.
	 *
	 * Similar to std::mutex::unlock() - http://en.cppreference.com/w/cpp/thread/mutex/unlock
	 * Similar to pthread_mutex_unlock() -
	 * http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_mutex_lock.html#
	 *
	 * The mutex must be locked by the current thread, otherwise, the behavior is undefined. If there are threads
	 * blocked on this mutex, the highest priority waiting thread shall be unblocked, and if there is more than one
	 * highest priority thread blocked waiting, then the highest priority thread that has been waiting the longest shall
	 * be unblocked.
	 */

	void unlock();

private:

	/// ThreadControlBlock objects blocked on this mutex
	ThreadControlBlockList blockedList_;

	/// owner of the mutex
	ThreadControlBlock* owner_;
};

}	// namespace scheduler

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_SCHEDULER_MUTEX_HPP_
