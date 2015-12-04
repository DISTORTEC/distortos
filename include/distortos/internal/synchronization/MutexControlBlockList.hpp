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
 * \date 2015-12-04
 */

#ifndef INCLUDE_DISTORTOS_INTERNAL_SYNCHRONIZATION_MUTEXCONTROLBLOCKLIST_HPP_
#define INCLUDE_DISTORTOS_INTERNAL_SYNCHRONIZATION_MUTEXCONTROLBLOCKLIST_HPP_

#include "distortos/internal/allocators/PoolAllocator.hpp"
#include "distortos/internal/allocators/SimpleFeedablePool.hpp"

#include <list>

namespace distortos
{

namespace internal
{

class MutexControlBlock;

/// type held by MutexControlBlockList
using MutexControlBlockListValueType = std::reference_wrapper<MutexControlBlock>;

/// type of allocator used by MutexControlBlockList
using MutexControlBlockListAllocator = PoolAllocator<MutexControlBlockListValueType, SimpleFeedablePool>;

/// list of references to mutex control blocks
using MutexControlBlockList = std::list<MutexControlBlockListValueType, MutexControlBlockListAllocator>;

}	// namespace internal

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_INTERNAL_SYNCHRONIZATION_MUTEXCONTROLBLOCKLIST_HPP_
