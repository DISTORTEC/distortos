/**
 * \file
 * \brief SoftwareTimerListNode class header
 *
 * \author Copyright (C) 2015 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_INTERNAL_SCHEDULER_SOFTWARETIMERLISTNODE_HPP_
#define INCLUDE_DISTORTOS_INTERNAL_SCHEDULER_SOFTWARETIMERLISTNODE_HPP_

#include "distortos/TickClock.hpp"

#include "estd/IntrusiveList.hpp"

namespace distortos
{

namespace internal
{

/**
 * \brief SoftwareTimerListNode class is a base for SoftwareTimerControlBlock that serves as a node in intrusive list of
 * software timers (software timer control blocks)
 *
 * This class is needed to break any potential circular dependencies.
 */

class SoftwareTimerListNode
{
public:

	/**
	 * \brief SoftwareTimerListNode's constructor
	 */

	constexpr SoftwareTimerListNode() :
			node{},
			timePoint_{}
	{

	}

	/**
	 * \return const reference to expiration time point
	 */

	const TickClock::time_point& getTimePoint() const
	{
		return timePoint_;
	}

	/// node for intrusive list
	estd::IntrusiveListNode node;

protected:

	/**
	 * \brief Sets time point of expiration
	 *
	 * \param [in] timePoint is the new time point of expiration
	 */

	void setTimePoint(const TickClock::time_point timePoint)
	{
		timePoint_ = timePoint;
	}

private:

	/// time point of expiration
	TickClock::time_point timePoint_;
};

}	// namespace internal

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_INTERNAL_SCHEDULER_SOFTWARETIMERLISTNODE_HPP_
