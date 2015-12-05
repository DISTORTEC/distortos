/**
 * \file
 * \brief Types used by ThreadControlBlockList
 *
 * \author Copyright (C) 2014-2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-12-05
 */

#ifndef INCLUDE_DISTORTOS_INTERNAL_SCHEDULER_THREADCONTROLBLOCKLIST_TYPES_HPP_
#define INCLUDE_DISTORTOS_INTERNAL_SCHEDULER_THREADCONTROLBLOCKLIST_TYPES_HPP_

#include "distortos/internal/allocators/PoolAllocator.hpp"
#include "distortos/internal/allocators/SimpleFeedablePool.hpp"

#include <list>
#include <functional>

namespace distortos
{

namespace internal
{

class ThreadControlBlock;

/// type held by ThreadControlBlockList
using ThreadListValueType = std::reference_wrapper<ThreadControlBlock>;

/// type of allocator used by ThreadControlBlockList
using ThreadListAllocator = PoolAllocator<ThreadListValueType, SimpleFeedablePool>;

/// underlying unsorted container of ThreadControlBlockList
using ThreadUnsortedList = std::list<ThreadListValueType, ThreadListAllocator>;

/// generic iterator for ThreadControlBlockList
using ThreadListIterator = ThreadUnsortedList::iterator;

}	// namespace internal

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_INTERNAL_SCHEDULER_THREADCONTROLBLOCKLIST_TYPES_HPP_
