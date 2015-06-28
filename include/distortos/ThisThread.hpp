/**
 * \file
 * \brief ThisThread namespace header
 *
 * \author Copyright (C) 2014-2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-06-28
 */

#ifndef INCLUDE_DISTORTOS_THISTHREAD_HPP_
#define INCLUDE_DISTORTOS_THISTHREAD_HPP_

#include "distortos/TickClock.hpp"

namespace distortos
{

class ThreadBase;

/// ThisThread namespace groups functions used to control current thread
namespace ThisThread
{

/**
 * \return reference to ThreadBase object of currently active thread
 */

ThreadBase& get();

/**
 * \return effective priority of calling (current) thread
 */

uint8_t getEffectivePriority();

/**
 * \return priority of calling (current) thread
 */

uint8_t getPriority();

/**
 * Changes priority of calling (current) thread.
 *
 * \param [in] priority is the new priority for thread
 * \param [in] alwaysBehind selects the method of ordering when lowering the priority
 * - false - the thread is moved to the head of the group of threads with the new priority (default),
 * - true - the thread is moved to the tail of the group of threads with the new priority.
 */

void setPriority(uint8_t priority, bool alwaysBehind = {});

/**
 * \brief Makes the calling (current) thread sleep for at least given duration.
 *
 * Current thread's state is changed to "sleeping".
 *
 * \note To fulfill the "at least" requirement, one additional tick is always added to the sleep duration.
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
 * \param Rep is type of tick counter
 * \param Period is std::ratio type representing the tick period of the clock, in seconds
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
 * \param Duration is a std::chrono::duration type used to measure duration
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
 */

void yield();

}	// namespace ThisThread

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_THISTHREAD_HPP_
