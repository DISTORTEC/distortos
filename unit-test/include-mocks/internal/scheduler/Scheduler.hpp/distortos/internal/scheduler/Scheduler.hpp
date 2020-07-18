/**
 * \file
 * \brief Mock of Scheduler class
 *
 * \author Copyright (C) 2017 Kamil Szczygiel http://www.distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef UNIT_TEST_INCLUDE_MOCKS_INTERNAL_SCHEDULER_SCHEDULER_HPP_DISTORTOS_INTERNAL_SCHEDULER_SCHEDULER_HPP_
#define UNIT_TEST_INCLUDE_MOCKS_INTERNAL_SCHEDULER_SCHEDULER_HPP_DISTORTOS_INTERNAL_SCHEDULER_SCHEDULER_HPP_

#include "unit-test-common.hpp"

#include "distortos/internal/scheduler/ThreadControlBlock.hpp"
#include "distortos/internal/scheduler/ThreadList.hpp"

namespace distortos
{

namespace internal
{

class Scheduler
{
public:

	MAKE_MOCK2(block, int(ThreadList&, ThreadState));
	MAKE_MOCK3(block, int(ThreadList&, ThreadState, const UnblockFunctor*));
	MAKE_MOCK3(block, int(ThreadList&, ThreadList::iterator, ThreadState));
	MAKE_MOCK4(block, int(ThreadList&, ThreadList::iterator, ThreadState, const UnblockFunctor*));
	MAKE_MOCK3(blockUntil, int(ThreadList&, ThreadState, TickClock::time_point));
	MAKE_MOCK4(blockUntil, int(ThreadList&, ThreadState, TickClock::time_point, const UnblockFunctor*));
	MAKE_CONST_MOCK0(getCurrentThreadControlBlock, ThreadControlBlock&());
	MAKE_MOCK1(unblock, void(ThreadList::iterator));
	MAKE_MOCK2(unblock, void(ThreadList::iterator, UnblockReason));
};

}	// namespace internal

}	// namespace distortos

#endif	// UNIT_TEST_INCLUDE_MOCKS_INTERNAL_SCHEDULER_SCHEDULER_HPP_DISTORTOS_INTERNAL_SCHEDULER_SCHEDULER_HPP_
