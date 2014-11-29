/**
 * \file
 * \brief SoftwareTimerControlBlockList class header
 *
 * \author Copyright (C) 2014 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2014-11-29
 */

#ifndef INCLUDE_DISTORTOS_SCHEDULER_SOFTWARETIMERCONTROLBLOCKLIST_HPP_
#define INCLUDE_DISTORTOS_SCHEDULER_SOFTWARETIMERCONTROLBLOCKLIST_HPP_

#include "distortos/scheduler/SoftwareTimerControlBlock.hpp"

#include "distortos/containers/SortedContainer.hpp"

namespace distortos
{

namespace scheduler
{

/// functor which gives ascending expiration time point order of elements on the list
struct SoftwareTimerControlBlockAscendingTimePoint
{
	/**
	 * \brief operator()
	 *
	 * \param [in] left is the object on the left side of comparison
	 * \param [in] right is the object on the right side of comparison
	 *
	 * \return true if left's expiration time point is greater than right's expiration time point
	 */

	bool operator() (const SoftwareTimerControlBlockListValueType& left,
			const SoftwareTimerControlBlockListValueType& right)
	{
		return left.get().getTimePoint() > right.get().getTimePoint();
	}
};

/// base of SoftwareTimerControlBlockList
using SoftwareTimerControlBlockListBase = containers::SortedContainer
		<
				SoftwareTimerControlBlockListContainer,
				SoftwareTimerControlBlockAscendingTimePoint
		>;

/// list of SoftwareTimerControlBlock objects in ascending order of expiration time point
class SoftwareTimerControlBlockList : public SoftwareTimerControlBlockListBase
{
public:

	using SoftwareTimerControlBlockListBase::SoftwareTimerControlBlockListBase;
};

}	// namespace scheduler

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_SCHEDULER_SOFTWARETIMERCONTROLBLOCKLIST_HPP_
