/**
 * \file
 * \brief initializeStack() declaration
 *
 * \author Copyright (C) 2014-2016 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_ARCHITECTURE_INITIALIZESTACK_HPP_
#define INCLUDE_DISTORTOS_ARCHITECTURE_INITIALIZESTACK_HPP_

#include <cstddef>

namespace distortos
{

class Thread;

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
 * \param [in] thread is a reference to Thread object passed to function
 * \param [in] run is a reference to Thread's "run" function
 * \param [in] preTerminationHook is a pointer to Thread's pre-termination hook, nullptr to skip
 * \param [in] terminationHook is a reference to Thread's termination hook
 *
 * \return value that can be used as thread's stack pointer, ready for context switching
 */

void* initializeStack(void* buffer, size_t size, Thread& thread, void (& run)(Thread&),
		void (* preTerminationHook)(Thread&), void (& terminationHook)(Thread&));

}	// namespace architecture

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_ARCHITECTURE_INITIALIZESTACK_HPP_
