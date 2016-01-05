/**
 * \file
 * \brief threadRunner() declaration
 *
 * \author Copyright (C) 2014-2016 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_INTERNAL_SCHEDULER_THREADRUNNER_HPP_
#define INCLUDE_DISTORTOS_INTERNAL_SCHEDULER_THREADRUNNER_HPP_

namespace distortos
{

class Thread;

namespace internal
{

/**
 * \brief Thread runner function - entry point of threads.
 *
 * Performs following actions:
 * - executes thread's "run" function;
 * - thread's pre-termination hook is executed (if provided);
 * - thread is terminated and removed from scheduler;
 * - thread's termination hook is executed;
 * - context switch is forced;
 *
 * This function never returns.
 *
 * \param [in] thread is a reference to Thread object that is being run
 * \param [in] run is a reference to Thread's "run" function
 * \param [in] preTerminationHook is a pointer to Thread's pre-termination hook, nullptr to skip
 * \param [in] terminationHook is a reference to Thread's termination hook
 */

void threadRunner(Thread& thread, void (& run)(Thread&), void (* preTerminationHook)(Thread&),
		void (& terminationHook)(Thread&)) __attribute__ ((noreturn));

}	// namespace internal

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_INTERNAL_SCHEDULER_THREADRUNNER_HPP_
