/**
 * \file
 * \brief Thread class header
 *
 * \author Copyright (C) 2014-2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-12-28
 */

#ifndef INCLUDE_DISTORTOS_THREAD_HPP_
#define INCLUDE_DISTORTOS_THREAD_HPP_

#include "distortos/SchedulingPolicy.hpp"
#include "distortos/SignalSet.hpp"
#include "distortos/ThreadState.hpp"

#include <csignal>

namespace distortos
{

/**
 * \brief Thread class is a pure abstract interface for threads
 *
 * \ingroup threads
 */

class Thread
{
public:

	/**
	 * \brief Thread's destructor
	 */

	virtual ~Thread() = 0;

	/**
	 * \brief Generates signal for thread.
	 *
	 * Similar to pthread_kill() - http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_kill.html
	 *
	 * Adds the signalNumber to set of pending signals. If this thread is currently waiting for this signal, it will be
	 * unblocked.
	 *
	 * \param [in] signalNumber is the signal that will be generated, [0; 31]
	 *
	 * \return 0 on success, error code otherwise:
	 * - EINVAL - \a signalNumber value is invalid;
	 * - ENOTSUP - reception of signals is disabled for this thread;
	 *
	 * \ingroup signals
	 */

	virtual int generateSignal(uint8_t signalNumber) = 0;

	/**
	 * \return effective priority of thread
	 */

	virtual uint8_t getEffectivePriority() const = 0;

	/**
	 * \brief Gets set of currently pending signals.
	 *
	 * Similar to sigpending() - http://pubs.opengroup.org/onlinepubs/9699919799/functions/sigpending.html
	 *
	 * This function shall return the set of signals that are blocked from delivery and are pending on the thread.
	 *
	 * \return set of currently pending signals
	 *
	 * \ingroup signals
	 */

	virtual SignalSet getPendingSignalSet() const = 0;

	/**
	 * \return priority of thread
	 */

	virtual uint8_t getPriority() const = 0;

	/**
	 * \return scheduling policy of the thread
	 */

	virtual SchedulingPolicy getSchedulingPolicy() const = 0;

	/**
	 * \return current state of thread
	 */

	virtual ThreadState getState() const = 0;

	/**
	 * \brief Waits for thread termination.
	 *
	 * Similar to std::thread::join() - http://en.cppreference.com/w/cpp/thread/thread/join
	 * Similar to POSIX pthread_join() - http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_join.html
	 *
	 * Blocks current thread until this thread finishes its execution. The results of multiple simultaneous calls to
	 * join() on the same target thread are undefined.
	 *
	 * \return 0 on success, error code otherwise:
	 * - EDEADLK - deadlock condition was detected,
	 * - EINVAL - this thread is not joinable,
	 * - ...
	 *
	 * \ingroup synchronization
	 */

	virtual int join() = 0;

	/**
	 * \brief Queues signal for thread.
	 *
	 * Similar to sigqueue() - http://pubs.opengroup.org/onlinepubs/9699919799/functions/sigqueue.html
	 *
	 * Adds the signalNumber and signal value (sigval union) to queue of SignalInformation objects. If this thread is
	 * currently waiting for this signal, it will be unblocked.
	 *
	 * \param [in] signalNumber is the signal that will be queued, [0; 31]
	 * \param [in] value is the signal value
	 *
	 * \return 0 on success, error code otherwise:
	 * - EAGAIN - no resources are available to queue the signal, maximal number of signals is already queued in
	 * associated queue of SignalInformation objects;
	 * - EINVAL - \a signalNumber value is invalid;
	 * - ENOTSUP - reception or queuing of signals are disabled for this thread;
	 *
	 * \ingroup signals
	 */

	virtual int queueSignal(uint8_t signalNumber, sigval value) = 0;

	/**
	 * \brief Changes priority of thread.
	 *
	 * If the priority really changes, the position in the thread list is adjusted and context switch may be requested.
	 *
	 * \param [in] priority is the new priority of thread
	 * \param [in] alwaysBehind selects the method of ordering when lowering the priority
	 * - false - the thread is moved to the head of the group of threads with the new priority (default),
	 * - true - the thread is moved to the tail of the group of threads with the new priority.
	 */

	virtual void setPriority(uint8_t priority, bool alwaysBehind = {}) = 0;

	/**
	 * param [in] schedulingPolicy is the new scheduling policy of the thread
	 */

	virtual void setSchedulingPolicy(SchedulingPolicy schedulingPolicy) = 0;

	/**
	 * \brief Starts the thread.
	 *
	 * This operation can be performed on threads in "New" state only.
	 *
	 * \return 0 on success, error code otherwise:
	 * - EINVAL - thread is already started;
	 * - error codes returned by scheduler::Scheduler::add();
	 */

	virtual int start() = 0;

protected:

	/**
	 * \brief Termination hook function of thread
	 *
	 * This function is called after run() completes, from Scheduler::remove().
	 */

	virtual void terminationHook() = 0;

	/**
	 * \brief Thread runner function - entry point of threads.
	 *
	 * After return from actual thread function, thread is terminated, so this function never returns.
	 *
	 * \param [in] thread is a reference to Thread object that is being run
	 */

	static void threadRunner(Thread& thread) __attribute__ ((noreturn));

private:

	/**
	 * \brief "Run" function of thread
	 *
	 * This should be overridden by derived classes.
	 */

	virtual void run() = 0;
};

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_THREAD_HPP_
