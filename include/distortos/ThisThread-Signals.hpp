/**
 * \file
 * \brief ThisThread::Signals namespace header
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-02-21
 */

#ifndef INCLUDE_DISTORTOS_THISTHREAD_SIGNALS_HPP_
#define INCLUDE_DISTORTOS_THISTHREAD_SIGNALS_HPP_

#include "distortos/TickClock.hpp"

#include <utility>

#include <cstdint>

namespace distortos
{

class SignalSet;

namespace ThisThread
{

/// Signals namespace groups signals-related functions operating on current thread
namespace Signals
{

/**
 * \brief Generates signal for current thread.
 *
 * Similar to raise() - http://pubs.opengroup.org/onlinepubs/9699919799/functions/raise.html
 *
 * Adds the signalNumber to set of pending signals of current thread.
 *
 * \param [in] signalNumber is the signal that will be generated, [0; 31]
 *
 * \return 0 on success, error code otherwise:
 * - EINVAL - \a signalNumber value is invalid;
 */

int generateSignal(uint8_t signalNumber);

/**
 * \brief Gets set of currently pending signals for current thread .
 *
 * Similar to sigpending() - http://pubs.opengroup.org/onlinepubs/9699919799/functions/sigpending.html
 *
 * This function shall return the set of signals that are blocked from delivery and are pending on the current thread.
 *
 * \return set of currently pending signals for current thread
 */

SignalSet getPendingSignalSet();

/**
 * \brief Tries to accept pending signals.
 *
 * Similar to sigtimedwait() - http://pubs.opengroup.org/onlinepubs/9699919799/functions/sigtimedwait.html
 *
 * This function shall select the lowest pending signal from provided set, atomically clear it from current thread's set
 * of pending signals and return that signal number. If no signal in provided set is pending at the time of the call,
 * then this function shall return immediately with an error.
 *
 * \param [in] signalSet is a reference to set of signals that may be accepted
 *
 * \return pair with return code (0 on success, error code otherwise) and signal number of signal that was accepted;
 * error codes:
 * - EAGAIN - no signal specified by \a signalSet was pending;
 */

std::pair<int, uint8_t> tryWait(const SignalSet& signalSet);

/**
 * \brief Tries to wait for signals for given duration of time.
 *
 * Similar to sigtimedwait() - http://pubs.opengroup.org/onlinepubs/9699919799/functions/sigtimedwait.html
 *
 * This function shall select the lowest pending signal from provided set, atomically clear it from current thread's set
 * of pending signals and return that signal number. If no signal in provided set is pending at the time of the call,
 * the thread shall be suspended until one or more becomes pending or until given duration of time expires.
 *
 * \param [in] signalSet is a reference to set of signals that will be waited for
 * \param [in] duration is the duration after which the wait for signals will be terminated
 *
 * \return pair with return code (0 on success, error code otherwise) and signal number of signal that was accepted;
 * error codes:
 * - ETIMEDOUT - no signal specified by \a signalSet was generated before the specified \a duration passed;
 */

std::pair<int, uint8_t> tryWaitFor(const SignalSet& signalSet, TickClock::duration duration);

/**
 * \brief Tries to wait for signals for given duration of time.
 *
 * Template variant of tryWaitFor(const SignalSet&, TickClock::duration).
 *
 * \param Rep is type of tick counter
 * \param Period is std::ratio type representing the tick period of the clock, in seconds
 *
 * \param [in] signalSet is a reference to set of signals that will be waited for
 * \param [in] duration is the duration after which the wait for signals will be terminated
 *
 * \return pair with return code (0 on success, error code otherwise) and signal number of signal that was accepted;
 * error codes:
 * - ETIMEDOUT - no signal specified by \a signalSet was generated before the specified \a duration passed;
 */

template<typename Rep, typename Period>
std::pair<int, uint8_t> tryWaitFor(const SignalSet& signalSet, const std::chrono::duration<Rep, Period> duration)
{
	return tryWaitFor(signalSet, std::chrono::duration_cast<TickClock::duration>(duration));
}

/**
 * \brief Tries to wait for signals until given time point.
 *
 * Similar to sigtimedwait() - http://pubs.opengroup.org/onlinepubs/9699919799/functions/sigtimedwait.html
 *
 * This function shall select the lowest pending signal from provided set, atomically clear it from current thread's set
 * of pending signals and return that signal number. If no signal in provided set is pending at the time of the call,
 * the thread shall be suspended until one or more becomes pending or until given time point is reached
 *
 * \param [in] signalSet is a reference to set of signals that will be waited for
 * \param [in] timePoint is the time point at which the wait for signals will be terminated
 *
 * \return pair with return code (0 on success, error code otherwise) and signal number of signal that was accepted;
 * error codes:
 * - ETIMEDOUT - no signal specified by \a signalSet was generated before specified \a timePoint;
 */

std::pair<int, uint8_t> tryWaitUntil(const SignalSet& signalSet, TickClock::time_point timePoint);

/**
 * \brief Tries to wait for signals until given time point.
 *
 * Template variant of tryWaitUntil(const SignalSet&, TickClock::time_point).
 *
 * \param Duration is a std::chrono::duration type used to measure duration
 *
 * \param [in] signalSet is a reference to set of signals that will be waited for
 * \param [in] timePoint is the time point at which the wait for signals will be terminated
 *
 * \return pair with return code (0 on success, error code otherwise) and signal number of signal that was accepted;
 * error codes:
 * - ETIMEDOUT - no signal specified by \a signalSet was generated before specified \a timePoint;
 */

template<typename Duration>
std::pair<int, uint8_t> tryWaitUntil(const SignalSet& signalSet,
		const std::chrono::time_point<TickClock, Duration> timePoint)
{
	return tryWaitUntil(signalSet, std::chrono::time_point_cast<TickClock::duration>(timePoint));
}

/**
 * \brief Waits for signals.
 *
 * Similar to sigwait() - http://pubs.opengroup.org/onlinepubs/9699919799/functions/sigwait.html
 *
 * This function shall select the lowest pending signal from provided set, atomically clear it from current thread's set
 * of pending signals and return that signal number. If no signal in provided set is pending at the time of the call,
 * the thread shall be suspended until one or more becomes pending.
 *
 * \param [in] signalSet is a reference to set of signals that will be waited for
 *
 * \return pair with return code (0 on success, error code otherwise) and signal number of signal that was accepted
 */

std::pair<int, uint8_t> wait(const SignalSet& signalSet);

}	// namespace Signals

}	// namespace ThisThread

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_THISTHREAD_SIGNALS_HPP_
