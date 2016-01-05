/**
 * \file
 * \brief ThisThread::Signals namespace header
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_THISTHREAD_SIGNALS_HPP_
#define INCLUDE_DISTORTOS_THISTHREAD_SIGNALS_HPP_

#include "distortos/SignalInformation.hpp"
#include "distortos/TickClock.hpp"

#include <utility>

#include <cstdint>

namespace distortos
{

class SignalAction;
class SignalSet;

namespace ThisThread
{

namespace Signals
{

/// \addtogroup signals
/// \{

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
 * - ENOTSUP - reception of signals is disabled for current thread;
 */

int generateSignal(uint8_t signalNumber);

/**
 * \brief Gets set of currently pending signals for current thread.
 *
 * Similar to sigpending() - http://pubs.opengroup.org/onlinepubs/9699919799/functions/sigpending.html
 *
 * This function shall return the set of signals that are blocked from delivery and are pending on the current thread.
 *
 * \return set of currently pending signals for current thread
 */

SignalSet getPendingSignalSet();

/**
 * \brief Gets SignalAction associated with given signal number.
 *
 * Similar to sigaction() - http://pubs.opengroup.org/onlinepubs/9699919799/functions/sigaction.html
 *
 * \param [in] signalNumber is the signal for which the association is requested, [0; 31]
 *
 * \return pair with return code (0 on success, error code otherwise) and SignalAction that is associated with
 * \a signalNumber, default-constructed object if no association was found;
 * error codes:
 * - EINVAL - \a signalNumber value is invalid;
 * - ENOTSUP - reception or catching/handling of signals are disabled for current thread;
 */

std::pair<int, SignalAction> getSignalAction(uint8_t signalNumber);

/**
 * \brief Gets signal mask for current thread.
 *
 * Similar to pthread_sigmask() - http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_sigmask.html#
 *
 * \return SignalSet with signal mask for current thread
 */

SignalSet getSignalMask();

/**
 * \brief Queues signal for current thread.
 *
 * Similar to sigqueue() - http://pubs.opengroup.org/onlinepubs/9699919799/functions/sigqueue.html
 *
 * Adds the signalNumber and signal value (sigval union) to queue of SignalInformation objects.
 *
 * \param [in] signalNumber is the signal that will be queued, [0; 31]
 * \param [in] value is the signal value
 *
 * \return 0 on success, error code otherwise:
 * - EAGAIN - no resources are available to queue the signal, maximal number of signals is already queued in
 * associated queue of SignalInformation objects;
 * - EINVAL - \a signalNumber value is invalid;
 * - ENOTSUP - reception or queuing of signals are disabled for current thread;
 */

int queueSignal(uint8_t signalNumber, sigval value);

/**
 * \brief Sets association for given signal number.
 *
 * Similar to sigaction() - http://pubs.opengroup.org/onlinepubs/9699919799/functions/sigaction.html
 *
 * \param [in] signalNumber is the signal for which the association will be set, [0; 31]
 * \param [in] signalAction is a reference to SignalAction that will be associated with given signal number, object in
 * internal storage is copy-constructed
 *
 * \return pair with return code (0 on success, error code otherwise) and SignalAction that was associated with
 * \a signalNumber, default-constructed object if no association was found;
 * error codes:
 * - EAGAIN - no resources are available to associate \a signalNumber with \a signalAction;
 * - EINVAL - \a signalNumber value is invalid;
 * - ENOTSUP - reception or catching/handling of signals are disabled for current thread;
 */

std::pair<int, SignalAction> setSignalAction(uint8_t signalNumber, const SignalAction& signalAction);

/**
 * \brief Sets signal mask for current thread.
 *
 * Similar to pthread_sigmask() - http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_sigmask.html#
 *
 * \param [in] signalMask is the SignalSet with new signal mask for current thread
 *
 * \return 0 on success, error code otherwise:
 * - ENOTSUP - reception or catching/handling of signals are disabled for current thread;
 */

int setSignalMask(SignalSet signalMask);

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
 * \return pair with return code (0 on success, error code otherwise) and SignalInformation object for accepted signal;
 * error codes:
 * - EAGAIN - no signal specified by \a signalSet was pending;
 * - ENOTSUP - reception of signals is disabled for current thread;
 */

std::pair<int, SignalInformation> tryWait(const SignalSet& signalSet);

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
 * \return pair with return code (0 on success, error code otherwise) and SignalInformation object for accepted signal;
 * error codes:
 * - EINTR - the wait was interrupted by an unmasked, caught signal;
 * - ENOTSUP - reception of signals is disabled for current thread;
 * - ETIMEDOUT - no signal specified by \a signalSet was generated before the specified \a duration passed;
 */

std::pair<int, SignalInformation> tryWaitFor(const SignalSet& signalSet, TickClock::duration duration);

/**
 * \brief Tries to wait for signals for given duration of time.
 *
 * Template variant of tryWaitFor(const SignalSet&, TickClock::duration).
 *
 * \tparam Rep is type of tick counter
 * \tparam Period is std::ratio type representing the tick period of the clock, in seconds
 *
 * \param [in] signalSet is a reference to set of signals that will be waited for
 * \param [in] duration is the duration after which the wait for signals will be terminated
 *
 * \return pair with return code (0 on success, error code otherwise) and SignalInformation object for accepted signal;
 * error codes:
 * - EINTR - the wait was interrupted by an unmasked, caught signal;
 * - ENOTSUP - reception of signals is disabled for current thread;
 * - ETIMEDOUT - no signal specified by \a signalSet was generated before the specified \a duration passed;
 */

template<typename Rep, typename Period>
std::pair<int, SignalInformation> tryWaitFor(const SignalSet& signalSet,
		const std::chrono::duration<Rep, Period> duration)
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
 * \return pair with return code (0 on success, error code otherwise) and SignalInformation object for accepted signal;
 * error codes:
 * - EINTR - the wait was interrupted by an unmasked, caught signal;
 * - ENOTSUP - reception of signals is disabled for current thread;
 * - ETIMEDOUT - no signal specified by \a signalSet was generated before specified \a timePoint;
 */

std::pair<int, SignalInformation> tryWaitUntil(const SignalSet& signalSet, TickClock::time_point timePoint);

/**
 * \brief Tries to wait for signals until given time point.
 *
 * Template variant of tryWaitUntil(const SignalSet&, TickClock::time_point).
 *
 * \tparam Duration is a std::chrono::duration type used to measure duration
 *
 * \param [in] signalSet is a reference to set of signals that will be waited for
 * \param [in] timePoint is the time point at which the wait for signals will be terminated
 *
 * \return pair with return code (0 on success, error code otherwise) and SignalInformation object for accepted signal;
 * error codes:
 * - EINTR - the wait was interrupted by an unmasked, caught signal;
 * - ENOTSUP - reception of signals is disabled for current thread;
 * - ETIMEDOUT - no signal specified by \a signalSet was generated before specified \a timePoint;
 */

template<typename Duration>
std::pair<int, SignalInformation> tryWaitUntil(const SignalSet& signalSet,
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
 * \return pair with return code (0 on success, error code otherwise) and SignalInformation object for accepted signal;
 * error codes:
 * - EINTR - the wait was interrupted by an unmasked, caught signal;
 * - ENOTSUP - reception of signals is disabled for current thread;
 */

std::pair<int, SignalInformation> wait(const SignalSet& signalSet);

/// \}

}	// namespace Signals

}	// namespace ThisThread

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_THISTHREAD_SIGNALS_HPP_
