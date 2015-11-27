/**
 * \file
 * \brief Types used by SoftwareTimerControlBlockList
 *
 * \author Copyright (C) 2014-2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-11-27
 */

#ifndef INCLUDE_DISTORTOS_SCHEDULER_SOFTWARETIMERCONTROLBLOCKLIST_TYPES_HPP_
#define INCLUDE_DISTORTOS_SCHEDULER_SOFTWARETIMERCONTROLBLOCKLIST_TYPES_HPP_

#include "distortos/internal/allocators/PoolAllocator.hpp"
#include "distortos/internal/allocators/SimpleFeedablePool.hpp"

#include <list>
#include <functional>

namespace distortos
{

namespace scheduler
{

class SoftwareTimerControlBlock;

/// type held by SoftwareTimerControlBlockList
using SoftwareTimerControlBlockListValueType = std::reference_wrapper<SoftwareTimerControlBlock>;

/// type of allocator used by SoftwareTimerControlBlockList
using SoftwareTimerControlBlockListAllocator =
		allocators::PoolAllocator<SoftwareTimerControlBlockListValueType, allocators::SimpleFeedablePool>;

/// underlying unsorted container of SoftwareTimerControlBlockList
using SoftwareTimerControlBlockUnsortedList =
		std::list<SoftwareTimerControlBlockListValueType, SoftwareTimerControlBlockListAllocator>;

/// generic iterator for SoftwareTimerControlBlockList
using SoftwareTimerControlBlockListIterator = SoftwareTimerControlBlockUnsortedList::iterator;

}	// namespace scheduler

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_SCHEDULER_SOFTWARETIMERCONTROLBLOCKLIST_TYPES_HPP_
