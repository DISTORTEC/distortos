/**
 * \file
 * \brief Architecture specific symbols
 *
 * \author Copyright (C) 2014-2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-04-28
 */

#ifndef INCLUDE_DISTORTOS_ARCHITECTURE_ARCHITECTURE_HPP_
#define INCLUDE_DISTORTOS_ARCHITECTURE_ARCHITECTURE_HPP_

#include "distortos/architecture/parameters.hpp"

#include <utility>

namespace distortos
{

class ThreadBase;

/// architecture namespace has symbols that need to be provided by selected architecture
namespace architecture
{

/**
 * \brief Gets the stack used to run main().
 *
 * \return beginning of stack and its size in bytes
 */

std::pair<void*, size_t> getMainStack();

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

void* initializeStack(void* buffer, size_t size, void (&function)(ThreadBase&), ThreadBase& threadBase);

/**
 * \brief Low level architecture initialization.
 *
 * This function is called before constructors for global and static objects from __libc_init_array() via address in
 * distortosPreinitArray[].
 */

void lowLevelInitialization();

/**
 * \brief Restores interrupt masking.
 *
 * Restores previous interrupt masking state (before matching enableInterruptMasking() or disableInterruptMasking() was
 * called), enabling some (maybe all, maybe none) interrupts.
 *
 * \param [in] interruptMask is the value of interrupts' mask, must come from previous call to enableInterruptMasking()
 * or disableInterruptMasking()
 */

void restoreInterruptMasking(InterruptMask interruptMask);

/**
 * \brief Architecture-specific request for context switch.
 *
 * Causes the architecture to do context save, call scheduler::getScheduler().switchContext() and do context restore.
 * The call to scheduler::getScheduler().switchContext() must be done from the context in which such call is valid
 * (usually system interrupt).
 */

void requestContextSwitch();

/**
 * \brief Architecture-specific start of scheduling.
 *
 * Initializes all required hardware/software to perform context switching and starts the scheduling.
 */

void startScheduling();

}	// namespace architecture

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_ARCHITECTURE_ARCHITECTURE_HPP_
