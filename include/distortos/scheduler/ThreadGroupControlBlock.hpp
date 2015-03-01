/**
 * \file
 * \brief ThreadGroupControlBlock class header
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-03-01
 */

#ifndef INCLUDE_DISTORTOS_SCHEDULER_THREADGROUPCONTROLBLOCK_HPP_
#define INCLUDE_DISTORTOS_SCHEDULER_THREADGROUPCONTROLBLOCK_HPP_

#include "distortos/scheduler/ThreadControlBlockList-types.hpp"

namespace distortos
{

namespace scheduler
{

/// ThreadGroupControlBlock class is a control block for ThreadGroup
class ThreadGroupControlBlock
{
public:

	/**
	 * \brief ThreadGroupControlBlock's constructor
	 */

	ThreadGroupControlBlock();

private:

	/// pool instance used by threadControlBlockListAllocator_
	ThreadControlBlockListAllocator::Pool threadControlBlockListAllocatorPool_;

	/// PoolAllocator<> of ThreadControlBlockList
	ThreadControlBlockListAllocator threadControlBlockListAllocator_;

	/// list of ThreadControlBlock elements in this group
	ThreadControlBlockUnsortedList threadControlBlockList_;
};

}	// namespace scheduler

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_SCHEDULER_THREADGROUPCONTROLBLOCK_HPP_
