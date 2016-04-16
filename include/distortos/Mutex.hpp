/**
 * \file
 * \brief Mutex class header
 *
 * \author Copyright (C) 2014-2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_MUTEX_HPP_
#define INCLUDE_DISTORTOS_MUTEX_HPP_

#include "distortos/internal/synchronization/MutexControlBlock.hpp"

namespace distortos
{

/**
 * \brief Mutex is the basic synchronization primitive
 *
 * Similar to std::mutex - http://en.cppreference.com/w/cpp/thread/mutex
 * Similar to POSIX pthread_mutex_t -
 * http://pubs.opengroup.org/onlinepubs/9699919799/functions/V2_chap02.html#tag_15_09 -> 2.9.3 Thread Mutexes
 *
 * \ingroup synchronization
 */

class Mutex
{
public:

	/// mutex protocols
	using Protocol = internal::MutexControlBlock::Protocol;

	/// type used for counting recursive locks
	using RecursiveLocksCount = uint16_t;

	/// type of mutex
	enum class Type : uint8_t
	{
		/// normal mutex, similar to PTHREAD_MUTEX_NORMAL
		normal,
		/// mutex with additional error checking, similar to PTHREAD_MUTEX_ERRORCHECK
		errorChecking,
		/// recursive mutex, similar to PTHREAD_MUTEX_RECURSIVE
		recursive
	};

	/**
	 * \brief Gets the maximum number of recursive locks possible before returning EAGAIN
	 *
	 * \note Actual number of lock() operations possible is getMaxRecursiveLocks() + 1.
	 *
	 * \return maximum number of recursive locks possible before returning EAGAIN
	 */

	constexpr static RecursiveLocksCount getMaxRecursiveLocks()
	{
		return std::numeric_limits<RecursiveLocksCount>::max();
	}

	/**
	 * \brief Mutex constructor
	 *
	 * Similar to std::mutex::mutex() - http://en.cppreference.com/w/cpp/thread/mutex/mutex
	 * Similar to pthread_mutex_init() -
	 * http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_mutex_init.html
	 *
	 * \param [in] type is the type of mutex, default - Type::normal
	 * \param [in] protocol is the mutex protocol, default - Protocol::none
	 * \param [in] priorityCeiling is the priority ceiling of mutex, ignored when protocol != Protocol::priorityProtect,
	 * default - 0
	 */

	constexpr explicit Mutex(const Type type = Type::normal, const Protocol protocol = Protocol::none,
			const uint8_t priorityCeiling = {}) :
			controlBlock_{protocol, priorityCeiling},
			recursiveLocksCount_{},
			type_{type}
	{

	}

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
	 *
	 * \return zero if the caller successfully locked the mutex, error code otherwise:
	 * - EAGAIN - the mutex could not be acquired because the maximum number of recursive locks for mutex has been
	 * exceeded;
	 * - EDEADLK - the mutex type is ErrorChecking and the current thread already owns the mutex;
	 * - EINVAL - the mutex was created with the protocol attribute having the value PriorityProtect and the calling
	 * thread's priority is higher than the mutex's current priority ceiling;
	 */

	int lock();

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
	 * \return zero if the caller successfully locked the mutex, error code otherwise:
	 * - EAGAIN - the mutex could not be acquired because the maximum number of recursive locks for mutex has been
	 * exceeded;
	 * - EBUSY - the mutex could not be acquired because it was already locked;
	 * - EINVAL - the mutex was created with the protocol attribute having the value PriorityProtect and the calling
	 * thread's priority is higher than the mutex's current priority ceiling;
	 */

	int tryLock();

	/**
	 * \brief Tries to lock the mutex for given duration of time.
	 *
	 * Similar to std::timed_mutex::try_lock_for() - http://en.cppreference.com/w/cpp/thread/timed_mutex/try_lock_for
	 * Similar to pthread_mutex_timedlock() -
	 * http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_mutex_timedlock.html#
	 *
	 * If the mutex is already locked, the calling thread shall block until the mutex becomes available as in lock()
	 * function. If the mutex cannot be locked without waiting for another thread to unlock the mutex, this wait shall
	 * be terminated when the specified timeout expires.
	 *
	 * Under no circumstance shall the function fail with a timeout if the mutex can be locked immediately. The validity
	 * of the duration parameter need not be checked if the mutex can be locked immediately.
	 *
	 * \param [in] duration is the duration after which the wait will be terminated without locking the mutex
	 *
	 * \return zero if the caller successfully locked the mutex, error code otherwise:
	 * - EAGAIN - the mutex could not be acquired because the maximum number of recursive locks for mutex has been
	 * exceeded;
	 * - EDEADLK - the mutex type is ErrorChecking and the current thread already owns the mutex;
	 * - EINVAL - the mutex was created with the protocol attribute having the value PriorityProtect and the calling
	 * thread's priority is higher than the mutex's current priority ceiling;
	 * - ETIMEDOUT - the mutex could not be locked before the specified timeout expired;
	 */

	int tryLockFor(TickClock::duration duration);

	/**
	 * Tries to lock the mutex for given duration of time.
	 *
	 * Template variant of tryLockFor(TickClock::duration duration).
	 *
	 * \tparam Rep is type of tick counter
	 * \tparam Period is std::ratio type representing the tick period of the clock, in seconds
	 *
	 * \param [in] duration is the duration after which the wait will be terminated without locking the mutex
	 *
	 * \return zero if the caller successfully locked the mutex, error code otherwise:
	 * - EAGAIN - the mutex could not be acquired because the maximum number of recursive locks for mutex has been
	 * exceeded;
	 * - EDEADLK - the mutex type is ErrorChecking and the current thread already owns the mutex;
	 * - EINVAL - the mutex was created with the protocol attribute having the value PriorityProtect and the calling
	 * thread's priority is higher than the mutex's current priority ceiling;
	 * - ETIMEDOUT - the mutex could not be locked before the specified timeout expired;
	 */

	template<typename Rep, typename Period>
	int tryLockFor(const std::chrono::duration<Rep, Period> duration)
	{
		return tryLockFor(std::chrono::duration_cast<TickClock::duration>(duration));
	}

	/**
	 * \brief Tries to lock the mutex until given time point.
	 *
	 * Similar to std::timed_mutex::try_lock_until() -
	 * http://en.cppreference.com/w/cpp/thread/timed_mutex/try_lock_until
	 * Similar to pthread_mutex_timedlock() -
	 * http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_mutex_timedlock.html#
	 *
	 * If the mutex is already locked, the calling thread shall block until the mutex becomes available as in lock()
	 * function. If the mutex cannot be locked without waiting for another thread to unlock the mutex, this wait shall
	 * be terminated when the specified timeout expires.
	 *
	 * Under no circumstance shall the function fail with a timeout if the mutex can be locked immediately. The validity
	 * of the timePoint parameter need not be checked if the mutex can be locked immediately.
	 *
	 * \param [in] timePoint is the time point at which the wait will be terminated without locking the mutex
	 *
	 * \return zero if the caller successfully locked the mutex, error code otherwise:
	 * - EAGAIN - the mutex could not be acquired because the maximum number of recursive locks for mutex has been
	 * exceeded;
	 * - EDEADLK - the mutex type is ErrorChecking and the current thread already owns the mutex;
	 * - EINVAL - the mutex was created with the protocol attribute having the value PriorityProtect and the calling
	 * thread's priority is higher than the mutex's current priority ceiling;
	 * - ETIMEDOUT - the mutex could not be locked before the specified timeout expired;
	 */

	int tryLockUntil(TickClock::time_point timePoint);

	/**
	 * \brief Tries to lock the mutex until given time point.
	 *
	 * Template variant of tryLockUntil(TickClock::time_point timePoint).
	 *
	 * \tparam Duration is a std::chrono::duration type used to measure duration
	 *
	 * \param [in] timePoint is the time point at which the wait will be terminated without locking the mutex
	 *
	 * \return zero if the caller successfully locked the mutex, error code otherwise:
	 * - EAGAIN - the mutex could not be acquired because the maximum number of recursive locks for mutex has been
	 * exceeded;
	 * - EDEADLK - the mutex type is ErrorChecking and the current thread already owns the mutex;
	 * - EINVAL - the mutex was created with the protocol attribute having the value PriorityProtect and the calling
	 * thread's priority is higher than the mutex's current priority ceiling;
	 * - ETIMEDOUT - the mutex could not be locked before the specified timeout expired;
	 */

	template<typename Duration>
	int tryLockUntil(const std::chrono::time_point<TickClock, Duration> timePoint)
	{
		return tryLockUntil(std::chrono::time_point_cast<TickClock::duration>(timePoint));
	}

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
	 *
	 * \return zero if the caller successfully unlocked the mutex, error code otherwise:
	 * - EPERM - the mutex type is ErrorChecking or Recursive, and the current thread does not own the mutex;
	 */

	int unlock();

private:

	/**
	 * \brief Internal version of tryLock().
	 *
	 * Internal version with no interrupt masking and additional code for ErrorChecking type (which is not required for
	 * tryLock()).
	 *
	 * \return zero if the caller successfully locked the mutex, error code otherwise:
	 * - EAGAIN - the mutex could not be acquired because the maximum number of recursive locks for mutex has been
	 * exceeded;
	 * - EBUSY - the mutex could not be acquired because it was already locked;
	 * - EDEADLK - the mutex type is ErrorChecking and the current thread already owns the mutex;
	 * - EINVAL - the mutex was created with the protocol attribute having the value PriorityProtect and the calling
	 * thread's priority is higher than the mutex's current priority ceiling;
	 */

	int tryLockInternal();

	/// instance of control block
	internal::MutexControlBlock controlBlock_;

	/// number of recursive locks, used when mutex type is Recursive
	RecursiveLocksCount recursiveLocksCount_;

	/// type of mutex
	Type type_;
};

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_MUTEX_HPP_
