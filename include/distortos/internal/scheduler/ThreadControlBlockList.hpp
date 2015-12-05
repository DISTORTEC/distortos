/**
 * \file
 * \brief ThreadControlBlockList class header
 *
 * \author Copyright (C) 2014-2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-12-05
 */

#ifndef INCLUDE_DISTORTOS_INTERNAL_SCHEDULER_THREADCONTROLBLOCKLIST_HPP_
#define INCLUDE_DISTORTOS_INTERNAL_SCHEDULER_THREADCONTROLBLOCKLIST_HPP_

#include "distortos/internal/containers/SortedContainer.hpp"

#include "distortos/internal/scheduler/ThreadControlBlock.hpp"

namespace distortos
{

namespace internal
{

/// functor which gives descending effective priority order of elements on the list
struct ThreadDescendingEffectivePriority
{
	/**
	 * \brief ThreadDescendingEffectivePriority's function call operator
	 *
	 * \param [in] left is the object on the left side of comparison
	 * \param [in] right is the object on the right side of comparison
	 *
	 * \return true if left's effective priority is less than right's effective priority
	 */

	bool operator()(const ThreadListValueType& left, const ThreadListValueType& right) const
	{
		return left.get().getEffectivePriority() < right.get().getEffectivePriority();
	}
};

/// base of ThreadControlBlockList
using ThreadControlBlockListBase = SortedContainer<ThreadUnsortedList, ThreadDescendingEffectivePriority>;

/// List of ThreadControlBlock objects in descending order of effective priority that configures state of kept objects
class ThreadControlBlockList : public ThreadControlBlockListBase
{
public:

	/// base of ThreadControlBlockList
	using Base = ThreadControlBlockListBase;

	using Base::Base;

	/**
	 * \brief ThreadControlBlockList's destructor
	 */

	~ThreadControlBlockList();

	/**
	 * \brief Wrapper for sortedSplice()
	 *
	 * \param [in] other is the container from which the object is transfered
	 * \param [in] otherPosition is the position of the transfered object in the other container
	 */

	void sortedSplice(ThreadControlBlockList& other, iterator otherPosition);
};

}	// namespace internal

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_INTERNAL_SCHEDULER_THREADCONTROLBLOCKLIST_HPP_
