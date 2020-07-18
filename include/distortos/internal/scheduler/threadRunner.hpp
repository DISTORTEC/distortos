/**
 * \file
 * \brief threadRunner() declaration
 *
 * \author Copyright (C) 2014-2017 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_INTERNAL_SCHEDULER_THREADRUNNER_HPP_
#define INCLUDE_DISTORTOS_INTERNAL_SCHEDULER_THREADRUNNER_HPP_

namespace distortos
{

namespace internal
{

class RunnableThread;

/**
 * \brief Thread runner function - entry point of threads
 *
 * Performs following actions:
 * - executes thread's "run" function;
 * - calls thread "exiter" function;
 *
 * This function never returns.
 *
 * \param [in] runnableThread is a reference to RunnableThread object that is being run
 */

__attribute__ ((noreturn))
void threadRunner(RunnableThread& runnableThread);

}	// namespace internal

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_INTERNAL_SCHEDULER_THREADRUNNER_HPP_
