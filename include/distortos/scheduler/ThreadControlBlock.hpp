/**
 * \file
 * \brief ThreadControlBlock class header
 *
 * \author Copyright (C) 2014 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2014-07-31
 */

#ifndef INCLUDE_DISTORTOS_SCHEDULER_THREADCONTROLBLOCK_HPP_
#define INCLUDE_DISTORTOS_SCHEDULER_THREADCONTROLBLOCK_HPP_

#include "distortos/architecture/Stack.hpp"

#include <cstdint>

namespace distortos
{

namespace scheduler
{

/// ThreadControlBlock class is a simple description of a Thread
class ThreadControlBlock
{
public:

	/**
	 * \brief ThreadControlBlock constructor.
	 *
	 * \param [in] buffer is a pointer to stack's buffer
	 * \param [in] size is the size of stack's buffer, bytes
	 * \param [in] function is a reference to thread's function
	 * \param [in] arguments is an argument for function
	 * \param [in] priority is the thread's priority, 0 - lowest, UINT8_MAX - highest
	 */

	ThreadControlBlock(void *buffer, size_t size, void * (&function)(void *), void *arguments, uint8_t priority);

	/// \return reference to internal Stack object
	architecture::Stack & getStack() { return stack_; }

	/// \return const reference to internal Stack object
	const architecture::Stack & getStack() const { return stack_; }

private:

	/// internal stack object
	architecture::Stack stack_;

	/// thread's priority, 0 - lowest, UINT8_MAX - highest
	uint8_t priority_;
};

}	// namespace scheduler

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_SCHEDULER_THREADCONTROLBLOCK_HPP_
