/**
 * \file
 * \brief Scheduler class header
 *
 * \author Copyright (C) 2014 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2014-07-24
 */

#ifndef INCLUDE_DISTORTOS_SCHEDULER_SCHEDULER_HPP_
#define INCLUDE_DISTORTOS_SCHEDULER_SCHEDULER_HPP_

namespace distortos
{

/// scheduler namespace has symbols related to scheduling
namespace scheduler
{

class ThreadControlBlock;

/// Scheduler class is a system's scheduler
class Scheduler
{
public:

	/**
	 * \brief Called by architecture-specific code to do final context switch.
	 *
	 * Current task is suspended and the next available task is started.
	 *
	 * \param [in] stack_pointer is the current value of current thread's stack pointer
	 *
	 * \return new thread's stack pointer
	 */

	void * switchContext(void *stack_pointer);

	 ThreadControlBlock *threadControlBlocks[2];

	 ThreadControlBlock *currentThreadControlBlock;
};

}	// namespace scheduler

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_SCHEDULER_SCHEDULER_HPP_
