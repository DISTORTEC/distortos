/**
 * \file
 * \brief Semaphore class header
 *
 * \author Copyright (C) 2014 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2014-08-14
 */

#ifndef INCLUDE_DISTORTOS_SCHEDULER_SEMAPHORE_HPP_
#define INCLUDE_DISTORTOS_SCHEDULER_SEMAPHORE_HPP_

#include "distortos/scheduler/ThreadControlBlockList.hpp"

namespace distortos
{

namespace scheduler
{

/**
 * \brief Semaphore is the basic synchronization primitive
 *
 * Similar to POSIX semaphores - http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/V1_chap04.html#tag_04_16
 */

class Semaphore
{
public:

	/**
	 * \brief Semaphore constructor
	 *
	 * Similar to sem_init() - http://pubs.opengroup.org/onlinepubs/9699919799/functions/sem_init.html#
	 *
	 * \param [in] value is the initial value of the semaphore
	 */

	Semaphore(int value);

	/**
	 * \brief Semaphore destructor
	 *
	 * Similar to sem_destroy() - http://pubs.opengroup.org/onlinepubs/9699919799/functions/sem_destroy.html#
	 *
	 * It is safe to destroy a semaphore upon which no threads are currently blocked. The effect of destroying a
	 * semaphore upon which other threads are currently blocked is system error.
	 */

	~Semaphore();

	/**
	 * \brief Gets current value of semaphore.
	 *
	 * Similar to sem_getvalue() - http://pubs.opengroup.org/onlinepubs/9699919799/functions/sem_getvalue.html#
	 *
	 * \return current value of semaphore, positive value if semaphore is not locked, negative value or zero otherwise;
	 * negative value represents the number of threads waiting on this semaphore
	 */

	int getValue() const { return value_; }

	/**
	 * \brief Unlocks the semaphore.
	 *
	 * Similar to sem_post() - http://pubs.opengroup.org/onlinepubs/9699919799/functions/sem_post.html#
	 *
	 * This function shall unlock the semaphore by performing a semaphore unlock operation. If the semaphore value
	 * resulting from this operation is positive, then no threads were blocked waiting for the semaphore to become
	 * unlocked; the semaphore value is simply incremented. Otherwise one of the threads blocked waiting for the
	 * semaphore shall be allowed to return successfully from its call to lock() - the highest priority waiting thread
	 * shall be unblocked, and if there is more than one highest priority thread blocked waiting for the semaphore, then
	 * the highest priority thread that has been waiting the longest shall be unblocked.
	 */

	void post();

	/**
	 * \brief Tries to lock the semaphore.
	 *
	 * Similar to sem_trywait() - http://pubs.opengroup.org/onlinepubs/9699919799/functions/sem_trywait.html#
	 *
	 * This function shall lock the semaphore only if the semaphore is currently not locked; that is, if the semaphore
	 * value is currently positive. Otherwise, it shall not lock the semaphore. Upon successful return, the state of the
	 * semaphore shall be locked and shall remain locked until unlock() function is executed.
	 *
	 * \return zero if the calling process successfully performed the semaphore lock operation, error code otherwise:
	 * - EAGAIN - semaphore was already locked, so it cannot be immediately locked by the tryWait() operation,
	 * - EDEADLK - a deadlock condition was detected,
	 * - EINTR - a signal interrupted this function;
	 */

	int tryWait();

	/**
	 * \brief Locks the semaphore.
	 *
	 * Similar to sem_wait() - http://pubs.opengroup.org/onlinepubs/9699919799/functions/sem_trywait.html#
	 *
	 * This function shall lock the semaphore by performing a semaphore lock operation on that semaphore. If the
	 * semaphore value is currently zero, then the calling thread shall not return from the call to lock() until it
	 * either locks the semaphore or the call is interrupted by a signal. Upon successful return, the state of the
	 * semaphore shall be locked and shall remain locked until unlock() function is executed.
	 *
	 * \return zero if the calling process successfully performed the semaphore lock operation, error code otherwise:
	 * - EDEADLK - a deadlock condition was detected,
	 * - EINTR - a signal interrupted this function;
	 */

	int wait();

private:

	/// ThreadControlBlock objects blocked on this semaphore
	ThreadControlBlockList blockedList_;

	/// internal value of the semaphore
	int value_;
};

}	// namespace scheduler

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_SCHEDULER_SEMAPHORE_HPP_
