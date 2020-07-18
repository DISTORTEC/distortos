/**
 * \file
 * \brief threadExiter() declaration
 *
 * \author Copyright (C) 2014-2017 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_INTERNAL_SCHEDULER_THREADEXITER_HPP_
#define INCLUDE_DISTORTOS_INTERNAL_SCHEDULER_THREADEXITER_HPP_

namespace distortos
{

namespace internal
{

class RunnableThread;

/**
 * \brief Thread "exiter" function
 *
 * Performs following actions:
 * - thread's "exit 0" hook is executed;
 * - thread is terminated and removed from scheduler;
 * - thread's "exit 1" hook is executed;
 * - context switch is forced;
 *
 * This function never returns.
 *
 * \param [in] runnableThread is a reference to RunnableThread object that is being exited
 */

__attribute__ ((noreturn))
void threadExiter(RunnableThread& runnableThread);

}	// namespace internal

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_INTERNAL_SCHEDULER_THREADEXITER_HPP_
