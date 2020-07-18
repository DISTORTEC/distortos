/**
 * \file
 * \brief ThreadListNode class header
 *
 * \author Copyright (C) 2015-2017 Kamil Szczygiel http://www.distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_INTERNAL_SCHEDULER_THREADLISTNODE_HPP_
#define INCLUDE_DISTORTOS_INTERNAL_SCHEDULER_THREADLISTNODE_HPP_

#include "estd/IntrusiveList.hpp"

namespace distortos
{

namespace internal
{

/**
 * \brief ThreadListNode class is a base for ThreadControlBlock that provides nodes for intrusive lists
 *
 * This class is needed to break circular dependency - MutexList is contained in ThreadControlBlock and ThreadList is
 * contained in MutexControlBlock.
 */

class ThreadListNode
{
public:

	/**
	 * \brief ThreadListNode's constructor
	 *
	 * \param [in] priority is the thread's priority, 0 - lowest, UINT8_MAX - highest
	 */

	constexpr explicit ThreadListNode(const uint8_t priority) :
			threadListNode{},
			threadGroupNode{},
			priority_{priority},
			boostedPriority_{}
	{

	}

	/**
	 * \return effective priority of thread
	 */

	uint8_t getEffectivePriority() const
	{
		return std::max(priority_, boostedPriority_);
	}

	/**
	 * \return priority of thread
	 */

	uint8_t getPriority() const
	{
		return priority_;
	}

	/// node for intrusive list in thread lists
	estd::IntrusiveListNode threadListNode;

	/// node for intrusive list in thread group
	estd::IntrusiveListNode threadGroupNode;

protected:

	/// thread's priority, 0 - lowest, UINT8_MAX - highest
	uint8_t priority_;

	/// thread's boosted priority, 0 - no boosting
	uint8_t boostedPriority_;
};

}	// namespace internal

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_INTERNAL_SCHEDULER_THREADLISTNODE_HPP_
