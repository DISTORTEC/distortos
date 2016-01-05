/**
 * \file
 * \brief requestFunctionExecution() declaration
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_ARCHITECTURE_REQUESTFUNCTIONEXECUTION_HPP_
#define INCLUDE_DISTORTOS_ARCHITECTURE_REQUESTFUNCTIONEXECUTION_HPP_

namespace distortos
{

namespace internal
{

class ThreadControlBlock;

}	// namespace internal

namespace architecture
{

/**
 * \brief Requests execution of provided function in the specified thread.
 *
 * Main use case for this function is to request execution of signals delivery function. In such case it is called when
 * an unblocked signal, which is not ignored, is generated or queued for specified thread.
 *
 * It must arrange for given function to be executed in specified thread as soon as possible. This generally requires
 * dealing with following scenarios:
 * - current thread is sending the request to itself - in that case just execute function right away;
 * - current thread is sending the request to non-current thread;
 * - interrupt is sending the request to current thread;
 * - interrupt is sending the request to non-current thread;
 *
 * \param [in] threadControlBlock is a reference to internal::ThreadControlBlock of thread in which \a function should
 * be executed
 * \param [in] function is a reference to function that should be executed in thread associated with
 * \a threadControlBlock
 */

void requestFunctionExecution(internal::ThreadControlBlock& threadControlBlock, void (& function)());

}	// namespace architecture

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_ARCHITECTURE_REQUESTFUNCTIONEXECUTION_HPP_
