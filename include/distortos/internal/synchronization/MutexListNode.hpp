/**
 * \file
 * \brief MutexListNode class header
 *
 * \author Copyright (C) 2015 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_INTERNAL_SYNCHRONIZATION_MUTEXLISTNODE_HPP_
#define INCLUDE_DISTORTOS_INTERNAL_SYNCHRONIZATION_MUTEXLISTNODE_HPP_

#include "estd/IntrusiveList.hpp"

namespace distortos
{

namespace internal
{

/**
 * \brief MutexListNode class is a base for MutexControlBlock that serves as a node in intrusive list of mutexes (mutex
 * control blocks)
 *
 * This class is needed to break circular dependency - MutexList is contained in ThreadControlBlock and ThreadList is
 * contained in MutexControlBlock.
 */

class MutexListNode
{
public:

	/**
	 * \brief MutexListNode's constructor
	 */

	constexpr MutexListNode() :
			node{}
	{

	}

	/// node for intrusive list
	estd::IntrusiveListNode node;
};

}	// namespace internal

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_INTERNAL_SYNCHRONIZATION_MUTEXLISTNODE_HPP_
