/**
 * \file
 * \brief Architecture specific symbols
 *
 * \author Copyright (C) 2014 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2014-07-24
 */

#ifndef INCLUDE_DISTORTOS_ARCHITECTURE_HPP_
#define INCLUDE_DISTORTOS_ARCHITECTURE_HPP_

#include <cstddef>

namespace distortos
{

/// architecture namespace has symbols that need to be provided by selected architecture
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
 * \param [in] function is a reference to thread's function
 * \param [in] arguments is an argument for function
 * \param [in] trap is a reference to trap function called when thread function returns
 *
 * \return value that can be used as thread's stack pointer, ready for context switching
 */

void * initializeStack(void *buffer, size_t size, void * (&function)(void *), void *arguments, void (&trap)(void *));

/**
 * \brief Architecture-specific start of scheduling.
 *
 * Initializes all required hardware/software to perform context switching and starts the scheduling.
 */

__attribute__ ((noreturn))
void startScheduling();

}	// namespace architecture

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_ARCHITECTURE_HPP_
