/**
 * \file
 * \brief MutexControlBlockListNode class header
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-12-05
 */

#ifndef INCLUDE_DISTORTOS_INTERNAL_SYNCHRONIZATION_MUTEXCONTROLBLOCKLISTNODE_HPP_
#define INCLUDE_DISTORTOS_INTERNAL_SYNCHRONIZATION_MUTEXCONTROLBLOCKLISTNODE_HPP_

#include "estd/IntrusiveList.hpp"

namespace distortos
{

namespace internal
{

/**
 * \brief MutexControlBlockListNode class is a base for MutexControlBlock that serves as a node in intrusive list
 *
 * This class is needed to break circular dependency - MutexList is contained in ThreadControlBlock and
 * ThreadControlBlockList is contained in MutexControlBlock.
 */

class MutexControlBlockListNode
{
public:

	/**
	 * \brief MutexControlBlockListNode's constructor
	 */

	constexpr MutexControlBlockListNode() :
			node{}
	{

	}

	/// node for intrusive list
	estd::IntrusiveListNode node;
};

}	// namespace internal

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_INTERNAL_SYNCHRONIZATION_MUTEXCONTROLBLOCKLISTNODE_HPP_
