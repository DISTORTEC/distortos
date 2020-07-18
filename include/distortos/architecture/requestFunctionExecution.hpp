/**
 * \file
 * \brief requestFunctionExecution() declaration
 *
 * \author Copyright (C) 2015-2017 Kamil Szczygiel https://distortec.com https://freddiechopin.info
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
 * - current thread is sending the request to non-current thread;
 * - interrupt is sending the request to current thread;
 * - interrupt is sending the request to non-current thread;
 *
 * \warning Current thread must not send the request to itself!
 *
 * \param [in] threadControlBlock is a reference to internal::ThreadControlBlock of thread in which \a function should
 * be executed
 * \param [in] function is a reference to function that should be executed in thread associated with
 * \a threadControlBlock
 *
 * \return 0 on success, error code otherwise:
 * - ENOSPC - amount of free stack is too small to request function execution;
 */

int requestFunctionExecution(internal::ThreadControlBlock& threadControlBlock, void (& function)());

}	// namespace architecture

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_ARCHITECTURE_REQUESTFUNCTIONEXECUTION_HPP_
