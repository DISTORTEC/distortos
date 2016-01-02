/**
 * \file
 * \brief threadRunner() declaration
 *
 * \author Copyright (C) 2014-2016 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2016-01-02
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
 * After return from thread's "run" function, thread is terminated, its termination hook is executed and context switch
 * is forced, so this function never returns.
 *
 * \param [in] thread is a reference to Thread object that is being run
 * \param [in] run is a reference to Thread's "run" function
 * \param [in] terminationHook is a reference to Thread's termination hook
 */

void threadRunner(Thread& thread, void (& run)(Thread&), void (& terminationHook)(Thread&)) __attribute__ ((noreturn));

}	// namespace internal

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_INTERNAL_SCHEDULER_THREADRUNNER_HPP_
