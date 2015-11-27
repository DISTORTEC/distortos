/**
 * \file
 * \brief ThreadGroupControlBlock class implementation
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-11-27
 */

#include "distortos/internal/scheduler/ThreadGroupControlBlock.hpp"

#include "distortos/internal/scheduler/ThreadControlBlock.hpp"

namespace distortos
{

namespace scheduler
{

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

ThreadGroupControlBlock::ThreadGroupControlBlock() :
		threadControlBlockListAllocatorPool_{},
		threadControlBlockListAllocator_{threadControlBlockListAllocatorPool_},
		threadControlBlockList_{threadControlBlockListAllocator_}
{

}

std::pair<ThreadControlBlockUnsortedList&, ThreadControlBlockListIterator>
ThreadGroupControlBlock::add(ThreadControlBlock& threadControlBlock)
{
	threadControlBlockListAllocatorPool_.feed(threadControlBlock.getThreadGroupLink());
	const auto iterator = threadControlBlockList_.emplace(threadControlBlockList_.end(), threadControlBlock);
	return {threadControlBlockList_, iterator};
}

}	// namespace scheduler

}	// namespace distortos
