/**
 * \file
 * \brief Mock of ThreadControlBlock class
 *
 * \author Copyright (C) 2017 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef UNIT_TEST_INCLUDE_MOCKS_INTERNAL_SCHEDULER_THREADCONTROLBLOCK_HPP_DISTORTOS_INTERNAL_SCHEDULER_THREADCONTROLBLOCK_HPP_
#define UNIT_TEST_INCLUDE_MOCKS_INTERNAL_SCHEDULER_THREADCONTROLBLOCK_HPP_DISTORTOS_INTERNAL_SCHEDULER_THREADCONTROLBLOCK_HPP_

#include "unit-test-common.hpp"

#include "distortos/internal/scheduler/ThreadListNode.hpp"
#include "distortos/internal/scheduler/UnblockFunctor.hpp"

#include "distortos/internal/synchronization/MutexList.hpp"

#include "distortos/ThreadState.hpp"

namespace distortos
{

namespace internal
{

class ThreadControlBlock : public ThreadListNode
{
public:

	MAKE_MOCK0(getOwnedProtocolMutexList, MutexList&());
	MAKE_MOCK1(setPriorityInheritanceMutexControlBlock, void(const MutexControlBlock*));
	MAKE_MOCK0(updateBoostedPriority, void());
	MAKE_MOCK1(updateBoostedPriority, void(uint8_t));
};

}	// namespace internal

}	// namespace distortos

#endif	// UNIT_TEST_INCLUDE_MOCKS_INTERNAL_SCHEDULER_THREADCONTROLBLOCK_HPP_DISTORTOS_INTERNAL_SCHEDULER_THREADCONTROLBLOCK_HPP_
