/**
 * \file
 * \brief initializeStack() declaration
 *
 * \author Copyright (C) 2014-2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-04-28
 */

#ifndef INCLUDE_DISTORTOS_ARCHITECTURE_INITIALIZESTACK_HPP_
#define INCLUDE_DISTORTOS_ARCHITECTURE_INITIALIZESTACK_HPP_

#include <cstddef>

namespace distortos
{

class ThreadBase;

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
 * \param [in] function is a reference to thread's function, this function must not return
 * \param [in] threadBase is a reference to ThreadBase object passed to function
 *
 * \return value that can be used as thread's stack pointer, ready for context switching
 */

void* initializeStack(void* buffer, size_t size, void (& function)(ThreadBase&), ThreadBase& threadBase);

}	// namespace architecture

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_ARCHITECTURE_INITIALIZESTACK_HPP_
