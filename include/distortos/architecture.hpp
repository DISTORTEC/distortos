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
 * \date 2014-09-18
 */

#ifndef INCLUDE_DISTORTOS_ARCHITECTURE_HPP_
#define INCLUDE_DISTORTOS_ARCHITECTURE_HPP_

#include "distortos/architecture/parameters.hpp"

#include <utility>

#include <cstddef>

namespace distortos
{

namespace scheduler
{

class ThreadControlBlock;

}	// namespace scheduler

/// architecture namespace has symbols that need to be provided by selected architecture
namespace architecture
{

/**
 * \brief Disables interrupt masking.
 *
 * Enables normal-priority interrupts.
 *
 * \return previous value of interrupts' mask, must be used for matched restoreInterruptMasking() call
 */

InterruptMask disableInterruptMasking();

/**
 * \brief Enables interrupt masking.
 *
 * Disables normal-priority interrupts.
 *
 * \note High-priority interrupts are not controlled by distortos, so they may be left enabled. Support for that feature
 * is architecture-dependent.
 *
 * \return previous value of interrupts' mask, must be used for matched restoreInterruptMasking() call
 */

InterruptMask enableInterruptMasking();

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
 * \param [in] threadControlBlock is a reference to scheduler::ThreadControlBlock object passed to function
 *
 * \return value that can be used as thread's stack pointer, ready for context switching
 */

void* initializeStack(void* buffer, size_t size, void (&function)(scheduler::ThreadControlBlock&),
		scheduler::ThreadControlBlock& threadControlBlock);

/**
 * \brief Restores interrupt masking.
 *
 * Restores previous interrupt masking state (before matching enableInterruptMasking() or disableInterruptMasking() was
 * called), enabling some (maybe all, maybe none) interrupts.
 *
 * \param [in] interrupt_mask is the value of interrupts' mask, must come from previous call to enableInterruptMasking()
 * or disableInterruptMasking()
 */

void restoreInterruptMasking(InterruptMask interrupt_mask);

/**
 * \brief Architecture-specific request for context switch.
 *
 * Causes the architecture to do context save, call scheduler::schedulerInstance.switchContext() and do context restore.
 * The call to scheduler::schedulerInstance.switchContext() must be done from the context in which such call is valid
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

#endif	// INCLUDE_DISTORTOS_ARCHITECTURE_HPP_
