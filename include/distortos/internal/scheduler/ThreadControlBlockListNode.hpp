/**
 * \file
 * \brief ThreadControlBlockListNode class header
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-12-05
 */

#ifndef INCLUDE_DISTORTOS_INTERNAL_SCHEDULER_THREADCONTROLBLOCKLISTNODE_HPP_
#define INCLUDE_DISTORTOS_INTERNAL_SCHEDULER_THREADCONTROLBLOCKLISTNODE_HPP_

#include "estd/IntrusiveList.hpp"

namespace distortos
{

namespace internal
{

/**
 * \brief ThreadControlBlockListNode class is a base for ThreadControlBlock that provides nodes for intrusive lists
 *
 * This class is needed to break circular dependency - MutexList is contained in ThreadControlBlock and ThreadList is
 * contained in MutexControlBlock.
 */

class ThreadControlBlockListNode
{
public:

	/**
	 * \brief ThreadControlBlockListNode's constructor
	 */

	constexpr ThreadControlBlockListNode() :
			threadGroupNode{}
	{

	}

	/// node for intrusive list in thread group
	estd::IntrusiveListNode threadGroupNode;
};

}	// namespace internal

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_INTERNAL_SCHEDULER_THREADCONTROLBLOCKLISTNODE_HPP_
