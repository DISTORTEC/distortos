/**
 * \file
 * \brief ThisThread namespace header
 *
 * \author Copyright (C) 2014-2019 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_THISTHREAD_HPP_
#define INCLUDE_DISTORTOS_THISTHREAD_HPP_

#include "distortos/SchedulingPolicy.hpp"
#include "distortos/TickClock.hpp"

namespace distortos
{

class Thread;
class ThreadIdentifier;

namespace ThisThread
{

/// \addtogroup threads
/// \{

#ifdef DISTORTOS_THREAD_DETACH_ENABLE

/**
 * \brief Detaches calling (current) thread.
 *
 * Similar to std::thread::detach() - https://en.cppreference.com/w/cpp/thread/thread/detach
 * Similar to POSIX pthread_detach() - http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_detach.html
 *
 * Detaches calling (current) thread from its Thread object, allowing execution to continue independently. All resources
 * allocated for the thread will be deallocated when the thread terminates.
 *
 * \warning This function must not be called from interrupt context!
 *
 * \return 0 on success, error code otherwise:
 * - EINVAL - current thread is already detached;
 * - ENOTSUP - current thread cannot be detached;
 */

int detach();

#endif	// def DISTORTOS_THREAD_DETACH_ENABLE

/**
 * \brief Exits calling (current) thread.
 *
 * Similar to POSIX pthread_exit() - http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_exit.html
 *
 * \warning This function must not be called from interrupt context!
 */

__attribute__ ((noreturn))
void exit();

/**
 * \warning This function must not be called from interrupt context!
 *
 * \return reference to Thread object of currently active thread
 */

Thread& get();

/**
 * \warning This function must not be called from interrupt context!
 *
 * \return effective priority of calling (current) thread
 */

uint8_t getEffectivePriority();

/**
 * \warning This function must not be called from interrupt context!
 *
 * \return thread identifier of calling (current) thread
 */

ThreadIdentifier getIdentifier();

/**
 * \warning This function must not be called from interrupt context!
 *
 * \return priority of calling (current) thread
 */

uint8_t getPriority();

/**
 * \return scheduling policy of calling (current) thread
 */

SchedulingPolicy getSchedulingPolicy();

/**
 * \warning This function must not be called from interrupt context!
 *
 * \return "high water mark" (max usage) of thread's stack, bytes
 */

size_t getStackHighWaterMark();

/**
 * \warning This function must not be called from interrupt context!
 *
 * \return size of calling (current) thread's stack, bytes
 */

size_t getStackSize();

/**
 * \brief Changes priority of calling (current) thread.
 *
 * \warning This function must not be called from interrupt context!
 *
 * \param [in] priority is the new priority for thread
 * \param [in] alwaysBehind selects the method of ordering when lowering the priority
 * - false - the thread is moved to the head of the group of threads with the new priority (default),
 * - true - the thread is moved to the tail of the group of threads with the new priority.
 */

void setPriority(uint8_t priority, bool alwaysBehind = {});

/**
 * \param [in] schedulingPolicy is the new scheduling policy of calling (current) thread
 */

void setSchedulingPolicy(SchedulingPolicy schedulingPolicy);

/**
 * \brief Makes the calling (current) thread sleep for at least given duration.
 *
 * Current thread's state is changed to "sleeping".
 *
 * \note To fulfill the "at least" requirement, one additional tick is always added to the sleep duration.
 *
 * \warning This function must not be called from interrupt context!
 *
 * \param [in] duration is the duration after which the thread will be woken
 *
 * \return 0 on success, error code otherwise:
 * - EINTR - the sleep was interrupted by an unmasked, caught signal;
 */

int sleepFor(TickClock::duration duration);

/**
 * \brief Makes the calling (current) thread sleep for at least given duration.
 *
 * Current thread's state is changed to "sleeping".
 *
 * \note To fulfill the "at least" requirement, one additional tick is always added to the sleep duration.
 *
 * \warning This function must not be called from interrupt context!
 *
 * \tparam Rep is type of tick counter
 * \tparam Period is std::ratio type representing the tick period of the clock, seconds
 *
 * \param [in] duration is the duration after which the thread will be woken
 *
 * \return 0 on success, error code otherwise:
 * - EINTR - the sleep was interrupted by an unmasked, caught signal;
 */

template<typename Rep, typename Period>
int sleepFor(const std::chrono::duration<Rep, Period> duration)
{
	return sleepFor(std::chrono::duration_cast<TickClock::duration>(duration));
}

/**
 * \brief Makes the calling (current) thread sleep until some time point is reached.
 *
 * Current thread's state is changed to "sleeping".
 *
 * \warning This function must not be called from interrupt context!
 *
 * \param [in] timePoint is the time point at which the thread will be woken
 *
 * \return 0 on success, error code otherwise:
 * - EINTR - the sleep was interrupted by an unmasked, caught signal;
 */

int sleepUntil(TickClock::time_point timePoint);

/**
 * \brief Makes the calling (current) thread sleep until some time point is reached.
 *
 * Current thread's state is changed to "sleeping".
 *
 * \warning This function must not be called from interrupt context!
 *
 * \tparam Duration is a std::chrono::duration type used to measure duration
 *
 * \param [in] timePoint is the time point at which the thread will be woken
 *
 * \return 0 on success, error code otherwise:
 * - EINTR - the sleep was interrupted by an unmasked, caught signal;
 */

template<typename Duration>
int sleepUntil(const std::chrono::time_point<TickClock, Duration> timePoint)
{
	return sleepUntil(std::chrono::time_point_cast<TickClock::duration>(timePoint));
}

/**
 * \brief Yields time slot of the scheduler to next thread.
 *
 * \warning This function must not be called from interrupt context!
 */

void yield();

/// \}

}	// namespace ThisThread

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_THISTHREAD_HPP_
