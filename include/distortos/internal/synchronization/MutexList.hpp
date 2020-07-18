/**
 * \file
 * \brief MutexList class header
 *
 * \author Copyright (C) 2014-2015 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_INTERNAL_SYNCHRONIZATION_MUTEXLIST_HPP_
#define INCLUDE_DISTORTOS_INTERNAL_SYNCHRONIZATION_MUTEXLIST_HPP_

#include "distortos/internal/synchronization/MutexListNode.hpp"

namespace distortos
{

namespace internal
{

class MutexControlBlock;

/// intrusive list of mutexes (mutex control blocks)
using MutexList = estd::IntrusiveList<MutexListNode, &MutexListNode::node, MutexControlBlock>;

}	// namespace internal

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_INTERNAL_SYNCHRONIZATION_MUTEXLIST_HPP_
