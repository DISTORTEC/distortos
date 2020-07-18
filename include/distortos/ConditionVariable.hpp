/**
 * \file
 * \brief ConditionVariable class header
 *
 * \author Copyright (C) 2014-2017 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_CONDITIONVARIABLE_HPP_
#define INCLUDE_DISTORTOS_CONDITIONVARIABLE_HPP_

#include "distortos/internal/scheduler/ThreadList.hpp"

#include "distortos/internal/CHECK_FUNCTION_CONTEXT.hpp"

#include "distortos/TickClock.hpp"

namespace distortos
{

class Mutex;

/**
 * \brief ConditionVariable is an advanced synchronization primitive
 *
 * Similar to std::condition_variable - http://en.cppreference.com/w/cpp/thread/condition_variable
 * Similar to POSIX pthread_cond_t
 *
 * \ingroup synchronization
 */

class ConditionVariable
{
public:

	/**
	 * \brief ConditionVariable's constructor
	 *
	 * Similar to std::condition_variable::condition_variable() -
	 * http://en.cppreference.com/w/cpp/thread/condition_variable/condition_variable
	 * Similar to pthread_cond_init() - http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_cond_init.html
	 */

	constexpr ConditionVariable() :
			blockedList_{}
	{

	}

	/**
	 * \brief ConditionVariable's destructor
	 *
	 * Similar to std::condition_variable::~condition_variable() -
	 * http://en.cppreference.com/w/cpp/thread/condition_variable/~condition_variable
	 * Similar to pthread_cond_destroy() -
	 * http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_cond_destroy.html
	 *
	 * It shall be safe to destroy an initialized condition variable upon which no threads are currently blocked.
	 * Attempting to destroy a condition variable upon which other threads are currently blocked results in undefined
	 * behavior.
	 */

	~ConditionVariable() = default;

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
	 * will be unblocked when notifyAll() or notifyOne() is executed. It may also be unblocked spuriously. When
	 * unblocked, regardless of the reason, lock is reacquired and wait exits.
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \param [in] mutex is a reference to mutex which must be owned by calling thread
	 *
	 * \return 0 if the wait was completed successfully, error code otherwise:
	 * - EPERM - the mutex type is errorChecking or recursive, and the current thread does not own the mutex;
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
	 * \warning This function must not be called from interrupt context!
	 *
	 * \tparam Predicate is a type of functor to check the predicate
	 *
	 * \param [in] mutex is a reference to mutex which must be owned by calling thread
	 * \param [in] predicate is the predicate that will be checked
	 *
	 * \return 0 if the wait was completed successfully, error code otherwise:
	 * - EPERM - the mutex type is errorChecking or recursive, and the current thread does not own the mutex;
	 */

	template<typename Predicate>
	int wait(Mutex& mutex, Predicate predicate);

	/**
	 * \brief Waits for notification for given duration of time.
	 *
	 * Similar to std::condition_variable::wait_for() -
	 * http://en.cppreference.com/w/cpp/thread/condition_variable/wait_for
	 * Similar to pthread_cond_timedwait() -
	 * http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_cond_timedwait.html#
	 *
	 * Atomically releases supplied mutex and blocks current thread until the condition variable is notified. The thread
	 * will be unblocked when notifyAll() or notifyOne() is executed or when given duration of time expires. It may also
	 * be unblocked spuriously. When unblocked, regardless of the reason, lock is reacquired and wait exits.
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \param [in] mutex is a reference to mutex which must be owned by calling thread
	 * \param [in] duration is the duration after which the wait for notification will be terminated
	 *
	 * \return 0 if the wait was completed successfully, error code otherwise:
	 * - EPERM - the mutex type is errorChecking or recursive, and the current thread does not own the mutex;
	 * - ETIMEDOUT - no notification was received before the specified timeout expired;
	 */

	int waitFor(Mutex& mutex, TickClock::duration duration);

	/**
	 * \brief Waits for notification for given duration of time.
	 *
	 * Similar to std::condition_variable::wait_for() -
	 * http://en.cppreference.com/w/cpp/thread/condition_variable/wait_for
	 * Similar to pthread_cond_timedwait() -
	 * http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_cond_timedwait.html#
	 *
	 * Template variant of waitFor(Mutex& mutex, TickClock::duration duration).
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \tparam Rep is type of tick counter
	 * \tparam Period is std::ratio type representing the tick period of the clock, seconds
	 *
	 * \param [in] mutex is a reference to mutex which must be owned by calling thread
	 * \param [in] duration is the duration after which the wait for notification will be terminated
	 *
	 * \return 0 if the wait was completed successfully, error code otherwise:
	 * - EPERM - the mutex type is errorChecking or recursive, and the current thread does not own the mutex;
	 * - ETIMEDOUT - no notification was received before the specified timeout expired;
	 */

	template<typename Rep, typename Period>
	int waitFor(Mutex& mutex, const std::chrono::duration<Rep, Period> duration)
	{
		return waitFor(mutex, std::chrono::duration_cast<TickClock::duration>(duration));
	}

	/**
	 * \brief Waits for predicate to become true for given duration of time.
	 *
	 * Similar to std::condition_variable::wait_for() -
	 * http://en.cppreference.com/w/cpp/thread/condition_variable/wait_for
	 * Similar to pthread_cond_timedwait() -
	 * http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_cond_timedwait.html#
	 *
	 * Overload for waitFor() which also checks the predicate. This function will return only if the predicate is true
	 * or when given duration of time expires.
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \tparam Rep is type of tick counter
	 * \tparam Period is std::ratio type representing the tick period of the clock, seconds
	 * \tparam Predicate is a type of functor to check the predicate
	 *
	 * \param [in] mutex is a reference to mutex which must be owned by calling thread
	 * \param [in] duration is the duration after which the wait for notification will be terminated
	 * \param [in] predicate is the predicate that will be checked
	 *
	 * \return 0 if the wait was completed successfully, error code otherwise:
	 * - EPERM - the mutex type is errorChecking or recursive, and the current thread does not own the mutex;
	 * - ETIMEDOUT - no notification was received before the specified timeout expired;
	 */

	template<typename Rep, typename Period, typename Predicate>
	int waitFor(Mutex& mutex, const std::chrono::duration<Rep, Period> duration, Predicate predicate)
	{
		return waitUntil(mutex, TickClock::now() + duration + TickClock::duration{1}, std::move(predicate));
	}

	/**
	 * \brief Waits for notification until given time point.
	 *
	 * Similar to std::condition_variable::wait_until() -
	 * http://en.cppreference.com/w/cpp/thread/condition_variable/wait_until
	 * Similar to pthread_cond_timedwait() -
	 * http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_cond_timedwait.html#
	 *
	 * Atomically releases supplied mutex and blocks current thread until the condition variable is notified. The thread
	 * will be unblocked when notifyAll() or notifyOne() is executed or when given time point is reached. It may also be
	 * unblocked spuriously. When unblocked, regardless of the reason, lock is reacquired and wait exits.
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \param [in] mutex is a reference to mutex which must be owned by calling thread
	 * \param [in] timePoint is the time point at which the wait for notification will be terminated
	 *
	 * \return 0 if the wait was completed successfully, error code otherwise:
	 * - EPERM - the mutex type is errorChecking or recursive, and the current thread does not own the mutex;
	 * - ETIMEDOUT - no notification was received before the specified timeout expired;
	 */

	int waitUntil(Mutex& mutex, TickClock::time_point timePoint);

	/**
	 * \brief Waits for notification until given time point.
	 *
	 * Similar to std::condition_variable::wait_until() -
	 * http://en.cppreference.com/w/cpp/thread/condition_variable/wait_until
	 * Similar to pthread_cond_timedwait() -
	 * http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_cond_timedwait.html#
	 *
	 * Template variant of waitUntil(Mutex& mutex, TickClock::time_point timePoint).
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \tparam Duration is a std::chrono::duration type used to measure duration
	 *
	 * \param [in] mutex is a reference to mutex which must be owned by calling thread
	 * \param [in] timePoint is the time point at which the wait for notification will be terminated
	 *
	 * \return 0 if the wait was completed successfully, error code otherwise:
	 * - EPERM - the mutex type is errorChecking or recursive, and the current thread does not own the mutex;
	 * - ETIMEDOUT - no notification was received before the specified timeout expired;
	 */

	template<typename Duration>
	int waitUntil(Mutex& mutex, const std::chrono::time_point<TickClock, Duration> timePoint)
	{
		return waitUntil(mutex, std::chrono::time_point_cast<TickClock::duration>(timePoint));
	}

	/**
	 * \brief Waits for predicate to become true until given time point.
	 *
	 * Similar to std::condition_variable::wait_until() -
	 * http://en.cppreference.com/w/cpp/thread/condition_variable/wait_until
	 * Similar to pthread_cond_timedwait() -
	 * http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_cond_timedwait.html#
	 *
	 * Overload for waitUntil() which also checks the predicate. This function will return only if the predicate is true
	 * or when given time point is reached.
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \tparam Duration is a std::chrono::duration type used to measure duration
	 * \tparam Predicate is a type of functor to check the predicate
	 *
	 * \param [in] mutex is a reference to mutex which must be owned by calling thread
	 * \param [in] timePoint is the time point at which the wait for notification will be terminated
	 * \param [in] predicate is the predicate that will be checked
	 *
	 * \return 0 if the wait was completed successfully, error code otherwise:
	 * - EPERM - the mutex type is errorChecking or recursive, and the current thread does not own the mutex;
	 * - ETIMEDOUT - no notification was received before the specified timeout expired;
	 */

	template<typename Duration, typename Predicate>
	int waitUntil(Mutex& mutex, std::chrono::time_point<TickClock, Duration> timePoint, Predicate predicate);

private:

	/// ThreadControlBlock objects blocked on this condition variable
	internal::ThreadList blockedList_;
};

template<typename Predicate>
int ConditionVariable::wait(Mutex& mutex, Predicate predicate)
{
	CHECK_FUNCTION_CONTEXT();

	while (predicate() == false)
	{
		const auto ret = wait(mutex);
		if (ret != 0)
			return ret;
	}

	return 0;
}

template<typename Duration, typename Predicate>
int ConditionVariable::waitUntil(Mutex& mutex, const std::chrono::time_point<TickClock, Duration> timePoint,
		Predicate predicate)
{
	CHECK_FUNCTION_CONTEXT();

	while (predicate() == false)
	{
		const auto ret = waitUntil(mutex, timePoint);
		if (ret != 0)
			return ret;
	}

	return 0;
}

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_CONDITIONVARIABLE_HPP_
