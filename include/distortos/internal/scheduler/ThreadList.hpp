/**
 * \file
 * \brief ThreadList class header
 *
 * \author Copyright (C) 2014-2018 Kamil Szczygiel http://www.distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_INTERNAL_SCHEDULER_THREADLIST_HPP_
#define INCLUDE_DISTORTOS_INTERNAL_SCHEDULER_THREADLIST_HPP_

#include "distortos/internal/scheduler/ThreadListNode.hpp"

#include "estd/SortedIntrusiveList.hpp"

namespace distortos
{

namespace internal
{

class ThreadControlBlock;

/// functor which gives descending effective priority order of elements on the list
struct ThreadDescendingEffectivePriority
{
	/**
	 * \brief ThreadDescendingEffectivePriority's constructor
	 */

	constexpr ThreadDescendingEffectivePriority()
	{

	}

	/**
	 * \brief ThreadDescendingEffectivePriority's function call operator
	 *
	 * \param [in] left is the object on the left-hand side of comparison
	 * \param [in] right is the object on the right-hand side of comparison
	 *
	 * \return true if left's effective priority is less than right's effective priority
	 */

	bool operator()(const ThreadListNode& left, const ThreadListNode& right) const
	{
		return left.getEffectivePriority() < right.getEffectivePriority();
	}
};

/// sorted intrusive list of threads (thread control blocks)
class ThreadList : public estd::SortedIntrusiveList<ThreadDescendingEffectivePriority, ThreadListNode,
		&ThreadListNode::threadListNode, ThreadControlBlock>
{
public:

	/**
	 * \brief ThreadList's constructor
	 */

	constexpr ThreadList() :
			SortedIntrusiveList{}
	{

	}
};

}	// namespace internal

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_INTERNAL_SCHEDULER_THREADLIST_HPP_
