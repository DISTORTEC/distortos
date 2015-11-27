/**
 * \file
 * \brief MutexControlBlockList class header
 *
 * \author Copyright (C) 2014-2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-11-27
 */

#ifndef INCLUDE_DISTORTOS_SCHEDULER_MUTEXCONTROLBLOCKLIST_HPP_
#define INCLUDE_DISTORTOS_SCHEDULER_MUTEXCONTROLBLOCKLIST_HPP_

#include "distortos/internal/allocators/PoolAllocator.hpp"
#include "distortos/internal/allocators/SimpleFeedablePool.hpp"

#include <list>

namespace distortos
{

namespace synchronization
{

class MutexControlBlock;

}	// namespace synchronization

namespace scheduler
{

/// type held by MutexControlBlockList
using MutexControlBlockListValueType = std::reference_wrapper<synchronization::MutexControlBlock>;

/// type of allocator used by MutexControlBlockList
using MutexControlBlockListAllocator = allocators::PoolAllocator<MutexControlBlockListValueType,
		allocators::SimpleFeedablePool>;

/// list of references to mutex control blocks
using MutexControlBlockList = std::list<MutexControlBlockListValueType, MutexControlBlockListAllocator>;

}	// namespace scheduler

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_SCHEDULER_MUTEXCONTROLBLOCKLIST_HPP_
