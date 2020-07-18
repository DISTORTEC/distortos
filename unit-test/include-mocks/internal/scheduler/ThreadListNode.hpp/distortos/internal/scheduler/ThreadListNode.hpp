/**
 * \file
 * \brief Mock of ThreadListNode class
 *
 * \author Copyright (C) 2017 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef UNIT_TEST_INCLUDE_MOCKS_INTERNAL_SCHEDULER_THREADLISTNODE_HPP_DISTORTOS_INTERNAL_SCHEDULER_THREADLISTNODE_HPP_
#define UNIT_TEST_INCLUDE_MOCKS_INTERNAL_SCHEDULER_THREADLISTNODE_HPP_DISTORTOS_INTERNAL_SCHEDULER_THREADLISTNODE_HPP_

#include "unit-test-common.hpp"

#include "estd/IntrusiveList.hpp"

namespace distortos
{

namespace internal
{

class ThreadListNode
{
public:

	MAKE_CONST_MOCK0(getEffectivePriority, uint8_t());
	MAKE_CONST_MOCK0(getPriority, uint8_t());

	estd::IntrusiveListNode threadListNode;
	estd::IntrusiveListNode threadGroupNode;
};

}	// namespace internal

}	// namespace distortos

#endif	// UNIT_TEST_INCLUDE_MOCKS_INTERNAL_SCHEDULER_THREADLISTNODE_HPP_DISTORTOS_INTERNAL_SCHEDULER_THREADLISTNODE_HPP_
