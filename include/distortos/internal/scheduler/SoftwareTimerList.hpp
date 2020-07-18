/**
 * \file
 * \brief SoftwareTimerList class header
 *
 * \author Copyright (C) 2014-2015 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_INTERNAL_SCHEDULER_SOFTWARETIMERLIST_HPP_
#define INCLUDE_DISTORTOS_INTERNAL_SCHEDULER_SOFTWARETIMERLIST_HPP_

#include "distortos/internal/scheduler/SoftwareTimerListNode.hpp"

#include "estd/SortedIntrusiveList.hpp"

namespace distortos
{

namespace internal
{

class SoftwareTimerControlBlock;

/// functor which gives ascending expiration time point order of elements on the list
struct SoftwareTimerAscendingTimePoint
{
	/**
	 * \brief SoftwareTimerAscendingTimePoint's constructor
	 */

	constexpr SoftwareTimerAscendingTimePoint()
	{

	}

	/**
	 * \brief SoftwareTimerAscendingTimePoint's function call operator
	 *
	 * \param [in] left is the object on the left side of comparison
	 * \param [in] right is the object on the right side of comparison
	 *
	 * \return true if left's expiration time point is greater than right's expiration time point
	 */

	bool operator()(const SoftwareTimerListNode& left, const SoftwareTimerListNode& right) const
	{
		return left.getTimePoint() > right.getTimePoint();
	}
};

/// sorted intrusive list of software timers (software timer control blocks)
using SoftwareTimerList = estd::SortedIntrusiveList<SoftwareTimerAscendingTimePoint, SoftwareTimerListNode,
		&SoftwareTimerListNode::node, SoftwareTimerControlBlock>;

}	// namespace internal

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_INTERNAL_SCHEDULER_SOFTWARETIMERLIST_HPP_
