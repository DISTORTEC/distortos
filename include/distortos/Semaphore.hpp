/**
 * \file
 * \brief Semaphore class header
 *
 * \author Copyright (C) 2014-2019 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_SEMAPHORE_HPP_
#define INCLUDE_DISTORTOS_SEMAPHORE_HPP_

#include "distortos/internal/scheduler/ThreadList.hpp"

#include "distortos/TickClock.hpp"

namespace distortos
{

/**
 * \brief Semaphore is the basic synchronization primitive
 *
 * Similar to POSIX semaphores - https://pubs.opengroup.org/onlinepubs/9699919799/basedefs/V1_chap04.html#tag_04_16
 *
 * \ingroup synchronization
 */

class Semaphore
{
public:

	/// type used for semaphore's "value"
	using Value = unsigned int;

	/**
	 * \brief Semaphore's constructor
	 *
	 * Similar to sem_init() - https://pubs.opengroup.org/onlinepubs/9699919799/functions/sem_init.html#
	 *
	 * \param [in] value is the initial value of the semaphore, if this value is greater than maxValue, it will be
	 * truncated
	 * \param [in] maxValue is the max value of the semaphore before post() returns EOVERFLOW, default - max for Value
	 * type
	 */

	constexpr explicit Semaphore(const Value value, const Value maxValue = std::numeric_limits<Value>::max()) :
			blockedList_{},
			value_{value < maxValue ? value : maxValue},
			maxValue_{maxValue}
	{

	}

	/**
	 * \brief Semaphore's destructor
	 *
	 * Similar to sem_destroy() - https://pubs.opengroup.org/onlinepubs/9699919799/functions/sem_destroy.html#
	 *
	 * It is safe to destroy a semaphore upon which no threads are currently blocked. The effect of destroying a
	 * semaphore upon which other threads are currently blocked is system error.
	 */

	~Semaphore() = default;

	/**
	 * \return max value of the semaphore
	 */

	Value getMaxValue() const
	{
		return maxValue_;
	}

	/**
	 * \brief Gets current value of semaphore.
	 *
	 * Similar to sem_getvalue() - https://pubs.opengroup.org/onlinepubs/9699919799/functions/sem_getvalue.html#
	 *
	 * \return current value of semaphore, positive value if semaphore is not locked, 0 otherwise
	 */

	Value getValue() const
	{
		return value_;
	}

	/**
	 * \brief Unlocks the semaphore.
	 *
	 * Similar to sem_post() - https://pubs.opengroup.org/onlinepubs/9699919799/functions/sem_post.html#
	 *
	 * This function shall unlock the semaphore by performing a semaphore unlock operation. If the semaphore value
	 * resulting from this operation is positive, then no threads were blocked waiting for the semaphore to become
	 * unlocked; the semaphore value is simply incremented. Otherwise one of the threads blocked waiting for the
	 * semaphore shall be allowed to return successfully from its call to lock() - the highest priority waiting thread
	 * shall be unblocked, and if there is more than one highest priority thread blocked waiting for the semaphore, then
	 * the highest priority thread that has been waiting the longest shall be unblocked.
	 *
	 * \return 0 if the calling process successfully "posted" the semaphore, error code otherwise:
	 * - EOVERFLOW - the maximum allowable value for a semaphore would be exceeded;
	 */

	int post();

	/**
	 * \brief Tries to lock the semaphore.
	 *
	 * Similar to sem_trywait() - https://pubs.opengroup.org/onlinepubs/9699919799/functions/sem_trywait.html#
	 *
	 * This function shall lock the semaphore only if the semaphore is currently not locked; that is, if the semaphore
	 * value is currently positive. Otherwise, it shall not lock the semaphore. Upon successful return, the state of the
	 * semaphore shall be locked and shall remain locked until unlock() function is executed.
	 *
	 * \return 0 if the calling process successfully performed the semaphore lock operation, error code otherwise:
	 * - EAGAIN - semaphore was already locked, so it cannot be immediately locked by the tryWait() operation;
	 */

	int tryWait();

	/**
	 * \brief Tries to lock the semaphore for given duration of time.
	 *
	 * Similar to sem_timedwait() - https://pubs.opengroup.org/onlinepubs/9699919799/functions/sem_timedwait.html#
	 *
	 * If the semaphore is already locked, the calling thread shall block until the semaphore becomes available as in
	 * wait() function. If the semaphore cannot be locked without waiting for another thread to unlock the semaphore,
	 * this wait shall be terminated when the specified timeout expires.
	 *
	 * Under no circumstance shall the function fail with a timeout if the semaphore can be locked immediately. The
	 * validity of the duration parameter need not be checked if the semaphore can be locked immediately.
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \param [in] duration is the duration after which the wait will be terminated without locking the semaphore
	 *
	 * \return 0 if the calling process successfully performed the semaphore lock operation, error code otherwise:
	 * - EINTR - the wait was interrupted by an unmasked, caught signal;
	 * - ETIMEDOUT - the semaphore could not be locked before the specified timeout expired;
	 */

	int tryWaitFor(TickClock::duration duration);

	/**
	 * \brief Tries to lock the semaphore for given duration of time.
	 *
	 * Template variant of tryWaitFor(TickClock::duration duration).
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \tparam Rep is type of tick counter
	 * \tparam Period is std::ratio type representing the tick period of the clock, seconds
	 *
	 * \param [in] duration is the duration after which the wait will be terminated without locking the semaphore
	 *
	 * \return 0 if the calling process successfully performed the semaphore lock operation, error code otherwise:
	 * - EINTR - the wait was interrupted by an unmasked, caught signal;
	 * - ETIMEDOUT - the semaphore could not be locked before the specified timeout expired;
	 */

	template<typename Rep, typename Period>
	int tryWaitFor(const std::chrono::duration<Rep, Period> duration)
	{
		return tryWaitFor(std::chrono::duration_cast<TickClock::duration>(duration));
	}

	/**
	 * \brief Tries to lock the semaphore until given time point.
	 *
	 * Similar to sem_timedwait() - https://pubs.opengroup.org/onlinepubs/9699919799/functions/sem_timedwait.html#
	 *
	 * If the semaphore is already locked, the calling thread shall block until the semaphore becomes available as in
	 * wait() function. If the semaphore cannot be locked without waiting for another thread to unlock the semaphore,
	 * this wait shall be terminated when the specified timeout expires.
	 *
	 * Under no circumstance shall the function fail with a timeout if the semaphore can be locked immediately. The
	 * validity of the timePoint parameter need not be checked if the semaphore can be locked immediately.
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \param [in] timePoint is the time point at which the wait will be terminated without locking the semaphore
	 *
	 * \return 0 if the calling process successfully performed the semaphore lock operation, error code otherwise:
	 * - EINTR - the wait was interrupted by an unmasked, caught signal;
	 * - ETIMEDOUT - the semaphore could not be locked before the specified timeout expired;
	 */

	int tryWaitUntil(TickClock::time_point timePoint);

	/**
	 * \brief Tries to lock the semaphore until given time point.
	 *
	 * Template variant of tryWaitUntil(TickClock::time_point timePoint).
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \tparam Duration is a std::chrono::duration type used to measure duration
	 *
	 * \param [in] timePoint is the time point at which the wait will be terminated without locking the semaphore
	 *
	 * \return 0 if the calling process successfully performed the semaphore lock operation, error code otherwise:
	 * - EINTR - the wait was interrupted by an unmasked, caught signal;
	 * - ETIMEDOUT - the semaphore could not be locked before the specified timeout expired;
	 */

	template<typename Duration>
	int tryWaitUntil(const std::chrono::time_point<TickClock, Duration> timePoint)
	{
		return tryWaitUntil(std::chrono::time_point_cast<TickClock::duration>(timePoint));
	}

	/**
	 * \brief Locks the semaphore.
	 *
	 * Similar to sem_wait() - https://pubs.opengroup.org/onlinepubs/9699919799/functions/sem_trywait.html#
	 *
	 * This function shall lock the semaphore by performing a semaphore lock operation on that semaphore. If the
	 * semaphore value is currently zero, then the calling thread shall not return from the call to lock() until it
	 * either locks the semaphore or the call is interrupted by a signal. Upon successful return, the state of the
	 * semaphore shall be locked and shall remain locked until unlock() function is executed.
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \return 0 if the calling process successfully performed the semaphore lock operation, error code otherwise:
	 * - EINTR - the wait was interrupted by an unmasked, caught signal;
	 */

	int wait();

	Semaphore(const Semaphore&) = delete;
	Semaphore(Semaphore&&) = default;
	const Semaphore& operator=(const Semaphore&) = delete;
	Semaphore& operator=(Semaphore&&) = delete;

private:

	/**
	 * \brief Internal version of tryWait().
	 *
	 * Internal version with no interrupt masking.
	 *
	 * \return 0 if the calling process successfully performed the semaphore lock operation, error code otherwise:
	 * - EAGAIN - semaphore was already locked, so it cannot be immediately locked by the tryWait() operation;
	 */

	int tryWaitInternal();

	/// ThreadControlBlock objects blocked on this semaphore
	internal::ThreadList blockedList_;

	/// internal value of the semaphore
	Value value_;

	/// max value of the semaphore
	Value maxValue_;
};

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_SEMAPHORE_HPP_
