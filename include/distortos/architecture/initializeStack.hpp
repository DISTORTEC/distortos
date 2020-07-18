/**
 * \file
 * \brief initializeStack() declaration
 *
 * \author Copyright (C) 2014-2017 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_ARCHITECTURE_INITIALIZESTACK_HPP_
#define INCLUDE_DISTORTOS_ARCHITECTURE_INITIALIZESTACK_HPP_

#include <utility>

#include <cstddef>

namespace distortos
{

namespace internal
{

class RunnableThread;

}	// namespace internal

namespace architecture
{

/**
 * \brief Architecture-specific stack initialization.
 *
 * This function fills provided buffer with hardware and software stack frame and calculates value of stack pointer
 * register. After this function completes, stack is ready for context switching.
 *
 * \attention buffer and size must be properly adjusted for architecture requirements
 *
 * \param [in] buffer is a pointer to stack's buffer
 * \param [in] size is the size of stack's buffer, bytes
 * \param [in] runnableThread is a reference to internal::RunnableThread object that is being run
 *
 * \return pair with return code (0 on success, error code otherwise) and value that can be used as thread's stack
 * pointer, ready for context switching; error codes:
 * - ENOSPC - size of provided buffer is too small for stack frame;
 */

std::pair<int, void*> initializeStack(void* buffer, size_t size, internal::RunnableThread& runnableThread);

}	// namespace architecture

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_ARCHITECTURE_INITIALIZESTACK_HPP_
