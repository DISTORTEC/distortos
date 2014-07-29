/**
 * \file
 * \brief Scheduler class implementation
 *
 * \author Copyright (C) 2014 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2014-07-29
 */

#include "distortos/scheduler/Scheduler.hpp"

#include "distortos/scheduler/ThreadControlBlock.hpp"

namespace distortos
{

namespace scheduler
{

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

void * Scheduler::switchContext(void *stack_pointer)
{
	(*currentThreadControlBlock).get().getStack().setStackPointer(stack_pointer);

	const auto previous_thread_control_block = *currentThreadControlBlock;
	threadControlBlockList.erase(currentThreadControlBlock);
	threadControlBlockList.emplace_back(previous_thread_control_block);

	currentThreadControlBlock = threadControlBlockList.begin();

	return (*currentThreadControlBlock).get().getStack().getStackPointer();
}

}	// namespace scheduler

}	// namespace distortos
